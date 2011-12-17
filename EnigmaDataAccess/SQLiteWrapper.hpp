#ifndef SQLITEWRAPPER_HPP_INCLUDED
#define SQLITEWRAPPER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#ifdef ENIGMA_PLATFORM_WINDOWS
    #pragma warning( push )
    #pragma warning( disable : 4061 )
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4127 )
    #pragma warning( disable : 4217 )
    #pragma warning( disable : 4242 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4280 )
    #pragma warning( disable : 4513 )
    #pragma warning( disable : 4514 )
    #pragma warning( disable : 4619 )
    #pragma warning( disable : 4668 )
    #pragma warning( disable : 4710 )
    #pragma warning( disable : 4820 )
    #include "sqlite3.h" //amalgamation
    #pragma warning( pop )
#else
    #include "sqlite3.h" //amalgamation
#endif

#include "DatabaseResultSet.hpp"
#include "DatabaseParameter.hpp"
#include "DatabaseCommand.hpp"

#include "boost_foreach.hpp"

namespace Enigma
{
    class DllExport SQLiteWrapper
    {
        private:

        sqlite3* mDatabase;
        char* mErrorMessage;
        char** result;
        int rc;
        int mNumberOfRows;
        int mNumberOfColumns;
        bool mDatabaseOpen;

        protected:

        //nothing here yet

        public:

        std::vector<std::string> vcol_head;
        std::vector<std::string> vdata;

        SQLiteWrapper()
        {
            this->Init();
        }

        SQLiteWrapper(const char* tableName)
        {
            this->Init(tableName);
        }

        SQLiteWrapper(const std::string& tableName)
        {
            this->Init(tableName.c_str());
        }

        ~SQLiteWrapper()
        {
            if(this->mDatabaseOpen)
            {
                sqlite3_close(mDatabase);
            }
        }

        void Init(const char* tableName="../Db/Enigma.db")
        {
            this->mErrorMessage=NULL;
            this->rc=0;
            this->mDatabaseOpen=false;
            this->mNumberOfRows=0;
            this->mNumberOfColumns=0;

            rc = sqlite3_open(tableName, &mDatabase);

            if(rc!=SQLITE_OK)
            {
                std::cout << "Can't open database: " << sqlite3_errmsg(mDatabase) << std::endl;
                sqlite3_close(mDatabase);
                return;
            }

            this->mDatabaseOpen=true;
        }

        int Execute(const std::string& query)
        {
            return Execute(query.c_str());
        }

        int Execute(const char* query)
        {
            rc = sqlite3_get_table(
                mDatabase,         /* An open database */
                query,             /* SQL to be executed */
                &result,           /* Result written to a char *[]  that this points to */
                &mNumberOfRows,    /* Number of result rows written here */
                &mNumberOfColumns, /* Number of result columns written here */
                &mErrorMessage     /* Error msg written here */
                );

           if(!vcol_head.empty())
           {
                vcol_head.clear();
           }

           if(!vdata.empty())
           {
                vdata.clear();
           }

           if(rc==SQLITE_OK)
           {
               for(int i=0; i < mNumberOfColumns; ++i)
               {
                    vcol_head.push_back(result[i]);   /* First row heading */
               }
               for(int i=0; i < mNumberOfColumns * mNumberOfRows; ++i)
               {
                    vdata.push_back(result[mNumberOfColumns+i]);
               }
           }

            sqlite3_free_table(result);

            return rc;
        }

        DatabaseResultSet Execute(DatabaseCommand& databaseCommand)
        {
            DatabaseResultSet databaseResultSet;

            const char* tail=NULL;
            sqlite3_stmt* statement=NULL;
            size_t row=0;

            size_t columnCount=0;
            size_t columnType=0;
            std::string columnName;

            if(!this->mDatabaseOpen)
            {
                std::cout << "Database connection is not open." << std::endl;
                return databaseResultSet; //no need to call finalize because prepare hasn't been called yet.
            }

            rc = sqlite3_prepare_v2(
                mDatabase,                          /* Database handle */
                databaseCommand.GetQuery().c_str(), /* SQL statement, UTF-8 encoded */
                databaseCommand.GetQuery().length(),/* Maximum length of zSql in bytes. */
                &statement,                         /* OUT: Statement handle */
                &tail                               /* OUT: Pointer to unused portion of zSql */
                );

            if(rc!=SQLITE_OK)
            {
                std::cout << sqlite3_errmsg(mDatabase) << std::endl;
                return databaseResultSet; //no need to call finalize because prepare failed.
            }

            BOOST_FOREACH(DatabaseParameter parameter, databaseCommand.GetDatabaseParameters())
            {
                switch(parameter.GetType())
                {
                    case DATABASE_DATATYPE_NULL:
                        break;
                    case DATABASE_DATATYPE_INTEGER:
                            rc = sqlite3_bind_int(statement,sqlite3_bind_parameter_index(statement,parameter.GetName().c_str()),parameter.GetValue<int>());
                        break;
                    case DATABASE_DATATYPE_TEXT:
                            rc = sqlite3_bind_text(statement,sqlite3_bind_parameter_index(statement,parameter.GetName().c_str()),parameter.GetValue<std::string>().c_str(),parameter.GetLength(),SQLITE_STATIC);
                        break;
                    case DATABASE_DATATYPE_DOUBLE:
                            rc = sqlite3_bind_double(statement,sqlite3_bind_parameter_index(statement,parameter.GetName().c_str()),parameter.GetValue<double>());
                        break;
                    case DATABASE_DATATYPE_BLOB:
							std::cout << "Blob not yet supported." << std::endl;
                        break;
					case DATABASE_DATATYPE_UUID:
							rc = sqlite3_bind_text(statement,sqlite3_bind_parameter_index(statement,parameter.GetName().c_str()),parameter.GetValue<std::string>().c_str(),parameter.GetLength(),SQLITE_STATIC);
						break;
                    default:
							rc = sqlite3_bind_text(statement,sqlite3_bind_parameter_index(statement,parameter.GetName().c_str()),parameter.GetValue<std::string>().c_str(),parameter.GetLength(),SQLITE_STATIC);
                        break;
                }
                if(rc!=SQLITE_OK)
                {
                    std::cout << sqlite3_errmsg(mDatabase) << std::endl;
                }
            }

            do
            {
                rc = sqlite3_step(statement);
            }
            while(rc==SQLITE_BUSY); //make sure this actually happens.

            if(rc!=SQLITE_OK && rc!=SQLITE_ROW && rc!=SQLITE_DONE)
            {
                goto JOBDONE;
            }

            columnCount = sqlite3_column_count(statement);
            databaseResultSet.SetColumnCapacity(columnCount);

            for(size_t i=0;i<columnCount;i++)
            {
                columnType = sqlite3_column_type(statement,i);
                columnName = sqlite3_column_name(statement,i);
                databaseResultSet.SetColumnType(i,columnType);
                databaseResultSet.SetColumnName(i,columnName);
            }

            while(rc==SQLITE_ROW)
            {
                for(size_t i=0;i<columnCount;i++)
                {
                    databaseResultSet.SetValue<const unsigned char*>(row,i,sqlite3_column_text(statement,i));
                }

                do
                {
                    rc = sqlite3_step(statement);
                }
                while(rc==SQLITE_BUSY); //make sure this actually happens.
                row++;
            }

            JOBDONE:
            if(rc!=SQLITE_OK && rc!=SQLITE_ROW && rc!=SQLITE_DONE)
            {
                std::cout << sqlite3_errmsg(mDatabase) << std::endl;
            }

            rc = sqlite3_finalize(statement);


            if(rc!=SQLITE_OK)
            {
                std::cout << sqlite3_errmsg(mDatabase) << std::endl;
            }

            return databaseResultSet;
        }
    };
};

#endif // SQLITEWRAPPER_HPP_INCLUDED

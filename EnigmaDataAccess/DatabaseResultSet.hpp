#ifndef DATABASERESULTSET_HPP_INCLUDED
#define DATABASERESULTSET_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "EnigmaException.hpp"

#include "vector.hpp"
#include "string.hpp"
#include <boost/uuid/uuid.hpp>
#include "boost_lexical_cast.hpp"
#include "EnigmaException.hpp"

namespace Enigma
{

    class DllExport DatabaseResultSet
    {

        private:

        std::vector<std::string> mColumnNames;
        std::vector<int> mColumnTypes;
        std::vector< std::vector<std::string> > mData;

        bool mHasValue;

        protected:

        public:

        DatabaseResultSet();
        ~DatabaseResultSet();

        template <class T>
        T GetValue(size_t row,size_t column)
        {
            if(this->mData.size()<=row)
            {
                throw EnigmaException("ResultSet row index out of bounds.");
            }
            if(this->mData[row].size()<=column)
            {
                throw EnigmaException("ResultSet column index out of bounds.");
            }

            return boost::lexical_cast<T>(this->mData[row][column]);
        }

        template <class T>
        T GetValue(size_t row,const std::string& column)
        {
            if(this->mData.size()<row)
            {
                throw EnigmaException("ResultSet row index out of bounds.");
            }

            return boost::lexical_cast<T>(this->mData[row][this->FindIndex(column)]);
        }

        template <class T>
        T GetValue(size_t row,const char* column)
        {
            if(this->mData.size()<row)
            {
                throw EnigmaException("ResultSet row index out of bounds.");
            }

            return boost::lexical_cast<T>(this->mData[row][this->FindIndex(column)]);
        }

        template <class T>
        void SetValue(size_t row,size_t column,T value)
        {
            //this makes sure there is an element in place to be replaced.
            //Although I'm not happy about the fact that an assignment throws
            //an exception otherwise it makes sense given the nature of the data structure.
            if(row>=this->mData.size())
            {
                this->mData.resize(row+1);
            }
            if(column>=this->mData[row].size())
            {
                this->mData[row].resize(column+1);
            }

            //set the value.
            this->mData[row][column] = boost::lexical_cast<std::string>(value);

            //now mark the resultset as having data.
            this->mHasValue=true;
        }

        void SetColumnCapacity(size_t index);

        const std::string& GetColumnName(size_t column) const;
        void SetColumnName(size_t column,std::string value);

        const int GetColumnType(size_t column) const;
        void SetColumnType(size_t column,int value);

        size_t FindIndex(const std::string& value);
        size_t FindIndex(const char* value);

        std::string ToString();

        size_t GetUpperBound();

        bool IsEmpty() const {return !this->mHasValue;}
    };

};

#endif // DATABASERESULTSET_HPP_INCLUDED

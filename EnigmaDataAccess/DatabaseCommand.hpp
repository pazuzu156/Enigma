#ifndef DATABASECOMMAND_HPP_INCLUDED
#define DATABASECOMMAND_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "DatabaseParameter.hpp"

namespace Enigma
{
    class DllExport DatabaseCommand
    {

        private:

        std::vector<DatabaseParameter> mDatabaseParameters;
        std::string mQuery;

        protected:

        public:

        DatabaseCommand();
        ~DatabaseCommand();

        const std::string& GetQuery() const;
        void SetQuery(std::string value);

        std::vector<DatabaseParameter> GetDatabaseParameters() const;

        void AddParameter(const std::string& name,const std::string& value);
        void AddParameter(const char* name,const char* value);

        void AddParameter(const std::string& name,int value);
        void AddParameter(const char* name,int value);

        void AddParameter(const std::string& name,double value);
        void AddParameter(const char* name,double value);

		void AddParameter(const std::string& name,const boost::uuids::uuid& value);
        void AddParameter(const char* name,const boost::uuids::uuid& value);

		void AddParameter(const std::string& name,size_t value);
        void AddParameter(const char* name,size_t value);
    };

};

#endif // DATABASECOMMAND_HPP_INCLUDED

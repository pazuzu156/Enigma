#ifndef DATABASEPARAMETER_HPP_INCLUDED
#define DATABASEPARAMETER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "string.hpp"
#include "vector.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost_lexical_cast.hpp"

#include "DatabaseDatatype.hpp"

namespace Enigma
{
    class DatabaseParameter
    {

        private:

        std::string mValue;
        std::string mName;
        size_t mType;
        int mDirection;

        protected:

        public:

        DatabaseParameter()
        {
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_NULL;
        }

        DatabaseParameter(const char* name, const char* value)
        {
            this->mName=name;
            this->mValue=value;
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_TEXT;
        }

        DatabaseParameter(const std::string& name, const std::string& value)
        {
            this->mName=name;
            this->mValue=value;
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_TEXT;

        }

        DatabaseParameter(const char* name, int value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_INTEGER;
        }

        DatabaseParameter(const std::string& name, int value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_INTEGER;
        }

        DatabaseParameter(const char* name, double value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_DOUBLE;
        }

        DatabaseParameter(const std::string& name, double value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_DOUBLE;
        }

        DatabaseParameter(const char* name, const boost::uuids::uuid& value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_UUID;
        }

        DatabaseParameter(const std::string& name, const boost::uuids::uuid& value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_UUID;
        }

        DatabaseParameter(const char* name, size_t value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_INTEGER;
        }

        DatabaseParameter(const std::string& name, size_t value)
        {
            this->mName=name;
            this->mValue=boost::lexical_cast<std::string>(value);
            this->mDirection=0;
            this->mType=DATABASE_DATATYPE_INTEGER;
        }

        ~DatabaseParameter()
        {

        }

        template <class T>
        T GetValue()
        {
            return boost::lexical_cast<T>(this->mValue);
        }

        template <class T>
        void SetValue(T value)
        {
            this->mValue = boost::lexical_cast<std::string>(value);
        }

        const std::string& GetName() const
        {
            return this->mName;
        }

        void SetName(std::string value)
        {
            this->mName = value;
        }

        size_t GetType() const
        {
            return this->mType;
        }

        void SetType(size_t value)
        {
            this->mType = value;
        }

        size_t GetLength()
        {
            switch(this->GetType())
            {
                case DATABASE_DATATYPE_NULL:
                        return 0; //not magic number there is simple nothing here.
                    break;
                case DATABASE_DATATYPE_INTEGER:
                        return 1; //not magic number but there is only one int so it's length is one.
                    break;
                case DATABASE_DATATYPE_TEXT:
                        return this->mValue.length();
                    break;
                case DATABASE_DATATYPE_DOUBLE:
                        return 1; //not magic number but there is only one double so it's length is one.
                    break;
                case DATABASE_DATATYPE_BLOB:
                        return this->mValue.length();
                    break;
				case DATABASE_DATATYPE_UUID:
						return this->mValue.length();
					break;
                default:
                        return 0; //this is invalid.
                    break;
            }
        }

        int GetDirection() const
        {
            return this->mDirection;
        }

        void SetDirection(int value)
        {
            this->mDirection = value;
        }

    };

    //typedef std::vector<DatabaseParameter> DatabaseParameterCollection;
};

#endif // DATABASEPARAMETER_HPP_INCLUDED

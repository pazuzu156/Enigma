/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "DatabaseResultSet.hpp"

namespace Enigma
{

    DatabaseResultSet::DatabaseResultSet()
    {
        this->mHasValue=false;
    }

    DatabaseResultSet::~DatabaseResultSet()
    {

    }

    void DatabaseResultSet::SetColumnCapacity(size_t index)
    {
        this->mColumnNames.resize(index,"");
        this->mColumnTypes.resize(index,0);
    }

    const std::string& DatabaseResultSet::GetColumnName(size_t column) const
    {
        return this->mColumnNames[column];
    }

    void DatabaseResultSet::SetColumnName(size_t column,std::string value)
    {
        this->mColumnNames[column] = value;
    }

    const int DatabaseResultSet::GetColumnType(size_t column) const
    {
        return this->mColumnTypes[column];
    }

    void DatabaseResultSet::SetColumnType(size_t column,int value)
    {
        this->mColumnTypes[column] = value;
    }

    size_t DatabaseResultSet::FindIndex(const std::string& value)
    {
        for(size_t i=0;i<this->mColumnNames.size();i++)
        {
            if(this->mColumnNames[i].compare(value)==0)
            {
                return i;
            }
        }

        return -1;//npos
    }

    size_t DatabaseResultSet::FindIndex(const char* value)
    {
        for(size_t i=0;i<this->mColumnNames.size();i++)
        {
            if(this->mColumnNames[i].compare(value)==0)
            {
                return i;
            }
        }

        return -1;//npos
    }

    std::string DatabaseResultSet::ToString()
    {
        std::string buffer;

        for(size_t i=0;i<this->mColumnNames.size();i++)
        {
            buffer.append(1,'{');
            buffer.append(this->mColumnNames[i]);
            buffer.append(1,'}');
            buffer.append(1,'\t');
        }

        buffer.append(1,'\n');

        if(this->mHasValue)
        {
            for(size_t i=0;i<this->mData.size();i++)
            {
                for(size_t j=0;j<this->mData[i].size();j++)
                {
                    buffer.append(1,'{');
                    buffer.append(this->mData[i][j]);
                    buffer.append(1,'}');
                    buffer.append(1,'\t');
                }
                buffer.append(1,'\n');
            }
        }

        return buffer;
    }

    size_t DatabaseResultSet::GetUpperBound()
    {
        return this->mData.size()-1;
    }
};

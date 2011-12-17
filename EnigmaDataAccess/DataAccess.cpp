/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "DataAccess.hpp"
#include "SQLiteWrapper.hpp"

namespace Enigma
{
    DataAccess::DataAccess()
    {

    }

    DataAccess::~DataAccess()
    {

    }

	DatabaseCommand DataAccess::PopDatabaseCommand(bool& isValid)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mMutex);
		DatabaseCommand result;

		if(!this->mDatabaseCommands.empty())
		{
			result = this->mDatabaseCommands.front();
			this->mDatabaseCommands.pop();
			isValid=true;
		}
		else
		{
			isValid=false;
		}

		return result;
	}

	void DataAccess::PushDatabaseCommand(const DatabaseCommand& value)
	{
		boost::unique_lock< boost::shared_mutex > lock(this->mMutex);
		this->mDatabaseCommands.push(value);
	}

	void DataAccess::Poll()
	{
		bool isValid=false;
		DatabaseCommand databaseCommand;
		databaseCommand = this->PopDatabaseCommand(isValid);
		while(isValid)
		{
			this->ExecuteSql(databaseCommand);
		}
	}

	void DataAccess::Load()
	{
		//data access is stateless other than queue.
	}

	void DataAccess::Unload()
	{
		//data access is stateless other than queue.
	}

	DatabaseResultSet DataAccess::ExecuteSql(DatabaseCommand& command)
	{
        SQLiteWrapper sql("../Db/Enigma.db");
		DatabaseResultSet result;
        result = sql.Execute(command);

		return result;
	}

	void DataAccess::QueueSql(const DatabaseCommand& command)
	{
		this->PushDatabaseCommand(command);
	}

	bool DataAccess::Authenticate(const std::string& username,const std::string& password,size_t& id)
    {
        if(username.empty())
        {
            std::cout << "Username must not be null." << std::endl;
            return false;
        }
        if(password.empty())
        {
            std::cout << "Password must not be null." << std::endl;
            return false;
        }

        return this->Authenticate(username.c_str(),password.c_str(),id);
    }

	bool DataAccess::Authenticate(const char* username,const char* password,size_t& id)
    {
        if(username==NULL || *username == '\0')
        {
            std::cout << "Username must not be null." << std::endl;
            return false;
        }
        if(password==NULL || *password == '\0')
        {
            std::cout << "Password must not be null." << std::endl;
            return false;
        }

        SQLiteWrapper sql("../Db/Enigma.db");
        std::string query = "SELECT Id,Username,Password FROM Users WHERE Username=@Username";
        DatabaseCommand command;
        DatabaseResultSet result;
        std::string dbPassword;

        command.SetQuery(query);
        command.AddParameter("@Username",username);

        result = sql.Execute(command);

        //this is mostly for debugging.
        std::cout << result.ToString() << std::endl;

        if(!result.IsEmpty())
        {
            dbPassword = result.GetValue<std::string>(0,(size_t)2);
            if(dbPassword.compare(password)==0)
            {
                std::cout << password  << " == " << dbPassword << std::endl;
				id = result.GetValue<size_t>(0,(size_t)0);
                return true;
            }
            else
            {
                std::cout << password  << " != " << dbPassword << std::endl;
                return false;
            }
        }
        else
        {
            return false;
        }
    }

	void DataAccess::UpdateCharacterGuild(int characterId,int guildId)
	{
			std::string query = "UPDATE Characters SET GuildId=@GuildId WHERE Id=@Id";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			command.AddParameter("@Id",characterId);
			command.AddParameter("@GuildId",guildId);

			this->QueueSql(command); //Allows for quick return.
	}
};
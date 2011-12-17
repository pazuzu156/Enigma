#ifndef DATAACCESS_HPP_INCLUDED
#define DATAACCESS_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "boost_foreach.hpp"

#include "TypeDefs.hpp"
#include "DatabaseResultSet.hpp"
#include "DatabaseCommand.hpp"

#include "ThreadedManager.hpp"

#include "std_queue.hpp"

namespace Enigma
{
	class DllExport DataAccess : public ThreadedManager
	{

		private:

		boost::shared_mutex mMutex;
		std::queue<DatabaseCommand> mDatabaseCommands;

		protected:

		DatabaseCommand PopDatabaseCommand(bool& isValid);
		void PushDatabaseCommand(const DatabaseCommand& value);

		public:

		DataAccess();
		~DataAccess();

		virtual void Poll();
		virtual void Load();
		virtual void Unload();

		DatabaseResultSet ExecuteSql(DatabaseCommand& command);
		void QueueSql(const DatabaseCommand& command);

        bool Authenticate(const std::string& username,const std::string& password,size_t& id);
        bool Authenticate(const char* username,const char* password,size_t& id);

        //Guilds
		template <class T>
        void CreateGuild(T& value)
		{
			std::string query = "INSERT INTO Guilds (LeaderId,Name,UniqueId) VALUES (@LeaderId,@Name,@UniqueId)";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			command.AddParameter("@LeaderId",value.GetLeaderId());
			command.AddParameter("@Name",value.GetName());
			command.AddParameter("@UniqueId",value.GetUniqueId());

			result = this->ExecuteSql(command);


			std::string query2 = "SELECT Id From Guilds WHERE UniqueId=@UniqueId";
			DatabaseCommand command2;
			DatabaseResultSet result2;

			command2.SetQuery(query2);
			command2.AddParameter("@UniqueId",value.GetUniqueId());
			 
			result2 = this->ExecuteSql(command2);

			if(!result2.IsEmpty())
			{
				value.SetId(result2.GetValue<size_t>(0,(size_t)0));
			}
		}

		template <class T>
        void LoadGuild(T& value,size_t id)
		{
			value.SetId(id);

			this->LoadGuild(value);
		}

		template <class T>
        void LoadGuild(T& value)
		{
			if(value.GetId()==0)
			{
				throw EnigmaException("Can not load a character with an id of 0 from the database!");
			}
			std::string query = "SELECT LeaderId,Name,UniqueId FROM Guilds WHERE Id=@Id";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			command.AddParameter("@Id",value.GetId());

			result = this->ExecuteSql(command);
			if(!result.IsEmpty())
			{
				value.SetLeaderId(result.GetValue<size_t>(0,(size_t)0));
				value.SetName(result.GetValue<std::string>(0,(size_t)1));
				value.SetUniqueId(result.GetValue<boost::uuids::uuid>(0,(size_t)2));
			}
		}

        //Characters

		void UpdateCharacterGuild(int characterId,int guildId);

		template <class T>
        void CreateCharacter(T& value)
		{
			std::string query = "INSERT INTO Characters (UserId,Name,UniqueId) VALUES (@UserId,@Name,@UniqueId)";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			command.AddParameter("@UserId",value.GetUserId());
			command.AddParameter("@Name",value.GetName());
			command.AddParameter("@UniqueId",value.GetUniqueId());

			result = this->ExecuteSql(command);


			std::string query2 = "SELECT Id From Characters WHERE UniqueId=@UniqueId";
			DatabaseCommand command2;
			DatabaseResultSet result2;

			command2.SetQuery(query2);
			command2.AddParameter("@UniqueId",value.GetUniqueId());
			 
			result2 = this->ExecuteSql(command2);

			if(!result2.IsEmpty())
			{
				value.SetId(result2.GetValue<size_t>(0,(size_t)0));
			}
		}

		template <class T>
        void LoadCharacter(T& value,size_t id)
		{
			value.SetId(id);

			this->LoadCharacter(value);
		}

		template <class T>
        void LoadCharacter(DatabaseResultSet& result,size_t index, T& value)
		{
			if(!result.IsEmpty())
			{
				value.SetId(result.GetValue<size_t>(index,(size_t)0));
				value.SetUserId(result.GetValue<size_t>(index,(size_t)1));
				value.SetName(result.GetValue<std::string>(index,(size_t)2));
				value.SetUniqueId(result.GetValue<boost::uuids::uuid>(index,(size_t)3));
				value.SetGuildId(result.GetValue<size_t>(index,(size_t)4));
			}
		}

		template <class T>
        void LoadCharacter(T& value)
		{
			if(value.GetId()==0)
			{
				throw EnigmaException("Can not load a character with an id of 0 from the database!");
			}
			std::string query = "SELECT Id,UserId,Name,UniqueId,GuildId FROM Characters WHERE Id=@Id";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			command.AddParameter("@Id",value.GetId());

			result = this->ExecuteSql(command);

			LoadCharacter(result,0,value);
		}

		template <class T>
        void LoadCharacters(std::vector< T >& values)
		{
			BOOST_FOREACH(T entity, values)
			{
				this->LoadCharacter(entity);
			}
		}

		template <class T>
        void LoadCharactersByUserId(std::vector< T >& values,size_t userId)
		{
			if(userId==0)
			{
				throw EnigmaException("Can not load a character with an userid of 0 from the database!");
			}
			std::string query = "SELECT Id,UserId,Name,UniqueId,GuildId FROM Characters WHERE UserId=@UserId";
			DatabaseCommand command;
			DatabaseResultSet result;
			T value;

			command.SetQuery(query);
			command.AddParameter("@UserId",userId);

			result = this->ExecuteSql(command);
			if(!result.IsEmpty())
			{
				for(size_t i=0;i<=result.GetUpperBound();i++)
				{
					value = T();

					LoadCharacter(result,i,value);

					values.push_back(value);
				}
			}
		}

        //Npcs
		template <class T>
        void LoadNpc(T& value,size_t id)
		{
			value.SetId(id);
			this->LoadNpc(value);
		}

		template <class T>
        void LoadNpc(T& value)
		{
			throw EnigmaException("Not Implemented!");

			std::string query = "";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			//command.AddParameter("@Id",id);

			result = this->ExecuteSql(command);
		}

		template <class T>
        void LoadNpc(std::vector< T >& value)
		{
			BOOST_FOREACH(T entity, value)
			{
				this->LoadNpc(entity);
			}
		}

        //Monsters
		template <class T>
        void LoadMonster(T& value,size_t id)
		{
			value.SetId(id);
			this->LoadMonster(value);
		}

		template <class T>
        void LoadMonster(T& value)
		{
			throw EnigmaException("Not Implemented!");

			std::string query = "";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			//command.AddParameter("@Id",id);

			result = this->ExecuteSql(command);
		}

		template <class T>
        void LoadMonster(std::vector< T >& value)
		{
			BOOST_FOREACH(T entity, value)
			{
				this->LoadMonster(entity);
			}
		}

        //Items
		template <class T>
        void LoadItem(T& value,size_t id)
		{
			value.SetId(id);
			this->LoadItem(value);
		}

		template <class T>
        void LoadItem(T& value)
		{
			throw EnigmaException("Not Implemented!");

			std::string query = "";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			//command.AddParameter("@Id",id);

			result = this->ExecuteSql(command);
		}

		template <class T>
        void LoadItem(std::vector< T >& value)
		{
			BOOST_FOREACH(T entity, value)
			{
				this->LoadItem(entity);
			}
		}

        //Maps
		template <class T>
        void LoadMap(T& value,size_t id)
		{
			value.SetId(id);
			this->LoadMap(value);
		}

		template <class T>
        void LoadMap(T& value)
		{
			throw EnigmaException("Not Implemented!");

			std::string query = "";
			DatabaseCommand command;
			DatabaseResultSet result;
			
			command.SetQuery(query);
			//command.AddParameter("@Id",id);

			result = this->ExecuteSql(command);
		}

		template <class T>
        void LoadMap(std::vector< T >& value)
		{
			BOOST_FOREACH(T entity, value)
			{
				this->LoadMap(entity);
			}
		}
	};
};

#endif // DATABASEACCESS_HPP_INCLUDED
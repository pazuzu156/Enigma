#ifndef ENIGMAEVENT_HPP_INCLUDED
#define ENIGMAEVENT_HPP_INCLUDED

/*
Copyright © 2011  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "TypeDefs.hpp"
#include "string.hpp"
#include "std_vector.hpp"

#include "ChatType.hpp"

namespace Enigma
{
    class DllExport EnigmaEvent
    {
        private:

        protected:

        public:

		static const size_t EVENT_NONE = 0;

		static const size_t EVENT_APPLICATION_EXIT = 1;
		static const size_t EVENT_APPLICATION_GAME_MODE_CHANGED = 2;
		static const size_t EVENT_APPLICATION_LOG = 3;
		static const size_t EVENT_APPLICATION_OPEN_BROWSER_WINDOW = 4;
		static const size_t EVENT_APPLICATION_OPEN_VIDEO_WINDOW = 5;
		static const size_t EVENT_APPLICATION_RECEIVED_CHARACTER_LIST = 6;

		static const size_t EVENT_SCENE_CHARACTER_ENTERED_MAP = 101;
		static const size_t EVENT_SCENE_CHARACTER_UPDATED = 102;
		static const size_t EVENT_SCENE_CHARACTER_EXITED_MAP = 103;

		static const size_t EVENT_SCENE_NPC_ENTERED_MAP = 104;
		static const size_t EVENT_SCENE_NPC_UPDATED = 105;
		static const size_t EVENT_SCENE_NPC_EXITED_MAP = 106;

		static const size_t EVENT_SCENE_MONSTER_ENTERED_MAP = 107;
		static const size_t EVENT_SCENE_MONSTER_UPDATED = 108;
		static const size_t EVENT_SCENE_MONSTER_EXITED_MAP = 109;

		static const size_t EVENT_SCENE_ITEM_ENTERED_MAP = 110;
		static const size_t EVENT_SCENE_ITEM_UPDATED = 111;
		static const size_t EVENT_SCENE_ITEM_EXITED_MAP = 112;

		static const size_t EVENT_SCENE_PLAYER_MOVED = 113;

		static const size_t EVENT_CHAT_INVITED = 201;
		static const size_t EVENT_CHAT_JOINED = 202;
		static const size_t EVENT_CHAT_RANK_MODIFIED = 203;
		static const size_t EVENT_CHAT_MODIFIED = 204;
		static const size_t EVENT_CHAT_EXPELLED = 205;
		static const size_t EVENT_CHAT_RECEIVED_MESSAGE = 206;
		static const size_t EVENT_CHAT_NPC_RESPONSE = 207;

		static const size_t EVENT_AUDIO_VOICE_RECEIVED = 301;
		static const size_t EVENT_AUDIO_RECORDER_STATE_CHANGED = 302;
		static const size_t EVENT_AUDIO_PLAY_FILE = 303;

		//Lets event processor know who needs notified of the event and what information they need.
		size_t EventType;

		//Lets the processor and in some cases the listener know the scope of the event.
		size_t ScopeId;

		//Lets the processor and the listener know what entity has been modified.
		size_t EntityId;

		//Lets the processor and the listener know what map has been modified.
		size_t MapId;

		//Lets the processor know what non-entity data to pass to the listener.
		size_t Index;

		//Holds any textual data associated with the event.
		std::string Text;

		//Holds the name of the player, entity, or process generating the event.
		std::string Sender;

		//Holds a collection of textual data associated with the event.
		std::vector<std::string> Values;

		//Holds other misc information.
		void* UserData;

		EnigmaEvent() : EventType(0), ScopeId(0), EntityId(0), Index(0), UserData(NULL), Sender("System"){};
		EnigmaEvent(const std::string& sender) : EventType(0), ScopeId(0), EntityId(0), Index(0), UserData(NULL), Sender(sender){};
        ~EnigmaEvent(){};

    };
};

#endif // ENIGMAEVENT_HPP_INCLUDED
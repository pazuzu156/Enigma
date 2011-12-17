#ifndef CLIENTCONFIGURATIONMANAGER_HPP_INCLUDED
#define CLIENTCONFIGURATIONMANAGER_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ClientDisplaySettings.hpp"
#include "ServerConnectionInformation.hpp"
#include "std_vector.hpp"

namespace Enigma
{
    class DllExport ClientConfigurationManager
    {
        private:

        std::vector<ServerConnectionInformation> mServers;
        std::vector<ClientDisplaySettings> mClientDisplaySettings;

        std::vector<std::string> mMeshes;
        std::vector<std::string> mCharacterMeshes;
        std::vector<std::string> mNpcMeshes;
        std::vector<std::string> mItemMeshes;
        std::vector<std::string> mItemIcons;
        std::vector<std::string> mMonsterMeshes;

        std::vector<std::string> mMaps;
        std::vector<std::string> mMiniMaps;
        std::vector<std::string> mLoadingScreens;

        protected:

        public:

        ClientConfigurationManager();
        ~ClientConfigurationManager();

        std::vector<ServerConnectionInformation>& GetServers();
        ServerConnectionInformation& GetDefaultServer();

        std::vector<ClientDisplaySettings>& GetClientDisplaySettings();
        ClientDisplaySettings& GetDefaultDisplaySettings();

        std::vector<std::string>& GetMeshes();
        std::string& GetDefaultMesh();

        std::vector<std::string>& GetCharacterMeshes();
        std::string& GetDefaultCharacterMesh();

        std::vector<std::string>& GetNpcMeshes();
        std::string& GetDefaultNpcMesh();

        std::vector<std::string>& GetItemMeshes();
        std::string& GetDefaultItemMesh();

        std::vector<std::string>& GetItemIcons();
        std::string& GetDefaultItemIcon();

        std::vector<std::string>& GetMonsterMeshes();
        std::string& GetDefaultMonsterMesh();

        std::vector<std::string>& GetMaps();
        std::string& GetDefaultMap();

        std::vector<std::string>& GetMiniMaps();
        std::string& GetDefaultMiniMap();

        std::vector<std::string>& GetLoadingScreens();
        std::string& GetDefaultLoadingScreen();
        std::string& GetRandomLoadingScreen();

        const std::string& GetGlossaryEntry(const std::string& name);
        const std::string& GetGlossaryEntry(const Enigma::c8* name);

        const std::string& GetUrl(const std::string& name);
        const std::string& GetUrl(const Enigma::c8* name);

        bool Validate(ServerConnectionInformation& server);
        bool Validate(ClientDisplaySettings& server);

        void Load();
        bool Load(const std::string& filename);
        bool Load(const Enigma::c8* filename);
		void Load(void* xml); //This is to keep the xml stuff from being pulled into other projects.
    };
};

#endif // CLIENTCONFIGURATIONMANAGER_HPP_INCLUDED

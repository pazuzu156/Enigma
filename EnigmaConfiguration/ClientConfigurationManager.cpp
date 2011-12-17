/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "ClientConfigurationManager.hpp"
#include "irrXML.h"
#include "string.hpp"
#include "iostream.hpp"
#include "time.hpp"

using namespace irr;
using namespace io;

namespace Enigma
{
    ClientConfigurationManager::ClientConfigurationManager()
    {
		this->Load();
    }

    ClientConfigurationManager::~ClientConfigurationManager()
    {

    }

    std::vector<ServerConnectionInformation>& ClientConfigurationManager::GetServers()
    {
        return this->mServers;
    }

    ServerConnectionInformation& ClientConfigurationManager::GetDefaultServer()
    {
        return this->mServers[0];
    }

    std::vector<ClientDisplaySettings>& ClientConfigurationManager::GetClientDisplaySettings()
    {
        return this->mClientDisplaySettings;
    }

    ClientDisplaySettings& ClientConfigurationManager::GetDefaultDisplaySettings()
    {
        return this->mClientDisplaySettings[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetMeshes()
    {
        return this->mMeshes;
    }

    std::string& ClientConfigurationManager::GetDefaultMesh()
    {
        return this->mMeshes[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetCharacterMeshes()
    {
        return this->mCharacterMeshes;
    }

    std::string& ClientConfigurationManager::GetDefaultCharacterMesh()
    {
        return this->mCharacterMeshes[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetNpcMeshes()
    {
        return this->mNpcMeshes;
    }

    std::string& ClientConfigurationManager::GetDefaultNpcMesh()
    {
        return this->mNpcMeshes[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetItemMeshes()
    {
        return this->mItemMeshes;
    }

    std::string& ClientConfigurationManager::GetDefaultItemMesh()
    {
        return this->mItemMeshes[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetItemIcons()
    {
        return this->mItemIcons;
    }

    std::string& ClientConfigurationManager::GetDefaultItemIcon()
    {
        return this->mItemIcons[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetMonsterMeshes()
    {
        return this->mMonsterMeshes;
    }

    std::string& ClientConfigurationManager::GetDefaultMonsterMesh()
    {
        return this->mMonsterMeshes[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetMaps()
    {
        return this->mMaps;
    }

    std::string& ClientConfigurationManager::GetDefaultMap()
    {
        return this->mMaps[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetMiniMaps()
    {
        return this->mMiniMaps;
    }

    std::string& ClientConfigurationManager::GetDefaultMiniMap()
    {
        return this->mMiniMaps[0];
    }

    std::vector<std::string>& ClientConfigurationManager::GetLoadingScreens()
    {
        return this->mLoadingScreens;
    }

    std::string& ClientConfigurationManager::GetDefaultLoadingScreen()
    {
        return this->mLoadingScreens[0];
    }

    std::string& ClientConfigurationManager::GetRandomLoadingScreen()
    {
        size_t index;
        srand((unsigned int)time(NULL));
        index = rand() % (this->mLoadingScreens.size()-1);

        if(index>=0 && index<this->mLoadingScreens.size())
        {
            return this->mLoadingScreens[index];
        }
        else
        {
            std::cout << "Incorrect rand usage." << std::endl;
            return this->GetDefaultLoadingScreen();
        }
    }

    const std::string& ClientConfigurationManager::GetGlossaryEntry(const std::string& name)
    {
        return this->GetGlossaryEntry(name.c_str());
    }

    const std::string& ClientConfigurationManager::GetGlossaryEntry(const Enigma::c8* name)
    {
        static std::string temp = std::string("Entry not found.");

        #ifdef ENIGMA_PLATFORM_WINDOWS
        IrrXMLReader* xml = createIrrXMLReader("..\\Configuration\\Glossary.cfg");
        #else
        IrrXMLReader* xml = createIrrXMLReader("../Configuration/Glossary.cfg");
        #endif

        if(xml!=NULL)
        {
            while(xml->read())
            {
                if(strcmp(xml->getNodeName(),"GlossaryEntry")==0)
                {
                    if(strcmp(xml->getAttributeValueSafe("Name"),name)==0)
                    {
                        temp = std::string(xml->getAttributeValueSafe("Description"));
                        break;
                    }
                }
            }
            delete xml;
        }
        else
        {
            temp = std::string("Glossary is corrupt or does not exist.");
        }

        return temp;
    }

    const std::string& ClientConfigurationManager::GetUrl(const std::string& name)
    {
        return this->GetUrl(name.c_str());
    }

    const std::string& ClientConfigurationManager::GetUrl(const Enigma::c8* name)
    {
        static std::string temp = std::string("about:blank");

        #ifdef ENIGMA_PLATFORM_WINDOWS
        IrrXMLReader* xml = createIrrXMLReader("..\\Configuration\\Url.cfg");
        #else
        IrrXMLReader* xml = createIrrXMLReader("../Configuration/Url.cfg");
        #endif

        if(xml!=NULL)
        {
            while(xml->read())
            {
                if(strcmp(xml->getNodeName(),"Url")==0)
                {
                    if(strcmp(xml->getAttributeValueSafe("Name"),name)==0)
                    {
                        temp = std::string(xml->getAttributeValueSafe("Value"));
                        break;
                    }
                }
            }
            delete xml;
        }
        else
        {
            temp = std::string("about:blank");
        }

        return temp;
    }

    bool ClientConfigurationManager::Validate(ServerConnectionInformation& server)
    {
        if(server.Name.length()>0 && server.Host.length()>0 && server.Port>0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ClientConfigurationManager::Validate(ClientDisplaySettings& display)
    {
        if(!display.Name.empty()
		&& !display.Renderer.empty()
        && display.Width>0
        && display.Height>0
        && display.BitsPerPixel>0
        && display.BitsPerPixel<=32)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void ClientConfigurationManager::Load()
    {
        #ifdef ENIGMA_PLATFORM_WINDOWS
        this->Load("..\\Configuration\\Client.cfg");
        #else
        this->Load("../Configuration/Client.cfg");
        #endif
    }

    bool ClientConfigurationManager::Load(const std::string& filename)
    {
        if(filename.length()>0)
        {
            return this->Load(filename.c_str());
        }
        else
        {
            return false;
        }
    }

    bool ClientConfigurationManager::Load(const Enigma::c8* filename)
    {
        if(filename!=NULL)
        {
            IrrXMLReader* xml = createIrrXMLReader(filename);
            if(xml!=NULL)
            {
                this->Load(xml);
                delete xml;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

	void ClientConfigurationManager::Load(void* value)
    {
        std::string configName;
		IrrXMLReader* xml = (IrrXMLReader*)value;

        if(xml!=NULL)
        {
            while(xml->read())
            {
                if(strcmp(xml->getNodeName(),"Configuration")==0)
                {
                    if(this->Load(xml->getAttributeValueSafe("Filename")))
                    {
                        configName = xml->getAttributeValueSafe("Name");
                        std::cout << configName << " configuration loaded." << std::endl;
                        if(strcmp(configName.c_str(),"Loading Screen")==0)
                        {
                            std::cout << configName << " configuration contains " << this->mLoadingScreens.size() << " entries." << std::endl;
                        }
                        else
                        if(strcmp(configName.c_str(),"Server")==0)
                        {
                            std::cout << configName << " configuration contains " << this->mServers.size() << " entries." << std::endl;
                        }
                        else
                        if(strcmp(configName.c_str(),"Display")==0)
                        {
                            std::cout << configName << " configuration contains " << this->mClientDisplaySettings.size() << " entries." << std::endl;
                        }
                        else
                        if(strcmp(configName.c_str(),"Map")==0)
                        {
                            std::cout << configName << " configuration contains " << this->mMaps.size() << " entries." << std::endl;
                        }
                    }
                }
                else if(strcmp(xml->getNodeName(),"Server")==0)
                {
                    ServerConnectionInformation server;
                    server.Name = xml->getAttributeValueSafe("Name");
                    server.Host = xml->getAttributeValueSafe("Host");
                    server.Port = xml->getAttributeValueAsInt("Port");
                    if(Validate(server))
                    {
                        this->mServers.push_back(server);
                        std::cout << "Server: " << server.Name << '\t' << server.Host << ':' << server.Port << std::endl;
                    }
                }
                else if(strcmp(xml->getNodeName(),"Display")==0)
                {
                    ClientDisplaySettings display;
                    display.Name = xml->getAttributeValueSafe("Name");
                    display.Width = xml->getAttributeValueAsInt("Width");
                    display.Height = xml->getAttributeValueAsInt("Height");
                    display.BitsPerPixel = xml->getAttributeValueAsInt("BitsPerPixel");
                    display.FullScreen = xml->getAttributeValueAsInt("FullScreen");
                    display.VSync = xml->getAttributeValueAsInt("VSync");
					display.Renderer = xml->getAttributeValueSafe("Renderer");

                    if(Validate(display))
                    {
                        this->mClientDisplaySettings.push_back(display);
                        std::cout << "Display: " << display.Name << '\t' << display.Width << 'x' << display.Height << 'x' << display.BitsPerPixel << " FullScreen:" << display.FullScreen << " VSync:" << display.VSync << std::endl;
                    }
                }
                else if(strcmp(xml->getNodeName(),"Mesh")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mMeshes.push_back(temp);
                    }
                }
                else if(strcmp(xml->getNodeName(),"CharacterMesh")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mCharacterMeshes.push_back(temp);
                    }
                }
                else if(strcmp(xml->getNodeName(),"NpcMesh")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mNpcMeshes.push_back(temp);
                    }
                }
                else if(strcmp(xml->getNodeName(),"ItemMesh")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mItemMeshes.push_back(temp);
                        temp = xml->getAttributeValueSafe("IconFilename");
                        if(temp.length()>0)
                        {
                            this->mItemIcons.push_back(temp);
                        }
                        else
                        {
                            temp="unknown.png";
                            this->mItemIcons.push_back(temp);
                        }
                    }


                }
                else if(strcmp(xml->getNodeName(),"MonsterMesh")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mMonsterMeshes.push_back(temp);
                    }
                }
                else if(strcmp(xml->getNodeName(),"Map")==0)
                {
                    std::string mapFilename;
                    std::string miniMapFilename;
                    mapFilename = xml->getAttributeValueSafe("Filename");
                    if(mapFilename.length()>0)
                    {
                        this->mMaps.push_back(mapFilename);
                        miniMapFilename = xml->getAttributeValueSafe("MiniMap");
                        if(miniMapFilename.length()>0)
                        {
                            this->mMiniMaps.push_back(miniMapFilename);
                        }
                        else
                        {
                            miniMapFilename = "unknown.jpg";
                            this->mMiniMaps.push_back(miniMapFilename);
                        }
                    }
                }
                else if(strcmp(xml->getNodeName(),"LoadingScreen")==0)
                {
                    std::string temp;
                    temp = xml->getAttributeValueSafe("Filename");
                    if(temp.length()>0)
                    {
                        this->mLoadingScreens.push_back(temp);
                    }
                    else
                    {
                        temp = "SimpleBlueGreen.bmp";
                        this->mLoadingScreens.push_back(temp);
                    }
                }
                else if(0)
                {

                }
            }
        }
    }
};

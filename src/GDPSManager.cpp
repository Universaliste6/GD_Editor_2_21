#include <fstream>
#include <string>
#include <sstream>
#include "hooking.h"
#include "cocos2d.h"


#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

//#include <gd.h>
#include <CCFileUtils.h>
#include "GDPSManager.h"

#include "json.hpp"
#include <cstdio>

using namespace std;
using namespace cocos2d;



GDPSManager *GDPSManager::sharedState()
{
	static GDPSManager* s_sharedGDPSManager = nullptr;
    if (!s_sharedGDPSManager)
    {
        s_sharedGDPSManager = new GDPSManager();
        s_sharedGDPSManager->init();
    }
    return s_sharedGDPSManager;
}



void GDPSManager::encodeDataTo()
{
	RSJresource jsonData("{}");
	jsonData["opacityLeft"] = this->opacityLeft;
	jsonData["opacityRight"] = this->opacityRight;
	jsonData["password"] = this->password;
	jsonData["newsCount"] = this->newsCount;
	this->jsonObject = jsonData;
}



void GDPSManager::dataLoaded()
{
	this->opacityRight = this->jsonObject["opacityRight"].as<int>(255);
	this->opacityLeft = this->jsonObject["opacityLeft"].as<int>(255);
	this->password = this->jsonObject["password"].as<std::string>("0");
	this->newsCount = this->jsonObject["newsCount"].as<int>(0);
}

void GDPSManager::firstLoad()
{
	this->opacityLeft = 255;
	this->opacityRight = 255;
	this->password = "0";
	this->newsCount = 0;
}

bool GDPSManager::init()
{
     this->m_sFileName = "GDPSManager.json";
     this->setup();
     return true;
}

void GDPSManager::writeToFile(const string& path, const char* content) 
{
	 ofstream outputFile;
     outputFile.open(path.c_str());
	 outputFile << content;
	 outputFile.close();
}


void GDPSManager::save()
{
	 this->encodeDataTo();
	 this->writeToFile(CCFileUtils::sharedFileUtils()->getWritablePath() + this->m_sFileName, jsonObject.as_str().c_str());
	// CCLog(jsonObject.as_str().c_str());
}

void GDPSManager::setup()
{
    auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + this->m_sFileName;
    std::ifstream infile(path.c_str());
    if (infile.good())
    {
        this->load();
	//	CCLog("file good");
    }
    else
    {
		this->jsonObject = RSJresource("{}");
        this->firstLoad();
	//	CCLog("file doesnt exist");
    }
}

void GDPSManager::load()
{
	 auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;
	 std::ifstream ifs(path);
	 
	 this->jsonObject = RSJresource(ifs);
	 //CCLog(jsonObject.as_str().c_str());
	 this->dataLoaded();
}

FUNCTIONHOOK(void*, trySaveGame, void* self, bool something) {
	GDPS->save();
	return trySaveGameO(self, something);
}

void GDPSManager::ApplyHooks() {
	HOOK("_ZN11AppDelegate11trySaveGameEb", trySaveGameH, trySaveGameO);
	
}
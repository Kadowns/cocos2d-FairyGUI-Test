/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "Network/SimplePadsGLoader.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace fairygui;
using namespace network;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    m_groot = GRoot::create(this);
    m_groot->retain();

    UIObjectFactory::setLoaderExtension([](){
        return SimplePadsGLoader::create();
    });

    UIPackage::addPackage("ui/Menu");
    m_view = UIPackage::createObject("Menu", "Main")->as<GComponent>();
    m_groot->addChild(m_view);



    //download do json
    auto request = new(std::nothrow)HttpRequest();
    request->setUrl("http://api.kondzilla.opalastudios.com/api/fetch?version=4");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback([this](HttpClient* sender, HttpResponse* response){
        if (!response){
            return;
        }

        const char* bytes = response->getResponseData()->data();
        rapidjson::Document document;
        document.Parse(bytes);

        auto kits = document["kits"].GetArray();
        m_kitsData.reserve(kits.Size());
        for (auto &kit : kits) {
            m_kitsData.push_back(KitData::fromJson(kit));
        }

        m_kitList = m_view->getChild("kitList")->as<GList>();
        m_kitList->setVirtual();
        m_kitList->itemRenderer = CC_CALLBACK_2(HelloWorld::renderListItem, this);
        m_kitList->setNumItems(m_kitsData.size());

    });
    HttpClient::getInstance()->sendImmediate(request);
    request->release();

    return true;
}

fairygui::GComponent *
HelloWorld::createKitSlot(const std::string &imageFile, const std::string &musicName, const std::string &artistNames) {
    auto kit = UIPackage::createObject("Menu", "kitSlot")->as<GComponent>();
    auto loader = kit->getChild("loader")->as<SimplePadsGLoader>();
    loader->setURL(imageFile);

    auto musicNameText = kit->getChild("musicName")->as<GTextField>();
    musicNameText->setText(musicName);

    auto artistNameText = kit->getChild("artistName")->as<GTextField>();
    artistNameText->setText(artistNames);
    return kit;
}

fairygui::GComponent *HelloWorld::createKitSlot(const KitData &kitData) {
    return createKitSlot(kitData.imageUrl, kitData.musicName, kitData.authorName);
}

void HelloWorld::renderListItem(int index, fairygui::GObject *obj) {
    auto& kitData = m_kitsData[index];
    auto kitSlot = obj->as<GComponent>();
    auto loader = kitSlot->getChild("loader")->as<SimplePadsGLoader>();
    loader->setURL(kitData.imageUrl);

    auto musicNameText = kitSlot->getChild("musicName")->as<GTextField>();
    musicNameText->setText(kitData.musicName);

    auto artistNameText = kitSlot->getChild("artistName")->as<GTextField>();
    artistNameText->setText(kitData.authorName);
}

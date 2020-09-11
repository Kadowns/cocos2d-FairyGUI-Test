//
// Created by Novak on 10/09/2020.
//

#ifndef PROJ_ANDROID_SIMPLEPADSGLOADER_H
#define PROJ_ANDROID_SIMPLEPADSGLOADER_H

#include "./SimplePadsGlobalDefinitions.h"

class SimplePadsGLoader : public fairygui::GLoader {
public:
    CREATE_FUNC(SimplePadsGLoader);
    SimplePadsGLoader();
protected:
    virtual void loadExternal() override;

private:
    cocos2d::network::Downloader m_downloader;
    std::shared_ptr<const cocos2d::network::DownloadTask> m_currentTask;
};



#endif //PROJ_ANDROID_SIMPLEPADSGLOADER_H

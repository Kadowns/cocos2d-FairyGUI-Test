//
// Created by Novak on 10/09/2020.
//



#include "SimplePadsGLoader.h"

USING_NS_CC;
using namespace network;

SimplePadsGLoader::SimplePadsGLoader() : GLoader() {
    m_downloader.onFileTaskSuccess = [this](const DownloadTask& task){
        setURL(task.storagePath);
    };
    m_downloader.onTaskError = [this](const DownloadTask& task,
                                      int errorCode,
                                      int errorCodeInternal,
                                      const std::string& errorStr){
        onExternalLoadFailed();
    };

    m_downloader.onTaskProgress = [](const DownloadTask& task,
                                     int64_t bytesReceived,
                                     int64_t totalBytesReceived,

                                     int64_t totalBytesExpected){
        printf("Progress %lli / %lli", totalBytesReceived, totalBytesExpected);
    };
}

void SimplePadsGLoader::loadExternal() {

    auto url = getURL();
    auto fileUtils = FileUtils::getInstance();

    bool isHttpURL = url.compare(0, 4, "http") == 0;

    if (isHttpURL){
        auto filename = url.substr(url.find_last_of("/\\") + 1);
        auto filepath = fileUtils->getWritablePath() + filename;
        bool fileExists = fileUtils->isFileExist(filepath);
        if (fileExists){
            setURL(filepath);
            return;
        } else {
            m_currentTask = m_downloader.createDownloadFileTask(url, filepath, filename);
        }
    } else {
        GLoader::loadExternal();
    }
}




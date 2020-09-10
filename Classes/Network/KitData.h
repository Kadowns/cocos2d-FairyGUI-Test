//
// Created by Novak on 04/09/2020.
//

#ifndef MYCPPGAME_KITDATA_H
#define MYCPPGAME_KITDATA_H

#include <iostream>
#include <json/document-wrapper.h>

struct KitData {
    std::string name;
    std::string authorName;
    std::string musicName;
    std::string imageUrl;

    KitData(){}
    KitData(KitData&& other) :
        name(std::move(other.name)),
        authorName(std::move(other.authorName)),
        musicName(std::move(other.musicName)),
        imageUrl(std::move(other.imageUrl))
        {}

    static KitData fromJson(const rapidjson::Value& object){
        KitData kit;
        assert(object.HasMember("name"));
        assert(object["name"].IsString());
        kit.name = object["name"].GetString();

        assert(object.HasMember("authorName"));
        assert(object["authorName"].IsString());
        kit.authorName = object["authorName"].GetString();

        assert(object.HasMember("musicName"));
        assert(object["musicName"].IsString());
        kit.musicName = object["musicName"].GetString();

        assert(object.HasMember("imageUrl"));
        assert(object["imageUrl"].IsString());
        kit.imageUrl = object["imageUrl"].GetString();
        return kit;
    }
};

#endif //MYCPPGAME_KITDATA_H

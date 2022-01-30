#include "Parsing.hpp"
using namespace Diglett;

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "rapidxml/rapidxml.hpp"

#include "main.hpp"

#include <utility>

std::map<std::string, std::u16string> Parsing::ParseJson(rapidjson::MemoryStream memoryStream) {
    rapidjson::Document document;
    document.ParseStream(memoryStream);

    auto map = std::map<std::string, std::u16string>();

    for (auto &obj : document.GetObject()) {
        getLogger().info("Adding locale to map - %s: %s", obj.name.GetString(), obj.value.GetString());
        map.insert(std::pair<std::string, std::u16string>(obj.name.GetString(), to_utf16(obj.value.GetString())));
    }

    return map;
}

std::map<std::string, std::u16string> Parsing::ParseXml(char *xml) {
    rapidxml::xml_document<> document;
    document.parse<0>(xml);

    auto map = std::map<std::string,std::u16string>();
    auto resources = document.first_node("resources");

    for (rapidxml::xml_node<> *node = resources->first_node("string"); node; node = node->next_sibling("string")) {
        getLogger().info("Adding locale to map - %s: %s", node->first_attribute("name")->value(), node->value());
        map.insert(std::pair<std::string, std::u16string>(node->first_attribute("name")->value(), to_utf16(node->value())));
    }

    return map;
}


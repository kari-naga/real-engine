#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "rapidjson/document.h"

#include "Actor.h"
#include "EngineUtils.h"

class TemplateDB {
  public:
    static inline auto LoadTemplate(const char *template_name) -> Actor {
        if (const auto template_it = loaded_templates.find(template_name); template_it != loaded_templates.end()) {
            return template_it->second.Copy();
        }
        auto template_file = "core/actor_templates/" + std::string(template_name) + ".template";
        if (!std::filesystem::exists(template_file)) {
            template_file = "resources/actor_templates/" + std::string(template_name) + ".template";
            if (!std::filesystem::exists(template_file)) {
                std::cout << "error: template " << template_name << " is missing";
                exit(0);
            }
        }
        auto tmpl = Actor{};
        auto doc = rapidjson::Document();
        EngineUtils::ReadJsonFile(template_file, doc);
        if (doc.IsObject()) {
            tmpl.ParseActor(doc);
        }
        loaded_templates.insert({template_name, tmpl});
        return tmpl.Copy();
    }

  private:
    static inline std::unordered_map<std::string, Actor> loaded_templates;
};

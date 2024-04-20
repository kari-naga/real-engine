#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "rapidjson/document.h"
#include "ldtk.hpp"

#include "Actor.h"
#include "EngineUtils.h"
#include "Scene.h"
#include "TemplateDB.h"

class SceneDB {
  public:
    static inline auto LoadScene(const std::string &scene_name) -> Scene {
        if (const auto scene_it = loaded_scenes.find(scene_name); scene_it != loaded_scenes.end()) {
            return CopyScene(scene_it->second);
        }
        const auto scene_file = "resources/scenes/" + scene_name + ".scene";
        if (!std::filesystem::exists(scene_file)) {
            std::cout << "error: scene " << scene_name << " is missing";
            exit(0);
        }
        auto scene = Scene{};
        scene.name = scene_name;
        auto doc = rapidjson::Document();
        EngineUtils::ReadJsonFile(scene_file, doc);
        if (const auto actors_it = doc.FindMember("actors"); actors_it != doc.MemberEnd() && actors_it->value.IsArray()) {
            const auto &arr = actors_it->value.GetArray();
            scene.actor_store.resize(arr.Size());
            auto i = 0U;
            for (const auto &val : arr) {
                auto &actor = scene.actor_store[i];
                if (const auto template_it = val.FindMember("template"); template_it != val.MemberEnd() && template_it->value.IsString()) {
                    actor = TemplateDB::LoadTemplate(template_it->value.GetString());
                }
                actor.ParseActor(val);
                ++i;
            }
        }
        loaded_scenes.insert({scene_name, scene});
        return CopyScene(scene);
    }

    static inline auto LoadWorld(const std::string &world_name) {
        const auto world_file = "resources/worlds/" + world_name + ".ldtk";
        if (!std::filesystem::exists(world_file)) {
            std::cout << "error: world " << world_file << " is missing";
            exit(0);
        }
        auto world_handle = std::ifstream();
        world_handle.open(world_file);
        ldtk::LdtkJson data = nlohmann::json::parse(world_handle);
        auto tilesets = std::unordered_map<int64_t, std::string>();
        for (const auto &tileset : data.get_defs().get_tilesets()) {
            tilesets[tileset.get_uid()] = tileset.get_identifier();
        }
        for (const auto &level : data.get_levels()) {
            const auto scene_name = level.get_identifier();
            const auto layers = level.get_layer_instances();
            if (layers) {
                auto num_actors = size_t{0};
                for (const auto &layer : layers.value()) {
                    if (layer.get_type() == "Entities") {
                        num_actors += layer.get_entity_instances().size();
                    } else if (layer.get_type() == "Tiles") {
                        num_actors += layer.get_grid_tiles().size();
                    } else if (layer.get_type() == "AutoLayer" || layer.get_type() == "IntGrid") {
                        num_actors += layer.get_auto_layer_tiles().size();
                    }
                }
                if (num_actors > 0) {
                    auto scene = Scene{};
                    scene.name = scene_name;
                    scene.actor_store.resize(num_actors);
                    auto layer_number = 0;
                    auto i = 0U;
                    const auto bg_prefix = std::string("_bg");
                    for (const auto &layer : layers.value()) {
                        const auto &identifier = layer.get_identifier();
                        const auto opacity = layer.get_opacity();
                        if (layer.get_type() == "Entities") {
                            for (const auto &entity : layer.get_entity_instances()) {
                                auto &actor = scene.actor_store[i];
                                actor.ParseLdtkEntity(layer_number, tilesets, opacity, entity);
                                ++i;
                            }
                        } else if (layer.get_type() == "Tiles") {
                            const auto is_bg = identifier.size() >= 2 && identifier.compare(identifier.length() - bg_prefix.length(), bg_prefix.length(), bg_prefix) == 0;
                            const auto tileset = tilesets[layer.get_tileset_def_uid().value()];
                            const auto grid_size = layer.get_grid_size();
                            const auto start_idx = i;
                            for (const auto &tile : layer.get_grid_tiles()) {
                                auto &actor = scene.actor_store[i];
                                const auto name = "__tile_" + identifier + "_" + std::to_string(start_idx - i);
                                actor.ParseLdtkTile(layer_number, tileset, opacity, grid_size, is_bg, tile, name);
                                ++i;
                            }
                        } else if ((layer.get_type() == "AutoLayer" || layer.get_type() == "IntGrid") && layer.get_auto_layer_tiles().size() > 0) {
                            const auto is_bg = identifier.size() >= 2 && identifier.compare(identifier.length() - bg_prefix.length(), bg_prefix.length(), bg_prefix) == 0;
                            const auto tileset = tilesets[layer.get_tileset_def_uid().value()];
                            const auto grid_size = layer.get_grid_size();
                            const auto start_idx = i;
                            for (const auto &tile : layer.get_auto_layer_tiles()) {
                                auto &actor = scene.actor_store[i];
                                const auto name = "__tile_" + identifier + "_" + std::to_string(start_idx - i);
                                actor.ParseLdtkTile(layer_number, tileset, opacity, grid_size, is_bg, tile, name);
                                ++i;
                            }
                        }
                        ++layer_number;
                    }
                    loaded_scenes.insert({scene_name, scene});
                }
            }
        }
    }

  private:
    static inline std::unordered_map<std::string, Scene> loaded_scenes;

    static inline auto CopyScene(const Scene &original) -> Scene {
        auto scene = Scene{};
        scene.name = original.name;
        for (const auto &actor : original.actor_store) {
            scene.actor_store.push_back(actor.Copy());
        }
        return scene; // return copy of actors
    }
};

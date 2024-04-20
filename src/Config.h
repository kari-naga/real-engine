#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "rapidjson/document.h"

#include "Actor.h"
#include "DocUtils.h"

class Config {
  public:
    std::string world_name;
    std::string initial_scene_name;
    std::string game_title;
    int window_width = 640;
    int window_height = 360;
    int clear_color_r = 255;
    int clear_color_g = 255;
    int clear_color_b = 255;
    float zoom_factor = 1;
    int pixels_per_meter = 32;
    float default_tile_friction = 0.3f;
    float default_tile_bounciness = 0.3f;
    glm::vec2 initial_camera_position;
    Uint32 min_milliseconds_between_frames = 16;

    inline auto ParseGameConfig(const rapidjson::Document &doc) -> void {
        world_name = DocUtils::GetString(doc, "world_name").value_or("");
        initial_scene_name = DocUtils::GetString(doc, "initial_scene").value();
        game_title = DocUtils::GetString(doc, "game_title").value_or("");
        default_tile_friction = DocUtils::GetFloat(doc, "default_tile_friction").value_or(0.3f);
        default_tile_bounciness = DocUtils::GetFloat(doc, "default_tile_bounciness").value_or(0.3f);
    }

    inline auto ParseRenderingConfig(const rapidjson::Document &doc) -> void {
        window_width = DocUtils::GetInt(doc, "x_resolution").value_or(640);
        window_height = DocUtils::GetInt(doc, "y_resolution").value_or(360);
        clear_color_r = DocUtils::GetInt(doc, "clear_color_r").value_or(255);
        clear_color_g = DocUtils::GetInt(doc, "clear_color_g").value_or(255);
        clear_color_b = DocUtils::GetInt(doc, "clear_color_b").value_or(255);
        zoom_factor = DocUtils::GetFloat(doc, "zoom_factor").value_or(1.f);
        pixels_per_meter = DocUtils::GetInt(doc, "pixels_per_meter").value_or(32);
        initial_camera_position.x = DocUtils::GetFloat(doc, "initial_camera_x").value_or(window_width / 2.f / pixels_per_meter);
        initial_camera_position.y = DocUtils::GetFloat(doc, "initial_camera_y").value_or(window_height / 2.f / pixels_per_meter);
        const auto requested_max_frame_rate = DocUtils::GetInt(doc, "max_frame_rate").value_or(60);
        if (requested_max_frame_rate > 0) {
            min_milliseconds_between_frames = 1000 / requested_max_frame_rate;
        } else {
            min_milliseconds_between_frames = 1;
        }
    }
};
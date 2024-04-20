#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

#include "box2d/box2d.h"
#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class EngineUtils {
public:
    static inline auto ReadJsonFile(const std::string &path, rapidjson::Document &out_document) -> void {
        FILE *file_pointer = nullptr;
#ifdef _WIN32
        fopen_s(&file_pointer, path.c_str(), "rb");
#else
        file_pointer = fopen(path.c_str(), "rb");
#endif
        char buffer[65536];
        rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
        out_document.ParseStream(stream);
        std::fclose(file_pointer);

        if (out_document.HasParseError()) {
            rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
            std::cout << "error parsing json at [" << path << "]" << std::endl;
            exit(0);
        }
    }

    static inline auto ObtainWordAfterPhrase(const std::string &input, const std::string &phrase) -> std::string {
        auto pos = input.find(phrase);
        if (pos == std::string::npos) {
            return "";
        }
        pos += phrase.length();
        while (pos < input.size() && std::isspace(input.at(pos))) {
            ++pos;
        }
        if (pos == input.size()) {
            return "";
        }
        auto endPos = pos;
        while (endPos < input.size() && !std::isspace(input.at(endPos))) {
            ++endPos;
        }
        return input.substr(pos, endPos - pos);
    }

    static inline auto PackIVec(const glm::ivec2 &ivec) -> uint64_t {
        return static_cast<uint64_t>(ivec.x) << 32 | static_cast<uint32_t>(ivec.y);
    }

    static inline auto PackVec(const glm::vec2 &vec) -> uint64_t {
        return static_cast<uint64_t>(*reinterpret_cast<const uint32_t *>(&vec.x)) << 32 | *reinterpret_cast<const uint32_t *>(&vec.y);
    }

    static inline auto DegToRad(const float deg) -> float {
        return deg * (b2_pi / 180.0f);
    }

    static inline auto RadToDeg(const float rad) -> float {
        return rad * (180.0f / b2_pi);
    }
};

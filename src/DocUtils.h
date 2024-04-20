#pragma once

#include <optional>
#include <string>
#include <vector>

#include "rapidjson/document.h"

class DocUtils {
public:
    static inline auto GetString(const rapidjson::Value &val, const char *key) -> std::optional<std::string> {
        if (val.IsObject()) {
            if (const auto it = val.FindMember(key); it != val.MemberEnd() && it->value.IsString()) {
                return it->value.GetString();
            }
        }
        return std::nullopt;
    }

    static inline auto GetInt(const rapidjson::Value &val, const char *key) -> std::optional<int> {
        if (val.IsObject()) {
            if (const auto it = val.FindMember(key); it != val.MemberEnd() && it->value.IsInt()) {
                return it->value.GetInt();
            }
        }
        return std::nullopt;
    }

    static inline auto GetFloat(const rapidjson::Value &val, const char *key) -> std::optional<float> {
        if (val.IsObject()) {
            if (const auto it = val.FindMember(key); it != val.MemberEnd()) {
                if (it->value.IsFloat()) {
                    return it->value.GetFloat();
                } else if (it->value.IsInt()) {
                    return static_cast<float>(it->value.GetInt());
                }
            }
        }
        return std::nullopt;
    }

    static inline auto GetBool(const rapidjson::Value &val, const char *key) -> std::optional<bool> {
        if (val.IsObject()) {
            if (const auto it = val.FindMember(key); it != val.MemberEnd() && it->value.IsBool()) {
                return it->value.GetBool();
            }
        }
        return std::nullopt;
    }

    static inline auto GetStringArray(const rapidjson::Value &val, const char *key) -> std::optional<std::vector<std::string>> {
        if (val.IsObject()) {
            if (const auto it = val.FindMember(key); it != val.MemberEnd() && it->value.IsArray()) {
                const auto arr = it->value.GetArray();
                auto vec = std::vector<std::string>();
                for (const auto &item : arr) {
                    vec.emplace_back(item.GetString());
                }
                if (!vec.empty()) {
                    return vec;
                }
            }
        }
        return std::nullopt;
    }
};

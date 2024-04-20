#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "rapidjson/document.h"
#include "SDL_mixer.h"

class AudioDB {
public:
    static inline auto LoadAudio(const std::string &audio_name) -> Mix_Chunk * {
        if (const auto audio_it = loaded_audios.find(audio_name); audio_it != loaded_audios.end()) {
            return audio_it->second;
        }
        Mix_Chunk *audio;
        auto audio_file_wav = "core/audio/" + audio_name + ".wav";
        auto audio_file_ogg = "core/audio/" + audio_name + ".ogg";
        if (std::filesystem::exists(audio_file_wav)) {
            audio = Mix_LoadWAV(audio_file_wav.c_str());
        } else if (std::filesystem::exists(audio_file_ogg)) {
            audio = Mix_LoadWAV(audio_file_ogg.c_str());
        } else {
            audio_file_wav = "resources/audio/" + audio_name + ".wav";
            audio_file_ogg = "resources/audio/" + audio_name + ".ogg";
            if (std::filesystem::exists(audio_file_wav)) {
                audio = Mix_LoadWAV(audio_file_wav.c_str());
            } else if (std::filesystem::exists(audio_file_ogg)) {
                audio = Mix_LoadWAV(audio_file_ogg.c_str());
            } else {
                std::cout << "error: failed to play audio clip " << audio_name;
                exit(0);
            }
        }
        loaded_audios.insert({ audio_name, audio });
        return audio;
    }

    static inline auto PlayAudio(int channel, const char *audio_name, bool does_loop) {
        const auto audio = LoadAudio(audio_name);
        Mix_PlayChannel(channel, audio, does_loop ? -1 : 0);
    }

    static inline auto HaltAudio(int channel) {
        Mix_HaltChannel(channel);
    }

    static inline auto SetVolume(int channel, float volume) {
        Mix_Volume(channel, static_cast<int>(volume));
    }

private:
    static inline std::unordered_map<std::string, Mix_Chunk *> loaded_audios;
};

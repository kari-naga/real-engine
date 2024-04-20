#pragma once

#include <chrono>

class Time {
  public:
    static inline auto DeltaTime() {
        return delta_time.count();
    }

    static inline auto UnscaledDeltaTime() {
        return unscaled_delta_time.count();
    }

    static inline auto GetTime() {
        return time.count();
    }

    static inline auto GetUnscaledTime() {
        return unscaled_time.count();
    }

    static inline auto GetTimeScale() {
        return time_scale;
    }

    static inline auto SetTimeScale(float ts) {
        time_scale = ts;
    }

    static inline auto Init() {
        last_frame_time = std::chrono::high_resolution_clock::now();
        current_frame_time = last_frame_time;
        delta_time = current_frame_time - last_frame_time;
        unscaled_delta_time = delta_time;
        time = unscaled_delta_time;
    }

    static inline auto Tick() {
        last_frame_time = current_frame_time;
        current_frame_time = std::chrono::high_resolution_clock::now();
        unscaled_delta_time = current_frame_time - last_frame_time;
        delta_time = unscaled_delta_time * time_scale;
        unscaled_time += unscaled_delta_time;
        time += delta_time;
    }

  private:
    static inline float time_scale = 1.f;
    static inline std::chrono::time_point<std::chrono::high_resolution_clock> last_frame_time;
    static inline std::chrono::time_point<std::chrono::high_resolution_clock> current_frame_time;
    static inline std::chrono::duration<float> delta_time;
    static inline std::chrono::duration<float> unscaled_delta_time;
    static inline std::chrono::duration<float> time;
    static inline std::chrono::duration<float> unscaled_time;
};
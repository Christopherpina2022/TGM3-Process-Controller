#pragma once

struct ResolutionConfig {
    int height;
    int width;
};

bool WriteConfig(const ResolutionConfig& config, const char* path);
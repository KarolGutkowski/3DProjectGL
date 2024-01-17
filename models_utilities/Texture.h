#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>

struct Texture {
    uint32_t id;
    std::string type;
    std::string path;
};

#endif

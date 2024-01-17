#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include <vector>
#include "GLFW/glfw3.h"
#include <cstdint>
#include <cassert>

class vertex_buffer_element {
public:
    uint32_t count;
    uint32_t type;
    uint32_t normalized;

    static uint32_t get_size_of_type(uint32_t type) {
        switch(type){
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(uint32_t);
            case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
        }
        assert(false);
        return 0;
    }
};

class vertex_buffer_layout {
private:
    std::vector<vertex_buffer_element> elements;
    size_t stride;
public:
    vertex_buffer_layout(): stride(0) {}

    template<typename T>
    void push(uint32_t count);

    inline const std::vector<vertex_buffer_element> get_elements() const { return elements; }
    inline size_t get_stride() const { return stride; }
};



#endif

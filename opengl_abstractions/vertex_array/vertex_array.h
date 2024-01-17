#ifndef ARRAY_H
#define ARRAY_H

#include "glad/glad.h"
#include "opengl_abstractions/vertex_buffer/vertex_buffer.h"
#include "opengl_abstractions/vertex_buffer_layout/vertex_buffer_layout.h"

class vertex_array {
private:
    uint32_t rendererId;
public:
    vertex_array();
    ~vertex_array();

    void bind() const;
    void unbind() const;

    void add_buffer(const vertex_buffer& vbo, const vertex_buffer_layout& layout);
};

#endif

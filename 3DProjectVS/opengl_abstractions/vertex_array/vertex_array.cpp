#include "vertex_array.h"

vertex_array::vertex_array() {
    vertexAtttribsCount = 0;
    glGenVertexArrays(1, &rendererId);
}

vertex_array::~vertex_array() {
    glDeleteVertexArrays(1, &rendererId);
}


void vertex_array::add_buffer(const vertex_buffer& vbo, const vertex_buffer_layout& layout) {
    bind();
    vbo.bind();
    const auto& elements = layout.get_elements();

    uint32_t offset = 0;
    for(uint32_t i=0; i<elements.size();i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (void*)offset);
        offset += element.count * vertex_buffer_element::get_size_of_type(element.type);
    }
    vbo.unbind();
}

void vertex_array::add_buffer_in_new_vbo(const vertex_buffer& vbo, const vertex_buffer_layout& layout)
{
    bind();
    vbo.bind();
    const auto& elements = layout.get_elements();

    uint32_t offset = 0;
    for (uint32_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(vertexAtttribsCount);
        glVertexAttribPointer(vertexAtttribsCount, element.count, element.type, element.normalized, layout.get_stride(), (void*)offset);
        offset += element.count * vertex_buffer_element::get_size_of_type(element.type);
        vertexAtttribsCount++;
    }
    vbo.unbind();
}

void vertex_array::bind() const {
    glBindVertexArray(rendererId);
}

void vertex_array::unbind() const {
    glBindVertexArray(0);
}
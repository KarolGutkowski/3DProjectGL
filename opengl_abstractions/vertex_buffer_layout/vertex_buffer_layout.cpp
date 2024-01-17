#include "vertex_buffer_layout.h"

template<>
void vertex_buffer_layout::push<float>(uint32_t count) {
    elements.push_back({ count, GL_FLOAT, GL_FALSE });
    stride+=vertex_buffer_element::get_size_of_type(GL_FLOAT)*count;
}

template<>
void vertex_buffer_layout::push<uint32_t>(uint32_t count) {
    elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
    stride+=vertex_buffer_element::get_size_of_type(GL_UNSIGNED_INT)*count;
}

template<>
void vertex_buffer_layout::push<unsigned char>(uint32_t count) {
    elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
    stride+=vertex_buffer_element::get_size_of_type(GL_UNSIGNED_BYTE)*count;
}
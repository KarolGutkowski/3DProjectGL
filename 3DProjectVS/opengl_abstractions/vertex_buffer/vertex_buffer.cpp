#include "vertex_buffer.h"
#include "renderer/renderer.h"

vertex_buffer::vertex_buffer(const void* data, unsigned int size){
    glGenBuffers(1,&rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
vertex_buffer::~vertex_buffer() {
    glDeleteBuffers(1, &rendererId);
}

void vertex_buffer::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
}

void vertex_buffer::unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
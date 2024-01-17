#include "index_buffer.h"
#include "renderer/renderer.h"

index_buffer::index_buffer(const uint32_t* data, unsigned int count): count(count){
    glGenBuffers(1,&rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}
index_buffer::~index_buffer() {
    glDeleteBuffers(1, &rendererId);
}

void index_buffer::bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
}

void index_buffer::unbind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
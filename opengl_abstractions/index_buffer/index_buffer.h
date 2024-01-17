#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <cstdint>

class index_buffer {
private:
    unsigned int rendererId;
    unsigned int count;
public:
    index_buffer(const uint32_t* data, unsigned int count);
    ~index_buffer();

    void bind() const;
    void unbind() const;

    inline uint32_t get_count() const {return count;}
};

#endif

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class vertex_buffer {
private:
    unsigned int rendererId;
public:
    vertex_buffer(const void* data, unsigned int size);
    ~vertex_buffer();

    void bind() const;
    void unbind() const;
};

#endif

#ifndef TRIANGLE_VERTEX_H
#define TRIANGLE_VERTEX_H

class TriangleVertex
{
public:
	TriangleVertex(float x, float y, float z) : x(x), y(y), z(z) {}
private:
	float x;
	float y;
	float z;
};

#endif // !TRIANGLE_VERTEX_H
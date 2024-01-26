#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include <array>
#include <vector>
#include "models_utilities/BezierSurface/Triangle.h"
#include "models_utilities/BezierSurface/Bernstein.h"

class BezierSurface
{
public:
	BezierSurface(int triangulization_level);
	void render();
	void ChangeControlPointTo(float value, int i, int j);
private:
	void InitializeControlPoints();
	Triangle GetUpperRightTriangleAt(int i, int j);
	Triangle GetLowerRightTriangleAt(int i, int j);
	float ZValueAt(float x, float y);
	bool isInTheMiddleOf4by4(int i, int j);

	int triangulization_level;
	std::vector<Triangle> triangles;
	const float default_low_control_point_value = 0.85f;
	const float default_high_control_point_value = 1.0f;
	Bernstein bernstein_calculator;
	std::array<std::array<float, 4>,4> control_points;
};




#endif // !

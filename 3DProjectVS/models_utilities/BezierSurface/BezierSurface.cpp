#include "BezierSurface.h"
#include "models_utilities/BezierSurface/TriangleVertex.h"


BezierSurface::BezierSurface(int triangulization_level): triangulization_level(triangulization_level)
{
	InitializeControlPoints();
	bernstein_calculator = Bernstein();

	for (int i = 0; i < triangulization_level; i++)
	{
		for (int j = 0; j < triangulization_level; j++)
		{ 
			auto upperTriangle = GetUpperRightTriangleAt(i, j);
			auto lowerTriangle = GetLowerRightTriangleAt(i, j);
			
			triangles.push_back(upperTriangle);
			triangles.push_back(lowerTriangle);
		}
	}
}

void BezierSurface::InitializeControlPoints()
{
	for (int i = 0; i < control_points.size(); i++)
	{
		for (int j = 0; j < control_points[i].size(); j++)
		{
			control_points[i][j] = isInTheMiddleOf4by4(i, j) ? 
				default_high_control_point_value : default_low_control_point_value;
		}
	}
}

void BezierSurface::ChangeControlPointTo(float value, int i, int j)
{
	control_points[i][j] = value;
}

bool BezierSurface::isInTheMiddleOf4by4(int i, int j)
{
	return ((i == 1 || i == 2) && (j == 1 || j == 2));
}

Triangle BezierSurface::GetUpperRightTriangleAt(int i, int j)
{
	float upper_x = (i * 2.0f) / (float)triangulization_level - 1.0f;
	float upper_y = 1.0f - (j * 2.0f) / (float)triangulization_level;
	float step = 2.0f/ (float)triangulization_level;

	auto v1 = TriangleVertex(upper_x, upper_y, ZValueAt(upper_x, upper_y));
	auto v2 = TriangleVertex(upper_x + step, upper_y, ZValueAt(upper_x + step, upper_y));
	auto v3 = TriangleVertex(upper_x, upper_y-step, ZValueAt(upper_x, upper_y - step));

	return Triangle(v1, v2, v3);
}

Triangle BezierSurface::GetLowerRightTriangleAt(int i, int j)
{
	float lower_x = (i + 1 * 2.0f) / (float)triangulization_level - 1.0f;
	float lower_y = 1.0f - (j + 1 * 2.0f) / (float)triangulization_level;
	float step = 2.0f / (float)triangulization_level;

	auto v1 = TriangleVertex(lower_x, lower_y, ZValueAt(lower_x, lower_y));
	auto v2 = TriangleVertex(lower_x - step, lower_y, ZValueAt(lower_x - step, lower_y));
	auto v3 = TriangleVertex(lower_x, lower_y + step, ZValueAt(lower_x, lower_y + step));
	
	return Triangle(v1, v2, v3);
}

float BezierSurface::ZValueAt(float x, float y)
{
	float z = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			z += control_points[i][j] * bernstein_calculator.Value(x, i, 3) * bernstein_calculator.Value(y, j, 3);
		}
	}

	return z;
}
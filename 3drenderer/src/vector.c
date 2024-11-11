#include <math.h>
#include "vector.h"

////////////////////////////////////////////////////////////////////////////////
// Vector 2D functions 
////////////////////////////////////////////////////////////////////////////////
float vec2_length(vec2_t v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

////////////////////////////////////////////////////////////////////////////////
// Vector 3D functions 
////////////////////////////////////////////////////////////////////////////////
float vec3_length(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
	vec3_t rotated_vector = 
	{
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};

	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
	vec3_t rotated_vector = 
	{
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};

	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
	vec3_t rotated_vector = 
	{
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.y * cos(angle) + v.x * sin(angle),
		.z = v.z
	};
	
	return rotated_vector;
}
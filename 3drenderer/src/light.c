#include "light.h"
#include "vector.h"

////////////////////////////////////////////////////////////////////////////////
// Get the alpha alighment of a face giving the light direction
////////////////////////////////////////////////////////////////////////////////
float get_face_alightment_alpha(light_t light, vec4_t face_a, vec4_t face_b, vec4_t face_c, uint32_t color)
{
	vec3_t vector_a = vec3_from_vec4(face_a); 	/*   A   */
	vec3_t vector_b = vec3_from_vec4(face_b); 	/*  / \  */
	vec3_t vector_c = vec3_from_vec4(face_c); 	/* C---B */

	// Get the vector substraction of B-A and C-A
	vec3_t vector_ab = vec3_sub(vector_b, vector_a);
	vec3_t vector_ac = vec3_sub(vector_c, vector_a);
	vec3_normalize(&vector_ab);
	vec3_normalize(&vector_ac);
	
	// Compute the face normal (using cross product to find perpendicular)
	vec3_t normal = vec3_cross(vector_ab, vector_ac);

	// Normalize the face normal vector
	vec3_normalize(&normal);

	// Find the light bounce vector 
	vec3_t light_bounce = 
	{ 
		.x = -light.direction.x,
		.y = -light.direction.y,
		.z = -light.direction.z
	};

	vec3_normalize(&light_bounce);

	// Calculate how aligned the camera ray is with the face normal (using dot product)
	float dot_light_face = vec3_dot(normal, light_bounce);


	float alpha_light = 0 + (dot_light_face - (-1)) * (1 - 0) / (1 - (-1));

	return (dot_light_face + 1) / 2;
}

////////////////////////////////////////////////////////////////////////////////
// Change color based on a percentage factor to represent light intensity
////////////////////////////////////////////////////////////////////////////////
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor) 
{
	uint32_t a = (original_color & 0xFF000000);
	uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
	uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
	uint32_t b = (original_color & 0x000000FF) * percentage_factor;

	uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

	return new_color;
}
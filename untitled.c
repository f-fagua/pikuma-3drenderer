typedef struct 
{
	vec3_t position;
	vec3_t direction;
} camera_t;


mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up) 
{
	vec3_t z = vec3_normalize(vec3_sub(target, eye));	// forward (z) vector
	vec3_t x = vec3_normalize(vec3_cross(up, z));		// right (x) vector
	vec3_t y = vec3_cross(z, x);						// up (y) vector

	// | x.x  x.y  x.z  -dot(x, eye) |
	// | y.x  y.y  y.z  -dot(y, eye) |
	// | z.x  z.y  z.z  -dot(x, eye) |
	// |   0    0    0             1 |

	mat4_t view_matrix = 
	{{
		{ x.x, x.y, x.z, -vec3_dot(x, eye) },
		{ y.x, y.y, y.z, -vec3_dot(y, eye) },
		{ z.x, z.y, z.z, -vec3_dot(z, eye) },
		{   0,   0,   0, 				 1 }
	}};

	return view_matrix;
}
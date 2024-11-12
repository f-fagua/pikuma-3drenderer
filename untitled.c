



typedef struct {
    float m[4][4];
} mat4_t;


mat4_t mat4_make_scale (float sx, float sy, float sz)
{
	// | sx  0  0  0 |
	// |  0 sy  0  0 |
	// |  0  0 sz  0 |
	// |  0  0  0  1 |

	mat4_t m = mat4_identity();
	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;
	return m;
}
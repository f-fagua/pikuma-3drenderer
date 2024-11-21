#include <math.h>
#include "clipping.h"
#include "vector.h"


#define NUM_PLANES 6
plane_t frustrum_planes[NUM_PLANES];

///////////////////////////////////////////////////////////////////////////////
// Frustum planes are defined by a point and a normal vector
///////////////////////////////////////////////////////////////////////////////
// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fov/2), sin(fov/2))
// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
///////////////////////////////////////////////////////////////////////////////
//
//        /|\
//       / | | 
//      /\ | |
//     /   | |
//  P*|-><-|*|   ----> +z-axis
//     \   | |
//      \/ | |
//       \ | | 
//        \|/
//
///////////////////////////////////////////////////////////////////////////////void initialize_frustrum_planes(float fov, float z_near, float z_far) 
void init_frustrum_planes(float fov, float z_near, float z_far)
{
	float cos_half_fov = cos(fov / 2);
	float sin_half_fov = cos(fov / 2);

	frustrum_planes[LEFT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov;
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustrum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);;
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov;
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustrum_planes[TOP_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);;
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov;
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustrum_planes[BOTTOM_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);;
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov;
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustrum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustrum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

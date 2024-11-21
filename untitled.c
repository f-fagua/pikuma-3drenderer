void initialize_frustrum_planes(float fov, float z_near, float z_far) 
{
	frustrum_planes[LEFT_FRUSTUM_PLANE].point = {0, 0, 0};
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos(fov/2);
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin(fov/2);

	frustrum_planes[RIGHT_FRUSTUM_PLANE].point = {0, 0, 0};
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos(fov/2);
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin(fov/2);

	frustrum_planes[TOP_FRUSTUM_PLANE].point = {0, 0, 0};
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos(fov/2);
	frustrum_planes[TOP_FRUSTUM_PLANE].normal.z = sin(fov/2);

	frustrum_planes[BOTTOM_FRUSTUM_PLANE].point = {0, 0, 0};
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos(fov/2);
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin(fov/2);

	frustrum_planes[NEAR_FRUSTUM_PLANE].point = {0, 0, z_near};
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustrum_planes[FAR_FRUSTUM_PLANE].point = {0, 0, z_far};
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustrum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}
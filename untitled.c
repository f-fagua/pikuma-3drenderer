#define MAX_NUM_VERTICES 10
typedef	struct 
{
	vec3_t vertices[MAX_NUM_VERTICES];
	int num_vertices;
} polygon_t;

void clip_polygon(polygon_t* polygon) 
{
	clip_polygon_against_plane(polygon, LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, TOP_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, FAR_FRUSTUM_PLANE);
}

// Create a polygon from the original triangle
polygon_t polygon = create_polygon(triangle_vertices);

// Clip the polygon, returning a clipped polygon back
clip_polygon(&polygon);
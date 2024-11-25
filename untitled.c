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

void clip_polygon_against_plane(polygon_t* polygon, int plane)
{
	vec3_t plane_point = frustrum_planes[pane].point;
	vec3_t plane_normal = frustrum_planes[pane].normal;

	// The array of inside vertices that will be part of the final polygon returned via paramenter
	vec3_t inside_vertices[MAX_NUM_POLYGON_VERTICES];
	int num_inside_vertices = 0;

	// Start current and previous vertex with the first and last polygon vertices
	vec3_t* current_vertex = &polygon->vertices[0];
	vec3_t* previous_vertex = &polygon->vertices[polygon_num_vertices - 1];

	// Start the current and previous dot product (if > 0 it is inside, if < 0 it is outside)
	float current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);
	float previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

	// While the current vertex is different than the last vertex
	while (current_vertex != &polygon->vertices[polygon->num_vertices]) 
	{
		current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

		// If we changed from inside to outside or vice-versa
		if(current_dot * previous_dot < 0) 
		{
			// TODO: Calculate the interpolation factor, t = dotQ1 / (dotQ1 - dotQ2)
			float t = previous_dot / (previous_dot - current_dot);
			// TODO: Calculate the intersection point, I = Q1 + t(Q2 - Q1)
			vec3_t intersection_point = vec3_add(current_vertex, vec3_mul(vec3_sub(current_vertex, previous_vertex), t));

			// Insert the new intersection point in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = vec3_clone(&intersection_point);
			num_inside_vertices++;
		}

		// If the current point is inside the plane
		if (current_dot > 0) 
		{
			// Inser current vertex in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
			num_inside_vertices++;
		}

		// Move to the next vertex
		previous_dot = current_dot;
		previous_vertex = current_vertex;
		current_vertex++;
	}

	// TODO: Copu all the vertices from the inside_vertices into the destination polygon
}

// Create a polygon from the original triangle
polygon_t polygon = create_polygon(triangle_vertices);

// Clip the polygon, returning a clipped polygon back
clip_polygon(&polygon);
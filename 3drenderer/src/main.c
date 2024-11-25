#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "array.h"
#include "camera.h"
#include "clipping.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "texture.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
////////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0;

////////////////////////////////////////////////////////////////////////////////
// Array of triangles that should be renderer frame by frame
////////////////////////////////////////////////////////////////////////////////
#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

////////////////////////////////////////////////////////////////////////////////
// Declaration of global transformation matrices
////////////////////////////////////////////////////////////////////////////////
mat4_t world_matrix;
mat4_t proj_matrix;
mat4_t view_matrix;

////////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
	// Initialize render mode and triangle culling method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

	// Allocate the required bytes in memory for the color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*) malloc(sizeof(float) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// Initialize the perspective projection matrix
	float aspecty = (float)window_height / (float)window_width;
	float aspectx = (float)window_width / (float)window_height;
	float fovy = M_PI / 3.0; // In radians, it is the same as 180 / 3
	float fovx = atan(tan(fovy/2) * aspectx) * 2.0;
	float z_near = 0.1;
	float z_far = 100.0;
	proj_matrix = mat4_make_perspective(fovy, aspecty, z_near, z_far);

	// Initialize frustrum planes with a point and a normal
	init_frustrum_planes(fovx, fovy, z_near, z_far);

	// Loads the cube values in the mesh data structure
	//load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj");

	// Load the texture information from an external PNG file
	load_png_texture_data("./assets/cube.png");
}

////////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
////////////////////////////////////////////////////////////////////////////////
void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		{
			is_running = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
			}
			if (event.key.keysym.sym == SDLK_1)
			{
				render_method = RENDER_WIRE_VERTEX;
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				render_method = RENDER_WIRE;
			}
			if (event.key.keysym.sym == SDLK_3)
			{
				render_method = RENDER_FILL_TRIANGLE;
			}
			if (event.key.keysym.sym == SDLK_4)
			{
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			}
			if (event.key.keysym.sym == SDLK_5)
			{
				render_method = RENDER_TEXTURED;
			}
			if (event.key.keysym.sym == SDLK_6)
			{
				render_method = RENDER_TEXTURED_WIRE;
			}
			if (event.key.keysym.sym == SDLK_c)
			{
				cull_method = CULL_BACKFACE;
			}
			if (event.key.keysym.sym == SDLK_x)
			{
				cull_method = CULL_NONE;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				camera.position.y += 3 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				camera.position.y += -3 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time);
				camera.position = vec3_add(camera.position, camera.forward_velocity); 
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				camera.yaw += 1.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				camera.forward_velocity = vec3_mul(camera.direction, -5.0 * delta_time);
				camera.position = vec3_add(camera.position, camera.forward_velocity); 
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				camera.yaw -= 1.0 * delta_time;
			}
			
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
////////////////////////////////////////////////////////////////////////////////
void update(void)
{
	// Wait some time ultin the reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	// Only delay execution if we are running too fast
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	// Get a delta time factor converte to seconds to be used to update our game objects
	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
	
	previous_frame_time = SDL_GetTicks();

	// Initialize the array of triangles to render for the current frame
	num_triangles_to_render = 0;

	// Change the mesh scale/rotation per animation frame
	mesh.rotation.x += 0.0 * delta_time;
	mesh.rotation.y += 0.0 * delta_time;
	mesh.rotation.z += 0.0 * delta_time ;
	mesh.translation.z = 5.0;

	// Initialize the target
	vec3_t target = { 0, 0, 1 };
	mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
	camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3(target)));

	// Offset the camera position in the direction where the camera is pointing at
	target = vec3_add(camera.position, camera.direction);
	vec3_t up_direction = { 0, 1, 0 };

	// Create the view matrix
	view_matrix = mat4_look_at(camera.position, target, up_direction);

	// Create a scale, rotation, and translation matrices that will be used to multiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_x_matrix = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_y_matrix = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_z_matrix = mat4_make_rotation_z(mesh.rotation.z);

	// Loop all the triangle faces of our mesh
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) 
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a];
		face_vertices[1] = mesh.vertices[mesh_face.b];
		face_vertices[2] = mesh.vertices[mesh_face.c];

		vec4_t transformed_vertices[3];

		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
			
			// Create a world matrix combining scale, rotation, and translation matrices
			world_matrix = mat4_identity();

			// Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
			world_matrix = mat4_mul_mat4(scale_matrix, 			world_matrix); // Matrix on the left TRANSFORM matrix on the right 
			world_matrix = mat4_mul_mat4(rotation_z_matrix, 	world_matrix); // The same as with vectors
			world_matrix = mat4_mul_mat4(rotation_y_matrix, 	world_matrix);
			world_matrix = mat4_mul_mat4(rotation_x_matrix, 	world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix,	world_matrix);

			// Multiply the world matrix by the original vector
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			// Multiply the view matrix by the vector to transform the scene to camera space
			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

			// Save transformed vertex in the array of transformed vertices
			transformed_vertices[j] = transformed_vertex;
		}

		// Get individual vectors from A, B, and C vertices to compute normal
		vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*   A   */
		vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
		vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C---B */

		// Get the vector substraction of B-A and C-A
		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);
		
		// Compute the face normal (using cross product to find perpendicular)
		vec3_t normal = vec3_cross(vector_ab, vector_ac);
		vec3_normalize(&normal);

		// Find the vector between vertex A in the triangle and the camera origin 
		vec3_t origin = { 0, 0, 0 };
		vec3_t camera_ray = vec3_sub(origin, vector_a);

		// Calculate how aligned the camera ray is with the face normal (using dot product)
		float dot_normal_camera = vec3_dot(normal, camera_ray);
		
		if (cull_method == CULL_BACKFACE) 
		{
			if (dot_normal_camera < 0) 
			{
				continue;
			}
		}

		// Create a polygom from the original transfrom 
		polygon_t polygon = create_polygon_from_triangle
		(
			vec3_from_vec4(transformed_vertices[0]),
			vec3_from_vec4(transformed_vertices[1]),
			vec3_from_vec4(transformed_vertices[2])
		);

		// Clip the polygon and returns a new polygon with potential new vertices
		clip_polygon(&polygon);

		// Break the clipped polygon apart back into individual triangles
		triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
		int num_triangles_after_clipping = 0;

		triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);
		
		for (int t = 0; t < num_triangles_after_clipping; t++) 
		{
			triangle_t triangle_after_clipping = triangles_after_clipping[t];

			vec4_t projected_points[3];

			// Loop all three vertices to perform projection
			for (int j = 0; j < 3; j++)
			{
				// Project the current vertex
				projected_points[j] = mat4_mul_vec4_project(proj_matrix, triangle_after_clipping.points[j]);

				// Scale into the view
				projected_points[j].x *= (window_width/2.0);
				projected_points[j].y *= (window_height/2.0);

				// Invert the y values to account for flipped screen y coordenate
				projected_points[j].y *= -1;

				// Translating the projected points to the middle of the screen
				projected_points[j].x += (window_width/2.0);
				projected_points[j].y += (window_height/2.0);
			}

			// Calculate the shade intensity based on how aligned is the face normal and the inverse of the light ray
			float light_intensity_factor = -vec3_dot(normal, light.direction);

			// Calculate the triangle color based on the light angle
			uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

			triangle_t triangle_to_render = 
			{
				.points = 
				{
					{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
					{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
					{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w}
				},
				.texcoords = 
				{
					{ mesh_face.a_uv.u, mesh_face.a_uv.v },
					{ mesh_face.b_uv.u, mesh_face.b_uv.v },
					{ mesh_face.c_uv.u, mesh_face.c_uv.v }
				},
				.color = triangle_color
			};

			// Save the projected triangle in the array of triangles to render
			if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
			{
				triangles_to_render[num_triangles_to_render] = triangle_to_render;
				num_triangles_to_render++;
			}	
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
////////////////////////////////////////////////////////////////////////////////
void render(void)
{
	draw_grid(10, 0xFF333333);

	// Loop all projected triangles and render them
	for (int i = 0; i < num_triangles_to_render; i++) 
	{
		triangle_t triangle = triangles_to_render[i];

		// Draw filled triangle
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,	// vertex A
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,	// vertex B
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,	// vertex C
				triangle.color
			);
		}
		 
		// Draw textured triangle
		if (render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE)
		{
			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,	// vertex A
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,	// vertex B
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,	// vertex C
				mesh_texture
			);	
		}

		// Draw unfilled triangle
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_TEXTURED_WIRE)
		{
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y, 	// vertex A
				triangle.points[1].x, triangle.points[1].y,		// vertex B
				triangle.points[2].x, triangle.points[2].y, 	// vertex C
				0xFFFFFFFF
			);	
		}

		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFF0000FF);
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFF0000FF);
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFF0000FF);
		}
	}

	render_color_buffer();
	
	clear_color_buffer(0xFF000000);
	clear_z_buffer();

	SDL_RenderPresent(renderer);
}


////////////////////////////////////////////////////////////////////////////////
// Frees memory that was dinamically allocated
////////////////////////////////////////////////////////////////////////////////
void free_resources(void) 
{
	free(color_buffer);
	free(z_buffer);
	upng_free(png_texture);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(void)
{
	is_running = initialize_window();

	setup();

	while(is_running) 
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;	
}
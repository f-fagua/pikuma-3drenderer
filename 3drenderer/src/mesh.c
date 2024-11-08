#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = 
{
	.vertices = NULL,
	.faces = NULL,
	.rotation = { 0, 0, 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ .x = -1, .y = -1, .z = -1 }, 	//1
	{ .x = -1, .y =  1, .z = -1 }, 	//2
	{ .x =  1, .y =  1, .z = -1 }, 	//3
	{ .x =  1, .y = -1, .z = -1 }, 	//4
	{ .x =  1, .y =  1, .z =  1 }, 	//5
	{ .x =  1, .y = -1, .z =  1 }, 	//6
	{ .x = -1, .y =  1, .z =  1 }, 	//7
	{ .x = -1, .y = -1, .z =  1 }  	//8
};

face_t cube_faces[N_CUBE_FACES] = 
{
	// front
	{ .a = 1, .b = 2, .c = 3 },
	{ .a = 1, .b = 3, .c = 4 },

	// right
	{ .a = 4, .b = 3, .c = 5 },
	{ .a = 4, .b = 5, .c = 6 },

	// back
	{ .a = 6, .b = 5, .c = 7 },
	{ .a = 6, .b = 7, .c = 8 },

	// left
	{ .a = 8, .b = 7, .c = 2 },
	{ .a = 8, .b = 2, .c = 1 },

	// top
	{ .a = 2, .b = 7, .c = 5 },
	{ .a = 2, .b = 5, .c = 3 },

	// bottom
	{ .a = 6, .b = 8, .c = 1 },
	{ .a = 6, .b = 1, .c = 4 },
};

void load_cube_mesh_data(void) 
{
	for (int i = 0; i < N_CUBE_VERTICES; i++) 
	{
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}

	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

void load_obj_file_data(char* filename) 
{
	// TODO: 
	// Read the contents of the .obj file
	// and load the vertices and faces in
	// our mesh.vertices and mesh.faces
	FILE *file;
	char line[256];

	file = fopen(filename, "r");
	if (file == NULL) 
	{
		perror("Error opening file");
	}

	while (fgets(line, sizeof(line), file)) {
        // Process the line here (e.g., print it)
        if (line[0] == 'v' && line[1] == ' ')
        {
        	vec3_t vertex = parse_vertex(&line[0]);
        	array_push(mesh.vertices, vertex);
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
        	face_t face = parse_face(&line[0]);
			array_push(mesh.faces, face);
        }
    }

    // Close the file
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

vec3_t parse_vertex(char* line) 
{
	vec3_t vertex_data = {.x = 0, .y = 0, .z = 0};
	
	float x = 0, y = 0, z = 0;
	sscanf(line, "v %f %f %f", &x, &y, &z);

	vertex_data.x = x;
	vertex_data.y = y;
	vertex_data.z = z;

	return vertex_data;
}

face_t parse_face(char* line) 
{
	face_t face = { .a = 0, .b = 0, .c = 0 };
	
	int a1, a2, a3, b1, b2, b3, c1, c2, c3;

	sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a1, &a2, &a3, &b1, &b2, &b3, &c1, &c2, &c3);

	face.a = a1;
	face.b = b1;
	face.c = c1;

	return face;
}	
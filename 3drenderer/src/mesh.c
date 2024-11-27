#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

#define MAX_NUM_MESHES 10
static mesh_t meses[MAX_NUM_MESHES];
static mesh_count = 0;

void load_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation)
{
	// TODO: 
	// Load the OBJ file to our mesh
	// Load the PNG file information to the mesh texture
	// 
	// Add the new mesh to the array of meshes
	// mesh_count++ 
}

void load_obj_file_data(char* filename) 
{
	FILE* file;
	file = fopen(filename, "r");
	
	char line[1024];

	tex2_t* texcoords = NULL;

	// fgets => File Get String
	while (fgets(line, 1024, file)) 		 
	{
		// Vertex information
		// strncmp => String Number Compare
		if(strncmp(line, "v ", 2) == 0) 	
		{
			vec3_t vertex = parse_vertex(&line[0]);
			array_push(mesh.vertices, vertex);
		}
		if(strncmp(line, "vt ", 3) == 0) 	
		{
			tex2_t texcoord;
			sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
			array_push(texcoords, texcoord);
		}
		// Face Information
		if (strncmp(line, "f ", 2) == 0)
		{
			face_t face = parse_face(&line[0], texcoords);
			array_push(mesh.faces, face);
		}
	}

	array_free(texcoords);

	if (fclose(file) != 0) 
	{
        perror("Error closing file");
    }
}

vec3_t parse_vertex(char* line) 
{
	vec3_t vertex_data;

	// sscanf => String Scan Format
	sscanf(
		line, 
		"v %f %f %f", 
		&vertex_data.x, 
		&vertex_data.y, 
		&vertex_data.z
	);

	return vertex_data;
}

face_t parse_face(char* line, tex2_t* texcoords) 
{
	int vertex_indices[3];
	int texture_indices[3];
	int normal_indices[3];

	sscanf(
		line, 
		"f %d/%d/%d %d/%d/%d %d/%d/%d", 
		&vertex_indices[0],	&texture_indices[0], &normal_indices[0],
		&vertex_indices[1], &texture_indices[1], &normal_indices[1], 
		&vertex_indices[2], &texture_indices[2], &normal_indices[2]
	);

	face_t face = 
	{
		.a = vertex_indices[0] - 1,
		.b = vertex_indices[1] - 1,
		.c = vertex_indices[2] - 1,
		.a_uv = texcoords[texture_indices[0] - 1],
		.b_uv = texcoords[texture_indices[1] - 1],
		.c_uv = texcoords[texture_indices[2] - 1],
		.color = 0xFFFFFFFF
	};

	return face;
}	
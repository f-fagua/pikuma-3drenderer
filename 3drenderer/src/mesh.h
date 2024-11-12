#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];


////////////////////////////////////////////////////////////////////////////////
// Degine a struct for dynamic size meshes
////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	vec3_t* vertices; 	// dynamic array of vertices
	face_t* faces;		// dynamic array of faces
	vec3_t rotation;	// rotation with x, y, and z values
	vec3_t scale;		// scale with x, y and z values
	vec3_t translation; // translation with x, y and z values
} mesh_t;

extern mesh_t mesh;		// global variable mesh

void load_cube_mesh_data(void);
void load_obj_file_data(char* filename);
vec3_t parse_vertex(char* line);
face_t parse_face(char* line);

#endif
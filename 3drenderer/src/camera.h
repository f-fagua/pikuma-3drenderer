#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct 
{
	vec3_t position;
	vec3_t direction;
	vec3_t forward_velocity;
	float yaw;
	float pitch;
} camera_t;

void init_camera(vec3_t position, vec3_t direction, vec3_t forward_velocity, float yaw, float pitch);
void increment_camera_position_x(float increment);
void increment_camera_position_y(float increment);
void increment_camera_position_z(float increment);
void set_camera_forward_velocity(vec3_t v);
void set_camera_direction(vec3_t v);
vec3_t get_camera_direction(void);
vec3_t get_camera_position(void);
void increment_camera_yaw(float increment);
void increment_camera_pitch(float increment);
float get_camera_yaw(void);
float get_camera_pitch(void);


#endif
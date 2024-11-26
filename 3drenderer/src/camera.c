#include "camera.h"

static camera_t camera = {
	.position = {0, 0, 0},
	.direction = {0, 0, 1},
	.forward_velocity = {0, 0, 0},
	.yaw = 0.0,
	.pitch = 0.0
};

void init_camera(vec3_t position, vec3_t direction, vec3_t forward_velocity, float yaw, float pitch) 
{
	camera.position = position;					// {0, 0, 0},
	camera.direction = direction;				// {0, 0, 1},
	camera.forward_velocity = forward_velocity;	// {0, 0, 0},
	camera.yaw = yaw;							// 0
	camera.pitch = pitch;						// 0
}

void increment_camera_position_x(float increment)
{
	camera.position.x += increment;
}

void increment_camera_position_y(float increment)
{
	camera.position.y += increment;
}

void increment_camera_position_z(float increment)
{
	camera.position.z += increment;
}

void set_camera_forward_velocity(vec3_t v)
{
	camera.forward_velocity = v;
	camera.position = vec3_add(camera.position, camera.forward_velocity);	
}

void set_camera_direction(vec3_t v)
{
	camera.direction = v;
}

vec3_t get_camera_direction(void)
{
	return camera.direction;
}

vec3_t get_camera_position(void)
{
	return camera.position;
}

void increment_camera_yaw(float increment)
{
	camera.yaw += increment;
}

float get_camera_yaw(void)
{
	return camera.yaw;
}

void increment_camera_pitch(float increment)
{
	camera.pitch += increment;
}

float get_camera_pitch(void)
{
	return camera.pitch;
}
#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>
#include "vector.h"

typedef struct 
{
    vec3_t direction;
} light_t;

float get_face_alightment_alpha(light_t light, vec4_t face_a, vec4_t face_b, vec4_t face_c, uint32_t color);
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

#endif
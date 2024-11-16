

typedef struct 
{
    vec3_t direction;
} light_t;

////////////////////////////////////////////////////////////////////////////////
// Change color based on a percentage factor to represent light intensity
////////////////////////////////////////////////////////////////////////////////
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor) 
{
	uint32_t a = (original_color & 0xFF000000);
	uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
	uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
	uint32_t b = (original_color & 0x000000FF) * percentage_factor;

	uint32_t new_color = a | (r & 0x00FF0000) | (r & 0x0000FF00) | (r & 0x000000FF);

	return new_color;
}
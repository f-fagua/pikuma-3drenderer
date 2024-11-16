
uint32_t* texture; // buffer in memory

typedef struct 
{
	float u;
	float v;
} tex2_t;

typedef struct
{
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
} face_t

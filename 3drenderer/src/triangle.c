#include "display.h"
#include "swap.h"
#include "triangle.h"
#include "vector.h"

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_flat_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float inv_slope1 = (float)(x1 - x0) / (y1 - y0); 	// m' = dX / dY
	float inv_slope2 = (float)(x2 - x0) / (y2 - y0);	// y = xm + c
														// x = ym' - c
	// Start x_start and x_end from the top vertex (x0, y0)
	float x_start = x0;
	float x_end = x0;

	// Loop all the scanlines from top to bottom
	for (int y = y0; y <= y2; y++) 
	{
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope1;
		x_end 	+= inv_slope2;	
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_flat_top(float x0, float y0, float x1, float y1, float x2, float y2, uint32_t color)
{
	// Find the two slopes (two trianble legs)
	float inv_slope1 = (float)(x2 - x0) / (y2 - y0); 	// m' = dX / dY
	float inv_slope2 = (float)(x2 - x1) / (y2 - y1);	// y = xm + c
														// x = ym' - c
	// Start x_start and x_end from the bottom vertex (x2, y2)
	float x_start 	= x2;
	float x_end 	= x2;

	// Loop all the scanlines from top to bottom
	for (int y = y2; y >= y0; y--) 
	{
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope1;
		x_end 	-= inv_slope2;	
	}
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 == y2)
	{
		// We can simply draw the flat-bottom triangle
		draw_flat_bottom(x0, y0, x1, y1, x2, y2, color);	
	}
	else if (y0 == y1) 
	{
		// We can simply draw the flat-top triangle
		draw_flat_top(x0, y0, x1, y1, x2, y2, color);	
	}
	else 
	{
		// Calculate the new vertex (Mx, My)
		int My = y1;
		int Mx = (((x2-x0) * (y1 - y0)) / (y2 - y0)) + x0;

		// Draw flat-bottom triangle
		draw_flat_bottom(x0, y0, x1, y1, Mx, My, color);

		// Draw flat-top triangle
		draw_flat_top(x1, y1, Mx, My, x2, y2, color);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Return the barycentric wights alpha, beta, and gamm a for point p
///////////////////////////////////////////////////////////////////////////////
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) 
{
	// Find the vectors between the vertices ABC and point p
	vec2_t ac = vec2_sub(c, a);
	vec2_t ab = vec2_sub(b, a);
	vec2_t pc = vec2_sub(c, p);
	vec2_t pb = vec2_sub(b, p);
	vec2_t ap = vec2_sub(p, a);

	// Find area of the full parallelogram (triangle ABC) using cross product 
	float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

	// Find area of the full parallelogram (triangle PBC) using cross product
	float area_parallelogram_pbc = (pc.x * pb.y - pc.y * pb.x); // || PC x PB ||

	// Find area of the full parallelogram (triangle APC) using cross product
	float area_parallelogram_apc = (ac.x * ap.y - ac.y * ap.x); // || AC x AP ||

	// Alpha = area of parallelogram-PBC over the area of the full parallelogram-ABC
	float alpha = area_parallelogram_pbc / area_parallelogram_abc;

	// Beta = area of parallelogram-APB over the area of the full parallelogram-ABC
	float beta = area_parallelogram_apc / area_parallelogram_abc;

	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	return weights;
}

///////////////////////////////////////////////////////////////////////////////
// Function to draw the textured pixel at position x and y using interpolation
///////////////////////////////////////////////////////////////////////////////
void draw_texel(
	int x, int y, uint32_t* texture,
	vec2_t point_a, vec2_t point_b, vec2_t point_c,
	float u0, float v0, float u1, float v1, float u2, float v2
	) 
{
	vec2_t point_p = {x, y};
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, point_p);
	
	float alpha = weights.x;
	float beta 	= weights.y;
	float gamma = weights.z;

	// Perform the interpolation of all U and V values using barycentric weights
	float interpolated_u = (u0) * alpha + (u1) * beta + (u2) * gamma;
	float interpolated_v = (v0) * alpha + (v1) * beta + (v2) * gamma;

	// Map the UV coordinate to the full texture width and height
	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));
	
	draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);
}


///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_textured_triangle(
	int x0, int y0, float u0, float v0, 
	int x1, int y1, float u1, float v1, 
	int x2, int y2, float u2, float v2,
	uint32_t* texture
) 
{
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	// Create vector points after we sort the vertices
	vec2_t point_a = {x0, y0};
	vec2_t point_b = {x1, y1};
	vec2_t point_c = {x2, y2};


	///////////////////////////////////////////////////////
	// Render the upper part of the triangle (flat bottom)
	///////////////////////////////////////////////////////
	float inv_slope1 = 0;
	float inv_slope2 = 0;

	if (y1 - y0 != 0) inv_slope1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0) 
	{
		for (int y = y0; y <= y1; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end 	= x0 + (y - y0) * inv_slope2;

			if (x_end < x_start) int_swap(&x_start, &x_end); // if x_start is to the left of x_end 

			for (int x = x_start; x < x_end; x++) 
			{
				// Draw our pixel with the color that comes from the texture
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
				 	u0, v0, u1, v1, u2, v2
				);
			}
		}	
	}

	///////////////////////////////////////////////////////
	// Render the down part of the triangle (flat top)
	///////////////////////////////////////////////////////
	inv_slope1 = 0;
	inv_slope2 = 0;

	if (y2 - y1 != 0) inv_slope1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1) 
	{
		for (int y = y1; y <= y2; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end 	= x0 + (y - y0) * inv_slope2;

			if (x_end < x_start) 
			{
				int_swap(&x_start, &x_end); // if x_start is to the left of x_end 
			}
			
			for (int x = x_start; x < x_end; x++) 
			{
				// Draw our pixel with the color that comes from the texture
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
				 	u0, v0, u1, v1, u2, v2
				);
			}
		}	
	}
}

void bubble_sort(triangle_t arr[], int n) 
{
	int i, j;
    for (i = 0; i < n; i++) 
    {
        for (j = i; j < n; j++) 
        {
            if (arr[i].avg_depth < arr[j].avg_depth) {
                // Swap the elements
                triangle_t temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

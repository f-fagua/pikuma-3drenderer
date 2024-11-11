#include "display.h"
#include "triangle.h"

void int_swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

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
void draw_line(int x0, int y0, int x1, int y1) 
{
	int delta_x = (x1 - x0); // run
	int delta_y = (y1 - y0); // rise

	int side_length = abs(delta_x);

	// Find how much we should increment in both x and y each step
	float x_inc = delta_x / (float)side_length; // 1, -1, or 0
	float y_inc = delta_y / (float)side_length; // not always 1, it depends on the slope

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_length; i++) 
	{
		draw_pixel(round(current_x), round(current_y));
		current_x += x_inc;
		current_y += y_inc;
	}
}
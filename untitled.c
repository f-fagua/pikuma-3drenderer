///////////////////////////////////////////////////////////////////////////////
// Return the barycentric wights alpha, beta, and gamm a for point p
///////////////////////////////////////////////////////////////////////////////
vect_3 barycentric_weights(vect_2 a, vect_2 b, vect_2 c, vect_2 p) 
{
	// Find the vectors between the vertices ABC and point p
	vect_2 ac = vec2_sub(c, a);
	vect_2 ab = vec2_sub(b, a);
	vect_2 pc = vec2_sub(c, p);
	vect_2 pb = vec2_sub(b, p);
	vect_2 ap = vec2_sub(p, a);

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

	float gamma = 1-alpha-beta;

	vect_3 weights = {alpha, beta, gamma};
	return weights;
}
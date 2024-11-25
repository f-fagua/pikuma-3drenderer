
for (int i = 0; i < (num_vertices - 2); i++) 
{
	int index_0 = 0;
	int index_1 = i + 1;
	int index_2 = i + 2;

	create_triangle(index_0, index_1, index_2);
}
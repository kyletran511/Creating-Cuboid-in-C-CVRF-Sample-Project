// Program: Take in 4 inputs. 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generate_cuboid(char *cuboid, int x_len, int y_len, int z_len);

void main(int argc, char ** argv)
{
	// Check to make sure user passes three arguments:
	if (argc != 4)
	{
		printf("Please pass the following three arguments: x, y, z\n");
		return;
	}	

	// Print out and store cuboid dimensions

	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int z = atoi(argv[3]);

	printf("Cuboid length: ");
	printf("%d\n", x);
	printf("Cuboid width: ");
	printf("%d\n", y);
	printf("Cuboid height: ");
	printf("%d\n", z);

	// Allocate memory to store cuboid shape characters. Multiple by
	// 2 because that will probably be enough space
	int buffer_size = (x + y + z) * (y + z) * 4;
	char *cuboid = malloc(buffer_size);

	// Generate the cuboid output we can pass to printf to print cuboid.
	generate_cuboid(cuboid, x, y, z);

	printf("%s\n", cuboid);
	
	free(cuboid);
}

void generate_cuboid(char *cuboid, int x_len, int y_len, int z_len)
{
	int lines = y_len + z_len;
	int index = 0;
	int i, j;
	for (i = 0; i < lines; i++)
	{

		// Calculate number of spaces for this line
		int spaces = y_len - i;
		spaces = abs(spaces);
		while (spaces > 0)
			cuboid[index++] = ' ';
			spaces--;

		// Insert first edge on each line
		if ((i <= y_len) && i != 0)
		{
			cuboid[index++] = ' ';
			cuboid[index++] = '/';
		}
		else if (i > y_len)
		{
			cuboid[index++] = '\\';
			cuboid[index++] = ' ';
		}
		else
		{
			cuboid[index++] = ' ';
			cuboid[index++] = ' ';
		}

		// Calculate underscores
		int m;
		for (m = 0; m < x_len; m++)
		{
			cuboid[index++] = '_';

			if ((i = 0))
			{
				cuboid[index++] = ' ';
				cuboid[index++] = ' ';
			}
			if ((i <= y_len) && i != 0)
			{
				cuboid[index++] = ' ';
				cuboid[index++] = '/';
			}
			else if (i > y_len)
			{
				cuboid[index++] = '\\';
				if (m != (x_len - 1))
					cuboid[index++] = ' ';
			}
		}

		// Calculate remaining edges
		int side_chars;
		if (i <= z_len) {
			side_chars = i;
		}
		else {
			side_chars = lines - i;
		}
		for (m = 0; m < side_chars; m++) {
			if ((i = 0))
			{
				cuboid[index++] = ' ';
				cuboid[index++] = ' ';
			}
			if (i <= y_len && i != 0)
			{
				if (m < y_len && m < z_len)
				{
					cuboid[index++] = '\\';
					if (m < (side_chars - 1))
						cuboid[index++] = '/';
				}			
			}
			else if (i > y_len)
			{
				if (m < y_len && m < z_len)
				{
					cuboid[index++] = '/';
					if (m < (side_chars - 1))
						cuboid[index++] = '\\';
				}
			}
		}
		//index = k;	
	}
}

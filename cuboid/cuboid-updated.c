// Program: Take in 4 inputs. 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//char* instead of char[] because memory is dynamically allocated, not fixed size
void generate_cuboid(char *cuboid, int x_len, int y_len, int z_len);

// changed return of main to be of type int for exit status (Standard C)
int main(int argc, char ** argv)
{
	// Check to make sure user passes three arguments (file name counts as 1 argument):
	if (argc != 4)
	{
		printf("Please pass the following three arguments: x, y, z\n");
		return 1;
	}	

	// Print out and store cuboid dimensions
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int z = atoi(argv[3]);

	//making sure within dimension range
	if (x <= 0 || x > 10 || y <= 0 || y > 10 || z <= 0 || z > 10) {
		return 1;
	}

	printf("Cuboid length: ");
	printf("%d\n", x);
	printf("Cuboid width: ");
	printf("%d\n", y);
	printf("Cuboid height: ");
	printf("%d\n", z);

	// Allocate memory to store cuboid shape characters
	// Max Volume = 10^3 = 1000, extra for safety
	size_t buffer_size = x * y * z * 4 + 1024;
	char *cuboid = malloc(buffer_size);

	//printing error incase of memory allocation failing
	if (!cuboid) {
		perror("Memory allocation failed");
		return 1;
	}

	memset(cuboid, 0, buffer_size);
	// Generate the cuboid output we can pass to printf to print cuboid.
	//expecting char* not char**
	generate_cuboid(cuboid, x, y, z);

	printf("%s\n", cuboid);
	
	free(cuboid);
	return 0;
}

void generate_cuboid(char *cuboid, int x_len, int y_len, int z_len)
{
	int lines = y_len + z_len + 1;
	int index = 0;
	
	for (int i = 0; i < lines; i++)
	{
		int k = index;

		// 1. Calculate number of leading spaces 
        int spaces = abs(y_len - i) + (i == 0 ? 1 : 0);
        for (int s = 0; s < spaces; s++) {
            cuboid[k++] = ' ';
        }

		//2. Insert left edge
        if (i > 0 && i <= y_len) {
			cuboid[k++] = ' '; //ADD
            cuboid[k++] = '/';
        } else if (i > y_len) {
            cuboid[k++] = '\\';
			cuboid[k++] = ' '; // ADD
        } else {
			cuboid[k++] = ' ';
		}

		// 3. Calculate underscores
		for (int m = 0; m < x_len; m++)
		{
			cuboid[k++] = '_';
            if (i == 0) {  // Use proper comparison
                cuboid[k++] = ' ';
                cuboid[k++] = ' ';
            } else if (i <= y_len) {
				cuboid[k++] = ' '; //ADD
                cuboid[k++] = '/';
            } else {
                cuboid[k++] = '\\';
                if (m != (x_len - 1)) {
                    cuboid[k++] = ' ';
                }
            }
		}

        // 4. Fixed side edge calculations
        // int side_chars = (i < z_len) ? i : lines - i - 1;
		int side_chars;
		if (i == 0) {
			side_chars = 0;
		} else if (i <= y_len) {
			side_chars = i;
		} else {
			side_chars = lines - i;
		}
        for (int m = 0; m < side_chars; m++) {
            if (i == 0) {  // Use correct comparison
                cuboid[k++] = ' ';
                cuboid[k++] = ' ';
            } else if (i <= y_len) {
                if (m < y_len && m < z_len) {
                    cuboid[k++] = '\\';
                    if (m < (side_chars - 1)) {
                        cuboid[k++] = '/';
                    }
                }
            } else if ( i > y_len ) {
                if (m < y_len && m < z_len) {
                    cuboid[k++] = '/';
                    if (m < (side_chars - 1)) {
                        cuboid[k++] = '\\';
                    }
                }
            }
        }

        if (i != lines-1) cuboid[k++] = '\n';
        index = k;
	}
	cuboid[index] = '\0'; //null-terminate
}

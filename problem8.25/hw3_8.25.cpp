#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	int total = atoi(argv[1]);
	int offset = 0;
	int page_size = pow(2.0, 12);
	printf("The address %d contains:\npage number=%d\noffset=%d\n", total, total/page_size, total-page_size*(total/page_size));
	return 0;
}


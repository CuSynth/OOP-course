//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <stdio.h>

#include "factory.h"

int main(int argc, char* argv[]) {

	const char* file_name = "data.txt";
	if (argc == 2)
		file_name = argv[1];
	else
		printf("Using default file.\n");
	printf("Opening file '%s'.\n", file_name);

	void* queue = parse_data(file_name);
	draw_scene(queue);
	delete_data(queue);
	
	//_CrtDumpMemoryLeaks();
	return 0;
}
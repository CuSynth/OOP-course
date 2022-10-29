#include "factory.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "new.h"
#include "point.h"
#include "circle.h"
#include "line.h"
#include "rectangle.h"
#include "queue.h"

#define BUFF_SIZE 256
#define POINT "point"
#define CIRCLE "circle"
#define LINE "line"
#define RECT "rect"

static void* parse(char* str) {
	if (!str)
		return NULL;
	
	char objName[BUFF_SIZE];	
	int a, b, c, d;
	int num = sscanf_s(str, "%s %d %d %d %d\n", objName, BUFF_SIZE, &a, &b, &c, &d);
	objName[BUFF_SIZE - 1] = '\0';
	if (num == -1)
		return NULL;

	if (!strcmp(objName, POINT) && num == 3) 
		return new(Point, a, b);
	else if (!strcmp(objName, CIRCLE) && num == 4)
		return new(Circle, a, b, c);
	else if (!strcmp(objName, LINE) && num == 5) 
		return new(Line, a, b, c, d);
	else if (!strcmp(objName, RECT) && num == 5)
		return new(Rect, a, b, c, d);

	printf("Incorrect param count in str: %s\n", str);
	return NULL;
}

void* parse_data(const char* file_name) {
	FILE* F;
	fopen_s(&F, file_name, "r");
	if (F == NULL) {
		printf("Can't open!\n");
		return NULL;
	}

	void* queue = queue_init();
	char str[BUFF_SIZE];
	
	while (fgets(str, BUFF_SIZE, F) != NULL) {
		void* obj = parse(str);
		if(!obj)
			continue;
		if (!enqueue(queue, obj))
			break;
	}

	return queue;
	fclose(F);
}

void draw_scene(void* _queue) {
	Queue* queue = _queue;
	if (!queue)
		return;

	for (size_t i = 0; i < queue_count(queue); i++)
		draw(queue_get(queue, i));
}

void delete_data(void* _queue) {
	Queue* queue = _queue;
	if (!queue)
		return;
	
	queue_destroy(queue, delete);
}

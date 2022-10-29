#include "point.h"

#include <stdarg.h>
#include <stdio.h>

#include "new.h"

static void point_draw(const void* _self) {
	const struct Point* self = _self;
	printf("Point at (%d, %d).\n", self->x, self->y);
}

static void* point_ctor(void* _self, va_list* app) {
	struct Point* self = _self;
	self->x = va_arg(*app, int);
	self->y = va_arg(*app, int);
	return self;
}

static const Class _Point = {
	sizeof(struct Point),
	point_ctor,
	NULL, 
	point_draw
};

const void* Point = &_Point;
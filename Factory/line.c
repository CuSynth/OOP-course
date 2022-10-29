#include "line.h"

#include <stdarg.h>
#include <stdio.h>

#include "new.h"

static void line_draw(const void* _self) {
	const struct Line* self = _self;
	printf("Line at (%d, %d) - (%d, %d).\n",
		self->x1, self->y1, self->x2, self->y2);
}

static void* line_ctor(void* _self, va_list* app) {
	struct Line* self = _self;
	self->x1 = va_arg(*app, int);
	self->y1 = va_arg(*app, int);
	self->x2 = va_arg(*app, int);
	self->y2 = va_arg(*app, int);
	return self;
}

static const Class _Line = {
	sizeof(struct Line),
	line_ctor,
	NULL,
	line_draw
};

const void* Line = &_Line;
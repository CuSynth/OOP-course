#include "rectangle.h"

#include <stdio.h>
#include <stdarg.h>

#include "new.h"

static void rect_draw(const void* _self) {
	const struct Rectangle* self = _self;
	printf("Rectangle at (%d, %d) - (%d, %d).\n",
		self->l, self->b, self->r, self->t);
}

static void* rect_ctor(void* _self, va_list* app) {
	struct Rectangle* self = _self;
	self->l = va_arg(*app, int);
	self->b = va_arg(*app, int);
	self->r = va_arg(*app, int);
	self->t = va_arg(*app, int);
	return self;
}

static const Class _Rect = {
	sizeof(struct Rectangle),
	rect_ctor,
	NULL,
	rect_draw
};

const void* Rect = &_Rect;
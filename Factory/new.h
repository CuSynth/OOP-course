#ifndef _new_h
#define _new_h

#include <stddef.h>
#include <stdarg.h>

typedef struct {
	size_t size;

	void* (*ctor)(void* self, va_list* app);
	void* (*dtor)(void* self);
	void (*draw)(const void* self);
} Class;

void* new(const void* class, ...);
void delete(void* self);
void draw(const void* self);


#endif
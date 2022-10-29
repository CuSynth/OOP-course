#include "new.h"

#include <stdlib.h>
#include <assert.h>

void* new(const void* _class, ...){
	const Class * class = _class;
	void* p = calloc(1, class->size);
	assert(p);
	*(const Class**)p = class;
	
	if (class->ctor) {
		va_list ap;
		va_start(ap, _class);
		p = class->ctor(p, &ap);
		va_end(ap);
	}

	return p;
}

void delete(void* self) {
	const Class** cp = self;

	if (self && *cp && (*cp)->dtor)
		self = (*cp)->dtor(self);
	free(self);
}

void draw(const void* self) {
	const Class *const *cp = self;
	assert(self && *cp && (*cp)->draw);
	(*cp)->draw(self);
}
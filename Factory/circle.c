#include "circle.h"

#include <stdio.h>

#include "new.h"

#define x(p) (((const struct Point *)(p)) -> x)
#define y(p) (((const struct Point *)(p)) -> y)

static void* ñircle_ctor(void* _self, va_list* app) {
    struct Circle* self = ((const Class*)Point)->ctor(_self, app);
    self->rad = va_arg(*app, int);

    return self;
}

static void ñircle_draw(const void* _self) {
    const struct Circle* self = _self;
    printf("Circle at (%d,%d) rad %d.\n", x(self), y(self), self->rad);
}

static const Class _Circle = {
    sizeof(struct Circle),
    ñircle_ctor,
    0,
    ñircle_draw
};

const void* Circle = &_Circle;
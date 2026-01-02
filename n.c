#include "src/init.h"
#include <stdio.h>
#include <stddef.h>

typedef __int8_t *Box;
typedef struct
{
    pos_t   size;
    _edges  position;
    Box     content;
    int     length;
} _shape;


typedef _shape *shape_t;

#define GET_SHAPE_FROM_CONTENT(p) \
    ((shape_t)((char *)(p) - offsetof(_shape, content)))

int main()
{
    _shape *n = malloc(sizeof(n));
    char *t = n->content;
    printf("%p\n", GET_SHAPE_FROM_CONTENT(t));
    printf("%p\n", n->content);
    return 0;
}
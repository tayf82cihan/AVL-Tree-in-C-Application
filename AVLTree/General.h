#ifndef GENERAL_H
#define GENERAL_H

#define BOOL	char
#define FALSE	0
#define TRUE	1

#define COPY    0
#define DEFAULT 1

#define NO_OP

typedef void* Object;
typedef int* Integer;
typedef int (*COMPARISON_FUNC)(Object, Object);
typedef Object (*COPY_FUNC)(Object);
typedef void (*DESTRUCTION_FUNC)(Object);

#endif

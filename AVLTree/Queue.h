#ifndef QUEUE_H

#define QUEUE_H

#ifdef ALLOW_SETJMP_H
#include <setjmp.h>
#endif

#include "General.h"

#define EXC_QUEUE_EMPTY -61

struct QUEUE;
typedef struct QUEUE* Queue;

typedef struct _Obj_funcs {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
} Queue_Component_Funcs;

extern Queue Queue_Create(int constructor_type, ...);
extern void Queue_Destroy(Queue*);

extern Object Queue_Peek(Queue) /* throws QueueEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf q_peek_caller;
#endif

extern Object Queue_Remove(const Queue) /* throws QueueEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf q_rmv_caller;
#endif

extern void Queue_Add(const Queue, Object);
extern BOOL Queue_IsEmpty(const Queue);

#endif

#ifndef LIST_H

#define LIST_H

#ifdef ALLOW_SETJMP_H
#include <setjmp.h>
#endif

#include "General.h"
#include "ListIterator.h"


struct LIST;

typedef struct LIST* List;

typedef struct _Object_func {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
} List_Component_Funcs;

typedef enum { ITERATOR_BACKWARD, ITERATOR_FORWARD } Iterator_type;

#define EXC_LIST_EMPTY -21

#define FROM_ARRAY 2
#define SUBLIST 3

extern List List_Create(int constructor_type, ...);
extern void List_Destroy(List*);

extern Object List_GetFirst(const List) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf get_first_caller;
#endif

extern Object List_GetLast(const List) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf get_last_caller;
#endif

extern void List_InsertAtEnd(const List, Object new_item);
extern void List_InsertInFront(const List, Object new_item);

extern Object List_RemoveFromEnd(const List) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf rmv_from_end_caller;
#endif

extern Object List_RemoveFromFront(const List) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf rmv_from_front_caller;
#endif

extern BOOL List_IsEmpty(const List);
extern int List_Size(const List);

extern List List_Merge(const List, const List otherList);
extern Object* List_ToArray(const List) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf to_array_caller;
#endif

extern ListIterator List_ListIterator(const List, Iterator_type it_type);

#endif

#ifndef LISTEXTENDED_H

#define LISTEXTENDED_H

#ifdef ALLOW_SETJMP_H
#include <setjmp.h>
#endif

#include "General.h"
#include "List.h"

#ifdef ALLOW_SETJMP_H
#define EXC_LIST_ILLEGALSTATE -23
#define EXC_LIST_INDEXOUTOFBOUNDS -24
#define EXC_LIST_UNSUPPORTEDOPERATION -25
#define EXC_LIST_NOSUCHITEM -26
#endif

extern void List_InsertAfterFirst(const List, Object, Object)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf ins_after_first_caller;
#endif

extern void List_InsertAfterLast(const List, Object, Object)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf ins_after_last_caller;
#endif

extern void List_InsertAfterNth(const List, Object, Object, int)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H         
extern jmp_buf ins_after_Nth_caller;
#endif

extern void List_InsertBeforeFirst(const List, Object, Object)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf ins_before_first_caller;
#endif

extern void List_InsertBeforeLast(const List, Object, Object)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf ins_before_last_caller;
#endif

extern void List_InsertBeforeNth(const List, Object, Object, int)
        /* throws NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H         
extern jmp_buf ins_before_Nth_caller;
#endif

extern void List_InsertInAscendingOrder(const List, Object);
extern void List_InsertInAscendingOrderEx(const List, Object, COMPARISON_FUNC);

extern void List_InsertInDescendingOrder(const List, Object);
extern void List_InsertInDescendingOrderEx(const List, Object, COMPARISON_FUNC);

extern int List_RemoveAll(const List, Object) /* throws ListEmpty */ ;
#ifdef ALLOW_SETJMP_H
extern jmp_buf rmv_all_caller;
#endif

extern void List_RemoveFirst(const List, Object)
       /* throws ListEmpty, NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H       
extern jmp_buf rmv_first_caller;
#endif

extern void List_RemoveLast(const List, Object)
       /* throws ListEmpty, NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf rmv_last_caller;
#endif

extern void List_RemoveNthFromEnd(const List, Object, int)
       /* throws ListEmpty, NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H          
extern jmp_buf rmv_Nth_from_end_caller;
#endif

extern void List_RemoveNthFromFront(const List, Object, int)
       /* throws ListEmpty, NoSuchItem */ ;
#ifdef ALLOW_SETJMP_H        
extern jmp_buf rmv_Nth_from_front_caller;
#endif

extern void List_Sort(const List);
       /* throws ListEmpty */
extern void List_SortEx(const List, COMPARISON_FUNC);
       /* throws ListEmpty */       
#ifdef ALLOW_SETJMP_H  
jmp_buf to_sort_caller;
#endif

extern void List_InsertAtIndex(const List, Object, int);
#ifdef ALLOW_SETJMP_H  
extern jmp_buf ins_at_index_caller;
#endif

extern BOOL List_Contains(const List, const Object);
extern Object List_Get(const List, int);

void List_Set(const List, Object, int);
#ifdef ALLOW_SETJMP_H
extern jmp_buf set_at_index_caller;
#endif

extern int List_NthIndexOf(const List, const Object, int);
extern int List_IndexOf(const List, const Object);
extern int List_LastNthIndexOf(const List, const Object, int);
extern int List_LastIndexOf(const List, const Object);

extern void List_RemoveAtIndex(const List, int)
       /* throws ListEmpy, ListIllegalState, IndexOutOfBounds, 
          UnsupportedOperation */;
#ifdef ALLOW_SETJMP_H
extern jmp_buf rmv_at_index_caller;
#endif


#endif





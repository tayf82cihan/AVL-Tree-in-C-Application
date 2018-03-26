#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include "General.h"

//struct AVL_NODE;
struct AVL_ROOT;

//typedef struct AVL_NODE* AVL_Node;
typedef struct AVL_ROOT* AVL_Root;

typedef struct _Object_func {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
} AVL_Tree_Component_Funcs;

extern AVL_Root AVL_Tree_Create(AVL_Tree_Component_Funcs);
extern void AVL_Tree_Destroy(AVL_Root*);
extern void AVL_Tree_InsertElement(const AVL_Root, const Object);
extern Object AVL_Tree_DeleteElement(const AVL_Root, Object);
//extern void AVL_Tree_RemoveElement(const AVL_Root, Object);
extern int AVL_Tree_GetNumberOfElements(const AVL_Root);
extern Object AVL_Tree_GetElement(const AVL_Root this, const Object info);
extern Object AVL_Tree_FindLargestElement(const AVL_Root);
extern Object AVL_Tree_FindSmallestElement(const AVL_Root);

#ifdef DEBUG
extern void AVL_Tree_PreorderTraversal(const AVL_Root);
extern void AVL_Tree_InorderTraversal(const AVL_Root);
extern void AVL_Tree_PostorderTraversal(const AVL_Root);
#endif

extern Object AVL_Tree_BFS_Search(const AVL_Root this, const Object info);

#endif

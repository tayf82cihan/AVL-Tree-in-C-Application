#include <stdio.h>
#include <stdlib.h>

#include "AVL_Tree.h"


/*
*   compare function must afford 
*   the following conditions
*
*   lhs < rhs,    1
*   lhs > rhs,   -1
*   lhs = rhs,    0
*
*/
int compare_ints(Object lhs, Object rhs) {

    int rhs_int = *((Integer) rhs);
    int lhs_int = *((Integer) lhs);

    if (rhs_int > lhs_int) return 1;
    else if (rhs_int < lhs_int) return -1;
    else return 0;

} /* end of int compare_ints(Object lhs, Object rhs) */

Object copy_int(Object int_to_be_copied) {

    Integer ret_val = (Integer) malloc(sizeof(int));

    *ret_val = *((Integer) int_to_be_copied);

    return ret_val;

} /* end of Object copy_int(Object) */

void destroy_int(Object int_to_be_destroyed) {

    free((Integer) int_to_be_destroyed);

} /* end of void destroy_int(Object) */

Object wrap_int(int int_to_be_wrapped) {

    Integer ip = (Integer) malloc(sizeof(int));
    *ip = int_to_be_wrapped;

    return ip;

} /* end of Object wrap_int(int) */


int main(void) {

    AVL_Tree_Component_Funcs int_funcs = { &compare_ints, &copy_int, &destroy_int };
    Object obj_array[] = { wrap_int(100), wrap_int(200), wrap_int(300), wrap_int(400), wrap_int(500),  
                           wrap_int(600), wrap_int(700), wrap_int(90), wrap_int(80), wrap_int(350), 
                           wrap_int(550), wrap_int(540), wrap_int(340), wrap_int(450)};
    Object obj_array2[] = { wrap_int(1100), wrap_int(1200), wrap_int(1300), wrap_int(1400), wrap_int(1500),  
                           wrap_int(1600), wrap_int(1700), wrap_int(190), wrap_int(180), wrap_int(1350), 
                           wrap_int(1550), wrap_int(1540), wrap_int(1340), wrap_int(1450)};
                           
    Object founded_obj, to_be_found = wrap_int(1540);
    AVL_Root root;
    int i;

    if ((root = AVL_Tree_Create(int_funcs)) == NULL) return;

    for (i = 0; i < (sizeof(obj_array)/ sizeof(Object)); i++)
        AVL_Tree_InsertElement(root, obj_array[i]);
   
    for (i = 0; i < (sizeof(obj_array)/ sizeof(Object)); i++)
        AVL_Tree_InsertElement(root, obj_array2[i]);


    founded_obj = AVL_Tree_BFS_Search(root, to_be_found);
    
    if (founded_obj)    
        printf("By using AVL_Tree_BFS_Search, find %d in AVL tree: %d\n", *(Integer)to_be_found,  *(Integer)founded_obj);
    
    destroy_int(to_be_found);

    printf("\n");
#ifdef DEBUG    
    printf("Preorder traversal...\n\n");
    AVL_Tree_PreorderTraversal(root);  
    
    printf("\n");  

    printf("Inorder traversal...\n\n");
    AVL_Tree_InorderTraversal(root);   
    
    printf("\n");
    
    printf("Postorder traversal...\n\n");
    AVL_Tree_PostorderTraversal(root);  
    
    printf("\n");  
#endif
    
    to_be_found = wrap_int(80);
    
    founded_obj = AVL_Tree_GetElement(root, to_be_found);
    
    if (founded_obj)    
        printf("By using AVL_Tree_GetElement Find %d in AVL tree: %d\n", *(Integer)to_be_found,  *(Integer)founded_obj);
    
    destroy_int(to_be_found);
   
    printf("Deleting 400, 340, 300, 500, 450, 350, 700...\n");
    
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[3]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[12]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[2]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[4]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[13]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[9]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[6]));
#ifdef DEBUG    
    printf("Inorder traversal after deleting 400, 340, 300, 500, 450, 350, 700... \n\n");    
    AVL_Tree_InorderTraversal(root);
#endif
    
    printf("\nDeleting 540, 100, 600, 550, 200, 80, 90...\n");
    
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[11]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[0]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[5]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[10]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[1]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[8]));
    destroy_int(AVL_Tree_DeleteElement(root, obj_array[7]));
#ifdef DEBUG         
    AVL_Tree_InorderTraversal(root);
#endif    
    AVL_Tree_Destroy(&root);
    
    return 0;

}


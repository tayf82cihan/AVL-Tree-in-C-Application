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

    if (int_to_be_destroyed)
        free((Integer) int_to_be_destroyed);

} /* end of void destroy_int(Object) */

Object wrap_int(int int_to_be_wrapped) {

    Integer ip = (Integer) malloc(sizeof(int));
    *ip = int_to_be_wrapped;

    return ip;

} /* end of Object wrap_int(int) */

Object* prepareObjectArray(int* int_arr, int n) {
    
	Object* ret_arr = (Object*) malloc(sizeof(Object) * n);
	int i;
	
	for (i = 0; i < n; i++)
	    ret_arr[i] = wrap_int(int_arr[i]);
	
	return ret_arr;
	
} /* end of Object* prepareObjectArray(int*, int) */


int main(void) {

    AVL_Tree_Component_Funcs int_funcs = { &compare_ints, &copy_int, &destroy_int };
    AVL_Root root;

    int insertList[] = { 
6342,1242,2378,7567,5773,1194,6484,4909,5883,1977,9631,7815,9977,1032,5299,7597,6126,3303,4925,2459,
4000,4370,7857,1011,5176,1289,9085,2722,4170,9574,4973,2131,5859,6161,534,2610,6468,6581,6464,9958,
5019,2623,5436,5520,5257,9163,4422,2124,2851,1507,9211,7172,315,5036,5781,1019,1736,8554,9889,8301,
1373,917,4061,262,7652,7159,6100,6928,8968,221,3697,8280,1021,2356,2787,7626,5333,9702,117,9384,189,
2892,1103,4372,1747,1472,965,1069,9181,6369,5960,266,9975,7546,7401,3881,5092,1066,7361,3638,3471,
4073,630,5030,7197,7488,4774,4402,5123,2604,7058,1169,9465,170,7183,3434,5489,567,7092,2261,9259,6367,
6022,5555,6285,1101,5658,8705,6286,9345,8069,3043,3871,3253,9553,1632,65,4414,1856,5060,2007,8777,
4990,3767,8943,9336,1125,1896,4433,7259,1851,3744,8356,1395,6827,40,4325,3108,9171,4095,2626,1904,
9853,736,4072,1804,6946,1110,1948,9972,9517,2791,3353,1598,4689,2575,185,9923,6945,9086,5118,6803,
4024,9173,2646,6850,1152,279,9496,662,4916,2797,735,833,529,2711,6216,9075,8619,4833,9804,6673,8111,
1266,4315,298,270,7331,9915,6718,1752,9557,5329,6881,364,6640,7457,713,751,5140,9362,6710,1724,5727,
8468,4269,8538,2192,7924,9602,2860,3294,6924,139,551,6577,9852,4648,819,4035,8611,6374,5517,4286,3624,
2341,5751,7225,8098,209,7568,9225,9080,2934,8586,9192,7577,8533,9591,5430,2208,4063,9974,1341,6978,
4612,7907,3885,9266,3945,1569,9988,7496,2270,8050,8229,9067,1769,2408,6171,495,4858,8195,4507,2792,
6540,3636,1872,9113,3374,5403,3791,9218,4183,8726,4724,2373,3409,2106,1023};
	
    int deleteList[] = {
5019,2623,5436,5520,5257,9163,4422,2124,2851,1507,9211,7172,315,5036,5781,1019,1736,8554,9889,8301,
1266,4315,298,270,7331,9915,6718,1752,9557,5329,6881,364,6640,7457,713,751,5140,9362,6710,1724,5727,
4000,4370,7857,1011,5176,1289,9085,2722,4170,9574,4973,2131,5859,6161,534,2610,6468,6581,6464,9958,
8468,4269,8538,2192,7924,9602,2860,3294,6924,139,551,6577,9852,4648,819,4035,8611,6374,5517,4286,3624,
1373,917,4061,262,7652,7159,6100,6928,8968,221,3697,8280,1021,2356,2787,7626,5333,9702,117,9384,189 
}; 
	
	int size_of_insertList = sizeof(insertList)/sizeof(int);
	int size_of_deleteList = sizeof(deleteList)/sizeof(int);
    Object *obj_array, *deleting_array;				 
    Object founded_obj, to_be_found = wrap_int(6484);    
    int i;

    obj_array = prepareObjectArray(insertList, sizeof(insertList)/sizeof(int));
	deleting_array = prepareObjectArray(deleteList, sizeof(deleteList)/sizeof(int));
	
    if ((root = AVL_Tree_Create(int_funcs)) == NULL) return -1;

    for (i = 0; i < size_of_insertList; i++)
        AVL_Tree_InsertElement(root, obj_array[i]);
		
	printf("\nAfter insertion, size of AVL tree: %d\n", AVL_Tree_GetNumberOfElements(root));
	printf("\nLargest Element: %d\n", *(Integer) AVL_Tree_FindLargestElement(root));
	printf("\nSmallest Element: %d\n",*(Integer) AVL_Tree_FindSmallestElement(root));
	
	printf("\nBFS Traversal\n");

    founded_obj = AVL_Tree_BFS_Search(root, to_be_found);
    
    if (founded_obj)    
        printf("\nBy using AVL_Tree_BFS_Search, find %d in AVL tree: %d\n", *(Integer)to_be_found,  *(Integer)founded_obj);
    destroy_int(to_be_found);
	
	
	printf("Removing Operation:\n");
	for (i = 0; i < size_of_deleteList /*61*/; i++) {
	    printf("Removing %d\n",  *(Integer)deleting_array[i]);
        destroy_int(AVL_Tree_DeleteElement(root, deleting_array[i]));
	}

#ifdef DEBUG	
    printf("\nAfter removing, BFS Traversal\n");
    AVL_Tree_BFS_Search(root, AVL_Tree_FindLargestElement(root)); 
#endif
	
	printf("\nAfter deletion, size of AVL tree: %d\n", AVL_Tree_GetNumberOfElements(root));
	
#ifdef DEBUG  
    printf("\nPre-Order Traversal\n");
    AVL_Tree_PreorderTraversal(root); 
    printf("\nIn-Order Traversal\n");	
    AVL_Tree_InorderTraversal(root);
    printf("\nPost-Order Traversal\n");	
    AVL_Tree_PostorderTraversal(root);  
#endif   

    AVL_Tree_Destroy(&root);
   
    return 0;

} /* end of int main(void) */


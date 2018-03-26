#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include "General.h"
#include "AVL_Tree.h"
#include "Queue.h"

#define LEFT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE    0x00
#define LEFT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE   0x01
#define RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE   0x02
#define RIGHT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE  0x03

#define NOTHING    0
#define LEFT_NODE  1
#define RIGHT_NODE 2

#define MAX(a,b) (((a)>(b))?(a):(b))

struct _NODE {
    Object info;
    int height;
    struct _NODE* parent;
    struct _NODE* left;
    struct _NODE* right;
};

typedef struct _NODE* _Node;   

struct AVL_ROOT {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
    int num_of_elements;    
    struct _NODE* childs;
};

static int getBalance(_Node this) {
    int left_height = 0, right_height = 0;

    if (this->left) left_height = this->left->height + 1;
    if (this->right) right_height = this->right->height + 1; 
    
    return left_height - right_height;
    
} /* end of static int getBalance(_Node) */ 

static void setNodeHeight(_Node this) {
    int left_height = 0, right_height = 0;

    if (this->left) left_height = this->left->height + 1;
    if (this->right) right_height = this->right->height + 1; 
    
    this->height = MAX(left_height, right_height);
        
} /* end of static void setNodeHeight(_Node) */      

static _Node create_Node(const Object info) {

    _Node this = (_Node) malloc(sizeof(struct _NODE));
    if (!this) {
        fprintf(stderr, "Out of memory for creating _Node...\n");
        return NULL;
    } /* end of if (!this) */
    
    this->info = info;
    this->height = 0;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
    
    return this;
    
} /* end of static _Node create_Node(const Object) */

static _Node Tree_GetNode(const AVL_Root this, const Object info) {

    _Node node;
    if (!this) return NULL;
    
    node = this->childs;
    
    while (node) {
        int cmp_result = this->compare_component(info, node->info);
        
        if (cmp_result > 0) node = node->left;
        else if (cmp_result < 0) node = node->right;
        else break;
    } /* end of while ((node && node->info) && (!ret_info)) */
    
    return node;
    
} /* end of static _Node Tree_GetNode(const AVL_Root, const Object) */


static _Node rotate(const _Node A, const _Node B, const _Node C, unsigned char rotate_type) {

    _Node parentA = A->parent, ret_node;
    unsigned char which_node = NOTHING;
    
    if (parentA != NULL) 
       which_node = (parentA->left == A) ? LEFT_NODE : RIGHT_NODE;  

    switch (rotate_type) {
        case LEFT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE:
        case RIGHT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE:
            if (rotate_type == LEFT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE) {
                A->left = B->right;
                if (B->right) B->right->parent = A;
                B->right = A;
            } else {
                A->right = B->left;
                if (B->left) B->left->parent = A;
                B->left = A;            
            }              
            B->parent = parentA;
            if (which_node != NOTHING)
                if (which_node == LEFT_NODE) parentA->left = B;
                else parentA->right = B;                
            A->parent = B;            
            break;
        case RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE:
        case LEFT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE:
        
            if (rotate_type == RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE) {
                B->right = C->left;
                if (C->left) C->left->parent = B;
                B->parent = C;
                A->left = C->right;
                if(C->right) C->right->parent = A;
                C->right = A;
                C->left = B;
            } else {
                B->left = C->right;
                if (C->right) C->right->parent = B;
                B->parent = C;
                A->right = C->left;
                if (C->left) C->left->parent = A;  
                C->left = A;  
                C->right = B;    
            }
            C->parent = parentA;
            if (which_node != NOTHING)
                if (which_node == LEFT_NODE) parentA->left = C;
                else parentA->right = C;            
            A->parent = C; 
    } /* end of switch (rotate_type) */
    
    setNodeHeight(A);   
    setNodeHeight(B);
    ret_node = B;
    
    if ((rotate_type == RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE) ||
        (rotate_type == LEFT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE)) { 
        setNodeHeight(C);
        ret_node = C;
    } /* end of if ((rotate_type...) */
    
    return ret_node;

} /* end of static void rotate(const _Node, const _Node, const _Node, unsigned char) */

static BOOL insert_node(const AVL_Root root, const _Node this, const _Node new_node) {

    _Node  nodeA, nodeB, nodeC, upper_node_after_rotate;    
    unsigned char rotate_type;
    COMPARISON_FUNC cmp = root->compare_component;

    if (!this) return FALSE;

    if (cmp(new_node->info, this->info) > 0) {
        /* insert at left */
        if(this->left == NULL) {
            this->left = new_node;
            new_node->parent = this;
        } else if (insert_node(root, this->left, new_node) == FALSE) 
            return FALSE;
    } else {
        /* insert at right */
        if (this->right == NULL) {
            this->right = new_node;
            new_node->parent = this;
        } else if (insert_node(root, this->right, new_node) == FALSE)
            return FALSE;
    } /* end of if-else */

    setNodeHeight(this);
        
    if (fabs( (double)getBalance(this) ) > (double)1.0) {
        nodeA = this;
                
        if (cmp(new_node->info, nodeA->info) > 0) {
           /* left sub-tree of critical node */
           nodeB = nodeA->left;
           if (cmp(new_node->info, nodeB->info) > 0){
               /* left sub-tree of left sub-tree of critical node */
               nodeC = nodeB->left;
               rotate_type = LEFT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE;
           } else {
               /* right sub-tree of left sub-tree of critical node */
               nodeC = nodeB->right;
               rotate_type = RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE;           
           }  /* end of if-else */         
        } else {
           /* right sub-tree of critical node */                      
           nodeB = nodeA->right;
           if (cmp(new_node->info, nodeB->info) > 0) {
               /* left sub-tree of right sub-tree of critical node */
               nodeC = nodeB->left;
               rotate_type = LEFT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE;
           } else {
               /* right sub-tree of right sub-tree of critical node */
               nodeC = nodeB->right;
               rotate_type = RIGHT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE;           
           } /* end of if-else */          

        } /* end of if if (cmp(new_node->info, nodeA->info) < 0) else ...*/
        
        upper_node_after_rotate = rotate(nodeA, nodeB, nodeC, rotate_type);
        if (upper_node_after_rotate->parent == NULL) 
            root->childs = upper_node_after_rotate; 
        
    } /* end of if (fabs( (double)(this->left->height - this->right->height) ) > (double)1.0) */
    
    return TRUE;

} /* static BOOL insert_node(const AVL_Root, const _Node, const _Node) */

static _Node Tree_FindLargestElement(const _Node this) {

    _Node ret_node = this;
    if (ret_node == NULL) return NULL;    
    while (ret_node->right) ret_node = ret_node->right;
        
    return ret_node;
        
} /* end of static _Node Tree_FindLargestElement(const _Node) */

Object AVL_Tree_FindLargestElement(const AVL_Root this) {

    _Node node;
    if (!this) return NULL;    
    node = Tree_FindLargestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object AVL_Tree_FindLargestElement(const AVL_Root) */ 

static _Node Tree_FindSmallestElement(const _Node this) {

    _Node ret_node = this;
    if (ret_node == NULL) return NULL;    
    while (ret_node->left) ret_node = ret_node->left;
        
    return ret_node;    

} /* end of static _Node Tree_FindSmallestElement(const _Node) */

Object AVL_Tree_FindSmallestElement(const AVL_Root this) {

    _Node node;
    if (!this) return NULL;    
    node = Tree_FindSmallestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object AVL_Tree_FindSmallestElement(const AVL_Root) */ 

static void apply_balance(const AVL_Root root, _Node *this) {

    int balance = getBalance(*this);
    
    if (fabs((double) balance) > (double)1.0) {
        _Node  nodeA, nodeB, nodeC, upper_node_after_rotate;
        unsigned char rotate_type;
        nodeA = *this;
        
        if (balance > 1) {
            nodeB = (*this)->left;
            balance = getBalance(nodeB);
            
            if (balance < 0) {
                nodeC = nodeB->right;
                rotate_type = RIGHT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE;
            } else {
                rotate_type = LEFT_SUBTREE_OF_LEFT_SUBTREE_OF_CRITICAL_NODE;
                nodeC = NULL;                
            }            
        } else {
            nodeB = (*this)->right;
            balance = getBalance(nodeB);
            
            if (balance > 0) {
                nodeC = nodeB->left;
                rotate_type = LEFT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE;
            } else {
                rotate_type = RIGHT_SUBTREE_OF_RIGHT_SUBTREE_OF_CRITICAL_NODE;
                nodeC = NULL;                
            }           
        } /* end of if (balance > 1) */
        
        upper_node_after_rotate = rotate(nodeA, nodeB, nodeC, rotate_type);
        if (upper_node_after_rotate->parent == NULL) 
            root->childs = upper_node_after_rotate; 
        *this = upper_node_after_rotate;       
        
    } /* end of if (fabs((double) balance) > (double)1.0) */

} /* end of static void apply_balance(const AVL_Root root, const _Node*) */

static void Tree_Restruct(const AVL_Root root, _Node this) {

    if (this == NULL) return;
    
    while (this) {
        setNodeHeight(this);
        apply_balance(root, &this);
        this = this->parent;
    } /* end of while (this) */
    
} /* end of static void Tree_Restruct(const AVL_Root, const _Node) */

static Object Tree_DeleteElement(const AVL_Root root, const Object info) {

    _Node to_be_deleted_node, largest_node, node;
    Object ret_info;
    int which_node;
    
    if ((root == NULL) || (root->childs == NULL)) return NULL;

    if ((to_be_deleted_node = Tree_GetNode(root, info)) == NULL) return NULL;
    
    ret_info = to_be_deleted_node->info;
    
    if (root->num_of_elements == 1) {
        free(root->childs);
        root->childs = NULL;
        return ret_info;
    } /* end of if ((root->num_of_elements == 1) && (compare_result == 0)) */
    
    if(to_be_deleted_node == root->childs) {
    
        if(to_be_deleted_node->left == NULL) {
            root->childs = to_be_deleted_node->right;
            to_be_deleted_node->right->parent = NULL;
            free(to_be_deleted_node);
            setNodeHeight(root->childs);
            return ret_info;
        } /* end of if(to_be_deleted_node->left == NULL) */
        
        if (to_be_deleted_node->left->right) {
           
            largest_node = Tree_FindLargestElement(to_be_deleted_node->left);
            to_be_deleted_node->info = largest_node->info;
            node = largest_node->parent;
            node->right = largest_node->left;
            
            if (largest_node->left) largest_node->left->parent = node;
            free(largest_node);
    
            Tree_Restruct(root, node);
            return ret_info;            
        } else {
        
            to_be_deleted_node->info = to_be_deleted_node->left->info;
            
            if (to_be_deleted_node->left->left)
                to_be_deleted_node->left->left->parent = to_be_deleted_node;
                            
            to_be_deleted_node->left = to_be_deleted_node->left->left;
            free(to_be_deleted_node->left);
            setNodeHeight(root->childs);
            
            Tree_Restruct(root, node);
            return  ret_info; 
        } /* end of if-else */
    } /* end of if(to_be_deleted_node == root->childs) */
        
    if (to_be_deleted_node->parent->left == to_be_deleted_node) 
        which_node = LEFT_NODE;
    else which_node = RIGHT_NODE;   
    
    node = to_be_deleted_node->parent; 
    
    if ((to_be_deleted_node->left == NULL) && (to_be_deleted_node->right == NULL)) {           
        
        if (which_node == LEFT_NODE) node->left = NULL;
        else node->right = NULL;
        free(to_be_deleted_node);
        
        Tree_Restruct(root, node);          
        return  ret_info;     
    } /* end of if ((to_be_deleted_node->left == NULL) && (to_be_deleted_node->right == NULL)) */
    
    if ((to_be_deleted_node->left == NULL) && (to_be_deleted_node->right)) {
     
        to_be_deleted_node->right->parent = node;
        
        if (which_node == LEFT_NODE) 
            node->left = to_be_deleted_node->right;
        else node->right = to_be_deleted_node->right; 
               
        free(to_be_deleted_node);
        
        Tree_Restruct(root, node);
        return ret_info;       
    } /* end of if ((to_be_deleted_node->left == NULL) && (to_be_deleted_node->right)) */
    
    if (to_be_deleted_node->left->right == NULL) {
    
        node = to_be_deleted_node;
        node->info = to_be_deleted_node->left->info;
        
        if (to_be_deleted_node->left->left == NULL) { 
            free(to_be_deleted_node->left);           
            node->left = NULL;           
        } else {
            to_be_deleted_node = to_be_deleted_node->left;
            node->left = to_be_deleted_node->left;
            to_be_deleted_node->left->parent = node;
            free(to_be_deleted_node);
        } /* end of if-else */ 
        
        Tree_Restruct(root, node);
        return ret_info; 
    } /* end of if (to_be_deleted_node->left->right == NULL) */
    
    largest_node = Tree_FindLargestElement(to_be_deleted_node->left);
    
    to_be_deleted_node->info = largest_node->info;
    node = largest_node->parent;
    node->right = largest_node->left;
    
    if (largest_node->left) 
        largest_node->left->parent = node;
        
    free(largest_node);
    
    Tree_Restruct(root, node);
    return ret_info;

} /* end of static Object Tree_DeleteElement(const AVL_Root root, const Object info) */

AVL_Root AVL_Tree_Create(AVL_Tree_Component_Funcs funcs) {
    AVL_Root ret_root;
    
    if ((ret_root = (AVL_Root) malloc(sizeof(struct AVL_ROOT))) == NULL) {
        fprintf(stderr, "Out of memory for creating AVL tree!!!\n");
        return NULL;
    }
    ret_root->compare_component = funcs.compare_component;
    ret_root->copy_component = funcs.copy_component;
    ret_root->destroy_component = funcs.destroy_component;
    ret_root->num_of_elements = 0;
    ret_root->childs = NULL;
    
    return ret_root;
    
} /* end of AVL_Root AVL_Tree_Create(AVL_Tree_Component_Funcs) */

void Tree_Destroy(_Node *this, DESTRUCTION_FUNC destruction_func) {

    if (*this != NULL) {        
        Tree_Destroy(&((*this)->left), destruction_func);
        Tree_Destroy(&((*this)->right), destruction_func);        
        destruction_func((*this)->info);
        (*this)->info = NULL;
        (*this)->parent = NULL;
        free(*this);
        *this = NULL;
    }
} /* end of void Tree_Destroy(Node *this, DESTRUCTION_FUNC) */

void AVL_Tree_Destroy(AVL_Root *this) {

    if (*this != NULL) {
        Tree_Destroy(&((*this)->childs), (*this)->destroy_component);        
        free(*this);
        *this = NULL;
    }
} /* end of void Tree_Destroy(Node *this) */

void AVL_Tree_InsertElement(const AVL_Root this, const Object info) {

    _Node temp_node;        
    
    if ((temp_node = create_Node(info)) == NULL) return;

    if (!this->childs) {        
        this->childs = temp_node;
        this->num_of_elements++;        
        return;
    } /* end of if (!this->childs) */
    
    if (insert_node(this, this->childs, temp_node) == TRUE)       
        this->num_of_elements++;    
    
    return;
    
} /* end of void AVL_Tree_Insert(const AVL_Root, const Object) */

Object AVL_Tree_DeleteElement(const AVL_Root this, Object info) {    
    
    Object ret_info;    
    if (info == NULL) return NULL;
    
    ret_info = Tree_DeleteElement(this, info);
    if (ret_info) this->num_of_elements--;
    
    return ret_info;      

} /* end of Object AVL_Tree_DeleteElement(const AVL_Root, Object) */

#ifdef DEBUG
static void Tree_PreorderTraversal(_Node this) {

    if (this != NULL) {
        printf("Node: %d Height: %d Parent: %d\n", *((Integer) this->info), this->height, (this->parent != NULL) ? *((Integer) this->parent->info) : 0);    
        Tree_PreorderTraversal(this->left);    
        Tree_PreorderTraversal(this->right);    
    } /* end of if (this != NULL) */
} /* end of static void Tree_PreorderTraversal(_Node) */

static void Tree_InorderTraversal(_Node this) {

    if (this != NULL) {
        Tree_InorderTraversal(this->left);    
        printf("Node: %d Height: %d Parent: %d\n", *((Integer) this->info), this->height, (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
        Tree_InorderTraversal(this->right);    
    } /* end of if (this != NULL) */
} /* end of static void Tree_InorderTraversal(_Node) */

static void Tree_PostorderTraversal(_Node this) {

    if (this != NULL) {    
        Tree_PostorderTraversal(this->left);    
        Tree_PostorderTraversal(this->right); 
        printf("Node: %d Height: %d Parent: %d\n", *((Integer) this->info), this->height, (this->parent != NULL) ? *((Integer) this->parent->info) : 0);         
    } /* end of if (this != NULL) */

} /* end of static void Tree_PostorderTraversal(_Node) */
#endif

int AVL_Tree_GetNumberOfElements(const AVL_Root root) {
    
    return (root != NULL) ? root->num_of_elements : 0;
    
} /* end of int AVL_Tree_GetNumberOfElements(const AVL_Root) */

Object AVL_Tree_GetElement(const AVL_Root this, const Object info) {

    _Node node;
    Object ret_info = NULL;
    if (!this) return NULL;
    
    node = this->childs;
    
    while ((node && node->info) && (!ret_info)) {
        int cmp_result = this->compare_component(info, node->info);
        
        if (cmp_result > 0) node = node->left;
        else if (cmp_result < 0) node = node->right;
        else ret_info = node->info;
    } /* end of while ((node && node->info) && (!ret_info)) */
    
    return ret_info;
    
} /* end of Object AVL_Tree_GetElement(const AVL_Root, const Object) */

#ifdef DEBUG
void AVL_Tree_PreorderTraversal(const AVL_Root this) {

    if (this->childs != NULL) 
        Tree_PreorderTraversal(this->childs); 
   
} /* end of void AVL_Tree_PreorderTraversal(const AVL_Root) */

void AVL_Tree_InorderTraversal(const AVL_Root this) {

    if (this->childs != NULL) 
        Tree_InorderTraversal(this->childs); 
   
} /* end of void AVL_Tree_InorderTraversal(const AVL_Root) */

void AVL_Tree_PostorderTraversal(const AVL_Root this) {

    if (this->childs != NULL) 
        Tree_PostorderTraversal(this->childs); 
   
} /* end of void AVL_Tree_PostorderTraversal(const AVL_Root) */
#endif

void empty_destroy(Object to_be_destroyed) {

} /* end of void destroy_int(Object) */

Object AVL_Tree_BFS_Search(const AVL_Root this, const Object info) {

    Queue_Component_Funcs queue_comp_funcs = { (COMPARISON_FUNC)this->compare_component, (COPY_FUNC)this->copy_component, (DESTRUCTION_FUNC) empty_destroy };
    Queue queue = Queue_Create(DEFAULT, queue_comp_funcs);    
    _Node node;
    Object ret_info = NULL;
    if (this == NULL) return NULL;
    
    node = this->childs;
    
#ifdef DEBUG
    while (node) {
#else    
    while (node && !ret_info) {
#endif	
#ifdef DEBUG    
        printf("Node: %d Height: %d Parent: %d\n", 
               *((Integer) node->info), node->height, 
               (node->parent != NULL) ? *((Integer) node->parent->info) : 0);
#endif               
        if (this->compare_component(info ,node->info) == 0) { 
            ret_info = node->info;
#ifndef DEBUG			
            continue;
#endif
        } /* end of if (this->compare_component(info ,node->info) == 0) */
            
        if (node->left) Queue_Add(queue, (Object)(node->left));
        if (node->right) Queue_Add(queue, (Object)(node->right));
        node =  (Queue_IsEmpty(queue) == FALSE) ? (_Node) Queue_Remove(queue) : NULL;
        
    } /* end of while (node && !ret_info) */
    
    Queue_Destroy(&queue);
    
    return ret_info;

} /* end of Object AVL_Tree_BFS_Search(const AVL_Root, const Object) */

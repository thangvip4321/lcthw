#include <lcthw/dbg.h>
#include <lcthw/bstree.h>
#include <stdlib.h>
#include <lcthw/bstrlib.h>
static int default_compare(void *a, void *b){
	return bstrcmp((bstring) a,(bstring) b);
}
BSTree *BSTree_create(BSTree_compare compare){
	BSTree *map = calloc(1,sizeof(BSTree));
	check_mem(map);
	map->compare = (compare ==NULL ? default_compare :compare);
	return map;
	error:
	if(map)
		BSTree_destroy(map);
	return NULL;
}
AVLTree *AVLTree_create(BSTree_compare compare){
	AVLTree *map = calloc(1,sizeof(BSTree));
	check_mem(map);
	map->compare = (compare ==NULL ? default_compare :compare);
	return map;
	error:
	if(map)
		AVLTree_destroy(map);
	return NULL;
}
static int BSTree_destroy_cb(BSTreeNode *node){
	free(node);
	return 0;
}
static int AVLTree_destroy_cb(AVLNode *node){
	free(node);
	return 0;
}
static int AVLTree_compare_cb(AVLNode *node){
	if(node->left){
		int cmp = default_compare(node->key,node->left->key);
		check((cmp > 0),"Tree is not in order: mid %s left %s",bdata((bstring)node->key),bdata((bstring)node->left->key) );
	}
	if(node->right){
		int cmp1 = default_compare(node->key,node->right->key);
		check((cmp1 <= 0), "Tree is not in order mid %s right %s",bdata((bstring)node->key),bdata((bstring)node->right->key));
	}
	error:
	return -1;
	
}
void AVLTree_check(AVLTree *map){
	if(map){
		AVLTree_traverse(map,AVLTree_compare_cb);
	}
}
void AVLTree_destroy(AVLTree *map){
	if(map){
		BSTree_traverse(map,AVLTree_destroy_cb);
		free(map);
	}
}
void BSTree_destroy(BSTree *map){
	if(map){
		BSTree_traverse(map,BSTree_destroy_cb);
		free(map);
	}
}
static inline int max(int a ,int b){
	return (a>=b) ? a : b ;
}
static int height(AVLNode *a){
	return (a ==NULL) ? 0 : a->height;
}
static int get_balance(AVLNode *a){
	return (height(a->left) - height(a->right));
}

static void right_rotate(AVLNode *a){
	AVLNode *x = a->left;
	AVLNode *T1 = x->right;
	AVLNode *y = a->parent;
	a->parent = x;
	a->left = T1;
	x->right = a;
	x->parent = y;
	if(y){
		if(a == y->left){
			y->left = x;
		}else{
			y->right = x;
		}
	}
	if(T1 !=NULL){
		T1->parent = a;
	}
	a->height = max(height(a->left),height(a->right))+1;
	x->height = max(height(x->left),height(x->right))+1;
	//if(y)
	//	y->height = max(height(y->left),height(y->right))+1;
}
static void left_rotate(AVLNode *a){
	AVLNode *x = a->right;
	AVLNode *T1 = x->left;
	AVLNode *y = a->parent;
	a->parent = x;
	a->right =T1;
	x->left = a;
	x->parent = y;
	if(T1 !=NULL){
		T1->parent = a;
	}
	if(y){
		if(a == y->left){
			y->left = x;
		}else{
			y->right = x;
		}
	}
	a->height = max(height(a->left),height(a->right))+1;
	x->height = max(height(x->left),height(x->right))+1;
	//if(y)
	//	y->height = max(height(y->left),height(y->right))+1;
}
static void rotate(AVLTree *map,AVLNode *node){    // somehow i implement this wrongly, either this or left and right rotate, fix this later.
	if(get_balance(node)>1){
		if(get_balance(node->left)>=0)
			right_rotate(node);
		else{
			debug("1:%x",node->left);
			left_rotate(node->left);
			debug("2:%x",node->left);
			right_rotate(node);
		}
		if(node->parent->parent == NULL)
			map->root = node->parent;   // here damnit, took 2 days to find out the error, so the point here is i should have changed map->root after all operations on the below nodes are done, instead of be4
	}else if(get_balance(node)<-1){     // because in some case node->left and right may be flipped downward.
		if(node->parent == NULL)
			map->root = node->right;
		if(get_balance(node->right)<=0)
			left_rotate(node);
		else{
			debug("3:%x",node->right);
			right_rotate(node->right);
			debug("4:%x",node->right);
			left_rotate(node);
		}
		if(node->parent->parent == NULL)
			map->root = node->parent;
	}
	node->height = max(height(node->left),height(node->right))+1;
}
static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent,void *key,void* data){
	BSTreeNode *node = calloc(1,sizeof(BSTreeNode));
	check_mem(node);
	node->data = data;
	node->key = key;
	node->parent = parent;
	return node;
	error:
	if(node)
		free(node);
	return NULL;
}
static inline AVLNode *AVLNode_create(AVLNode *parent,void* key,void* data){
	AVLNode *node = calloc(1,sizeof(AVLNode));
	check_mem(node);
	node->data = data;
	node->key = key;
	node->parent = parent;
	node->height = 1;
	return node;
	error:
	if(node)
		free(node);
	return NULL;
}
static inline void BSTree_setnode(BSTree *map,BSTreeNode *node,void *key,void* data){ 
	check((node !=NULL),"sth wrong here");

	int cmp = map->compare(node->key,key);
	if(cmp <= 0){
		if(node->left){
			BSTree_setnode(map,node->left,key,data);
	}else{
		node->left = BSTreeNode_create(node,key,data);
		}
	}
	else{
		if(node->right){
			BSTree_setnode(map,node->right,key,data);
		}else{
			node->right = BSTreeNode_create(node,key,data);
		}
	}
	error:
	return NULL;
}
static inline void AVLTree_setnode(AVLTree *map,AVLNode *node,void* key,void* data){
	check(node !=NULL,"node cant be null");
	int cmp = map->compare(node->key,key);
	if(cmp>=0){
		if(node->left){
			AVLTree_setnode(map,node->left,key,data);
		}else{
			node->left = AVLNode_create(node,key,data);
		}
	}else{
		if(node->right){
			AVLTree_setnode(map,node->right,key,data);
		}else{
			node->right = AVLNode_create(node,key,data);
		}
	}
	rotate(map,node);
	error:
	return NULL;
}
int BSTree_set(BSTree *map,void *key,void *data){
	if(map->root == NULL){
		map->root =BSTreeNode_create(NULL,key,data);
		check_mem(map->root);
	}else{
		BSTree_setnode(map,map->root,key,data);
	}
	return 0;
	error:
	return -1;
}
int AVLTree_set(AVLTree *map,void *key,void *data){
	if(map->root == NULL){
		map->root =AVLNode_create(NULL,key,data);
		check_mem(map->root);
	}else{
		AVLTree_setnode(map,map->root,key,data);
	}
	return 0;
	error:
	return -1;
}
static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node,void* key){
	int cmp =map->compare(node->key,key);
	if(cmp ==0){
		return node;
	}else if (cmp >0){
		if (node->right){
			return BSTree_getnode(map,node->right,key);
		}
		else{
			return NULL;
		}
	}else{
		if(node->left){
			return BSTree_getnode(map,node->left,key);
		}else{
			return NULL;
		}
	}
}
static inline AVLNode *AVLTree_getnode(AVLTree *map, AVLNode *node,void* key){
	int cmp =map->compare(node->key,key);
	if(cmp ==0){
		return node;
	}else if (cmp < 0){
		if (node->right){
			return AVLTree_getnode(map,node->right,key);
		}
		else{
			return NULL;
		}
	}else{
		if(node->left){
			return AVLTree_getnode(map,node->left,key);
		}else{
			return NULL;
		}
	}
}
void *BSTree_get(BSTree *map,void* key){
	if(map->root == NULL)
		return NULL;
	BSTreeNode *node = BSTree_getnode(map,map->root,key);
	return (node ==NULL )? NULL : node->data;
}
void *AVLTree_get(AVLTree *map,void* key){
	if(map->root == NULL){
		debug("wow wtf");
		return NULL;
	}
	AVLNode *node = AVLTree_getnode(map,map->root,key);
	return (node ==NULL )? NULL : node->data;
}
static inline int BSTree_traverse_nodes(BSTreeNode *node,BSTree_traverse_cb traverse_cb){
	int rc = 0;
	if(node->left){
		rc = BSTree_traverse_nodes(node->left,traverse_cb);
		if(rc != 0)
			return rc;
	}
	if(node->right){
		rc = BSTree_traverse_nodes(node->right,traverse_cb);
		if(rc !=0)
			return rc;
	}
	return traverse_cb(node);
}
static inline int AVLTree_traverse_nodes(AVLNode *node,AVLTree_traverse_cb traverse_cb){
	int rc = 0;
	if(node->left){
		rc = AVLTree_traverse_nodes(node->left,traverse_cb);
		if(rc !=0)
			return rc;
	}
	if(node->right){
		rc = AVLTree_traverse_nodes(node->right,traverse_cb);
		if(rc !=0)
			return rc;
	}
	return traverse_cb(node);
}
int BSTree_traverse(BSTree *map,BSTree_traverse_cb traverse_cb){
	if(map->root){
		return BSTree_traverse_nodes(map->root,traverse_cb);
	}
	return 0;
}
int AVLTree_traverse(AVLTree *map,AVLTree_traverse_cb traverse_cb){
	if(map->root){
		return AVLTree_traverse_nodes(map->root,traverse_cb);
	}
	return 0;
}
static inline BSTreeNode *BSTree_find_min(BSTreeNode *node){
	while(node->left){
		node = node->left;
	}
	return node;
}
static inline AVLNode *AVLTree_find_min(AVLNode *node){
	while(node->left){
		node = node->left;
	}
	return node;
}
static inline void BSTree_replace_node_in_parent(BSTree *map,BSTreeNode *node,BSTreeNode *new_value){
	if(node->parent){
		if(node == node->parent->left){
			node->parent->left = new_value;
		}else{
			node->parent->right = new_value;
		}
	}else{
		map->root = new_value;
	}
	if(new_value){
		new_value->parent = node->parent;
	}
}
static inline void AVLTree_replace_node_in_parent(AVLTree *map,AVLNode *node,AVLNode *new_value){
	if(node->parent){
		if(node== node->parent->left)
			node->parent->left = new_value;
		else
			node->parent->right = new_value;
	}else{
		map->root = new_value;
	}if(new_value){
		new_value->parent = node->parent;
	}
}
static inline void BSTree_swap(BSTreeNode *a,BSTreeNode *b){
	void *temp = NULL;
	temp = b->key;
	b->key = a->key;
	a->key = temp;
	temp = b->data;
	b->data = a->data;
	a->data = temp;
}
static inline void AVLTree_swap(AVLNode *a,AVLNode *b){
	void* temp = NULL;
	temp = b->key;
	b->key = a->key;
	a->key = temp;
	temp = b->data;
	b->data = a->data;
	a->data = temp;
}
static inline AVLNode *AVL_node_delete(AVLTree *map,AVLNode *node,void*key){
	AVLNode *deleted = NULL;
	check(node !=NULL,"node cant be null");
	int cmp = map->compare(node->key,key);
	if (cmp > 0){
		if(node->left != NULL){
			return AVL_node_delete(map,node->left,key);
		}
		else{
			return NULL;
		}
	}else if(cmp < 0){
		if(node->right){
			return AVL_node_delete(map,node->right,key);
		}else{
			return NULL;
		}
	}else{
		if(node->left && node->right){
			AVLNode *successor = AVLTree_find_min(node->right);
			AVLTree_swap(successor,node);
			AVLTree_replace_node_in_parent(map,successor,successor->right);
			deleted = successor;
			return successor;
		}else if(node->left){
			AVLTree_replace_node_in_parent(map,node,node->left);
			deleted = node;
			return node;
		}else if(node->right){
			AVLTree_replace_node_in_parent(map,node,node->right);
			deleted = node;
			return node;
		}else{
			AVLTree_replace_node_in_parent(map,node,NULL);                     
			deleted  = node;
			return node;
		}
		AVLNode *root = NULL;
			root =deleted->parent;
		while(root){
			rotate(map,root);
			root = root->parent;
		}
	}
	error:
	return NULL;
}
static inline BSTreeNode *BSTree_node_delete(BSTree *map,BSTreeNode *node,void* key){
	int cmp = map->compare(node->key,key);
	if(cmp<0){
		if(node->left !=NULL){
			return BSTree_node_delete(map,node->left,key);
		}else{
			return NULL;
		}
	}else if(cmp>0){
		if(node->right){
			return BSTree_node_delete(map,node->right,key);
		}else{
			return NULL;
		}
	}else{
		if(node->left && node->right){
			BSTreeNode *successor = BSTree_find_min(node->right);
			BSTree_swap(successor,node);
		BSTree_replace_node_in_parent(map,successor,successor->right);
		return successor;
	}else if (node->left){
		BSTree_replace_node_in_parent(map,node,node->left);
	}else if (node->right){
		BSTree_replace_node_in_parent(map,node,node->right);
	}else{
		BSTree_replace_node_in_parent(map,node,NULL);
	}
	return node;
	}
}
void* BSTree_delete(BSTree *map,void* key){
	void *data = NULL;
	if(map->root){
		BSTreeNode *node = BSTree_node_delete(map,map->root,key);
		if(node){
			data = node->data;
			free(node);
		}
	}
	return data;
}
void* AVLTree_delete(AVLTree *map,void* key){
	void *data = NULL;
	if(map->root){
		AVLNode *node = AVL_node_delete(map,map->root,key);
		if(node){
			data = node->data;
			free(node);
		}
	}
	return data;
}
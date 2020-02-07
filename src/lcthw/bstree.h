#ifndef lcthw_BSTree_h
#define lcthw_BSTree_h
typedef struct BSTreeNode{
	void *key;
	void *data;
	struct BSTreeNode *parent;
	struct BSTreeNode *left;
	struct BSTreeNode *right;
}BSTreeNode;
typedef struct  AVLNode
{
	void *key;
	void *data;
	struct AVLNode *parent;
	struct AVLNode *left;
	struct AVLNode *right;
	int height;
}AVLNode;
typedef int (*BSTree_compare)(void* a,void* b);
typedef struct BSTree
{
	int count;
	BSTree_compare compare;
	BSTreeNode *root;
}BSTree;
typedef struct AVLTree{
	int count;
	BSTree_compare compare;
	AVLNode *root;
}AVLTree;
typedef int (*BSTree_traverse_cb)(BSTreeNode *node);
typedef int (*AVLTree_traverse_cb) (AVLNode *node);
BSTree *BSTree_create(BSTree_compare compare);
AVLTree *AVLTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree *map);
void AVLTree_destroy(AVLTree *map);
int BSTree_set(BSTree *map,void* key,void* data);
int AVLTree_set(AVLTree *map,void* key,void* data);
void* BSTree_get(BSTree *map,void* key);
void* AVLTree_get(AVLTree *map,void* key);
void* AVLTree_delete(AVLTree *map,void* key);
int BSTree_traverse(BSTree 	*map,BSTree_traverse_cb traverse_cb);
int AVLTree_traverse(AVLTree *map,AVLTree_traverse_cb traverse_cb);
void* BSTree_delete(BSTree *map,void* key);
#endif
#ifndef __LCTHW_SUFFIX_TREE_
#define __LCTHW_SUFFIX_TREE_
#include <lcthw/darray.h>
#include <lcthw/bstrlib.h>
int remainder;
typedef struct Suffix_Node Suffix_Node;
typedef struct Suffix_Edge{
	int start;
	int* end;
	Suffix_Node * node;
}Suffix_Edge;
typedef struct Suffix_Node{
	Suffix_Edge* edges[26];
	struct Suffix_Node* link;
}Suffix_Node;
typedef struct Active_point{
	Suffix_Node *node;
	char order;
	int active_length;
}Active_point;
void Suffix_link (Suffix_Node *node1 ,Suffix_Node *node2);
Suffix_Node* Suffix_tree_build(bstring string);
static inline int Suffix_active_point(Active_point point){
	if(point.order == '\0')
		return -1;
	Suffix_Node *node = point.node;
	Suffix_Edge *edge = node->edges[point.order-97];
	if(edge ==NULL)
		return -1;
	return edge->start + point.active_length;
}
#endif
#include <lcthw/suffix_tree.h>
#include <lcthw/dbg.h>
#include <stdio.h>
#include <lcthw/bstrlib.h>

Suffix_Node* Suffix_create_node(Suffix_Edge *edge, int active_length){
	Suffix_Node *node = NULL;
	if(edge == NULL){
		log_warn("Creating root node, if you see this line twice, then there are 2 roots in your tree");
		node = malloc(sizeof(Suffix_Node));
	}else{
		node = malloc(sizeof(Suffix_Node));
		edge->node = node;
		int* temp = edge->end;
		edge->end = malloc(sizeof(int));
		*(edge->end) = edge->start + active_length -1;
		Suffix_Edge* new_edge = malloc(sizeof(Suffix_Edge));
		new_edge->start = *(edge->end) + 1;
		node->edges[new_edge->start] = new_edge;
		new_edge->end = temp;
	}
	return node;
}
void Suffix_create_edge(Suffix_Node *node,char order, int* end){
	 Suffix_Edge* edge = malloc(sizeof(Suffix_Edge));
	 edge->start = *end;
	 edge->end = end;
	 node->edges[order-97] = edge;
}
Suffix_Edge* get_current_edge(Active_point ap){
	return ap.node->edges[ap.order -97]; // maybe ap.node == NULL, dunno ,bug maybe
}
Suffix_Node* Suffix_tree_build(bstring string){
	Suffix_Node *root = Suffix_create_node(NULL,NULL);
	Active_point ap = {.node = root, .order = '\0',.active_length = 0};
	Suffix_Node *node = root;
	Suffix_Node *prev_node = NULL;
	int temp_num = 0;
	int remain = 0;
	int length = blength(string);
	char* String = bdata(string);
	for(int end =0; end <length;end++){
		++remain;
		debug("\nend: %c",String[end]);
		prev_node = NULL;
		while(remain > 0){
		if(ap.active_length == 0){
			if(ap.node->edges[String[end]-97] ==NULL){
				Suffix_create_edge(ap.node,String[end],&end);
				remain--;
				if(prev_node !=NULL)
					prev_node->link = ap.node;
				prev_node = ap.node;
				ap.node = ap.node->link ? ap.node->link : root;
				debug(" creating new edge node: %p \n prev_node %p:\n active_node : %p active_edge: %c active_len: %d \n remain: %d ",node,prev_node,ap.node,ap.order,ap.active_length,remain);
			}else{
				ap.order = String[end];
				ap.active_length++;
				debug("already exist edge node: %p \n prev_node %p:\n active_node : %p active_edge: %c active_len: %d \n remain: %d ",node,prev_node,ap.node,ap.order,ap.active_length,remain);
				break;
			}
		}else{
			while(Suffix_active_point(ap)>*(get_current_edge(ap)->end)){
				temp_num = *(get_current_edge(ap)->end) +1; //why becausethe former active_len prove that it was smaller than the former node end =>it is a continuous char streams 
				ap.active_length = Suffix_active_point(ap) - temp_num;
				ap.node = get_current_edge(ap)->node;
				ap.order = String[temp_num]; 
				debug(" active_node : %p active_edge: %c active_len: %d ",ap.node,ap.order,ap.active_length);
			}if(String[Suffix_active_point(ap)] == String[end]){
				ap.active_length++;
				debug("already exist suffix,node: %p \n prev_node %p:\n active_node : %p active_edge: %c active_len: %d \n remain: %d ",node,prev_node,ap.node,ap.order,ap.active_length,remain);
				break;
			}else{
				 node =Suffix_create_node(get_current_edge(ap),ap.active_length);
				 Suffix_create_edge(node,String[end],&end);
				 if(prev_node != NULL)
				 	prev_node->link = node;
				 prev_node = node;
				 remain--;
				 if(ap.node == root){
				 	ap.active_length--;                              //hmm this whole shit need code review
				 	ap.order = String[end -ap.active_length];
				 }else{
				 	debug("ap.node %p",ap.node);
				 	ap.node = ap.node->link;
				 }
				 debug("splitting existing edge, node: %p \n prev_node %p:\n active_node : %p active_edge: %c active_len: %d \n remain: %d ",node,prev_node,ap.node,ap.order,ap.active_length,remain);
			}
		}
	}
}
return root;
}


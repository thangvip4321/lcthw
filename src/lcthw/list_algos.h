#ifndef lcthw_list_algos
#define lcthw_list_algos
#include <lcthw/list.h>
typedef int (*List_compare) ( void*A, void*B );
int List_bubble_sort(List *list,List_compare cmp);
List* List_merge_sort(List *list,List_compare cmp);
int List_insert_sorted(List *list,List_compare cmp,void* value);
#endif
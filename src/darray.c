#include <lcthw/darray.h>
#include <assert.h>
static void swap(void** array,int a,int b){
	void *temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}
int DArray_bin_search(DArray *array,void *el,compare_DA cmp){
	int start = 0;
	int end = array->end-1;
	void** contents = array->contents;
	while(start<=end){
		int middle = start+(end-start)/2;
		if(cmp(contents[middle],el)==0)
			return middle;
		else if(cmp(contents[middle],el)>0)
			start = middle+1;
		else
			end = middle-1;
	}
	return -1;
}
DArray *DArray_create(size_t element_size,size_t initial_max){
	DArray *array =malloc(sizeof(DArray));
	check_mem(array);
	array->max =initial_max;
	check(array->max>0 ,"You must set an initial_max >0");
	array->contents = calloc(initial_max,sizeof(void*));
	check_mem(array->contents);
	array->end =0;
	array->element_size = element_size;
	array->expand_rate =DEFAULT_EXPAND_RATE;
	return array;
	error:
	DArray_destroy(array);
	return NULL;
}
void DArray_clear(DArray *array){
	int i=0;
	check(array != NULL,"invalid array");
	if(array->element_size>0){
		for(i=0;i<array->max;i++){
			if(array->contents[i]!=NULL)
				free(array->contents[i]);
		}
	}
	error:
	return;
}
static inline int DArray_resize(DArray *array,size_t newsize){
	array->max =newsize;
	check(array->max >0,"The new size must >0");
	void* contents	=realloc(array->contents,array->max *sizeof(void *));
	check_mem(contents);
	array->contents = contents;
	return 0;
	error:
	return -1;
}
int DArray_expand(DArray *array){
	size_t old_max =array->max;
	check(DArray_resize(array,array->max +array->expand_rate)==0,"Failed to expand array to a new size %d",array->max +(int)array->expand_rate);
	memset(array->contents + old_max,0,array->expand_rate +1);
	return 0;
	error:
	return -1;
}
int DArray_contract(DArray *array){
	int new_size = array->end < (int) array->expand_rate ? (int) array->expand_rate : array->end;
	return DArray_resize(array,new_size +1);
}
void DArray_destroy(DArray * array){
	if(array){
		if(array->contents)
			free(array->contents);
		free(array);
	}
}
void DArray_clear_destroy(DArray *array){
	DArray_clear(array);
	DArray_destroy(array);
}
int DArray_push(DArray *array,void *el){
	array->contents[array->end] = el;
	array->end++;
	if(DArray_end(array)>=DArray_max(array))
		return DArray_expand(array);
	else{
		return 0;
	}	
}
void *DArray_pop(DArray *array){
	check(array->end -1 ==0 ,"Attempt to pop from empty array");
	void *el = DArray_remove(array,array->end-1);
	array->end--;
	if(DArray_end(array)> (int)array->expand_rate && DArray_end(array) % array->expand_rate){
		DArray_contract(array);
	}
	return el;
	error:
	return NULL;
}
int DArray_push_sort(DArray *array,void *el,compare_DA cmp){
	int rc =DArray_push(array,el);
	check(rc == 0,"failed to push element");
	for(int i =array->end -1;i>0;i--){
		if(cmp(array->contents[i],array->contents[i-1])>0)
			break;
		else{
			swap(array->contents,i,i-1);
		}
	}
	return 0;
	error:
	return -1;
}
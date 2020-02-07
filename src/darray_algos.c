#include <lcthw/darray_algos.h>
#include <stdlib.h>
static void swap(void** array,int a,int b){
	void* temp = array[a];
	array[a] =array[b];
	array[b] = temp;
}
static int quicksort(void **array,int start,int end,compare_DA cmp){
	if((end-start) >=1){
		void* pivot = array[end];
	int i = start-1;
	int j = start;
	for(j=start;j<=end-1;j++){
		if(cmp(array[j],pivot)<0){
			i++;
			swap(array,i,j);
		}
	}
		swap(array,end,i+1);
		debug("start %d -end %d",start,end);
		debug("piv_location: %d, pivot %s,same %s",i+1,pivot,array[i+1]);
		quicksort(array,start,i,cmp);
		quicksort(array,i+2,end,cmp);
	}
}
static inline int build_maxheap(void **array,int heapsize,compare_DA cmp){
	if((heapsize-1)%2 == 0){
		for(int i = heapsize-1;i>0;i -= 2){
			int large = 0;
			if(cmp(array[i],array[i-1])>0){
				large = i;
			}else{
				large = i-1;
			}if(cmp(array[(i-1)/2],array[large])<0){
				swap(array,(i-1)/2,large);
			}
		}
	}else{
		for(int i = heapsize-1;i>0;i-=2){
			int large = 0;
			if(i == heapsize-1 || (cmp(array[i],array[i+1])>0))
				large = i;
			else{
				large = i+1;
			}if(cmp(array[(i-1)/2],array[large])<0){
				swap(array,(i-1)/2,large);
			}
		}
	}
}
static int heapsort(void **array,int heapsize,compare_DA cmp){
	build_maxheap(array,heapsize,cmp);
	for(int i = heapsize-1;i>0;i--){
		swap(array,0,i);
		build_maxheap(array,i,cmp);

	}
	return 1;
}
int DArray_qsort(DArray *array,compare_DA cmp){
	quicksort(array->contents,0,DArray_end(array)-1,cmp);
	return 1;
}
int DArray_heapsort(DArray *array,compare_DA cmp){
	return heapsort(array->contents,DArray_count(array),cmp);
}
static void** merger(void **ar1, void **ar2, int num1,int num2,compare_DA cmp){
	check((ar1 !=NULL) && (ar2 !=NULL),"null array not allowed");
	if(!ar1[0])
		return ar2;
	if(!ar2[0])
		return ar1;
	void **ar = calloc(sizeof(void *),num1+num2);
	int order = 0 ;
	int i = 0;
	int j = 0;
	while((i <num1) && (j<num2)){
		if((cmp(ar1[i],ar2[j])>0)){
			ar[order] = ar2[j];
			order++;
			j++;
		}else{
			ar[order] = ar1[i];
			order++;
			i++;
		}
	}
	if(i == num1){
		for(int k = order;k <(num1+num2);k++,i++){
			ar[k] =ar2[j];
			}
		}else{
			for(int k = order;k <(num1+num2);k++,i++){
				ar[k] = ar1[i];
			}
		}
		free(ar1);
		free(ar2);	
		return ar;
		error:
		return NULL;
}
static void** mergesort(void** array,int count, compare_DA cmp){
	if(count <=1){
		return array;
	}
	int middle = count/2;
	debug("sorting array: %lx count %d",array,count);
	void** array1 = calloc(middle,sizeof(void*));
	void** array2 = calloc(count-middle,sizeof(void*));
	int j = 0;
	for(int i = 0 ;i<middle;i++){
		array1[i] =array[j];
		j++;
	}
	for(int k = 0;k <count-middle;k++){
		array2[k]=array[j];
		j++;
	}
	void** sort_left = mergesort(array1,middle,cmp);
	void** sort_right = mergesort(array2,count-middle,cmp);
	debug("left and right and count %lx ,%lx,%d",sort_left,sort_right,count);
	if(sort_left != array1)
		free(array1);
	if(sort_right != array2)
		free(array2);
	return merger(sort_left,sort_right,middle,count-middle,cmp);
}
int DArray_mergesort(DArray *array,compare_DA cmp){
	void** ar1 =  mergesort(array->contents,DArray_count(array),cmp);
	array->contents = ar1;
	return 1;
}

#include <stdio.h>
#include <lcthw/radixmap.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <stdlib.h>
RadixMap *RadixMap_create(size_t max){
	RadixMap *map =calloc(sizeof(RadixMap),1);
	check_mem(map);
	map->contents = calloc(sizeof(RMElement),max);
	check_mem(map->contents);
	map->temp = calloc(sizeof(RMElement),max);
	check_mem(map->temp);
	map->max =max;
	map->end = 0;
	return map;
	error:
	return NULL;
}
void RadixMap_destroy(RadixMap *map){
	check(map,"map should not be null");
	free(map->contents);
	free(map->temp);
	free(map);
	error:
	return;
}
#define byteof(x,y) (((uint8_t *)x)[(y)])
static inline void radix_sort(short offset,uint64_t max,uint64_t *source,uint64_t *dest){
	uint64_t count[256]={0};
	uint64_t *cp =NULL;
	uint64_t *sp =NULL;
	uint64_t *end =NULL;
	uint64_t s = 0;
	uint64_t c = 0;
	//count occurences of every byte values
	for(sp =source,end = source +max;sp<end;sp++){
		count[byteof(sp,offset)]++;
	}
	//transform count in2 index by summing elements and storing into same array
	for (s=0,cp = count,end = count+256;cp<end;cp++){
		c = *cp;
		*cp =s;
		s +=c;
	}
		// {1,3,0,6,0,0,0,3,4}->{0,1,4,4,10,10,10}
	// fill dest with the right values in the right place
	for(sp = source,end = source+max; sp<end;sp++){
		cp = count + byteof(sp,offset);
		dest[*cp] = *sp;
		++(*cp);
	}

}
void RadixMap_sort(RadixMap *map,int key_start){
	uint64_t *source =&map->contents[key_start].raw;
uint64_t *temp = &map->temp[key_start].raw;
radix_sort(0,map->end - key_start,source,temp);
radix_sort(1,map->end - key_start,temp,source);
radix_sort(2,map->end - key_start,source,temp);
radix_sort(3,map->end - key_start,temp,source);
}
RMElement *RadixMap_find(RadixMap * map,uint32_t to_find){
	int low = 0 ;
	int high =map->end -1;
	RMElement *data = map->contents;
	while(low<=high){
		int middle =low + (high-low)/2;
		uint32_t key =data[middle].data.key;
		if(to_find <key){
			high = middle -1;
		}
		else if(to_find >key){
			low = middle +1;
		}else{
			return &data[middle];
		}
	}
	return NULL;
}
int RadixMap_add(RadixMap *map,uint32_t key, uint32_t value){
	check((key < UINT32_MAX) ,"key cant be equal to UINT32_MAX");
	RMElement element = {.data={.key=key,.value=value}};
	map->contents[map->end++] = element;
	int begin=0;
	int end = map->end-1;
	int sortkey=0;
	RMElement *data = map->contents;
	while(begin<end-1){
		int middle = (begin+end)/2;
		uint32_t midkey = data[middle].data.key;
		if(key>midkey){
			begin = middle;
		}
		else if(key<midkey){
			end = middle;
		}
		else{
			sortkey = middle;
			break;
		}
		sortkey = end-1;
	}
	RadixMap_sort(map,sortkey);
	return 0;
	error:
	return -1;
}
int RadixMap_delete(RadixMap *map,RMElement *el){
	check(map->end >0,"there's nothing to delete");
	check(el != NULL,"cant delete a null element");
	el->data.key =UINT32_MAX;
	if(map->end>1){
		RadixMap_sort(map,0);
	}
	map->end--;
	return 0;
	error:
	return -1;
}


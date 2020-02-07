#include <lcthw/string_algos.h>
#include <lcthw/darray.h>
#include <lcthw/bstrlib.h>
#include <lcthw/dbg.h>

 void String_setup_skipchars(char*needle,int nlen,size_t* skip_chars){
	int last = nlen-1;
	for(int i =0;i<UCHAR_MAX;i++){
		skip_chars[i] =nlen;
	}
	for(int i =0;i<last;i++){
		skip_chars[needle[i]] = last - i; // not same variable type here, size_t vs int 
	}
}
static inline const unsigned char* String_base_search(const unsigned char* haystack,ssize_t hlen,const unsigned char* needle,ssize_t nlen,size_t *skip_chars){
	size_t i=0;
	size_t last = nlen-1;
	check((haystack !=NULL),"inappropriate haystack");
	check((needle !=NULL),"invalid substring to find");
	check((nlen >0),"nlen should >0");
	check((hlen >0),"hlen should >0");
	while(hlen>nlen){
		for(i = last;haystack[i] ==needle[i];i--){
			if(i==0)
				return haystack;
		}
		hlen -= skip_chars[haystack[last]];
		haystack +=skip_chars[haystack[last]];
		
	}
	error:
	return NULL;
}
int String_find(bstring in,bstring what){
	static int set = 0;
	 int nlen = blength(what);
	 const unsigned char* needle = bdata(what);
	 ssize_t hlen = blength(in);
	 const unsigned char* haystack = bdata(in);

	static size_t skip_chars[UCHAR_MAX+1] = {0};
	if(set == 0){
		set =1;
		String_setup_skipchars(needle,nlen, skip_chars);
	} 
	const unsigned char *found = String_base_search(haystack,hlen,needle,nlen,skip_chars);
	return (found !=NULL) ? (found -haystack) : -1;
}
StringScanner *StringScanner_create(bstring in){
	StringScanner *scan = calloc(1,sizeof(StringScanner));
	check_mem(scan);
	scan->in = in;
	scan->haystack = (const unsigned char*) bdata(in);
	scan->hlen = blength(in);
	assert(scan != NULL && "fuck");
	return scan;
	error:
	free(scan);
	return NULL;
}
static inline void StringScanner_set_needle(StringScanner *scan,bstring what){
	scan->needle = (const unsigned char*) bdata(what);
	scan->nlen = blength(what);
	String_setup_skipchars(scan->needle,scan->nlen,scan->skip_chars);
}
static inline void StringScanner_reset(StringScanner *scan){
	scan->haystack = (const unsigned char*) bdata(scan->in);
	scan->hlen = blength(scan->in);
}
int StringScanner_scan(StringScanner *scan,bstring tofind){
	const unsigned char* found = NULL;
	ssize_t found_at = 0;
	if(scan->hlen <=0){
		StringScanner_reset(scan);
		return -1;
	}
	if((const unsigned char*)bdata(tofind) != scan->needle){
		StringScanner_set_needle(scan,tofind);
	}
	found = String_base_search(scan->haystack,scan->hlen,scan->needle,scan->nlen,scan->skip_chars);
	if(found){
		found_at = found -(const unsigned char*)bdata(scan->in);
		scan->haystack = found +scan->nlen;
		scan->hlen -=found_at + scan->nlen ;//sth wrong here
	}else{
		StringScanner_reset(scan);
		found_at = -1;
	}
	return found_at;
}
void StringScanner_destroy(StringScanner *scan){
	if(scan){
		free(scan);
	}
}

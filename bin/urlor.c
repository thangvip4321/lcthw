#include <lcthw/tstree.h>
#include <lcthw/bstrlib.h>
typedef int (*func_cb)(int mode,char* url);
typedef struct handler{
	char* dest;
	func_cb cb;
	int mode;
}handler;
int read_url(int mode,char*url){
	printf("mode: %d, url: %s",mode,url);
}
TSTree* add_route_data(TSTree *routes,bstring line){
	struct bstrList *data = bsplit(line,' ');
	check((data->qty ==2),"Line '%s' does not have 2 columns",bdata(line));
	handler* handle1 = calloc(1,sizeof(handler));
	handle1->dest = bstrcpy(data->entry[1]);
	handle1->mode = atoi(bstrcpy(data->entry[2]));
	handle1->cb = read_url;
	routes = TSTree_insert(routes,bdata(data->entry[0]),blength(data->entry[0]),handle1);
	bstrListDestroy(data);
	return routes;
	error:
	return NULL;
}
TSTree* load_routes(const char* file){
	TSTree* routes = NULL;
	bstring line = NULL;
	FILE* routes_map = NULL;
	routes_map = fopen(file,"r");
	check((routes_map !=NULL),"Failed to open routes: %s",file);
	while((line = bgets((bNgetc) fgetc,routes_map,'\n')) !=NULL){
		check(btrimws(line) ==BSTR_OK,"Failed to trim line");
		routes = add_route_data(routes,line);
		check((routes !=NULL),"failed to add route");
		bdestroy(line);
	}
	fclose(routes_map);
	return routes;
	error:
	if(routes_map)
		fclose(routes_map);
	if(line)
		bdestroy(line);
	return NULL;
}
bstring match_url(TSTree *routes,bstring url){
	bstring route = TSTree_search(routes,bdata(url),blength(url));
	if(route == NULL){
		printf("No exact match found, trying prefix. \n");
		route = TSTree_search_prefix(routes,bdata(url),blength(url));
	}
	return route;
}
bstring read_line(const char* prompt){
	printf("%s",prompt);
	bstring result = bgets((bNgetc) fgetc,stdin,'\n');
	check_debug((result !=NULL),"stdin close");
	check((btrimws(result) == BSTR_OK),"failed to trim");
	return result;
	error:
	return NULL;
}
void bdestroy_cb(void* value,void *ignored){
	(void)ignored;
	bdestroy((bstring) value);
}
void destroy_routes(TSTree *routes){
	TSTree_traverse(routes,bdestroy_cb,NULL);
	TSTree_destroy(routes);
}
int main(int argc,char* argv[]){
	bstring url = NULL;
	bstring route = NULL;
	TSTree *routes = NULL;
	check((argc == 2),"USAGE: urlor <urlfile>");
	routes = load_routes(argv[1]);
	check((routes !=NULL),"your route file has an error");
	#if 0
	while(1){
		url = read_line("URL> ");
		check_debug((url !=NULL),"goodbye");
		route = match_url(routes,url);

		if(!route){
			printf("Failed: %s \n",bdata(url));
		}else{
			printf("MATCH: %s == %s\n",bdata(url),bdata(route));
		}
		bdestroy(url);
	}
	destroy_routes(routes);
	#endif
	return 0;
	error:
	destroy_routes(routes);
	return 1;
}

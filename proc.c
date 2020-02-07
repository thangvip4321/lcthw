#include <math.h>
#include <lcthw/stats.h>
#include <stdio.h>
#include <lcthw/darray.h>
#include <lcthw/dbg.h>
#define HEAD_NUM 20
typedef struct column{
	char* header;
	double* data;
}column;
 // maybe not initialized right
char wow[100] = {0};
int main(int argc, char* argv[]){
	DArray *headers = DArray_create(sizeof(char*),HEAD_NUM);
	char* file_name = argv[1];
	FILE *stream = fopen(file_name,"r");
	int line = 0;
	char buffer;
	char* name = malloc(20);
	char num[16] ={0};
	double *doub = malloc(sizeof(double));
	char i = 0;
	char spaced = 0;
	int rc = 0 ;
	int elms =0;
	DArray **contents = NULL;
	check(stream != NULL,"wtf happened?");
	while (line == 0){
		buffer = fgetc(stream);
		if (buffer == '\n'){
			++line;
		}
		if(buffer != '\n' && buffer != ' '){
			check((i <=19),"exceed the maximum amount of words"); 
			name[i] = buffer;
			++i;
			spaced = 0;
		}else if (spaced == 0){
			rc = DArray_push(headers,name);
			check(rc == 0,"Sth went wrong here");
			name = malloc(20);
			spaced = 1;
			i = 0 ;
		}
	}
	contents = calloc(DArray_count(headers),sizeof(DArray*));
	for(i = 0; i<DArray_count(headers);i++){
		contents[i]= DArray_create(sizeof(double),20);
	}
	i = 0 ;
	debug("wtf %d",feof(stream));

	while (feof(stream) == 0){
		debug(" line %d, elements %d charnum %d and charread %c ",line,elms,i,buffer);
		buffer = fgetc(stream);
		if (buffer != '\n' && buffer != ' '){
			check((elms < DArray_count(headers)),"too many numbers on a line");
			check((i <= 15),"exceed the maximum amount of nums"); //error here with i?
			num[i] = buffer;
			++i;
			spaced = 0;
		}else if (spaced ==0){
			sscanf(num,"%lf",doub);
			debug(" %s %lf",num,*doub);
			DArray_push(contents[elms],doub);
			doub = malloc(sizeof(double));
			++elms;
			spaced = 1;
			i = 0;
		}
		if(buffer == '\n'){
			++line;
			elms = 0;
		}
	}
	if(spaced == 0){
		sscanf(num,"%lf",doub);
		DArray_push(contents[elms],doub);
	}
	Stats* medium;
	for(int c = 0; c <DArray_count(headers);c++){
		medium = Stats_create();
		debug("%s  ",headers->contents[c]);
		for(int z = 0; z <DArray_count(contents[i]);z++){
			double data = *(double*) ((contents[c]->contents)[z]);
			debug("%f",data);
			Stats_sample(medium,*(double*)(contents[c]->contents[z]));
		}
		Stats_dump(medium);
	}

	fclose(stream);
	error:
	if(headers !=NULL)
		DArray_clear_destroy(headers);
	if(contents != NULL){
		for(i =0; i<DArray_count(headers);i++){
			DArray_clear_destroy(contents[i]);
	}
	free(contents);
	}
	fclose(stream);
}
#include <lcthw/dbg.h>
#include <lcthw/stats.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char* argv[]){
	char* path = argv[1];
	check(path != NULL,"need a filename");
	FILE *file = fopen(path,"r");
	char buffer;
	char nums[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int rc = 0;
	int i = 19;
	int num = 0;
	double array[200];
	int count = 0;
	Stats *stats = Stats_create();
	while(feof(file) ==0){
		rc = fread(&buffer,1,1,file);
		check(rc ==1,"sth went wrong");
		if(buffer != '\n' && buffer != ' '){
			check((i >=0),"exceeded the amount of expected chars");
			nums[i] = buffer;
			i--;
		}else{
			sscanf(nums,"%f",&array[count]);
			for(int c = i+1;c<20;c++){
				nums[c]= 0;
			}
			i = 19;
			++count;
		}
	}
	for(i = 0;i<count;i++){
		print(array[i]);
		Stats_sample(stats,array[i]);
	}
	Stats_dump(stats);
	fclose(file);
	return 0;
	error:
	if(file !=NULL)
		fclose(file);
	return -1;
}

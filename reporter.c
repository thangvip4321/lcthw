#include <lcthw/dbg.h>
#include <lcthw/stats.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char* argv[]){
	char* path = argv[1];
	check(path != NULL,"need a filename");
	FILE *file = fopen(path,"r");
	char buffer;
	char nums[20] ={0};
	int rc = 0;
	int i = 0;
	int num = 0;
	double array[200];
	int count = 0;
	double mid = 0;
	int spaced = 0; 
	Stats *stats = Stats_create();
	while(feof(file) ==0){
		buffer = fgetc(file);
		if(buffer != '\n' && buffer != ' '){
			check((i <= 19),"exceeded the amount of expected chars");
			nums[i] = buffer;
			i++;
			spaced = 0;
		}else if(spaced == 0){
			sscanf(nums,"%lf",&mid); // not %f
			//printf("%f",mid);
			array[count] = mid;
			for(int c = 0;c<i+1;c++){
				nums[c]= 0;
			}
			i = 0;
			++count;
			spaced = 1;
		}
	}
	if(spaced ==0){
		sscanf(nums,"%lf",&mid);
		array[count] = mid;
		++count;
	}
	for(i = 0;i<count;i++){
		debug("%f",array[i]);
		Stats_sample(stats,array[i]);
	}
	Stats_dump(stats);
	fclose(file);
	return 0;
	error:
	fclose(file);
	return -1;
}

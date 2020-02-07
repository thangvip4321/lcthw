#include <math.h>
#include <lcthw/stats.h>
#include <stdlib.h>
#include <lcthw/dbg.h>
Stats *Stats_recreate(double sum,double sumsq,unsigned long n,double min,double max){
	Stats *st = malloc(sizeof(Stats));
	check_mem(st);
	st->sum = sum;
	st->sumsq = sumsq;
	st->n = n;
	st->min = min;
	st->max = max;
	return st;
	error:
	return NULL;
}
Stats *Stats_create(){
	return Stats_recreate(0.0,0.0,0L,0.0,0.0);
}
void Stats_sample(Stats *st,double s){
	st->sum +=s;
	st->sumsq += s*s;
	if(st->n ==0){
		st->min = s;
		st->max = s;
	}else{
		st->min = (st->min > s) ? s : st->min;
		st->max = (st->max < s) ? s : st->max;
	}
	st->n +=1;
}
void Stats_dump(Stats *st){
	fprintf(stderr,"sum : %f, sumsq %f, n: %ld,min:%f, max: %f,mean: %f, stddev: %f \n",st->sum,st->sumsq,st->n,st->min,st->max,Stats_mean(st),Stats_stddev(st));
}

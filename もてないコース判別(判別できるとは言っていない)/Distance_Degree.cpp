#include <iostream>
#include <string>
#include <iostringstream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATANUM 255 //the number of distance range
#define DELTA 30 //the number of degree range

bool JudgeRunningCollectly(int distance, int degree);

//int MAP[DATANUM]; //MAP[DISTANCE]=DEGREE
double *arr0, *arr1; //arr0:distance arr1:degree
int datanum;

int main(int argv, char* args){

  while(1){
    JudgeRunningCollectly(60, 30);
  }
  
  return 0;
}

bool JudgeRunningCollectly(int distance, int degree){
 bool result = false; 
 int count=0;
 
 while(1){
 	if(count>=datanum){
 		break;
 	}else{
 		if((arr0[count]+10)<=distance && distance<(arr0[++count]-10)){
 			if(arr1[count-1]==degree){
 				result=true;
 			}
 		}
 	}
 }
 
 return result;
}

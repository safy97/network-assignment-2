#include <cstdlib>
#include <stdio.h>
//
// Created by mostafa on 09/12/2019.
//
int checkPlp(unsigned short seed , double plp){
    int num = rand()%100;
    int intPlp = plp*100;
    printf("seed: %d , random number: %d , plp: %d\n",seed,num,intPlp);
    if(num <= intPlp ){
        return  1;
    }else{
        return 0;
    }
}

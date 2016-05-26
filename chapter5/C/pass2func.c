#include<stdio.h>

int func(int a, const int b, int *c, int arr1[], int *arr2){
    //a - pass by value, copyed to this func
    //b - pass by value, not allow to change
    //c - pass by pointer/address, change automaticlly applied to main
    //arr1 - pass by address of array
    //arr2 - pass by pointer to the array
    a = a + 1;
    //b = b + 1; //error
    *c = *c + 1;
    arr1[1] = 100;
    *(arr2+1) = 200;
    return a+b+*c;
}

void main(){
    int a=100, b=200, c=300;
    int arr1[]={1,2,3};
    int temp[]={4,5,6};
    int *arr2=temp;
    int ret;
    ret = func(a,b,&c,arr1,arr2);
    printf("a=%d, b=%d, c=%d, ret=%d\n", a, b, c, ret);
    printf("arr1[1]=%d, arr2[1]=%d\n", arr1[1], *(arr2+1));
}
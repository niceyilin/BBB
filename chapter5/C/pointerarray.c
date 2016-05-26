#include<stdio.h>

int main(){
    double x[] = {100, 200, 300, 400, 500};
    int i = 0, len = 0, j = 0;
    double *p;
    void *vp;
    
    len = sizeof(x)/sizeof(double);
    //printf("len=%d\n", len);
    for(i=0; i<len; i++){
        printf(" %f ", x[i]);
    }
    printf("\n");
    
    p = x;
    //printf("p=%d, *p=%d\n", p, *p);
    for (j=0; j<len; j++){
        printf(" / *p=%5.1f ", *p);
        printf(" p=%p /", p);
        p = p + 1;
    }
    printf("\n");
    
    vp = &x[0];
    for (j=0; j<len; j++){
        printf(" / *vp=%5.1f ", *((double *)vp));
        printf(" vp=%p /", vp);
        vp = vp + sizeof(double);
    }
    printf("\n");
    
    
}
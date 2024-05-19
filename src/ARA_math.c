
#include "ARA_math.h"


int mod(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}
int max(int a, int b){
    return a < b ? b : a;
}
int min(int a, int b){
    return a > b ? b : a;
}

int randint(int a, int b){
    if (a > b)
    {
        ARA_error(VALUE_ERROR);
    }
    if (a == b)
    {
        return a;
    }
    return a + rand()%(b-a);
}

int is_in(int x, int a, int b){
    return x <= max(a,b) && x >= min(a,b);
    
}

int stick_in_range(int x,int a,int b){
    if (x < a)
    {
        return a;
    }
    if (x > b)
    {
        return b;
    }
    return x;
    
}
//------------------------------------------------------------------------------
//
// Name:       pi.c
// 
// Purpose:    compute pi
//
//             
//------------------------------------------------------------------------------

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <openacc.h>

#define N 100000000

int main() {
	double pi = 0.0; long i;
    #pragma acc parallel loop reduction(+:pi)
    for (i=0; i<N; i++){
        double t = (double)((i+0.05)/N);
        pi += 4.0/(1.0+t*t);
        }
    printf("pi = %f\n", pi/N);
    return 0;

}

        #include<stdio.h>
        #include<math.h>
        #include<stdlib.h>
        #include<time.h>
        #include<mpi.h>
        #include"MyMPI.h"

        #define NUMBER_OF_SAMPLES 100000000                       //Total number of neutrons
        #define COMPONENT_OF_SCATTER 0.7                            // Scattering component of C
        #define ABSORBING_COMPONENT  0.3                            // Absorbing  component of C
        #define H 4                            //Thickness of the Plate
        #define NUMBER_OF_ITERATIONS 100000000
        #define BLOCK_LOW(id,p,n)   ((id)*(n)/(p))
        #define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)

        #define MAX(x, y) (((x) > (y)) ? (x) : (y))


        double lagged_fibonacci_generator(long int *xi,int my_rank) /* m=2**32 */
        {

        long int a=75889+(80000*(my_rank*my_rank*my_rank*my_rank)), b=1, m=8294967296+(100000000*(my_rank*my_rank*my_rank));
        double rm=8294967296.0+(50000000*(my_rank*my_rank*my_rank));

        *xi = (*xi*a+b)%m;
        return (double)*xi/rm;
        }



        long int getSeedValue()
        {

        long int i;

        struct timeval tp;

        if (gettimeofday(&tp,(struct timezone *)NULL)==0)
                {
                i=tp.tv_sec+tp.tv_usec;
                i=i%10000000|1;
                return i;
                }
                else {
                return -1;
            }
        }
 

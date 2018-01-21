//Implemented By mihirp1


#include"monte_carlo.h"


int main(int argc, char *argv[])
{
        int rank;                                 //Rank Of Process

        int size;                            //Number of Processors

        MPI_Init(&argc,&argv);

        MPI_Comm_rank(MPI_COMM_WORLD,&rank);                 

        MPI_Comm_size(MPI_COMM_WORLD,&size);             

        long double neutron_direction;                                

        long double neutron_position;    

	long int seed;

	int i=0;

	double uniform_random_number1, uniform_random_number2;

	double  random_number;

        int random_seq_length;

        random_seq_length = NUMBER_OF_SAMPLES;

        double *random_list;

        random_list = malloc(random_seq_length * sizeof(double));

	seed = getSeedValue();

	/* Generation Of Random Number Pairs */
	for(i=1; i<=random_seq_length; i++) 

{
	uniform_random_number1 = lagged_fibonacci_generator(&seed,rank); uniform_random_number2 = lagged_fibonacci_generator(&seed,rank);

	random_number = MAX(uniform_random_number1,uniform_random_number2);

        random_list[i] = random_number; 



  }
    int counter_of_all_neutrons=0;

    int problem_chunk=0;

    double C ;                                      // average distance between the neutron interactions

    C = COMPONENT_OF_SCATTER + ABSORBING_COMPONENT;   

        if(C != 1)
                 {
       
        printf("\nAddition of Scattering and Absorbing Components Should be 1.\n");
        MPI_Finalize();
        exit(1);
                 }          
 
    int a= 1;                                                    
    double uniform_random_number;                                 // Uniform random number

    double distance_travelled_by_neutron_before_collision;                         // Distance travelled by neutron before collision

    double number_of_neutrons_absorbed,number_of_neutrons_reflected,number_of_neutrons_transmitted;

    int count_of_reflected_absorbed_transmitted[3];

    count_of_reflected_absorbed_transmitted[0]=0;

    count_of_reflected_absorbed_transmitted[1]=0;

    count_of_reflected_absorbed_transmitted[2]=0;

    int combined_count_of_reflected_absorbed_transmitted_neutrons[3];

    problem_chunk= BLOCK_SIZE(rank,size,NUMBER_OF_SAMPLES);

    double total_processing_time;                   //Processing Time

    total_processing_time= -MPI_Wtime();

    if(size > NUMBER_OF_SAMPLES)

    {
        if(!rank) 

            printf("Number of processes exceeds the sample space. Please provide less number of processes than stated in the file currently.\n");

        MPI_Finalize();

        exit(1);

    }
    
    for(i=1;i<=problem_chunk;i++)

    {
        uniform_random_number=random_list[i*rank];

        neutron_direction=0;   //Directionality of the neutron as measured in radians between 0 and pi

        neutron_position=0;    //Position of particle in plate(0<=x<=H) where H is the thickness of the plate

        a=1;

        distance_travelled_by_neutron_before_collision=-(1/C)*log(uniform_random_number);

        neutron_position=distance_travelled_by_neutron_before_collision*cos(neutron_direction);

        while(a)
                {
            distance_travelled_by_neutron_before_collision=-(1/C)*log(uniform_random_number);

            neutron_position=distance_travelled_by_neutron_before_collision*cos(neutron_direction);

            if (neutron_position<0)
            {
                count_of_reflected_absorbed_transmitted[0]++;

                a=0;
            }
            else if(neutron_position>=H)
            {
                count_of_reflected_absorbed_transmitted[2]++;

                a=0;
            }
            else
            {
                if(uniform_random_number<(ABSORBING_COMPONENT/C))
                {
                    count_of_reflected_absorbed_transmitted[1]++;

                    a=0;
                }
                else
                {
                    neutron_direction=uniform_random_number*(22.0/7.0);

                    counter_of_all_neutrons++;

                    if(counter_of_all_neutrons==NUMBER_OF_ITERATIONS)

                    {
                        count_of_reflected_absorbed_transmitted[1]++;

                        a=0;

                        counter_of_all_neutrons=0;

                    }
                }
            }
        }
    }
    for(i= 0 ; i < 3 ; i ++)

        MPI_Reduce(&count_of_reflected_absorbed_transmitted[i], &combined_count_of_reflected_absorbed_transmitted_neutrons[i], 1, MPI_INT,MPI_SUM,0, MPI_COMM_WORLD) ; 

    total_processing_time+= MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0)

    {
        number_of_neutrons_reflected=((double)combined_count_of_reflected_absorbed_transmitted_neutrons[0]/(double)NUMBER_OF_SAMPLES)*100.0;

        number_of_neutrons_absorbed=((double)combined_count_of_reflected_absorbed_transmitted_neutrons[1]/(double)NUMBER_OF_SAMPLES)*100.0;

        number_of_neutrons_transmitted=((double)combined_count_of_reflected_absorbed_transmitted_neutrons[2]/(double)NUMBER_OF_SAMPLES)*100.0;

        printf("\nCount of Reflected, Absorbed and Transmitted Neutrons are respectively : %d, %d, %d \n",combined_count_of_reflected_absorbed_transmitted_neutrons[0],combined_count_of_reflected_absorbed_transmitted_neutrons[1],combined_count_of_reflected_absorbed_transmitted_neutrons[2]);

        printf("\nPercentage/Probability of Neutrons Reflected,Absorbed and Transmitted are are: %lf, %lf, %lf\n", number_of_neutrons_reflected, number_of_neutrons_absorbed,   number_of_neutrons_transmitted);

        printf("\nTotal time taken in sec %0.6f\n",total_processing_time);




        printf("\nNeutron Transport Simulation Initial Assumptions :-\n");

        printf("Number Of Samples :  %d\n",NUMBER_OF_SAMPLES);

        printf("Component Of Scatter: %f\n",COMPONENT_OF_SCATTER);

        printf("Component Of Absorption: %f\n",ABSORBING_COMPONENT);

        printf("H - Thickness Of Plate : %d\n",H);



    }
    free(random_list);

    MPI_Finalize();

    return 0;
}

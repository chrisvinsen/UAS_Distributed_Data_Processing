#include <stdio.h> 
#include <stdlib.h>
#include <Windows.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

double F(double x) { // Function to calculate integral value
    return (2 / 3.0 * x - 2);
}
double Integral(float lower, float upper, long total_iteration, int id, int n_processor) { // Function Integral Trapezoidal
    srand(time(NULL));
    double y = 0;

    for (int i = id; i < total_iteration; i += n_processor) {
        double xi = ((upper - lower) * ((float) rand() / RAND_MAX)) + lower; // Random x value from lower to upper
        y += F(xi);
    }
    return y;
}

double GetTime() {
    SYSTEMTIME time;
    GetSystemTime(&time);
    LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds; // msec     
    return(time_ms / 1000.0); // second 
}

int main(int argc, char* argv[]) {
    int id, n_processor;
    float lower_limit = -3, upper_limit = 9;
    long sample;
    double elapsed_time, local_result, global_result;

    if (argc < 2) {
        printf("Error..\nSintak: %s Jumlah Sampel \n", argv[0]); fflush(stdout);
        return 1;
    }
    sample = atoi(argv[1]);

    MPI_Init(&argc, &argv); // Initialize the MPI Execution Environment
    elapsed_time = -GetTime(); // Get the Initial Time

    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Get Id of each processor
    MPI_Comm_size(MPI_COMM_WORLD, &n_processor); // Get a total running processor
    
    // Divide lower limit (-3) and upper limit (9) for each processor
    float start_limit = (id * (upper_limit - lower_limit) / n_processor) + lower_limit;
    float end_limit = (((id + 1) * (upper_limit - lower_limit)) / n_processor) + lower_limit;

    local_result = Integral(start_limit, end_limit, sample, id, n_processor); // Get the local result from each processor

    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); // Add local results to global results

    if (id == 0) {
        double final_result = fabs((upper_limit - lower_limit) / sample * global_result); // Get final result
        elapsed_time += GetTime(); // Get ends time
        printf("Nilai Integral = %lf \nWaktu (Tp)     = %lf second\n", final_result, elapsed_time); fflush(stdout);
    }

    MPI_Finalize(); // Terminates MPI execution environment
    return 0;
}
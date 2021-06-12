#include <iostream>
#include <mpi.h>
#include <Windows.h>

int main(int argc, char* argv[])
{
    int process_id, number_of_processes;
    int grades[] = { 72, 80, 80, 80, 80, 80, 75, 75, 75, 80, 65, 80, 75, 85, 70, 65, 73, 70, 78, 78, 77, 70, 65, 73, 63, 63, 63, 63, 80, 80, 63, 78, 68, 70, 80, 75, 75, 85, 68, 73 };
    int total_data = sizeof(grades) / sizeof(int);
    float sum = 0.0, sum_all = 0.0;
    int max = 0, max_all, min = INT_MAX, min_all, summary_grades[9] = { 0 }, summary_grades_all[9];

    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id); // Get Processor ID
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes); // Get Total Processor

    for (int i = process_id; i < total_data; i += number_of_processes) {
        sum += grades[i];
        max = grades[i] > max ? grades[i] : max;
        min = grades[i] < min ? grades[i] : min;

        if (grades[i] >= 85) summary_grades[0]++; // Grade A
        else if (grades[i] >= 80) summary_grades[1]++; // Grade A-
        else if (grades[i] >= 75) summary_grades[2]++; // Grade B+
        else if (grades[i] >= 70) summary_grades[3]++; // Grade B
        else if (grades[i] >= 65) summary_grades[4]++; // Grade B-
        else if (grades[i] >= 60) summary_grades[5]++; // Grade C+
        else if (grades[i] >= 55) summary_grades[6]++; // Grade C
        else if (grades[i] >= 45) summary_grades[7]++; // Grade D
        else summary_grades[8]++; // Grade E
    }

    MPI_Reduce(&sum, &sum_all, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD); // Gather total grades
    MPI_Reduce(&max, &max_all, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD); // Get Max Grades
    MPI_Reduce(&min, &min_all, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); // Get Min Grades
    MPI_Reduce(&summary_grades, &summary_grades_all, 9, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (process_id == 0) {
        printf("Nilai rata-rata = %.2f\n", sum_all / total_data);
        printf("Nilai tertinggi = %d\n", max_all);
        printf("Nilai terendah  = %d\n", min_all);
        printf("Dapat nilai A   = %d orang\n", summary_grades_all[0]);
        printf("Dapat nilai A-  = %d orang\n", summary_grades_all[1]);
        printf("Dapat nilai B+  = %d orang\n", summary_grades_all[2]);
        printf("Dapat nilai B   = %d orang\n", summary_grades_all[3]);
        printf("Dapat nilai B-  = %d orang\n", summary_grades_all[4]);
        printf("Dapat nilai C+  = %d orang\n", summary_grades_all[5]);
        printf("Dapat nilai C   = %d orang\n", summary_grades_all[6]);
        printf("Dapat nilai D   = %d orang\n", summary_grades_all[7]);
        printf("Dapat nilai E   = %d orang\n", summary_grades_all[8]);
    }

    MPI_Finalize(); // Finalize and close MPI
}
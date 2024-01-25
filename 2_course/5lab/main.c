#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void partition(int* v, uint32_t low, uint32_t high, uint32_t* left, uint32_t* right) {
    uint32_t i = low;
    uint32_t j = high;
    int pivot = v[i / 2 + j / 2];

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(&v[i], &v[j]);
            i++;
            j--;
        }
    }

    *left = i;
    *right = j;
}

void quicksort_serial(int* v, uint32_t low, uint32_t high) {
    uint32_t left = 0;
    uint32_t right = 0;
    partition(v, low, high, &left, &right);

    if (low < right) quicksort_serial(v, low, right);
    if (left < high) quicksort_serial(v, left, high);
}

uint32_t ARR_THRESHOLD = 1000;

void quicksort_tasks(int* v, uint32_t low, uint32_t high) {
    uint32_t left = 0;
    uint32_t right = 0;
    partition(v, low, high, &left, &right);

    if (high - low < ARR_THRESHOLD || right - low < ARR_THRESHOLD || high - left < ARR_THRESHOLD) {
        if (low < right) quicksort_tasks(v, low, right);
        if (left < high) quicksort_tasks(v, left, high);
    } else {
        #pragma omp task
        { quicksort_tasks(v, low, right); }
        quicksort_tasks(v, left, high);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s ARRAY_SIZE\n", argv[0]);
        return 0;
    }

    uint32_t size = strtoull(argv[1], NULL, 10);
    if (size == 0) {
        printf("ARRAY_SIZE must be positive number!\n");
        return 1;
    }

    char* arr_threshold_str = getenv("QS_ARRAY_THRESHOLD");
    if (arr_threshold_str) {
        int tmp = atoi(arr_threshold_str);
        if (tmp < 1) {
            printf("QS_ARRAY_THRESHOLD value must be positive number!\n");
            return 1;
        }

        ARR_THRESHOLD = tmp;
    }

    int* arr = malloc(size * sizeof(int));
    if (!arr) {
        printf("Malloc failed, exitting...\n");
        exit(1);
    }

    // printf("Unsorted: ");
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = rand() % size + 1;
        // printf("%d ", arr[i]);
    }

    double t = 0.0;
    if (omp_get_max_threads() == 1) {
        t = omp_get_wtime();
        quicksort_serial(arr, 0, size - 1);
        t = omp_get_wtime() - t;
    } else {
        t = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            quicksort_tasks(arr, 0, size - 1);
        }
        t = omp_get_wtime() - t;
    }

    // printf("Sorted:   ");
    // for (int i = 0; i < size; i++) {
    //     if (i != 0 && arr[i] < arr[i - 1]) printf("!");
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    printf("qsort: size = %u | threshold = %d | max_threads = %d | time (sec) = %.6f\n", size,
           ARR_THRESHOLD, omp_get_max_threads(), t);

    free(arr);
    return 0;
}

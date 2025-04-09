#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 20
#define THREADS 3

// Initial integer array
int arr[MAX_SIZE] = {10, 9, 7, 5, 8, 61, 2, 4, 13, 1, 20, 19, 33, 3, 11, 23, 50, 16, 45, 39};

// The sorting algorithm use bubble sort
void bubble_sort(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// The merge action
void merge(int *arr, int *left, int left_size, int *right, int right_size) {
	int i = 0, j = 0, k = 0;
	while (i < left_size && j < right_size) {
		if (left[i] < right[j]) {
			arr[k++] = left[i++];
		} else {
			arr[k++] = right[j++];
		}
	}

	// Merge the remaining elements
	while (i < left_size) {
		arr[k++] = left[i++];
	}
	while (j < right_size) {
     		arr[k++] = right[j++];
    	}
}

// The sort thread function
void* sort_thread(void *arg) {
    int *subarr = (int *)arg;
    bubble_sort(subarr, MAX_SIZE / 2);
    return NULL;
}

// The merge thread function
void* merge_thread(void *arg) {
    int *arr = (int *)arg;
    int mid = MAX_SIZE / 2;
    int left_size = mid;
    int right_size = MAX_SIZE - mid;

    int left[left_size];
    int right[right_size];

    for (int i = 0; i < left_size; i++) left[i] = arr[i];
    for (int i = 0; i < right_size; i++) right[i] = arr[mid + i];

    merge(arr, left, left_size, right, right_size);
    return NULL;
}

int main() {
	pthread_t threads[THREADS];
	int mid = MAX_SIZE / 2;

	pthread_create(&threads[0], NULL, sort_thread, (void *)arr);
	pthread_create(&threads[1], NULL, sort_thread, (void *)&arr[mid]);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	pthread_create(&threads[2], NULL, merge_thread, (void *)arr);
	pthread_join(threads[2], NULL);

	printf("Sorted array:\n");
	for (int i = 0; i < MAX_SIZE; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

void printArray(int *array, int size)
{

    printf("[");
    for (int i = 0; i < size; i++)
    {

        if (i < size && i > 0)
        {
            printf(",");
        }
        printf("%d", array[i]);
    }
    printf("]");
}

void swap(int *array, int current, int next)
{
    int temp = array[current];
    array[current] = array[next];
    array[next] = temp;
}
int partition(int *array, int low, int high)
{
    int pivot = array[high];
    int i=low-1; ;
    for (int j = low; j < high; j++)
    {
        
        if (array[j] <= pivot)
        {
            i++;
            swap(array, i, j);
        }
    }
    swap(array,i+1,high);
    return i + 1;
}

void QuickSort(int *array,int size, int low, int high)
{
    int pivotIndex;

    if (low < high)
    {
        pivotIndex = partition(array, low, high);
        QuickSort(array,size, low, pivotIndex - 1);
        QuickSort(array,size, pivotIndex + 1, high);
    }
    
    
}

int main()
{
    int array[] = {8, 4, 5, 2, 7, 1, 2, 9, 2};
    int size = sizeof(array) / sizeof(array[0]);
    printf("Before sorting: ");
    printArray(array, size);
    printf("\n");
    printf("After sorting: ");
    QuickSort(array,size, 0, size - 1);
    printArray(array,size);

    return 0;
}
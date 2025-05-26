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
void merge(int *array, int left,int mid, int right){
    int i,j,k;
    int s1=mid-left+1;
    int s2=right-mid;
    int *L=(int*)malloc(s1*sizeof(int));
    int *R=(int*)malloc(s2*sizeof(int));
    for(i=0;i<s1;i++){
        L[i]=array[left+i];
    }
    for(j=0;j<s2;j++){
        R[j]=array[mid+1+j];
    }
    i=0;
    j=0;
    k=left;
    while(i<s1 && j<s2){
        if(L[i]<=R[j]){
            array[k]=L[i];
            i++;
        }else{
            array[k]=R[j];
            j++;
        }
        k++;
    }

    while(i<s1){
        array[k]=L[i];
        i++;
        k++;
    }
    while(j<s2){
        array[k]=R[j];
        j++;
        k++;
    }
}

void mergeSort(int *array, int left, int right){
    if(left<right){
        int mid=(left+right)/2;
        mergeSort(array,left,mid);
        mergeSort(array,mid+1, right);
        merge(array,left,mid,right);
    }
    
}

int main(){
    int array[] = {8, 4, 5, 2, 7, 1, 2, 9, 2};
    int size = sizeof(array) / sizeof(array[0]);
    printf("Before sorting: ");
    printArray(array, size);
    mergeSort(array,0,size-1);
    printf("\nAfter sorting: ");
    printArray(array, size);
    return 0;

}
#include <stdio.h>

int P[] = {1, 0, 1, 0, 0, 1, 0, 1};
int K[] = {0, 0, 1, 0, 0, 1, 0, 1, 1, 1};

int P4[] = {2, 4, 3, 1};
int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP1[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
int S0[4][4] = {
    1, 0, 3, 2,
    3, 2, 1, 0,
    0, 3, 1, 3,
    3, 1, 3, 2};

int S1[4][4] = {
    0, 1, 2, 3,
    2, 0, 1, 3,
    3, 0, 1, 0,
    2, 1, 0, 3};

int key1[8];
int key2[8];

void printArray(int arr[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void copyArray(int source[], int destination[], int size)
{
    for (int i = 0; i < size; i++)
    {
        destination[i] = source[i];
    }
}

/// @brief xor of two arrays
/// @param arr1  this should be the destination
/// @param arr2
/// @param sizeOfArray
void xor (int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++)
    {
        arr1[i] = arr1[i] ^ arr2[i];
    }
}

    void leftCircularShift(int arr[], int start)
{

    int temp1 = arr[start];
    for (int i = start; i < start + 4; i++)
    {
        arr[i] = arr[i + 1];
    }
    arr[start + 4] = temp1;
}

void sBox(int arr[], int compressed1[])
{
    int row = (arr[0] * 2) + (arr[3]);
    int column = (arr[1] * 2) + (arr[2]);
    int temp1 = S0[row][column];

    // printf("%d", temp1);
    compressed1[0] = temp1 / 2;
    compressed1[1] = temp1 % 2;

    row = (arr[4] * 2) + (arr[7]);
    column = (arr[5] * 2) + (arr[6]);
    int temp2 = S1[row][column];

    // printf("%d", temp2);

    compressed1[2] = temp2 / 2;
    compressed1[3] = temp2 % 2;

    // printArray(compressed1, 4);
}

void swap(int nibble1[], int nibble2[], int size)
{
    int temp[size];
    copyArray(nibble1, temp, size);
    copyArray(nibble2, nibble1, size);
    copyArray(temp, nibble2, size);
}

void encrypt()
{
    int temp[10];
    copyArray(K, temp, 10);

    // P10
    for (int i = 0; i < 10; i++)
    {
        K[i] = temp[P10[i] - 1];
    }
    // printArray(K, 10);

    // Left Circular Shift

    leftCircularShift(K, 0);
    leftCircularShift(K, 5);

    // printArray(K, 10);

    // P8
    copyArray(K, temp, 10);
    for (int i = 0; i < 8; i++)
    {
        key1[i] = temp[P8[i] - 1];
    }

    printf("Key1: ");
    printArray(key1, 8);

    // shift
    leftCircularShift(K, 0);
    leftCircularShift(K, 5);

    // 2 times
    leftCircularShift(K, 0);
    leftCircularShift(K, 5);

    // printArray(K, 10);

    // P8
    copyArray(K, temp, 10);
    for (int i = 0; i < 8; i++)
    {
        key2[i] = temp[P8[i] - 1];
    }

    printf("Key2: ");
    printArray(key2, 8);

    // Plain text operations

    // IP
    int tempPT[8];
    copyArray(P, tempPT, 8);
    for (int i = 0; i < 8; i++)
    {
        P[i] = tempPT[IP[i] - 1];
    }
    // printArray(P, 8);

    int nibble1[4];
    int nibble2[4];

    for (int i = 0; i < 4; i++)
    {
        nibble1[i] = P[i];
        nibble2[i] = P[i + 4];
    }

    // Expansion on nibble 2 and also preserving it

    int expandNibble2[8];

    int tempNib[4];
    copyArray(nibble2, tempNib, 4);
    for (int i = 0; i < 8; i++)
    {
        expandNibble2[i] = tempNib[EP[i] - 1];
    }
    //  printArray(expandNibble2, 8);

    // XOR with key1

    xor(expandNibble2, key1, 8);
    // printArray(expandNibble2, 8);

    int compressed1[4];
    sBox(expandNibble2, compressed1);

    // P4

    copyArray(compressed1, tempNib, 4);
    for (int i = 0; i < 8; i++)
    {
        compressed1[i] = tempNib[P4[i] - 1];
    }

    // printArray(compressed1, 4);

    // XOR
    xor(nibble1, compressed1, 4);
    //   printArray(nibble1, 4);

    // Swap
    swap(nibble1, nibble2, 4);

    // printArray(nibble1, 4);
    // printArray(nibble2, 4);

    // Expansion on nibble 2 and also preserving it

    copyArray(nibble2, tempNib, 4);
    for (int i = 0; i < 8; i++)
    {
        expandNibble2[i] = tempNib[EP[i] - 1];
    }

    // printArray(expandNibble2, 8);

    // xor with key2
    xor(expandNibble2, key2, 8);
    // printArray(expandNibble2, 8);

    // sBox
    sBox(expandNibble2, compressed1);

    // printArray(compressed1, 4);

    // continuing with compressed1

    // P4

    copyArray(compressed1, tempNib, 4);
    for (int i = 0; i < 4; i++)
    {
        compressed1[i] = tempNib[P4[i] - 1];
    }
    // printArray(compressed1, 4);

    // xor with nibble1 after swap

    xor(nibble1, compressed1, 4);
    //  printArray(nibble1, 4);
    // printArray(nibble2, 4);

    // Updating plain text
    for (int i = 0; i < 4; i++)
    {
        P[i] = nibble1[i];
        P[i + 4] = nibble2[i];
    }
    // printArray(P, 8);
    // IP inverse
    copyArray(P, tempPT, 8);
    for (int i = 0; i < 8; i++)
    {
        P[i] = tempPT[IP1[i] - 1];
    }
}

int main()
{
    printf("Plain Text: ");
    printArray(P, 8);
    printf("Key: ");
    printArray(K, 10);

    encrypt();
    printf("Encrypted Text: ");
    printArray(P, 8);

        return 0;
}

#include <stdio.h>
#include <stdlib.h>

void matrix_mult(int *a, int *b, int *c, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            int sum = 0;
            for (int h = 0; h < n; h++)
            {
                sum += a[i * n + h] * b[h * k + j];
            }
            c[i * k + j] = sum;
        }
    }
}

int main()
{
    int m, n, k;
    printf("Enter the dimensions of the matrices (m, n, k): ");
    scanf("%d %d %d", &m, &n, &k);

    // Allocate memory for matrices
    int *a = (int *)malloc(m * n * sizeof(int));
    int *b = (int *)malloc(n * k * sizeof(int));
    int *c = (int *)malloc(m * k * sizeof(int));

    // Initialize matrices with random values
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            a[i * n + j] = rand() % 10;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            b[i * k + j] = rand() % 10;
        }
    }

    // Perform matrix multiplication
    matrix_mult(a, b, c, m, n, k);

    // Print the resulting matrix
    printf("Resultant matrix:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            printf("%d ", c[i * k + j]);
        }
        printf("\n");
    }

    // Free memory
    free(a);
    free(b);
    free(c);

    return 0;
}
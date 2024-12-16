
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // definisi matrixSize dan matrixColSize
    int matrixSize = 2;
    int* matrixColSize = malloc(sizeof(int) * matrixSize);

    matrixColSize[0] = 4;  // jumlah kolom pertama
    matrixColSize[1] = 4;  // jumlah kolom kedua (sama dengan kolom pertama)

    // definisi perbaris dari matriks
    int* a1 = malloc(matrixColSize[0] * sizeof(int));
    int* a2 = malloc(matrixColSize[1] * sizeof(int));

    a1[0] = 1; a1[1] = 2; a1[2] = 3; a1[3] = 4;
    a2[0] = 5; a2[1] = 6; a2[2] = 7; a2[3] = 8;

    int** matrix = malloc(sizeof(int*) * matrixSize);

    if (matrix == NULL) {
        printf("fail to malloc int**\n");
        return EXIT_FAILURE;
    }

    matrix[0] = a1;
    matrix[1] = a2;

    for (int i = 0; i < 4; i++) {
        printf("%d ", matrix[0][i]);
    }
    printf("\n"); //ok

    for(int i = 0; i < 4; i++) {
        printf("%d ", matrix[1][i]);
    }
    printf("\n"); //ok

    /*
     * Sejauh ini kita punya
     * int matrixSize
     * int* matrixColSize
     * int** matrix
    */

    // Begin transpose with intrinsic
    // kita tahu dimensi matriks adalah 2x4, akan di transpose menjadi 4x2
    __m256i row1 = _mm256_loadu_si256((__m256i*)matrix[0]);
    __m256i row2 = _mm256_loadu_si256((__m256i*)matrix[1]);

    // Unpack the matrix rows into transpose pairs
    __m256i t1 = _mm256_unpacklo_epi64(row1, row2); // Unpack low halves of row1 and row2
    __m256i t2 = _mm256_unpackhi_epi64(row1, row2); // Unpack high halves of row1 and row2

    // Transpose the pairs to get the final result
    row1 = _mm256_unpacklo_epi64(t1, t2); // First transpose step (final row1)
    row2 = _mm256_unpackhi_epi64(t1, t2); // Second transpose step (final row2)

    // Storing results back into matrix
    _mm256_storeu_si256((__m256i*)matrix[0], row1); // Store the first transposed row
    _mm256_storeu_si256((__m256i*)matrix[1], row2); // Store the second transposed row

    // Menampilkan hasil transpose (4x2)
    printf("Transposed Matrix:\n");
    for (int i = 0; i < 4; i++) {   // Loop untuk 4 kolom
        for (int j = 0; j < 2; j++) { // Loop untuk 2 baris
            printf("%d ", matrix[j][i]);
        }
        printf("\n");
    }

    // Cleanup
    free(matrixColSize);
    free(a1);
    free(a2);
    free(matrix);

    return EXIT_SUCCESS;
}

#include <emmintrin.h>
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
    
    // siapkan memori baru hasil transpose karena dari 2x4 hasilnya adalah 4x2
    int** tmatrix = (int**)malloc(sizeof(int*) * 4);
    for(int i = 0; i < 4; i++) {
        tmatrix[i] = (int*)malloc(sizeof(int) * 2);
    }
    


    /*
     * Sejauh ini kita punya
     * int matrixSize
     * int* matrixColSize
     * int** matrix
    */

    // Begin transpose with intrinsic
    // kita tahu dimensi matriks adalah 2x4, akan di transpose menjadi 4x2
    __m256i row0 = _mm256_set_epi32(matrix[1][3], matrix[1][2], matrix[1][1], matrix[1][0], matrix[0][3], matrix[0][2], matrix[0][1], matrix[0][0]);
    /*
    __m256i row1 = _mm256_loadu_si256((__m256i*)matrix[0]);
    __m256i row2 = _mm256_loadu_si256((__m256i*)matrix[1]);
    */

    // Unpack the matrix rows into transpose pairs
    __m256i t1 = _mm256_unpacklo_epi32(row0, row0); // Unpack low halves of row1 and row2
    __m256i t2 = _mm256_unpackhi_epi32(row0, row0); // Unpack high halves of row1 and row2
    
    printf("extract\n");
    // Extract and store the results in the correct order
    __m128i lo0 = _mm256_extracti128_si256(t1, 0);  // Lower 128 bits of tmp0
    __m128i hi0 = _mm256_extracti128_si256(t1, 1);  // Higher 128 bits of tmp0
    __m128i lo1 = _mm256_extracti128_si256(t2, 0);  // Lower 128 bits of tmp1
    __m128i hi1 = _mm256_extracti128_si256(t2, 1);  // Higher 128 bits of tmp1
    
    printf("store\n");
    _mm_storeu_si128((__m128i*)tmatrix[0], _mm_unpacklo_epi32(lo0, hi0));  // Store 4 elements: 1, 5
    _mm_storeu_si128((__m128i*)tmatrix[1], _mm_unpackhi_epi32(lo0, hi0));  // Store 4 elements: 2, 6
    _mm_storeu_si128((__m128i*)tmatrix[2], _mm_unpacklo_epi32(lo1, hi1));  // Store 4 elements: 3, 7
    _mm_storeu_si128((__m128i*)tmatrix[3], _mm_unpackhi_epi32(lo1, hi1));  // Store 4 elements: 4, 8

    // Menampilkan hasil transpose (4x2)
    printf("Transposed Matrix:\n");
    for (int i = 0; i < 4; i++) {   // Loop untuk 4 kolom
        for (int j = 0; j < 2; j++) { // Loop untuk 2 baris
            printf("%d ", tmatrix[i][j]);
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

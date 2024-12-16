#What?
Karna saya ada di arsitektur x64, makanya pakai `_mm256_unpacklo_epi64`

#How to Run
`gcc -mavx2 -o main main.c`

dengan flag `-mavx2` untuk mengaktifkan avx pada kompiler sehingga dapat menjalankan fungsi intrisik

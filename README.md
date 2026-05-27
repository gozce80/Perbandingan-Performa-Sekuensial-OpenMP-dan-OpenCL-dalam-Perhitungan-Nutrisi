# Perbandingan Performa Sekuensial, OpenMP, dan OpenCL dalam Perhitungan Nutrisi

## Nama Penyusun

* Alexander Theodore Valerian Hardjo Soemarto (25032014041)
* Gavra Arva Maheswara (25032014072)
* Maria Bryghita Sora Gracya (25032014044)


## Deskripsi Singkat Proyek

Proyek ini merupakan program simulasi untuk membandingkan performa komputasi antara metode Sekuensial, OpenMP, dan OpenCL dalam menghitung skor fitness nutrisi. Program menggunakan data kalori dan protein yang dibangkitkan secara acak sebagai simulasi kombinasi jadwal makanan.

## Fitur Utama

* Membuat data kalori dan protein secara acak.
* Menghitung skor fitness nutrisi berdasarkan target kalori dan protein.
* Menjalankan komputasi menggunakan metode Sekuensial.
* Menjalankan komputasi paralel menggunakan OpenMP.
* Menjalankan komputasi menggunakan OpenCL.
* Menampilkan waktu eksekusi setiap metode.
* Menghitung speedup OpenMP dan OpenCL terhadap metode Sekuensial.

## Langkah-langkah Menjalankan Sistem

1. **Prasyarat:**
   - Pastikan kompiler C++ (seperti G++) udah terinstal.
   - Pastikan driver GPU yang mendukung OpenCL udah terinstal di sistem.
   - Compiler lain yang mendukung OpenMP dan OpenCL
   - Pastikan File Kernel OpenCL Tersedia (src/kernel.cl)
2. **Kompilasi:**
   - Masuk ke direktori `src/`.
   - Lakukan perintah kompilasi (sesuaikan dengan lingkungan buildnya):
     `g++ -o meal_scheduler main.cpp -fopenmp -lOpenCL`
   - Jika tidak bisa :
     `g++ src/main.cpp -O3 -fopenmp -I "C:\Users\nama\vcpkg\installed\x64-windows\include" -o scheduler_test C:\Windows\System32\OpenCL.dll`
3. **Eksekusi:**
   - Jalankan file hasil kompilasi:
     `.\scheduler_test.exe`
   - Masukkan jumlah kombinasi data yang ingin diuji (contoh: 50000000).

TIPS : SETELAH GANTI JUMLAH DATA MAKANAN DI CODE, LAKUKAN NOMOR 2 (KOMPILASI) LALU NOMOR 3 (EKSEKUSI)

## Hasil Pengujian

Berdasarkan hasil pengujian, metode OpenMP memiliki waktu eksekusi paling cepat yaitu 391 ms. Metode Sekuensial membutuhkan waktu 661 ms, sedangkan OpenCL membutuhkan waktu 16853 ms.

OpenMP menghasilkan speedup sebesar 1.69054x terhadap metode Sekuensial. Sementara itu, OpenCL belum memberikan peningkatan performa karena waktu eksekusinya lebih lama. Hal ini dapat terjadi karena adanya overhead seperti proses setup OpenCL, alokasi buffer GPU, transfer data ke VRAM, dan pengambilan hasil kembali ke RAM.

## Link Video Penjelasan

[Masukkan Link YouTube Anda Di Sini]

# Perbandingan Performa Sekuensial, OpenMP, dan OpenCL dalam Perhitungan Nutrisi

## Nama Penyusun

* Alexander Theodore Valerian Hardjo Soemarto (25032014041)
* Gavra Arva Maheswara (25032014072)
* Maria Bryghita Sora Gracya (25032014044)


## Deskripsi Singkat Proyek

Proyek ini adalah implementasi Heterogeneous Computing yang bertujuan untuk mengoptimalkan penjadwalan nutrisi harian (Smart Meal Scheduler). Sistem ini membandingkan kinerja komputasi dalam menentukan kombinasi menu makanan yang memenuhi target kalori dan makronutrisi menggunakan tiga metode:
1. **Sekuensial (CPU Tunggal):** Sebagai baseline pengukuran performa.
2. **OpenMP (CPU Multi-core):** Paralelisasi berbasis thread untuk beban kerja menengah.
3. **OpenCL (GPU Accelerator):** Paralelisasi berbasis kernel pada GPU untuk komputasi masif.

## Fitur Utama

* Optimasi kombinasi menu menggunakan pendekatan brute-force yang diparalelkan.
* Perbandingan performa (benchmark) waktu eksekusi secara real-time.
* Analisis scalability sistem pada berbagai skala beban kerja.

## Langkah-langkah Menjalankan Sistem

1. **Prasyarat:**
   - Pastikan kompiler C++ (seperti G++) udah terinstal.
   - Pastikan driver GPU yang mendukung OpenCL udah terinstal di sistem.
2. **Kompilasi:**
   - Masuk ke direktori `src/`.
   - Lakukan perintah kompilasi (sesuaikan dengan lingkungan buildnya):
     `g++ -o meal_scheduler main.cpp -fopenmp -lOpenCL`
3. **Eksekusi:**
   - Jalankan file hasil kompilasi:
     `./meal_scheduler`
   - Masukkan jumlah kombinasi data yang ingin diuji (contoh: 50000000).

## Hasil Pengujian

Data hasil pengujian dan visualisasi grafik performa bisa dilihat di folder `test/` dan `docs/`. Dari sini menunjukkan bahwa OpenMP memberikan peningkatan speedup stabil, sementara OpenCL menunjukkan potensi tinggi namun sedikit bottleneck pada transfer data untuk beban kerja masif atau besar.

## Link Video Penjelasan

[Masukkan Link YouTube Anda Di Sini]
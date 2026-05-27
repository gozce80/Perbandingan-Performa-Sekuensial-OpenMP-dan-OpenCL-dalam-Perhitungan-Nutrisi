#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <CL/cl.h>

using namespace std;
using namespace std::chrono;

// SEKUENSIAL
void evaluasi_sekuensial(const vector<float>& kalori, 
                         const vector<float>& protein, 
                         vector<float>& skor_fitness, 
                         int n, float target_kalori, float target_protein) {
    for (int i = 0; i < n; i++) {
        skor_fitness[i] = abs(kalori[i] - target_kalori) + abs(protein[i] - target_protein);
    }
}

// OPENMP
void evaluasi_openmp(const vector<float>& kalori, 
                     const vector<float>& protein, 
                     vector<float>& skor_fitness, 
                     int n, float target_kalori, float target_protein) {
    
    // Pragma OpenMP: Memecah iterasi loop ke seluruh core CPU yang tersedia
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        skor_fitness[i] = abs(kalori[i] - target_kalori) + abs(protein[i] - target_protein);
    }
}

int main() {
    // Simulasi 50 jt kombinasi jadwal makanan
    const int TOTAL_KOMBINASI = 500000000; 
    
    // Target spesifik (Misal: 1800 kcal, 150g protein)
    const float TARGET_KALORI = 1800.0f;
    const float TARGET_PROTEIN = 150.0f;

    cout << "Mengalokasikan memori untuk " << TOTAL_KOMBINASI << " kombinasi...\n";
    
    // Alokasi memori
    vector<float> kalori(TOTAL_KOMBINASI);
    vector<float> protein(TOTAL_KOMBINASI);
    vector<float> skor_fitness_seq(TOTAL_KOMBINASI);
    vector<float> skor_fitness_omp(TOTAL_KOMBINASI);

    // Inisialisasi data acak
    for (int i = 0; i < TOTAL_KOMBINASI; i++) {
        kalori[i] = 1000.0f + static_cast<float>(rand() % 1500); // 1000 - 2500 kcal
        protein[i] = 50.0f + static_cast<float>(rand() % 150);     // 50 - 200g protein
    }

    cout << "\nMulai komputasi :\n";

    // UJI SEKUENSIAL
    auto waktu_mulai_seq = high_resolution_clock::now();
    evaluasi_sekuensial(kalori, protein, skor_fitness_seq, TOTAL_KOMBINASI, TARGET_KALORI, TARGET_PROTEIN);
    auto waktu_berhenti_seq = high_resolution_clock::now();
    auto durasi_seq = duration_cast<milliseconds>(waktu_berhenti_seq - waktu_mulai_seq);
    
    cout << "[Sekuensial] Waktu Eksekusi : " << durasi_seq.count() << " ms\n";

    // UJI OPENMP
    auto waktu_mulai_omp = high_resolution_clock::now();
    evaluasi_openmp(kalori, protein, skor_fitness_omp, TOTAL_KOMBINASI, TARGET_KALORI, TARGET_PROTEIN);
    auto waktu_berhenti_omp = high_resolution_clock::now();
    auto durasi_omp = duration_cast<milliseconds>(waktu_berhenti_omp - waktu_mulai_omp);
    
    cout << "[OpenMP]     Waktu Eksekusi : " << durasi_omp.count() << " ms\n";

    float rasio_speedup_omp = static_cast<float>(durasi_seq.count()) / static_cast<float>(durasi_omp.count());
    cout << ">>> Speedup OpenMP terhadap Sekuensial: " << rasio_speedup_omp << "x <<<\n";

    // UJI OPENCL 
    cout << "\nMenyiapkan eksekusi OpenCL\n";

    // a. baca file kernel.cl 
    ifstream file_kernel("src/kernel.cl");
    if (!file_kernel.is_open()) {
        cerr << "Gagal buka file src/kernel.cl! Pastiin lokasi benar.\n";
        return 1;
    }
    string kode_kernel((istreambuf_iterator<char>(file_kernel)), istreambuf_iterator<char>());
    const char* sumber_kernel = kode_kernel.c_str();

    // b. setup platform n device GPU
    cl_platform_id platform;
    cl_uint jumlah_platform;
    clGetPlatformIDs(1, &platform, &jumlah_platform);
    if (jumlah_platform == 0) {
        cerr << "Gak ada platform OpenCL yang ditemuin di sistem!\n";
        return 1;
    }

    cl_device_id perangkat;
    cl_uint jumlah_perangkat;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &perangkat, &jumlah_perangkat);
    if (jumlah_perangkat == 0) {
        cerr << "Gak perangkat OpenCL yang ditemuin\n";
        return 1;
    }

    // c. buat context & command queue atau jalur komunikasi CPU-GPU
    cl_context konteks = clCreateContext(NULL, 1, &perangkat, NULL, NULL, NULL);
    cl_command_queue antrean = clCreateCommandQueue(konteks, perangkat, 0, NULL);

    // d. Compile Program & Kernel OpenCL
    cl_program program = clCreateProgramWithSource(konteks, 1, &sumber_kernel, NULL, NULL);
    cl_int status_build = clBuildProgram(program, 1, &perangkat, NULL, NULL, NULL);
    if (status_build != CL_SUCCESS) {
        cerr << "Gagal lakukan compile pada program kernel OpenCL!\n";
        return 1;
    }
    
    // pastikin nama ini sama dengan nama fungsi __kernel di dalam file kernel.cl
    cl_kernel kernel = clCreateKernel(program, "evaluasi_fitness", NULL);

    // e. alokasi buffer di VRAM (Memori GPU)
    size_t ukuran_array = TOTAL_KOMBINASI * sizeof(float);
    cl_mem d_kalori = clCreateBuffer(konteks, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, ukuran_array, kalori.data(), NULL);
    cl_mem d_protein = clCreateBuffer(konteks, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, ukuran_array, protein.data(), NULL);
    cl_mem d_skor_fitness = clCreateBuffer(konteks, CL_MEM_WRITE_ONLY, ukuran_array, NULL, NULL);

    // f. masukkan argumen ke fungsi kernel.cl
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_kalori);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_protein);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_skor_fitness);
    clSetKernelArg(kernel, 3, sizeof(int), &TOTAL_KOMBINASI);
    clSetKernelArg(kernel, 4, sizeof(float), &TARGET_KALORI);
    clSetKernelArg(kernel, 5, sizeof(float), &TARGET_PROTEIN);

    cout << "Mengeksekusi perhitungan di GPU :\n";

    // g. eksekusi kernel & catet waktu mulai
    size_t ukuran_global = TOTAL_KOMBINASI;
    auto waktu_mulai_ocl = high_resolution_clock::now();

    clEnqueueNDRangeKernel(antrean, kernel, 1, NULL, &ukuran_global, NULL, 0, NULL, NULL);
    clFinish(antrean); 

    // h. tarik hasil dari VRAM GPU kembali ke RAM utama CPU
    vector<float> skor_fitness_ocl(TOTAL_KOMBINASI);
    clEnqueueReadBuffer(antrean, d_skor_fitness, CL_TRUE, 0, ukuran_array, skor_fitness_ocl.data(), 0, NULL, NULL);

    auto waktu_berhenti_ocl = high_resolution_clock::now();
    auto durasi_ocl = duration_cast<milliseconds>(waktu_berhenti_ocl - waktu_mulai_ocl);

    // i. Analisis hasil OpenCL
    cout << "[OpenCL]     Waktu Eksekusi : " << durasi_ocl.count() << " ms\n";
    float rasio_speedup_ocl = static_cast<float>(durasi_seq.count()) / static_cast<float>(durasi_ocl.count());
    cout << ">>> Speedup OpenCL terhadap Sekuensial: " << rasio_speedup_ocl << "x <<<\n";

    // j. clear memori GPU
    clReleaseMemObject(d_kalori);
    clReleaseMemObject(d_protein);
    clReleaseMemObject(d_skor_fitness);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(antrean);
    clReleaseContext(konteks);

    return 0;
}
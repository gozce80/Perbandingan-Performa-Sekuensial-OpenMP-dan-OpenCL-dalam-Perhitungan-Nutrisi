__kernel void evaluasi_fitness(__global const float* kalori,
                               __global const float* protein,
                               __global float* skor_fitness,
                               const int total_kombinasi,
                               const float target_kalori,
                               const float target_protein) {
    
    // get_global_id(0) mengambil ID dari thread GPU yang sedang berjalan.
    // menggunakan loop 'for' seperti di CPU. ID ini menjadi indeks array kita.
    int id = get_global_id(0);

    // mastiin thread yang dieksekusi tidak ngelebihi batas jumlah data yang dialokasi
    if (id < total_kombinasi) {
        
        // hitung selisih (error) antara nilai gizi acak dengan target diet
        float selisih_kalori = kalori[id] - target_kalori;
        float selisih_protein = protein[id] - target_protein;
        
        // Ubah nilai negatif menjadi absolut (positif)
        if (selisih_kalori < 0) {
            selisih_kalori = -selisih_kalori;
        }
        if (selisih_protein < 0) {
            selisih_protein = -selisih_protein;
        }
        
        // hitung Skor fitness 
        skor_fitness[id] = selisih_kalori + selisih_protein;
    }
}
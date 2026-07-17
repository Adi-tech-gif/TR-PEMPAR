## 1. Identitas Praktikan

| Item          | Keterangan                        |
|---------------|------------------------------------|
| Nama          | KALFIN ADI PRSETIO                  |
| NIM           | 622023002                   |
| Mata Kuliah   | CE 602 — Praktikum Pemrosesan Paralel |
| Tugas         | Tugas Rancang — Particle System Paralel |

## 2. Langkah Kompilasi
Buka terminal/command prompt pada direktori tempat file main.cpp berada, kemudian jalankan perintah kompilasi berikut:

Bash
g++ -O3 main.cpp -o simulasi_hujan -lSDL2 -fopenmp
Keterangan Flag:

-O3 : Mengaktifkan optimasi tingkat tinggi agar eksekusi program berjalan maksimal.

-lSDL2 : Menghubungkan program dengan library SDL2 untuk visualisasi grafis.

-fopenmp : Mengaktifkan fitur multi-threading OpenMP pada kompiler GCC.


## 3. Langkah Kompilasi

Masuk ke folder project

cd /mnt/hgfs/SHARED/TRPEMPAR

Compile program

g++ -O3 -fopenmp main.cpp -o simulasi_hujan -lSDL2

Menjalankan program

./simulasi_hujan


## 4. Cara Kerja Program

Program bekerja dengan langkah-langkah berikut.

Membuat window SDL2.
Menginisialisasi seluruh partikel hujan.
Setiap partikel memiliki:
posisi X
posisi Y
kecepatan jatuh
Pada setiap frame:
posisi partikel diperbarui
jika mencapai bawah layar maka kembali ke atas
Seluruh partikel kemudian digambar kembali ke layar.
Program melakukan benchmark serial dan paralel.
Hasil benchmark ditampilkan pada terminal.

### 4.1 Flowchart Program
                Mulai
                   │
                   ▼
        Inisialisasi SDL2
                   │
                   ▼
      Membuat Seluruh Partikel
                   │
                   ▼
        Selama Window Aktif
                   │
        ┌──────────┴──────────┐
        │                     │
        ▼                     ▼
 Update Posisi Partikel   Benchmark
 (OpenMP Parallel)      Serial & Parallel
        │
        ▼
 Render Semua Partikel
        │
        ▼
 Apakah Window Ditutup?
        │
   Ya ──┴── Tidak
        │
        ▼
      Selesai

### 4.3 Implementasi Paralel OpenMP

Implementasi paralel dilakukan pada proses update posisi seluruh partikel.

Contoh implementasi:

#pragma omp parallel for
for(int i=0;i<NUM_PARTICLES;i++)
{
    particles[i].update();
}

Penjelasan:

OpenMP membagi indeks partikel ke beberapa thread.
Setiap thread memperbarui posisi partikel yang berbeda.
Tidak terjadi race condition karena setiap thread hanya mengakses data partikelnya sendiri.
Setelah seluruh thread selesai, hasil digabungkan sebelum proses render.

Keuntungan:

Memanfaatkan multicore processor.
Waktu update semakin kecil jika jumlah partikel sangat besar.

### 4.4 Hasil Pengujian

Pengujian dilakukan dengan jumlah partikel 2000.

Output terminal:

MEMULAI SIMULASI HUJAN DENGAN 2000 PARTIKEL

[BENCHMARK]
Serial   : 0.007 ms
Parallel : 0.995 ms

[BENCHMARK]
Serial   : 0.007 ms
Parallel : 0.908 ms

[BENCHMARK]
Serial   : 0.007 ms
Parallel : 0.837 ms

====================================
KESIMPULAN AKHIR PENGUJIAN
====================================

Rata-rata Serial   : 0.007 ms
Rata-rata Parallel : 0.868 ms
Speedup            : 0.008x
Dokumentasi Program
Hasil Visualisasi

Program berhasil menampilkan simulasi hujan menggunakan SDL2.

(Masukkan screenshot animasi hujan yang Anda lampirkan pada bagian ini.)

Hasil Benchmark

Program juga berhasil menampilkan hasil benchmark serial dan paralel pada terminal.

(Masukkan screenshot terminal benchmark yang Anda lampirkan pada bagian ini.)

---

## 5. Analisis Hasil

Berdasarkan hasil pengujian diperoleh:

Parameter	Nilai
Jumlah Partikel	2000
Serial	0.007 ms
Parallel	0.868 ms
Speedup	0.008x

Terlihat bahwa implementasi paralel menghasilkan waktu yang lebih lama dibandingkan implementasi serial.

Hal ini disebabkan karena jumlah partikel yang relatif sedikit sehingga overhead pembuatan dan sinkronisasi thread OpenMP lebih besar daripada waktu komputasi yang sebenarnya. Pada kondisi seperti ini, implementasi serial lebih efisien.

Namun apabila jumlah partikel diperbesar (misalnya 50.000 hingga 500.000 partikel), maka OpenMP akan mulai menunjukkan keuntungan karena beban kerja dapat dibagi ke beberapa inti prosesor.

9. Kesimpulan

Berdasarkan hasil implementasi dapat disimpulkan bahwa:

Program simulasi hujan berhasil dibuat menggunakan SDL2.
OpenMP berhasil digunakan untuk melakukan update posisi partikel secara paralel.
Program dapat dikompilasi menggunakan GCC dengan opsi -fopenmp.
Pada pengujian dengan 2000 partikel, implementasi paralel belum memberikan peningkatan performa karena overhead thread masih lebih besar dibandingkan waktu komputasi.
Implementasi OpenMP akan lebih efektif apabila jumlah partikel jauh lebih besar sehingga setiap thread memiliki beban kerja yang cukup.
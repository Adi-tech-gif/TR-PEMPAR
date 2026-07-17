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
g++ -O3 -fopenmp main.cpp -o simulasi_hujan -lSDL2
Keterangan Flag:

-O3 : Mengaktifkan optimasi tingkat tinggi agar eksekusi program berjalan maksimal.

-lSDL2 : Menghubungkan program dengan library SDL2 untuk visualisasi grafis.

-fopenmp : Mengaktifkan fitur multi-threading OpenMP pada kompiler GCC.

Menjalankan program

./simulasi_hujan


## 3. Cara Kerja Program

Program simulasi hujan ini dibuat menggunakan SDL2 sebagai library grafis untuk menampilkan animasi hujan secara real-time dan OpenMP untuk mempercepat proses pembaruan posisi partikel melalui komputasi paralel. Secara umum alur kerja program adalah sebagai berikut:

Inisialisasi SDL2
Program memulai dengan menginisialisasi library SDL2.
Setelah berhasil, program membuat sebuah window dan renderer yang digunakan sebagai media untuk menggambar seluruh objek hujan.
Inisialisasi Partikel
Program mengalokasikan sejumlah partikel sesuai nilai NUM_PARTICLES (misalnya 2000 partikel).
Setiap partikel diberikan nilai awal secara acak, meliputi:
Posisi horizontal (X)
Posisi vertikal (Y)
Kecepatan jatuh
Panjang tetesan hujan
Arah gerakan horizontal yang kecil untuk memberikan efek hujan tertiup angin.
Memasuki Loop Utama
Setelah seluruh partikel berhasil dibuat, program memasuki loop utama (main loop).
Loop ini akan terus berjalan hingga pengguna menutup jendela aplikasi.
Pengecekan Event
Pada setiap iterasi loop, SDL2 memeriksa apakah terdapat event dari pengguna, seperti menutup window.
Jika event penutupan terdeteksi, program keluar dari loop dan melakukan proses pembersihan memori.
Update Posisi Partikel Secara Paralel
Pembaruan posisi seluruh partikel dilakukan menggunakan directive OpenMP #pragma omp parallel for.
Setiap thread bertugas memperbarui sebagian partikel sehingga proses dapat dikerjakan secara bersamaan pada beberapa inti prosesor.
Untuk setiap partikel dilakukan proses berikut:
Jika partikel sedang berada pada fase percikan (splash), maka waktu splash akan dikurangi hingga habis.
Setelah splash selesai, partikel dikembalikan ke bagian atas layar dengan posisi acak.
Jika partikel sedang jatuh, maka kecepatan akan bertambah akibat efek gravitasi.
Posisi X dan Y diperbarui sesuai kecepatan yang dimiliki.
Apabila partikel mencapai batas bawah layar, maka status splash akan diaktifkan sehingga menghasilkan efek percikan sebelum kembali ke atas.
Rendering Grafis
Setelah seluruh posisi partikel selesai diperbarui, renderer membersihkan layar dari frame sebelumnya.
Program kemudian menggambar seluruh partikel hujan beserta efek percikan air sesuai posisi terbarunya.
Hasil gambar ditampilkan ke layar menggunakan fungsi SDL_RenderPresent() sehingga animasi terlihat berjalan secara real-time.
Pengaturan Frame Rate
Setelah proses rendering selesai, program memberikan delay sekitar 16 ms.
Delay ini bertujuan menjaga kecepatan animasi tetap stabil pada sekitar 60 FPS (Frame Per Second) sehingga pergerakan hujan terlihat lebih halus.
Benchmark Performa
Selain menjalankan simulasi, program juga melakukan pengujian performa antara implementasi serial dan paralel.
Waktu eksekusi masing-masing metode diukur menggunakan timer beresolusi tinggi.
Hasil pengukuran ditampilkan pada terminal berupa waktu eksekusi rata-rata dan nilai speedup sehingga dapat dibandingkan efektivitas penggunaan OpenMP terhadap implementasi serial.

# 4. Flowchart Program

```mermaid
flowchart TD
    A([Mulai Program]) --> B[Inisialisasi SDL2 & Window]
    B --> C[Alokasi & Inisialisasi 2000 Partikel<br/>Posisi, Kecepatan, dan Panjang Acak]
    C --> D{{Loop Utama Real-Time}}

    D --> E{Event Window<br/>Ditutup?}
    E -- Ya --> F[Hancurkan Window<br/>SDL_Quit]
    F --> G([Selesai])

    E -- Tidak --> H[PARALELISASI OPENMP]
    H --> I["#pragma omp parallel for<br/><br/>Untuk setiap partikel (0 hingga N-1)<br/>secara paralel"]

    I --> J{Splash Timer > 0?}

    J -- Ya --> K[Kurangi splashTimer]
    K --> L{Splash Timer Habis?}
    L -- Ya --> M[Reset Posisi ke Atas]
    L -- Tidak --> R

    J -- Tidak --> N[Tambah Kecepatan Jatuh<br/>Efek Gravitasi]
    N --> O[Update Posisi Y & Posisi X]
    O --> P{Menyentuh Batas Bawah?}
    P -- Ya --> Q[Aktifkan splashTimer]
    P -- Tidak --> R
    Q --> R

    R --> S[Bersihkan Layar]
    S --> T[Gambar Hujan & Percikan]
    T --> U[SDL_RenderPresent]
    U --> V[Delay 16 ms]
    V --> D
```

### 5. Implementasi Paralel OpenMP

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

### 6. Hasil Pengujian

Pengujian dilakukan dengan jumlah partikel 2000.

Output terminal:

![Output Terminal](outputterminal.png)

MEMULAI SIMULASI HUJAN DENGAN 2000 PARTIKEL
![Gambarhujan](gambarhujan.png)

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

## 7. Analisis Hasil

Berdasarkan hasil pengujian diperoleh:

| Pengujian     | Serial (ms) | Parallel (ms) |    Speedup |
| ------------- | ----------: | ------------: | ---------: |
| 1             |       0.007 |         0.995 |     0.007x |
| 2             |       0.007 |         0.908 |     0.008x |
| 3             |       0.007 |         0.837 |     0.008x |
| **Rata-rata** |   **0.007** |     **0.868** | **0.008x** |


Terlihat bahwa implementasi paralel menghasilkan waktu yang lebih lama dibandingkan implementasi serial.

Hal ini disebabkan karena jumlah partikel yang relatif sedikit sehingga overhead pembuatan dan sinkronisasi thread OpenMP lebih besar daripada waktu komputasi yang sebenarnya. Pada kondisi seperti ini, implementasi serial lebih efisien.

Namun apabila jumlah partikel diperbesar (misalnya 50.000 hingga 500.000 partikel), maka OpenMP akan mulai menunjukkan keuntungan karena beban kerja dapat dibagi ke beberapa inti prosesor.

## 8. Kesimpulan

Berdasarkan hasil implementasi dapat disimpulkan bahwa:

Program simulasi hujan berhasil dibuat menggunakan SDL2.
OpenMP berhasil digunakan untuk melakukan update posisi partikel secara paralel.
Program dapat dikompilasi menggunakan GCC dengan opsi -fopenmp.
Pada pengujian dengan 2000 partikel, implementasi paralel belum memberikan peningkatan performa karena overhead thread masih lebih besar dibandingkan waktu komputasi.
Implementasi OpenMP akan lebih efektif apabila jumlah partikel jauh lebih besar sehingga setiap thread memiliki beban kerja yang cukup.

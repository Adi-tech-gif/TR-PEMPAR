#include <SDL2/SDL.h>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Sesuai Spek Tabel Anda: Ubah nilai ini untuk menguji performa (500, 2000, 10000, atau 50000)
const int NUM_PARTICLES = 2000; 

struct Particle {
    float x, y;
    float speedY;   
    float speedX;   
    float length;
    int splashTimer; 
};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    
    SDL_Window* window = SDL_CreateWindow("Simulasi Hujan Paralel OpenMP (Dengan Benchmark)", 
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Particle> particles(NUM_PARTICLES);
    srand(time(NULL));

    // Inisialisasi awal partikel master
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles[i].x = rand() % SCREEN_WIDTH;
        particles[i].y = rand() % SCREEN_HEIGHT;
        particles[i].speedY = (rand() % 5) + 6.0f; 
        particles[i].speedX = 0.5f; 
        particles[i].length = (rand() % 12) + 8.0f;
        particles[i].splashTimer = 0;
    }

    bool quit = false;
    SDL_Event e;

    // Variabel kalkulasi Benchmark kinerja
    double total_serial_time = 0.0;
    double total_parallel_time = 0.0;
    int frame_count = 0;

    std::cout << "==========================================================" << std::endl;
    std::cout << "  MEMULAI SIMULASI HUJAN DENGAN " << NUM_PARTICLES << " PARTIKEL" << std::endl;
    std::cout << "==========================================================" << std::endl;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        // Duplikat data agar pengujian serial & paralel berjalan pada kondisi data yang sama persis
        std::vector<Particle> particles_serial = particles;
        std::vector<Particle> particles_parallel = particles;

        // ---- 1. PENGUJIAN VERSI SERIAL (Single-Thread) ----
        double start_serial = omp_get_wtime();
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            if (particles_serial[i].splashTimer > 0) {
                particles_serial[i].splashTimer--;
                if (particles_serial[i].splashTimer == 0) {
                    particles_serial[i].y = -(rand() % 50);
                    particles_serial[i].x = rand() % SCREEN_WIDTH;
                    particles_serial[i].speedY = (rand() % 5) + 6.0f;
                }
            } else {
                particles_serial[i].speedY += 0.05f; 
                particles_serial[i].y += particles_serial[i].speedY;
                particles_serial[i].x += particles_serial[i].speedX;
                if (particles_serial[i].y >= SCREEN_HEIGHT - 10) {
                    particles_serial[i].y = SCREEN_HEIGHT - 10;
                    particles_serial[i].splashTimer = 8;
                }
            }
        }
        double end_serial = omp_get_wtime();
        double serial_time_ms = (end_serial - start_serial) * 1000.0; // Konversi ke milidetik (ms)

        // ---- 2. PENGUJIAN VERSI PARALEL (OpenMP Multi-Thread) ----
        double start_parallel = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            if (particles_parallel[i].splashTimer > 0) {
                particles_parallel[i].splashTimer--;
                if (particles_parallel[i].splashTimer == 0) {
                    particles_parallel[i].y = -(rand() % 50);
                    particles_parallel[i].x = rand() % SCREEN_WIDTH;
                    particles_parallel[i].speedY = (rand() % 5) + 6.0f;
                }
            } else {
                particles_parallel[i].speedY += 0.05f; 
                particles_parallel[i].y += particles_parallel[i].speedY;
                particles_parallel[i].x += particles_parallel[i].speedX;
                if (particles_parallel[i].y >= SCREEN_HEIGHT - 10) {
                    particles_parallel[i].y = SCREEN_HEIGHT - 10;
                    particles_parallel[i].splashTimer = 8;
                }
            }
        }
        double end_parallel = omp_get_wtime();
        double parallel_time_ms = (end_parallel - start_parallel) * 1000.0; // Konversi ke milidetik (ms)

        // Gunakan hasil perhitungan paralel untuk render utama
        particles = particles_parallel;

        // Akumulasi rata-rata performa
        total_serial_time += serial_time_ms;
        total_parallel_time += parallel_time_ms;
        frame_count++;

        // Setiap 60 frame sekali (kira-kira 1 detik), tampilkan statistik real-time ke layar terminal
        if (frame_count % 60 == 0) {
            double avg_serial = total_serial_time / frame_count;
            double avg_parallel = total_parallel_time / frame_count;
            double speedup = avg_serial / avg_parallel;

            std::cout << "\r[BENCHMARK] Partikel: " << NUM_PARTICLES 
                      << " | Serial: " << std::fixed << std::setprecision(3) << avg_serial << " ms"
                      << " | Paralel: " << avg_parallel << " ms"
                      << " | Speedup: " << speedup << "x" << std::flush;
        }

        // ---- 3. RENDERING GRAFIS (SDL2) ----
        SDL_SetRenderDrawColor(renderer, 10, 15, 25, 255); 
        SDL_RenderClear(renderer);

        for (int i = 0; i < NUM_PARTICLES; ++i) {
            if (particles[i].splashTimer > 0) {
                SDL_SetRenderDrawColor(renderer, 170, 210, 255, 255);
                SDL_RenderDrawLine(renderer, 
                    (int)particles[i].x - 4, (int)particles[i].y, 
                    (int)particles[i].x + 4, (int)particles[i].y);
            } else {
                SDL_SetRenderDrawColor(renderer, 130, 180, 255, 200);
                SDL_RenderDrawLine(renderer, 
                    (int)particles[i].x, (int)particles[i].y, 
                    (int)particles[i].x, (int)particles[i].y + (int)particles[i].length);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); 
    }

    // Kesimpulan akhir yang tercetak di konsol setelah program ditutup
    std::cout << std::endl << "==========================================================" << std::endl;
    std::cout << "                  KESIMPULAN AKHIR PENGUJIAN              " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << "Rata-rata Waktu Update Serial   : " << (total_serial_time / frame_count) << " ms" << std::endl;
    std::cout << "Rata-rata Waktu Update Paralel  : " << (total_parallel_time / frame_count) << " ms" << std::endl;
    std::cout << "Faktor Percepatan (Speedup)     : " << (total_serial_time / total_parallel_time) << "x" << std::endl;
    std::cout << "==========================================================" << std::endl;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

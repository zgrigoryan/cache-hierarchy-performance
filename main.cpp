#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>  



static const int ARRAY_SIZE = 16 * 1024 * 1024;  // 16 million
static const int NUM_ACCESSES = 10000000;     

int main() {
    int* array = new int[ARRAY_SIZE];

    std::vector<int> stride_sizes = {1, 2, 4, 8, 16, 64, 256, 1024, 4096, 16384};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = dist(gen);
    }

    // warm-up: traverse the entire array sequentially
    volatile int temp = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        temp += array[i];
    }

    std::cout << std::setw(10) << "Stride" << " | " 
              << std::setw(12) << "Avg Time (us)" << std::endl;
    std::cout << std::string(25, '-') << std::endl;

    const int TRIALS = 5;

    for (auto stride : stride_sizes) {
        long long total_time = 0;

        for (int t = 0; t < TRIALS; t++) {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < NUM_ACCESSES; i++) {
                int index = (i * stride) % ARRAY_SIZE;
                array[index] += 1;
            }

            auto end = std::chrono::high_resolution_clock::now();
            long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_time += us;
        }

        long long avg_time = total_time / TRIALS;

        std::cout << std::setw(10) << stride << " | " 
                  << std::setw(12) << avg_time << std::endl;
    }

    delete[] array;
    return 0;
}

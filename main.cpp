#include <iostream>
#include <chrono>
#include <random>

int main() {
    const int ARRAY_SIZE = 1000000;
    int* array = new int[ARRAY_SIZE];

    int stride_sizes[] = {1, 4, 16, 64, 256, 1024, 4096, 16384, 65536};
    int num_strides = sizeof(stride_sizes) / sizeof(stride_sizes[0]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = dist(gen);
    }

    // access the array to load it into cache
    volatile int temp = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        temp += array[i];
    }

    std::cout << "Stride Size, Avg Time (microseconds)" << std::endl;

    for (int i = 0; i < num_strides; i++) {
        // run multiple trials and average the time
        const int trials = 5;  
        long long total_time = 0;

        for (int t = 0; t < trials; t++) {
            auto start = std::chrono::high_resolution_clock::now();

            for (int j = 0; j < ARRAY_SIZE; j += stride_sizes[i]) {
                array[j] += 1; // accessing memory
            }

            auto end = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        std::cout << stride_sizes[i] << ", " << (total_time / trials) << std::endl;
    }

    delete[] array;  
    return 0;
}

#include <functional>
#include <iostream>
#include <queue>
#include <ctime>
#include <chrono>
#include "graph.hpp"
#include "random.hpp"
#include "tools.hpp"
#include <string>
int main(){

    double *weights = new double[2000];
    verFromVerToDistance v;

    for (size_t d = 0; d < 2; ++d) {
        std::ofstream out("tests/" + std::to_string(d) + ".txt");
        std::cout << "=======naiveDijkstraHeapForAll========" << "\n";
        for (size_t n = 200 ; n <= 2000; n+=200) {
            Graph g(adjacencyMatrix(n, d));
            auto start = std::chrono::high_resolution_clock::now();
            naiveDijkstraHeapForAll(g, v);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end-start;
            std::cout << n << "\t" << diff.count() << "\n";
        }

        std::cout << "=======floyd========" << "\n";
        for (size_t n = 200 ; n <= 2000; n+=200) {
            Graph g(adjacencyMatrix(n, d));
            auto start = std::chrono::high_resolution_clock::now();
            floyd(g, v);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end-start;
            std::cout << n << "\t" << diff.count() << "\n";
        }

        std::cout << "=======naiveDijkstraForAll========" << "\n";
        for (size_t n = 200 ; n <= 2000; n+=200) {
            Graph g(adjacencyMatrix(n, d));
            auto start = std::chrono::high_resolution_clock::now();
            naiveDijkstraForAll(g, v);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end-start;
            std::cout << n << "\t" << diff.count() << "\n";
        }

        std::cout << "=======bellmanForAll========" << "\n";
        for (size_t n = 200 ; n <= 2000; n+=200) {
            Graph g(adjacencyMatrix(n, d));
            auto start = std::chrono::high_resolution_clock::now();
            bellmanForAll(g, v);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end-start;
            std::cout << n << "\t" << diff.count() << "\n";
        }

    }



}
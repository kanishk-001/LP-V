# LP-V

# HPC Lab: Parallel Algorithms using OpenMP and MPI

This document covers the implementation and execution of Parallel BFS, DFS, Sorting (Bubble, Merge, Quick), and Reduction operations)

---

## 1. Implementation Overview

| Task | Parallel Strategy | Tool |
| :--- | :--- | :--- |
| **BFS** | Level-order parallelization using `#pragma omp parallel for` | OpenMP |
| **DFS** | Recursive branch exploration using `#pragma omp task` | OpenMP |
| **Bubble Sort** | Odd-Even Transposition algorithm | OpenMP |
| **Merge Sort** | Recursive division using `#pragma omp parallel sections` | OpenMP |
| **Reduction** | Data aggregation (Min/Max/Sum) using `reduction` clause | OpenMP |
| **Quicksort** | Distributed data processing using Scatter/Gather | MPI |

---

### 2. Environment Setup (Ubuntu)

sudo apt update

sudo apt install build-essential libomp-dev mpi-default-bin libmpi-dev -y

### 3. Compilation

## Pract-1 Parallel BFS & DFS (OpenMP):
Compilation: g++ -fopenmp bfs_dfs.cpp -o bfs_dfs

Run-  ./bfs_dfs

## Pract-2 Parallel Bubble & Merge Sort (OpenMP):
Compilation: g++ -fopenmp sorting.cpp -o sorting

Run-  ./sorting

## Pract-3 Parallel Reduction (Min, Max, Sum, Avg):
Compilation: g++ -fopenmp reduction.cpp -o reduction

Run-   ./reduction

## Pract-4: Parallel Quicksort (MPI):
Compilation: mpic++ quick_mpi.cpp -o quick_mpi

Run-  mpirun -np 2 ./quick_mpi









# thread_block简介

`thread_block`是对线程块的抽象，它代表了核函数（Kernel）启动的单个线程块（线程块内线程数量等）。

## Public成员函数

```cpp
static void sync();
static unsigned int thread_rank();
static dim3 group_index();
static dim3 thread_index();
static dim3 dim_threads();
static unsigned int num_threads();
static unsigned int size();
static dim3 group_dim();
```

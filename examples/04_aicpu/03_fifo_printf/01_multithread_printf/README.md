# AICPU 多线程 printf 样例

## 概述

本样例演示在 AICPU 内核中创建多个线程，并发调用 `AscendC::printf`，验证 FIFO ringbuf 在多线程场景下的写入正确性。

## 原理

- AICPU 运行在 Linux 用户态，支持标准 C++ 多线程（`std::thread`）
- FIFO printf 内部使用 `std::mutex` 保证多线程写入的原子性
- 每条 printf 输出为一个独立 entry：`[PrintfInfo 头 | 格式化字符串\0]`，8 字节对齐
- 多线程并发时，mutex 串行化空间预留 + 数据写入，确保 entry 完整性

## 样例内容

- `aicpu_multithread.aicpu`：AICPU 内核，创建 4 个线程，每个线程输出 10 条 printf
- `main.asc`：主机端入口，分配参数内存并下发内核
- `kernel_args.h`：参数结构体定义
- `CMakeLists.txt`：构建配置

## 运行方式

```bash
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201
make
./demo
```

## 预期输出

rts 端读取 ringbuf 后输出类似：

```
MultiThreadPrintfKernel start, threadNum=4
[thread 0] print 0: value=0 str=hello
[thread 0] print 1: value=1 str=hello
...
[thread 3] print 9: value=309 str=hello
MultiThreadPrintfKernel done, all threads joined
```

## 注意事项

- 需 rts 提供的 `AicpuGetDfxInfo` weak symbol 才能走 FIFO 路径
- 若 rts 未提供，会自动回退到 legacy 线性缓冲区路径

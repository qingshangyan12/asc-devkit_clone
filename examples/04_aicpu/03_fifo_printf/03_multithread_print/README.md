# AICPU 多线程执行同一打印算子样例

## 概述

本样例演示在 host 侧创建多个线程，每个线程同时下发同一个 AICPU 打印算子到各自的 stream，验证多线程并发调用 AICPU printf 时，FIFO ringbuf 的写入是否正确、打印信息是否完整无错乱。

## 原理

- host 侧创建 4 个 `std::thread`，每个线程独立创建 stream 并下发 `MultiThreadPrintKernel`
- AICPU 内核内循环执行多次 `AscendC::printf`，输出带线程标识和序号的信息
- FIFO printf 内部使用 `std::mutex` 保证多线程写入 ringbuf 的原子性
- 每个 entry 完整写入后才将 `printfType` 设为 READY，rts 读取时不会看到半成品

## 样例内容

| 文件 | 说明 |
| ---- | ---- |
| `aicpu_multithread_print.aicpu` | AICPU 内核，循环执行 printf |
| `main.asc` | host 端入口，多线程下发内核 |
| `kernel_args.h` | 参数结构体定义 |
| `CMakeLists.txt` | 构建配置 |

## 运行方式

```bash
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201
make
./demo
```

## 预期输出

多个线程的 printf 输出交错但每条信息完整：

```
MultiThreadPrintKernel start, threadNum=4
[thread 0] print 0: hello from aicpu
[thread 1] print 0: hello from aicpu
[thread 0] print 1: hello from aicpu
...
MultiThreadPrintKernel done
MultiThreadPrintKernel done
...
```

## 验证要点

1. 每条 printf 信息完整，不存在截断或乱序拼接
2. 各线程的输出可以交错，但同一条信息不会被其他线程的输出打断
3. 总输出条数 = 线程数 × (2 + threadNum × printCount) 的内核启动次数

# AICPU 多进程注册执行算子样例

## 概述

本样例演示 host 侧 `fork` 多个子进程，每个进程独立执行 `aclInit` → 下发 AICPU 打印算子，验证多进程场景下每个进程的 AICPU printf 信息是否正常隔离和输出。

## 原理

- 父进程 `fork` 3 个子进程，每个子进程独立调用 `aclInit`/`aclrtSetDevice`/`aclrtCreateStream`
- 每个子进程下发 `MultiprocessPrintKernel`，内核内执行多次 printf 输出进程标识信息
- 每个进程有独立的 AICPU 上下文，FIFO ringbuf 应按进程隔离
- rts 为每个进程维护独立的 `AicpuGetDfxInfo` 和 ringbuf 内存

## 样例内容

| 文件 | 说明 |
| ---- | ---- |
| `aicpu_multiprocess_print.aicpu` | AICPU 内核，输出带进程标识的 printf |
| `main.asc` | host 端入口，fork 多个子进程 |
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

各进程的 printf 输出按进程隔离，每个进程的信息完整：

```
MultiprocessPrintKernel start, procId=0
[proc 0] print 0: hello from aicpu
[proc 0] print 1: hello from aicpu
...
MultiprocessPrintKernel done, procId=0
MultiprocessPrintKernel start, procId=1
[proc 1] print 0: hello from aicpu
...
MultiprocessPrintKernel done, procId=1
MultiprocessPrintKernel start, procId=2
...
All 3 child processes completed
```

## 验证要点

1. 每个进程的 printf 信息只包含自己的 `procId`，不混入其他进程的信息
2. 每条 printf 信息完整，不存在截断
3. 3 个进程的输出可以交错（取决于 rts 读取时机），但同一条信息不会被打断
4. 所有子进程正常退出，父进程打印 "All 3 child processes completed"

# 使用CAPI关闭数据搬运随路原子操作

## 概述

本样例验证`asc_disable_dma_atomic`关闭数据搬运随路原子操作的功能。样例先开启原子累加并执行一次搬运，再调用新接口关闭原子操作，最后执行一次普通搬运。

## 支持的产品

- Atlas A2训练系列产品/Atlas A2推理系列产品

## 目录结构

```text
asc_disable_dma_atomic/
├── CMakeLists.txt
├── README.md
├── README_en.md
├── asc_disable_dma_atomic.asc
└── run.sh
```

## 验证方法

输入数据为`2.0`，两个输出缓冲区初始值均为`1.0`。开启原子累加后，第一次搬运结果应为`3.0`；调用`asc_disable_dma_atomic`后，第二次搬运结果应为`2.0`。

## 合入条件分析

1. 新接口在本项目的`include/c_api/atomic/memcpy_atomic.h`中声明，并在本项目的2201实现中定义。
2. CMake通过本项目的`include`和`impl`目录优先查找头文件，CANN包只提供编译器和运行时环境。
3. 用例同时验证关闭前后的搬运结果，能够证明新接口实际执行并生效。

结论：满足合入条件。

## 编译运行

```bash
./run.sh
```

预期输出：

```text
[Success] asc_disable_dma_atomic verification passed.
```

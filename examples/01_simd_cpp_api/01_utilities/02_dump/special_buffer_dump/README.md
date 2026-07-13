# 3510量化MatMul特殊Buffer Dump样例

## 概述

本样例以HiFloat8量化矩阵乘算子为背景，展示在Ascend 950PR/Ascend 950DT上使用`DumpTensor`打印
L1 Buffer、Bias Table Buffer和Fixpipe Buffer数据的方法。算子完成
`C = (A * B + bias) * scale`，其中A、B为HiFloat8，累加和Bias为float，Fixpipe使用per-channel
scale将结果转换为bfloat16。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 | HDK版本 |
|------|-------------|---------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 | >= 25.7.0 |

仓内`DumpTensor`和`asc_dump`接口资料明确要求：在Ascend 950PR/Ascend 950DT上打印L1 Buffer数据时，
HDK版本需要为25.7.0及以上。本样例包含L1 Buffer打印，因此整体采用该版本下限。L1、Bias Table
Buffer和Fixpipe Buffer打印均通过dav-3510调试总线完成。

## 目录结构介绍

```text
├── special_buffer_dump
│   ├── scripts
│   │   └── verify_dump.py       // 解析并校验dump日志
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── special_buffer_dump.asc  // 量化MatMul实现、DumpTensor调用和Host校验
│   ├── README.md                // 中文样例说明文档
│   └── README_en.md             // 英文样例说明文档
```

## 算子流程

样例使用固定矩阵形状`M=16, K=32, N=16`，单核执行如下流程：

1. 将HiFloat8矩阵A、B从Global Memory搬入L1，再搬入L0A、L0B。
2. 将float Bias从Global Memory搬入L1并以`desc=101`打印，再搬入Bias Table Buffer并以
   `desc=201`打印。MMAD使用Bias Table Buffer作为C矩阵初值，完成`A * B + bias`。
3. 将per-channel `uint64_t` scale从Global Memory搬入L1并以`desc=102`打印，再搬入
   Fixpipe Buffer并以`desc=301`打印。
4. Fixpipe读取该scale，将L0C中的float结果随路量化为bfloat16并写回Global Memory。
5. Host校验矩阵乘输出，`scripts/verify_dump.py`同时校验四组DumpTensor日志。

输入A、B均为HiFloat8数值`1.0`，Bias为`0~15`，偶数列scale为`1.0`，奇数列scale为`0.5`。
因此每行第`n`列的期望结果为`(32 + bias[n]) * scale[n]`。

## 打印标识

| desc | 数据位置 | 算子中的用途 |
|------|----------|--------------|
| 101 | L1 Buffer | 搬入Bias Table Buffer前的float Bias |
| 201 | Bias Table Buffer | MMAD用于初始化C矩阵的float Bias |
| 102 | L1 Buffer | 搬入Fixpipe Buffer前的per-channel `uint64_t` scale |
| 301 | Fixpipe Buffer | Fixpipe随路量化实际读取的`uint64_t` scale |

## Fixpipe Buffer数据规则

在dav-3510上，量化参数从L1 Buffer写入Fixpipe Buffer后，物理Buffer中可见的数据满足：

```text
fixpipe_value = l1_value & 0x00007fe0ffffe0ff
```

该掩码保留位`[7:0]`、`[31:13]`和`[46:37]`，清零位`[12:8]`、`[36:32]`和`[63:47]`。
本样例按照per-channel量化参数的有效格式构造输入：低32位保存截断后的float scale，第46位为Sign标志，
因此`desc=301`应与对`desc=102`应用上述掩码后的结果一致。

Fixpipe Buffer逻辑地址的高16位选择参数区域：`0`表示量化参数区，`1`表示ReLU参数区，低16位表示
区内偏移。本样例从地址`0`使用量化参数区。

## 编译运行

在本样例根目录下执行：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
./demo 2>&1 | tee run.log
python3 ../scripts/verify_dump.py run.log
```

`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

校验通过时输出：

```text
Bias L1 raw input check: PASS
Bias Table raw data check: PASS
Fixpipe L1 raw input check: PASS
Fixpipe quant visible-mask check: PASS
QuantMatmul result check: PASS
test pass!
```

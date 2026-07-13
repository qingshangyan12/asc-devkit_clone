# HiFloat8量化矩阵乘算子样例

## 概述

本样例实现`QuantMatmulHifp8`算子。算子输入矩阵A、B的数据类型为HiFloat8，使用float类型的Bias完成
矩阵乘累加，再由Fixpipe按照per-channel scale将结果转换为bfloat16，计算公式为
`C = (A * B + bias) * scale`。算子执行过程中使用`DumpTensor`查看Bias和scale在L1 Buffer、
Bias Table Buffer和Fixpipe Buffer中的数据，重点介绍dav-3510特殊Buffer的数据打印方法。

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
│   │   ├── gen_data.py          // 生成输入数据和golden数据
│   │   ├── verify_result.py     // 校验算子输出和golden数据
│   │   └── verify_dump.py       // 解析并校验dump日志
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 二进制文件读写函数
│   ├── special_buffer_dump.asc  // 量化MatMul实现、DumpTensor调用和Host调用
│   ├── README.md                // 中文样例说明文档
│   └── README_en.md             // 英文样例说明文档
```

## 样例描述

- 样例功能：

  使用Cube编程模式实现带Bias和per-channel scale的HiFloat8量化矩阵乘。对于输出矩阵中的元素
  `C[m, n]`，计算过程如下：

  ```text
  C[m, n] = (sum(A[m, k] * B[k, n]) + bias[n]) * scale[n]
  ```

  MMAD以Bias Table Buffer中的float Bias作为C矩阵初值，完成HiFloat8矩阵乘累加；Fixpipe从
  Fixpipe Buffer读取per-channel scale，将float累加结果转换为bfloat16并写回Global Memory。
  `DumpTensor`用于观察Bias和scale在上述计算路径中的实际数据。

- 样例规格：

  样例使用固定参数`M=16, K=32, N=16`，shape信息如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">QuantMatmulHifp8</td></tr>
  <tr><td rowspan="5" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">hifloat8</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">hifloat8</td><td align="center">ND</td></tr>
  <tr><td align="center">bias</td><td align="center">[N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">scale</td><td align="center">[N]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">bfloat16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">QuantMatmulHifp8</td></tr>
  </table>

- 算子实现：

  算子由单个AI Core执行，处理流程如下：

  1. 将HiFloat8矩阵A、B从Global Memory搬入L1，再搬入L0A、L0B。
  2. 将float Bias从Global Memory搬入L1，再搬入Bias Table Buffer。MMAD使用Bias Table Buffer
     作为C矩阵初值，完成`A * B + bias`。
  3. 将per-channel `uint64_t` scale从Global Memory搬入L1，再搬入Fixpipe Buffer。
  4. Fixpipe读取scale，将L0C中的float结果转换为bfloat16并写回Global Memory。
  5. Host从二进制文件读取输入，执行算子并将结果写入`output/output.bin`。

  输入A、B均为HiFloat8数值`1.0`，Bias为`0~15`，偶数列scale为`1.0`，奇数列scale为`0.5`。
  因此每行第`n`列的期望结果为`(32 + bias[n]) * scale[n]`。

- DumpTensor接口用法：

  算子在Bias和scale进入计算单元前分别打印L1 Buffer和目标特殊Buffer，用相同数据在搬运前后的
  打印结果验证数据路径。`desc`用于区分打印记录，`dumpSize`为打印的元素个数。本样例的打印位置如下：

  | desc | 数据位置 | 算子中的用途 |
  |------|----------|--------------|
  | 101 | L1 Buffer | 搬入Bias Table Buffer前的float Bias |
  | 201 | Bias Table Buffer | MMAD用于初始化C矩阵的float Bias |
  | 102 | L1 Buffer | 搬入Fixpipe Buffer前的per-channel `uint64_t` scale |
  | 301 | Fixpipe Buffer | Fixpipe量化实际读取的`uint64_t` scale |

  `scripts/gen_data.py`生成四个输入文件和`output/golden.bin`。`scripts/verify_result.py`独立校验算子
  输出，`scripts/verify_dump.py`直接读取Python生成的Bias和scale文件，校验四组`DumpTensor`日志。

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
python3 ../scripts/gen_data.py
./demo 2>&1 | tee run.log
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
python3 ../scripts/verify_dump.py run.log input/bias_gm.bin input/scale_gm.bin
```

`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

校验通过时输出：

```text
test pass!
Bias L1 raw input check: PASS
Bias Table raw data check: PASS
Fixpipe L1 raw input check: PASS
Fixpipe quant visible-mask check: PASS
test pass!
```

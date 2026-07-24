# SimdVF Printf接口功能说明

## 概述

本样例介绍simd_vf侧printf接口的FIFO传输能力。核函数通过单次`asc_vf_call`打印超过2KB的数据，验证打印数据跨越预留UB边界后仍可完整输出。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── simd_vf_printf
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── simd_vf_printf.asc      // Ascend C样例实现&调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：

  使用vector编程模式，在一个simd_vf函数中连续打印32条带序号的固定格式记录。每条记录至少96字节，总文本数据至少3072字节，超过2KB预留UB空间。

- 调用实现

  在`__global__ __vector__`核函数内只调用一次`asc_vf_call<SimdVfPrintFifoData>(PRINT_RECORD_COUNT)`。simd_vf函数内部循环调用`printf`，打印从`record=0`到`record=31`的完整序列。输出跨越FIFO缓冲区边界，因此完整看到首尾记录可验证FIFO持续传输正常。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程
  ./demo                                                                     # 执行样例
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |

- 执行结果
  执行日志中应包含从`record=0`到`record=31`的全部32条simd_vf记录。以下省略中间记录：
  ```bash
  [AIV Block 0/1] [aicore] simd_vf fifo printf demo start ...
  [simd_vf][fifo] record=0 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  [simd_vf][fifo] record=1 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  ...
  [simd_vf][fifo] record=31 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  [AIV Block 0/1] [aicore] simd_vf fifo printf demo end ...
  ```

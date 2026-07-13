# 3510特殊Buffer Dump样例

## 概述

本样例展示在Ascend 950PR/Ascend 950DT上使用`DumpTensor`打印L1 Buffer、Bias Table Buffer和Fixpipe Buffer数据的方法，并通过固定输入说明Fixpipe Buffer量化参数区的数据掩码规则。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 | HDK版本 |
|------|-------------|---------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 | >= 25.7.0 |

仓内`DumpTensor`和`asc_dump`接口资料明确要求：在Ascend 950PR/Ascend 950DT上打印L1 Buffer数据时，HDK版本需要为25.7.0及以上。本样例包含L1 Buffer打印，因此整体采用该版本下限。Bias Table Buffer和Fixpipe Buffer与L1 Buffer打印均通过dav-3510调试总线完成，未在接口资料中规定更高的独立HDK版本下限。

## 目录结构介绍

```text
├── special_buffer_dump
│   ├── scripts
│   │   └── verify_dump.py       // 解析并校验dump日志
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── special_buffer_dump.asc  // Ascend C样例实现及调用代码
│   ├── README.md                // 中文样例说明文档
│   └── README_en.md             // 英文样例说明文档
```

## 样例描述

- 样例功能：

  1. 将`int32_t`输入从Global Memory搬入L1 Buffer并打印，再搬入Bias Table Buffer并打印。两次打印结果与输入一致。
  2. 将`uint64_t`量化参数从Global Memory搬入L1 Buffer并打印，再搬入Fixpipe Buffer量化参数区并打印。L1 Buffer打印保留原始输入，Fixpipe Buffer打印展示硬件写入后的有效位。
  3. 使用`scripts/verify_dump.py`解析日志，根据`desc`字段区分四组数据并自动校验。

- 打印标识：

  | desc | 数据位置 | 说明 |
  |------|----------|------|
  | 101 | L1 Buffer | 搬入Bias Table Buffer前的`int32_t`原始数据 |
  | 201 | Bias Table Buffer | Bias Table Buffer中的`int32_t`数据 |
  | 102 | L1 Buffer | 搬入Fixpipe Buffer前的`uint64_t`原始数据 |
  | 301 | Fixpipe Buffer | Fixpipe Buffer量化参数区中的`uint64_t`数据 |

## Fixpipe Buffer数据掩码规则

在dav-3510上，量化参数从L1 Buffer写入Fixpipe Buffer后，物理Buffer中可见的数据满足如下规则：

```text
fixpipe_value = l1_value & 0x00007fe0ffffe0ff
```

该掩码保留位`[7:0]`、`[31:13]`和`[46:37]`，清零位`[12:8]`、`[36:32]`和`[63:47]`。因此，`desc=301`的打印结果可能与`desc=102`的L1原始数据不同，该现象由L1到Fixpipe Buffer搬运时的硬件位域写入规则导致，并非`DumpTensor`修改数据。

随路量化接口资料中，dav-3510量化参数的API有效字段为`[31:13]`的M1、`[45:37]`的Offset和第`46`位的Sign标志位；`[7:0]`虽可在物理Buffer dump中观察到，但不属于量化参数的API有效字段，不应作为量化配置使用。

Fixpipe Buffer的逻辑地址还包含区域选择信息：高16位为`0`表示量化参数区，高16位为`1`表示ReLU参数区，低16位表示区内偏移。本样例通过`LocalMemAllocator<Hardware::FIXBUF>`从地址`0`分配量化参数区，只展示量化参数区的掩码规则。

## 编译运行

在本样例根目录下执行以下步骤。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 编译并运行样例

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  ./demo 2>&1 | tee run.log
  python3 ../scripts/verify_dump.py run.log
  ```

- 执行结果

  日志中包含四组`DumpTensor`输出。Fixpipe Buffer输入包含无效位时，`desc=102`保留原始值，`desc=301`仅保留掩码规定的可见位。校验通过时输出如下：

  ```text
  Bias L1 raw input check: PASS
  Bias Table raw data check: PASS
  Fixpipe L1 raw input check: PASS
  Fixpipe quant visible-mask check: PASS
  test pass!
  ```

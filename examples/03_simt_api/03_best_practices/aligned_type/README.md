# SIMT 实现类型对齐样例

## 概述

本样例量化展示 **结构体类型对齐对 Global Memory 访存吞吐的影响**：对 15 种结构体做"逐元素数据搬运"，每个结构体测试32次，求 `搬运平均时长(ms)` 与 `搬运吞吐量(GB/s)`，让用户直观看到类型对齐的收益。

## 支持的产品

- Ascend 950PR / Ascend 950DT

## 支持的 CANN 软件版本

- \>= CANN 9.0.0

## 目录结构

```
├── aligned_type
│   ├── aligned_type.asc       # SIMT 实现 & 调用样例
│   ├── CMakeLists.txt         # cmake 编译文件
│   └── README.md
```

## 样例描述

### 样例规格

  15 个结构体按 sizeof 分组，相邻排列便于直接对照。几个特殊用例：
  - `Align3` —— padding 演示（用 1B padding 把 alignof 从 1 顶到 4）。
  - `Align12 / Misalign12` —— sizeof 非 2 的幂，`Misalign12` 数组 stride=12 带真实地址错位。
  - `Align16_8` / `Align32_16` / `Align32_8` —— 命名为 `Align<sizeof>_<alignof>`，是同尺寸结构体强制到中间对齐值的变体。16B 组凑齐 16/8/4 三档、32B 组凑齐 32/16/8/4 四档，构成"同尺寸、同元素数、仅指令宽度不同"的对照，用来定位"吞吐随元素增大下降是不是指令宽度造成的"。

  | 名称          | 字段     | 元素大小             | 对齐    | 期望编译器策略 |
  |---------------|----------|---------------------|---------|----------------|
  | `Align3`      | 3 × u8   | 4 B（含 1B padding）| 4 B     | 单条 32-bit 访存（含 1B padding） |
  | `Misalign3`   | 3 × u8   | **3 B**             | **1 B** | 拆为 3 条 8-bit 访存 |
  | `Align4`      | 4 × u8   | 4 B                 | 4 B     | 单条 32-bit 访存 |
  | `Misalign4`   | 4 × u8   | 4 B                 | **1 B** | 拆为 4 条 8-bit 访存 |
  | `Align8`      | 2 × u32  | 8 B                 | 8 B     | 单条 64-bit 访存 |
  | `Misalign8`   | 2 × u32  | 8 B                 | **4 B** | 拆为 2 条 32-bit 访存 |
  | `Align12`     | 3 × u32  | 16 B（含 4B padding）| 16 B   | 单条 128-bit 访存 |
  | `Misalign12`  | 3 × u32  | **12 B**            | **4 B** | 拆为 3 条 32-bit 访存，stride=12 真错位 |
  | `Align16`     | 4 × u32  | 16 B                | 16 B    | 单条 128-bit 访存 |
  | `Align16_8`   | 4 × u32  | 16 B                | **8 B** | 拆为 2 条 64-bit 访存 |
  | `Misalign16`  | 4 × u32  | 16 B                | **4 B** | 拆为 4 条 32-bit 访存 |
  | `Align32`     | 8 × u32  | 32 B                | 32 B    | 2 条 128-bit 访存 |
  | `Align32_16`  | 8 × u32  | 32 B                | **16 B**| 2 条 128-bit 访存 |
  | `Align32_8`   | 8 × u32  | 32 B                | **8 B** | 拆为 4 条 64-bit 访存 |
  | `Misalign32`  | 8 × u32  | 32 B                | **4 B** | 拆为 8 条 32-bit 访存 |

### 关键参数

  | 参数 | 值 | 说明 |
  |------|-----|------|
  | `MEM_BYTES`      | **512 MiB** | 输入/输出 buffer 字节数 |
  | `THREAD_COUNT`   | 2048   | 单核2048线程，gride固定<<<1, 2048>>> |
  | `NUM_ITERATIONS` | 32     | 同一 kernel 重复次数，取平均时间抹平计时抖动 |
  | `numElements`    | `MEM_BYTES / sizeof(TData)` | 每个用例的元素数，随结构体大小自动伸缩 |

### 核函数

  对长度为 `numElements` 的输入数组 `d_idata` 整体复制到 `d_odata`：`d_odata[i] = d_idata[i]`。

## 编译运行

  在本样例根目录下执行如下步骤，编译并执行算子。

  - 配置环境变量

    请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

    - 默认路径，root 用户安装 CANN 软件包

      ```bash
      source /usr/local/Ascend/cann/set_env.sh
      ```

    - 默认路径，非 root 用户安装 CANN 软件包

      ```bash
      source $HOME/Ascend/cann/set_env.sh
      ```

    - 指定路径 `install_path`，安装 CANN 软件包

      ```bash
      source ${install_path}/cann/set_env.sh
      ```

- 样例执行

  ```bash
  mkdir -p build && cd build;   # 创建并进入 build 目录
  cmake ..; make -j;            # 编译工程
  ./demo                        # 执行样例
  ```

## 典型输出

每个用例打印一行实测（元素数对应 `MEM_BYTES=512 MiB`）：

```
[Align3      ] sizeof= 4B alignof= 4B elems=134217728 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
[Misalign3   ] sizeof= 3B alignof= 1B elems=178956970 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
[Align4      ] sizeof= 4B alignof= 4B elems=134217728 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
[Misalign4   ] sizeof= 4B alignof= 1B elems=134217728 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
...
[Align32     ] sizeof=32B alignof=32B elems= 16777216 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
[Misalign32  ] sizeof=32B alignof= 4B elems= 16777216 Avg.time:   ... ms / Throughput:  ... GB/s.   [TEST OK]
```

末尾汇总表便于横向对比，对齐与非对齐成对相邻排列：

```
============================ Summary ============================
 TData       | sizeof | alignof |  elements |    ms    |  GB/s
-------------+--------+---------+-----------+----------+--------
 Align3      |     4B |      4B | 134217728 |   ...    |  ...
 Misalign3   |     3B |      1B | 178956970 |   ...    |  ...
 Align4      |     4B |      4B | 134217728 |   ...    |  ...
 Misalign4   |     4B |      1B | 134217728 |   ...    |  ...
 Align8      |     8B |      8B |  67108864 |   ...    |  ...
 Misalign8   |     8B |      4B |  67108864 |   ...    |  ...
 Align12     |    16B |     16B |  33554432 |   ...    |  ...
 Misalign12  |    12B |      4B |  44739242 |   ...    |  ...
 Align16     |    16B |     16B |  33554432 |   ...    |  ...
 Align16_8   |    16B |      8B |  33554432 |   ...    |  ...
 Misalign16  |    16B |      4B |  33554432 |   ...    |  ...
 Align32     |    32B |     32B |  16777216 |   ...    |  ...
 Align32_16  |    32B |     16B |  16777216 |   ...    |  ...
 Align32_8   |    32B |      8B |  16777216 |   ...    |  ...
 Misalign32  |    32B |      4B |  16777216 |   ...    |  ...
=================================================================

[aligned_type] -> Test Results: 0 failures.
```

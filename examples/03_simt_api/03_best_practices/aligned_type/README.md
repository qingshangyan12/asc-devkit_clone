# SIMT 实现类型对齐样例

## 概述

本样例量化展示 **结构体类型对齐对 Global Memory 访存吞吐的影响**：对 8 种结构体（4 组"对齐 / 非对齐"对照）做"逐元素数据搬运"，每个结构体测试 32 次，求 `搬运平均时长(ms)` 与 `搬运吞吐量(GB/s)`，让用户直观看到类型对齐的收益。

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

本样例选取 8 个结构体（按 sizeof 分 4 / 8 / 16 / 32 B 四组，每组各含一对"对齐 / 非对齐"对照），对每个结构体跑同一个 Global Memory 搬运核函数 `d_odata[i] = d_idata[i]`，重复 32 次取平均，输出搬运吞吐量（GB/s）。对照同组内"对齐 / 非对齐"两行的吞吐差距，就能直观看到类型对齐的收益。

所有用例搬运的总字节数相同（都是 `MEM_BYTES = 512 MiB`），吞吐 = 数据量 ÷ 总耗时，因此**吞吐差距完全取决于"搬完这些字节需要发射多少条访存指令"**。编译器为每次访存挑选指令宽度时，同时受两个上界约束：

- **硬件最大单次访存宽度**：本平台为 8 B（64-bit），所以即便 `Align16` / `Align32` 也看不到单条 128-bit 访存，只能拆成多条 64-bit 指令完成；
- **编译期可证明的 `alignof`**：`alignof=4` 把单次宽度封顶到 32-bit，`alignof=1` 封顶到 8-bit。`Misalign4` / `Misalign16` / `Misalign32` 都是被这一条压住的。

两条约束取更紧的一个，即为下表「单次访存」列；元素剩下的字节按该宽度切片，即为「次数」列——也就是搬运一个元素需要发射的访存指令条数。**指令条数越少，吞吐越高**。四组对照分别说明：

- **4 B 组**：`Align4` 1 条 32-bit 指令 vs `Misalign4` 4 条 8-bit 指令，指令数差 **4 倍**；
- **8 B 组**：两者都是 1 条 64-bit 指令——8 B 结构体能作为整体载入单个 64-bit 寄存器，编译器会把整个结构体合并成一条访存指令，`alignof=4` 在这个尺寸上没成为瓶颈。**这是 8 B 档独有的红利，不要指望它在更大的结构体上同样奏效**；
- **16 B 组**：`Align16` 2 条 64-bit vs `Misalign16` 4 条 32-bit，搬运总位宽相同，但指令数差 **2 倍**；
- **32 B 组**：`Align32` 4 条 64-bit vs `Misalign32` 8 条 32-bit，指令数同样差 **2 倍**。

**实践建议**：对热点路径上承载大量数据搬运的结构体，显式用 `alignas` 拉到 8 B 及以上，让编译器拿到 64-bit 访存指令，是收益最稳定的对齐策略。

### 样例规格

  8 个结构体按 sizeof 分 4 组（4 / 8 / 16 / 32 B），每组一对「对齐 / 非对齐」相邻排列，便于直接对照。`Align<N>` 用 `alignas(N)` 显式拉到自然对齐；`Misalign<N>` 不显式 `alignas`，由字段类型决定（4×u8 → alignof=1，其余 → alignof=4）。

  | 名称        | 字段     | sizeof | alignof | 单次访存     | 次数 |
  |-------------|----------|--------|---------|--------------|------|
  | `Align4`    | 4 × u8   |  4 B   |   4 B   | 4 B (32-bit) |  1   |
  | `Misalign4` | 4 × u8   |  4 B   | **1 B** | 1 B  (8-bit) |  4   |
  | `Align8`    | 2 × u32  |  8 B   |   8 B   | 8 B (64-bit) |  1   |
  | `Misalign8` | 2 × u32  |  8 B   | **4 B** | 8 B (64-bit) |  1   |
  | `Align16`   | 4 × u32  | 16 B   |  16 B   | 8 B (64-bit) |  2   |
  | `Misalign16`| 4 × u32  | 16 B   | **4 B** | 4 B (32-bit) |  4   |
  | `Align32`   | 8 × u32  | 32 B   |  32 B   | 8 B (64-bit) |  4   |
  | `Misalign32`| 8 × u32  | 32 B   | **4 B** | 4 B (32-bit) |  8   |

  - **单次访存**：编译器为本结构体每次访存实际选择的指令宽度。  
  - **次数**：搬运一个元素需要发射的访存指令条数，等于 `sizeof / 单次访存字节数`。

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
 Align4      |     4B |      4B | 134217728 |   ...    |  ...
 Misalign4   |     4B |      1B | 134217728 |   ...    |  ...
 Align8      |     8B |      8B |  67108864 |   ...    |  ...
 Misalign8   |     8B |      4B |  67108864 |   ...    |  ...
 Align16     |    16B |     16B |  33554432 |   ...    |  ...
 Misalign16  |    16B |      4B |  33554432 |   ...    |  ...
 Align32     |    32B |     32B |  16777216 |   ...    |  ...
 Misalign32  |    32B |      4B |  16777216 |   ...    |  ...
=================================================================

[aligned_type] -> Test Results: 0 failures.
```

对照「次数」列读这张汇总表：吞吐差距应当与指令数比例（4 / 1 / 2 / 2 倍）大致一致；`Misalign8` 因为编译器对 8 B 结构体做了整体合并加载，吞吐会贴近 `Align8`。

# SIMT 类型对齐优化搬运效率最佳实践样例

## 概述

本样例量化展示 **结构体类型对齐对 Global Memory 访存的影响**：结合本平台访存指令支持 1 B、2 B、4 B、8 B 四档数据宽度的特性，对 14 个结构体做逐元素数据搬运，对比类型对齐带来的搬运效率差异。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的 CANN 软件版本

- \>= CANN 9.0.0

## 目录结构

```
├── 02_simt_aligned_type
│   ├── aligned_type.asc       // SIMT 实现 & 调用样例
│   ├── CMakeLists.txt         // cmake 编译文件
│   └── README.md
```

## 样例描述

本样例选取 14 个结构体（按 sizeof 分 4 / 8 / 16 / 32 B 五组等字段类型对照——其中 4 B 组覆盖 `alignof=1` 与 `alignof=2` 两档窄对齐，再补充 2 组字段类型不一致的混合结构体对照），依次将长度为 `numElements` 的输入数组 `d_idata` 整体复制到 `d_odata`：`d_odata[i] = d_idata[i]`，对比搬运效率。

### 基础概念：`sizeof`、`alignof`、`alignas`

#### `alignof`

编译期运算符，返回类型的对齐要求，即该类型对象的起始地址必须是多少字节的倍数。结构体的自然 alignof 等于其所有字段 alignof 的最大值（如下例 `T`），可被 `alignas` 进一步加严（如下例 `T2`）。

```cpp
struct T { uint8_t a; uint32_t b; };  // alignof = max(1, 4) = 4
struct alignas(8) T2 { uint8_t a; uint32_t b; };  // alignof = 8 (alignas 放大)
```

#### `alignas`

C++11 引入的对齐说明符，在声明类型/对象时**强制加严**对齐。它不改变成员排布，只改变整体对齐和必要的尾部填充。

`alignas` 的几条限制：

- **只能放大，不能缩小**：`alignas(N)` 中的 `N` 必须 ≥ 类型的"自然 alignof"（即所有字段 alignof 的最大值），否则编译器会忽略或报错。
- **取值必须是 2 的幂**：`alignas(N)` 中的 `N` 必须是 2 的幂。
- **会同步抬升 sizeof**：`sizeof` 必须是 `alignof` 的整数倍，所以放大 alignof 时，sizeof 会向上进位。

```cpp
struct alignas(4) Align3 { unsigned char r, g, b; }; // alignof=4 sizeof=4 (alignas 抬升)
struct         Misalign3 { unsigned char r, g, b; }; // alignof=1 sizeof=3 (自然)
```

#### `sizeof`

编译期运算符，返回类型在内存中占用的字节数，**包含为对齐而插入的填充字节**。

```cpp
struct alignas(4) AlignU8PadU16 {
    uint8_t  a;   // 偏移 0，占 1 字节（该 struct 要求 2 字节对齐，编译器在 a 之后插入了 1 字节 padding）
    uint16_t b;   // 偏移 2，占 2 字节（要求 2 字节对齐）
};                // sizeof=4, alignof=4
```

### 关键参数

| 参数 | 值 | 说明 |
|------|-----|------|
| `MEM_BYTES`      | **512 MiB** | 输入/输出 buffer 字节数 |
| `THREAD_COUNT`   | 2048   | 单核 2048 线程，grid 固定 `<<<1, 2048>>>` |
| `numElements`    | `MEM_BYTES / sizeof(TData)` | 每个用例的元素数，随结构体大小自动伸缩 |

### 样例规格

| 名称        | 字段     | sizeof | alignof | numElements  |
|-------------|----------|--------|---------|-----------|
| `Align4`    | 4 × u8   |  4 B   |   4 B   | 134217728 |
| `Misalign4` | 4 × u8   |  4 B   | **1 B** | 134217728 |
| `Align2U16`    | 2 × u16  |  4 B   |   4 B   | 134217728 |
| `Misalign2U16` | 2 × u16  |  4 B   | **2 B** | 134217728 |
| `Align8`    | 2 × u32  |  8 B   |   8 B   | 67108864  |
| `Misalign8` | 2 × u32  |  8 B   | **4 B** | 67108864  |
| `Align16`   | 4 × u32  | 16 B   |  16 B   | 33554432  |
| `Misalign16`| 4 × u32  | 16 B   | **4 B** | 33554432  |
| `Align32`   | 8 × u32  | 32 B   |  32 B   | 16777216  |
| `Misalign32`| 8 × u32  | 32 B   | **4 B** | 16777216  |
| `AlignU8U16`      | 2 × u8 + u16    |  4 B   |   4 B   | 134217728 |
| `MisalignU8U16`   | 2 × u8 + u16    |  4 B   | **2 B** | 134217728 |
| `AlignU8PadU16`   | u8 (1B padding) + u16    |  4 B   |   4 B   | 134217728 |
| `MisalignU8PadU16`| u8 (1B padding) + u16    |  4 B   | **2 B** | 134217728 |

## 性能分析

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Task 整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |
| aiv_time(us)        | Task 在 AI Vector Core 上的理论执行时间，单位为 us                                            |
| aiv_total_cycles    | 该 Task 被分配到每个 AI Vector Core 计算单元上后，每个 AI Vector Core 计算单元上的执行 cycle 总数 |
| aiv_vec_time(us)    | vec 类型指令（向量类运算指令）耗时，单位为 us                                               |
| aiv_vec_ratio       | vec 类型指令（向量类运算指令）的 cycle 数在 total cycle 数中的占用比                           |
| aiv_scalar_time(us) | scalar 类型指令（标量类运算指令）耗时，单位为 us                                            |

除 Task Duration 外，本例中其余指标均展示的为所有 block 上性能指标的平均值。

### 性能数据

|       TData       | Task Duration(us) | aiv_total_cycles |
| :---------------: | :---------------: | :--------------: |
|      Align4       |     27889.33      |   46005213.32    |
|     Misalign4     |     81223.02      |   134028903.60   |
|     Align2U16     |     27833.56      |   45918496.16    |
|   Misalign2U16    |     46656.61      |   76990974.19    |
|      Align8       |     17417.23      |   28735088.65    |
|     Misalign8     |     17415.04      |   28730779.26    |
|      Align16      |     32983.77      |   54417477.84    |
|    Misalign16     |     38353.60      |   63284236.16    |
|      Align32      |     60901.24      |   100477599.40   |
|    Misalign32     |     77790.93      |   128352303.30   |
|    AlignU8U16     |     27854.56      |   45939796.81    |
|   MisalignU8U16   |     46666.11      |   76970946.68    |
|   AlignU8PadU16   |     27809.72      |   45872788.00    |
| MisalignU8PadU16  |     46664.42      |   76969440.94    |

### 综合分析

本次所有测试用例的总搬运数据量统一为 512 MiB。在数据总量固定的前提下，访存带宽越高，`aiv_total_cycles`越小。可以看到，各用例的`Task Duration`与`aiv_total_cycles`呈明显正相关，二者变化倍数基本一致。因此下文将统一以`Task Duration`作为核心指标，开展后续性能分析。

#### 结论 1：类型对齐可以提升搬运效率，且 alignof 越严效率越高（上限 8 B）

| 对照组          | Task Duration(对齐) | Task Duration(非对齐) | 非对齐/对齐 |
| :-------------: | :-----------------: | :-------------------: | :---------: |
| 4 B (alignof=1) | 27889.33 us         | 81223.02 us           | **2.91x**   |
| 4 B (alignof=2) | 27833.56 us         | 46656.61 us           | **1.68x**   |
|  8 B            | 17417.23 us         | 17415.04 us           | **1.00x**   |
| 16 B            | 32983.77 us         | 38353.60 us           | **1.16x**   |
| 32 B            | 60901.24 us         | 77790.93 us           | **1.28x**   |

在同一 `sizeof` 下，非对齐版的访存指令宽度由 `alignof` 决定，带宽越小，相应访存流水 cycle 越多，搬运耗时越长。

- 4 B 组两行的对齐版`Task Duration`基本一致，但非对齐版随 `alignof` 由 1 → 2 → 4呈现典型渐变：差距从 2.91x → 1.68x → 1.00x，访存指令宽度同步翻倍。但需要注意的是，**指令宽度翻倍并不等于耗时减半**，因为在 kernel 函数里用for循环逐元素去load和store数据，不同元素间的load/store指令是可以流水并行的。以元素 `x1`、`x2` 为例，`store x1`指令要等待`load x1`搬运完成（写后读依赖），但是`store x1`执行期间 `load x2` 可同步进入访存流水。多条访存的执行时间被流水部分掩盖，因此 4 B 组实测加速比小于按指令条数推算的理论上界。

- 8 B 组因编译器合并加载例外。

#### 结论 2： sizeof 与 alignof 一致时，搬运效率相同

| 对照组（字段）              | Task Duration(对齐) | Task Duration(非对齐) | 非对齐/对齐 |
| :-------------------------- | :-----------------: | :-------------------: | :---------: |
| 4 B (2×u16)                 | 27833.56 us         | 46656.61 us           | **1.68x**   |
| 4 B (2×u8 + u16)            | 27854.56 us         | 46666.11 us           | **1.68x**   |
| 4 B (u8 + 1B padding + u16) | 27809.72 us         | 46664.42 us           | **1.68x**   |

三组结构体 sizeof / alignof 完全相同，Task Duration 也几乎完全一致：编译器只关心结构体整体的 `sizeof` 与 `alignof`，**字段是同类型还是混合类型、padding 是编译器隐式插入还是用户显式声明，对生成的访存指令序列与耗时没有影响**。

### 调优建议
  通过`alignas`对结构体进行加严对齐，确保其对齐值`alignof`至少匹配结构体大小`sizeof`，且不超过硬件单次最大访存宽度 8 B。这样可让单条访存指令尽可能覆盖整个数据元素，避免因窄位宽访存造成耗时增加。


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

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

### 样例执行

```bash
mkdir -p build && cd build           # 创建并进入 build 目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j   # 编译工程
./demo                                # 执行样例
```

### 编译选项说明

| 选项                      | 可选值     | 说明                                                          |
| ------------------------- | ---------- | ------------------------------------------------------------- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT）  |

### 执行结果

执行结果如下，说明样例运行成功。

```bash
[aligned_type] -> Test Results: 0 failures.
```
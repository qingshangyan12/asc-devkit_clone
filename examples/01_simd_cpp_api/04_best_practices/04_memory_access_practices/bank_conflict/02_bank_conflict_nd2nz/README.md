# ND2NZ bank冲突直调样例
## 概述
本样例展示 `ND2NZ` 随路转换过程中 `dstNzC0Stride` 参数对 UB bank group 写入地址分布的影响，直观对比冲突布局与均匀分散布局，并通过调整 `dstNzC0Stride` 给出规避方案。实现采用静态 tensor 编程风格，参考 best practices 目录下的 `00/01/02`，不使用 `TPipe/TQue`。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── 02_bank_conflict_nd2nz
│   ├── scripts
│   │   ├── gen_data.py                    // 输入数据和真值数据生成脚本
│   │   └── verify_result.py               // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                     // 编译工程文件
│   ├── data_utils.h                       // 数据读入写出函数
│   └── bank_conflict_nd2nz.asc            // Ascend C 算子实现 & 调用样例
```

## 算子描述
- 算子功能：
  展示 `ND2NZ` 搬运中的 bank 写写冲突，以及通过调整 `dstNzC0Stride` 分散 bank group 的方法。

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">AIV算子</td></tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">8192 * 8192</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">shape depends on scenario</td><td align="center">half</td><td align="center">NZ</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">bank_conflict_nd2nz</td></tr>
  </table>

- 算子实现：
  - `SCENARIO=1`：`dstNzC0Stride=256`，用于展示 bank group 写写冲突。
  - `SCENARIO=2`：`dstNzC0Stride=257`，用于展示冲突规避。
  - 输入大矩阵切分为 `1024` 个 `256x256` tile，48 个核均分任务并行处理。
  - UB 内部使用静态 `LocalTensor`，每个 tile 先 `Duplicate` 清零，再补齐尾行后写回 GM，避免空洞残留旧值。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 场景1编译运行
  ```bash
  mkdir -p build_s1 && cd build_s1
  cmake .. -DSCENARIO=1
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output_s1.bin output/golden_s1.bin
  ```
  脚本会使用固定随机种子生成随机输入，并同步生成两套场景的 golden。

- 场景2编译运行
  ```bash
  mkdir -p build_s2 && cd build_s2
  cmake .. -DSCENARIO=2
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output_s2.bin output/golden_s2.bin
  ```

执行结果如下，说明精度对比成功。
```bash
test pass!
```

## 性能说明

### ND2NZ 是怎么写数据的

ND2NZ 把源矩阵的每一行（256 个 half = 16 个 C0 块，每块 32B）写入 UB 时，**不是连续写入一整行**，而是把这 16 个 C0 块按照 `dstNzC0Stride` 指定的步长分散写到 UB 的不同位置。

用坐标 `(c, n)` 表示 UB 中的格子（c = D 方向第几个 C0 块，n = 第几行，每格 32B），写入公式为：

```
线性位置 = n × dstNzNStride + c × dstNzC0Stride
```

以 256×256 的 tile 为例（dstNzNStride=1），搬运结果如下图：

```
GM (ND格式，每格=32B):
Row 0:   |R0,C0 |R0,C1 |R0,C2 | ... |R0,C15 |
Row 1:   |R1,C0 |R1,C1 |R1,C2 | ... |R1,C15 |
...
Row 255: |R255,C0| ...                |R255,C15|

         ↓ ND2NZ 搬运到 UB ↓

UB (NZ格式，(c,n)坐标视图，每格=32B):
         c=0      c=1      c=2    ...   c=15
n=0:  |R0,C0 ||R0,C1 ||R0,C2 |  ... |R0,C15 |
n=1:  |R1,C0 ||R1,C1 ||R1,C2 |  ... |R1,C15 |
...
n=15: |R15,C0| ...                    |R15,C15|
n=16: |R16,C0| ...                    |R16,C15|
...
n=255:|R255,C0| ...                   |R255,C15|
```

### UB Bank 结构

UB 被划分为 48 个 bank，每个 bank 4KB。16 个相邻 bank 构成一个 bank group（共 16 个 bank group）。地址映射规则如下：

```
bank_id       = (地址 / 32B) % 48
bank_group_id = bank_id % 16
```

**bank group 的关键性质**：bank group 每隔 512B（= 16 × 32B）循环一次。同一时刻若有多个写操作打向同一个 bank group，就会产生写写冲突，MTE2 被迫串行化，吞吐下降。

### SCENARIO=1（dstNzC0Stride=256）：为什么产生冲突

`dstNzC0Stride=256` 是 256×256 tile 的**标准紧凑列主序 NZ 布局**，UB 线性内存如下：

```
UB 线性内存 (stride=256):

位置   0: |R0,C0 |  ← (c=0, n=0)
位置   1: |R1,C0 |
...
位置  15: |R15,C0|              ← Fractal(N=0, D=0): rows 0-15,  cols 0-15
位置  16: |R16,C0|
...
位置  31: |R31,C0|              ← Fractal(N=1, D=0): rows 16-31, cols 0-15
...
位置 255: |R255,C0|             ← Fractal(N=15,D=0): rows 240-255,cols 0-15
位置 256: |R0,C1 |  ← (c=1, n=0)
...
位置 511: |R255,C1|             ← D=1 的全部分形块
...
位置3840: |R0,C15|
...
位置4095: |R255,C15|
```

当 ND2NZ 处理**第 0 行**时，16 个 C0 块的写入 bank group：

```
写 R0,C0  → 位置    0，bank_group = (   0) % 16 = 0  ←┐
写 R0,C1  → 位置  256，bank_group = ( 256) % 16 = 0  ←┤ 全部打向 bank_group_0！
写 R0,C2  → 位置  512，bank_group = ( 512) % 16 = 0  ←┤
...                                                     │
写 R0,C15 → 位置 3840，bank_group = (3840) % 16 = 0  ←┘
```

**根本原因**：`256 % 16 == 0`，步长是 bank group 周期（16）的整数倍，每次都绕回同一个 bank group。同一行的 16 次写全部冲突。

### SCENARIO=2（dstNzC0Stride=257）：冲突如何规避

将 stride 从 256 改为 257（每步多走 1 个 C0_SIZE = 32B），同样处理**第 0 行**：

```
写 R0,C0  → 位置    0，bank_group = (   0) % 16 =  0
写 R0,C1  → 位置  257，bank_group = ( 257) % 16 =  1
写 R0,C2  → 位置  514，bank_group = ( 514) % 16 =  2
写 R0,C3  → 位置  771，bank_group = ( 771) % 16 =  3
...
写 R0,C15 → 位置 3855，bank_group = (3855) % 16 = 15
```

16 次写均匀分散到 bank_group 0\~15，无冲突。**根本原因**：`257 % 16 == 1`，每步前进一个 bank group，恰好轮转一圈。

### 代价：空洞与 Duplicate 清零

stride 从 256 改为 257，相当于每两个相邻 D 方向 C0 列之间插入 1 个 32B 的空洞。16 个 C0 列之间有 15 个间隔，因此整个 tile 输出多出 **15 个空洞（共 480B）**：

```
UB 线性内存 (stride=257):

位置   0 ~ 255: c=0 的全部 256 行
位置 256:       ← 空洞（ND2NZ 不写这里）
位置 257 ~ 512: c=1 的全部 256 行
位置 513:       ← 空洞
位置 514 ~ 769: c=2 的全部 256 行
...
```

由于 `tileLocal` 是静态 LocalTensor，tile 之间复用同一块 UB。处理第二个 tile 时，空洞位置仍残留上一个 tile 的旧数据。写回 GM 时若不处理，这些脏数据会污染输出。

代码中在每个 tile 开始时先用 `Duplicate` 将整块 `tileLocal` 填零，确保空洞位置为 0：

```cpp
half zero = static_cast<half>(0);
AscendC::Duplicate(tileLocal, zero, TILE_OUT_ELEMS);  // 先清零
AscendC::PipeBarrier<PIPE_V>();
AscendC::DataCopy(tileLocal, xGm[srcOffset], params);  // 再 ND2NZ 搬运
```

两个场景的输出规格对比：

| | SCENARIO=1 (stride=256) | SCENARIO=2 (stride=257) |
|--|--|--|
| NZ 布局 | 标准紧凑列主序，无空洞 | 每列间有 32B 空洞，共 15 个 |
| tile 输出 C0 块数 | 4096 | 4111 |
| tile 输出大小 | 128KB | 128KB + 480B |
| bank group 分布 | 同行 16 写全打 bank_group_(n%16) | 同行 16 写均匀分散到 0\~15 |
| 需要 Duplicate 清零 | 不需要 | 需要 |

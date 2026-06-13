# reduce样例

## 概述
本样例基于RegBase编程范式实现Reduce运算，支持多种归约模式：
- SUM（求和）
- MAX（求最大值及索引）
- MIN（求最小值及索引）
对于求和运算，提供了三种不同的实现方式

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍
```
├── reduce
│   ├── scripts
│   │   ├── gen_all_reduce_data.py    // 所有reduce核函数的测试数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── reduce_sum_2level.asc          // 两阶段归约求和样例
│   ├── reduce_sum_seq.asc             // 线性累加归约求和样例
│   ├── reduce_sum_binary.asc          // 二分累加归约求和样例
│   ├── reduce_max.asc                 // 归约求最大值+索引样例
│   ├── reduce_min.asc                 // 归约求最小值+索引样例
│   └── README.md                      // 样例介绍
```

## 样例描述
<<<<<<< Updated upstream:examples/01_simd_cpp_api/03_basic_api/02_reg_vector_compute/reduce/README.md
- 样例功能：
  对输入向量做归约求和运算，向量shape为[1, 256]，数据类型为float，输出为归约求和结果。
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[1]</td><td align="center">float</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">reduce</td></tr>
  </table>
- 样例实现：
   ReduceSumVF函数内调用Reduce接口进行归约计算：
   - 使用LoadAlign加载数据到寄存器
   - 使用Reduce(ReduceType::SUM)对单个repeat内所有元素求和
   - 使用Add累加多个repeat的局部和
   - 使用StoreAlign将最终结果写回UB
  - 调用实现
    使用内核调用符<<<>>>调用核函数。
=======

### 1. reduce_sum_2level.asc - 两阶段归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_sum_2level`
- 样例实现：每个 repeat 先做 Reduce<SUM> 归约得到标量写出到 UB，再对中间结果做一次 Reduce<SUM>。
- 累加方式：先 repeat 内：硬件 Reduce<SUM> 二叉树归约（64→1）；后 repeat 间：将中间标量写出到 UB，再做一次硬件 Reduce<SUM> 二叉树归约（9→1）
  ![alt text](image-1.png)

### 2. reduce_sum_seq.asc - 线性累加归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_sequential`
- 样例实现：将所有 repeat 加载到寄存器，通过向量加法顺序累加至第一个寄存器，最后做一次 Reduce<SUM> 归约。
- 累加方式：先 repeat 间：顺序向量加法，r1..r7 逐个 Add 到 r0（共 7 次串行 Add）；后 repeat 内：硬件 Reduce<SUM> 二叉树归约（64→1）
  ![alt text](image-3.png)

### 3. reduce_sum_binary.asc - 二分累加归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_sum_binary`
- 样例实现：将所有 repeat 加载到寄存器，通过二分折半向量加法聚合到一个 repeat，最后做一次 Reduce<SUM> 归约。
- 累加方式：先 repeat 间：二分折半向量加法（8→4→2→1，共 log₂8=3 轮配对 Add）；后 repeat 内：硬件 Reduce<SUM> 二叉树归约（64→1）
  ![alt text](image-2.png)

### 三种求和方案对比

> **测试环境：** Ascend 950PR，NPU 模式

| 方案 | 策略 | cycles | repeat间组合方式 | 各vl块向量Add次数 | 数值精度 |
|------|------|--------|-----------------|------------------|---------|
| binary | 二分折半 | **1002** | 折半配对 Add（log₂N 轮） | 均等（各3次） | 均衡 |
| seq | 线性累加 | 1165 | 顺序 Add 到 r0（N-1 次串行） | 不均等（r0..r7: 7→1次） | 有风险 |
| 2level | 两阶段归约 | 1454 | repeat内 Reduce + repeat间 Reduce | 均等（全二叉树） | 均衡 |

**各 vl 块参与向量 Add 次数明细：**

| 方案 | r0 | r1 | r2 | r3 | r4 | r5 | r6 | r7 |
|------|----|----|----|----|----|----|----|-----|
| binary | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 |
| seq | 7 | 7 | 6 | 5 | 4 | 3 | 2 | 1 |
| 2level | — | — | — | — | — | — | — | — |

> **说明：** 2level 方案每个 repeat 先经 Reduce\<SUM\> 归约为标量，repeat 间再做一次 Reduce\<SUM\>，元素级别的加和路径完全一致（全二叉树），不受 vl 块位置影响，无需按 vl 块统计。

**推荐顺序：**

1. **binary（二分累加归约求和）** — 性能最优（1002 cycles），各 vl 块 Add 次数均等，数值精度均衡；需将 repeat 数对齐到 2 的幂次，代码复杂度适中
2. **seq（线性累加归约求和）** — 性能居中（1165 cycles），实现最简单直接；但 r0 块始终参与 Add，各 vl 块累加次数不均衡（r0 7 次 vs r7 1 次），浮点舍入误差累积程度不一致，对精度敏感场景需注意
3. **2level（两阶段归约求和）** — 性能最慢（1454 cycles），但精度最优：repeat 内和 repeat 间均通过硬件 Reduce\<SUM\> 二叉树完成，每个元素 Add 次数完全一致；代价是多占用 UB 中转缓冲区

### 4. reduce_max.asc - 归约求最大值及索引
- 样例功能：求输入向量的最大值及其对应的索引，向量shape为[1, 1024]，数据类型为float
- 核函数名：`reduce_max`
- 样例实现：同时返回最大值和索引

### 5. reduce_min.asc - 归约求最小值及索引
- 样例功能：求输入向量的最小值及其对应的索引，向量shape为[1, 1024]，数据类型为float
- 核函数名：`reduce_min`
- 样例实现：同时返回最小值和索引
>>>>>>> Stashed changes:examples/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce/README.md

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_all_reduce_data.py                                 # 生成所有reduce核函数的测试数据
  ./demo_reduce_sum_2level     # 执行reduce_sum_2level样例
  ./demo_reduce_sum_seq        # 执行reduce_sum_seq样例
  ./demo_reduce_sum_binary     # 执行reduce_sum_binary样例
  ./demo_reduce_max            # 执行reduce_max样例
  ./demo_reduce_min            # 执行reduce_min样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  **编译选项说明：**

| 选项　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　|
| ------------| -----------------------------| ---------------------------------------------------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真　　　　　　　|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

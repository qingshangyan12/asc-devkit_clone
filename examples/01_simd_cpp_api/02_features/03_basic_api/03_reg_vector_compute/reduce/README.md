# reduce样例

## 概述
本样例基于RegBase编程范式实现Reduce运算，支持多种归约模式：
- SUM（求和）
- MAX（求最大值及索引）
- MIN（求最小值及索引）
对于求和运算，提供了三种不同的实现方式

## 支持的产品
- Ascend 950PR/Ascend 950DT

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
- 实现伪代码：
  ```cpp
  // 归约求最大值及索引: 维护(value, index)对, repeat间逐元素竞争
  vlSize = 64, nRepeats = CeilDiv(1024, 64) = 16
  dstValue ← Duplicate(FLOAT_MIN)                     // 值寄存器初始化为最小浮点数
  dstIndex ← Arange(1, 2, ..., 64)                    // 索引寄存器初始化为1..64
  curIndex ← dstIndex
  // Step1: repeat间 value-index 竞争
  for rep in 0..nRepeats-1:
      src ← LoadAlign(row + rep*vlSize, 64)
      tmpValue ← Max(dstValue, src, mask)             // 逐元素取max
      tmpValue ← Select(tmpValue, dstValue, mask)     // mask外保留旧值
      changed ← Compare(NE, dstValue, tmpValue)       // 检测哪些位置值发生了更新
      dstIndex ← Select(changed, curIndex, dstIndex)  // 值变则索引同步更新
      curIndex ← curIndex + 64                        // 当前repeat的全局索引偏移
      dstValue ← tmpValue
  // Step2: 寄存器内最终归约
  finalVal ← ReduceMax(dstValue, FULL_MASK)           // 硬件二叉树: 64→1最大值
  eqMask ← Compare(EQ, dstValue, finalVal)            // 标记等于最大值的候选位置
  packedIdx ← GatherMask(dstIndex, eqMask)            // 提取出对应索引
  finalIdx ← ReduceMin(packedIdx, eqMask) - 1         // 多候选时取最小原始索引
  StoreUnalign(dst[0..1], {finalVal, finalIdx})
  ```

### 5. reduce_min.asc - 归约求最小值及索引
- 样例功能：求输入向量的最小值及其对应的索引，向量shape为[1, 1024]，数据类型为float
- 核函数名：`reduce_min`
- 样例实现：同时返回最小值和索引
- 实现伪代码：
  ```cpp
  // 归约求最小值及索引: 与reduce_max镜像, Max→Min, FLOAT_MIN→FLOAT_MAX
  vlSize = 64, nRepeats = CeilDiv(1024, 64) = 16
  dstValue ← Duplicate(FLOAT_MAX)                     // 值寄存器初始化为最大浮点数
  dstIndex ← Arange(1, 2, ..., 64)
  curIndex ← dstIndex
  for rep in 0..nRepeats-1:
      src ← LoadAlign(row + rep*vlSize, 64)
      tmpValue ← Min(dstValue, src, mask)             // 逐元素取min
      tmpValue ← Select(tmpValue, dstValue, mask)     // mask外保留旧值
      changed ← Compare(NE, dstValue, tmpValue)       // 检测哪些位置值发生了更新
      dstIndex ← Select(changed, curIndex, dstIndex)  // 值变则索引同步更新
      curIndex ← curIndex + 64                        // 当前repeat的全局索引偏移
      dstValue ← tmpValue
  finalVal ← ReduceMin(dstValue, FULL_MASK)           // 硬件二叉树: 64→1最小值
  eqMask ← Compare(EQ, dstValue, finalVal)            // 标记等于最小值的候选位置
  packedIdx ← GatherMask(dstIndex, eqMask)            // 提取出对应索引
  finalIdx ← ReduceMin(packedIdx, eqMask) - 1         // 多候选时取最小原始索引
  StoreUnalign(dst[0..1], {finalVal, finalIdx})
  ```

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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

- 样例执行
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

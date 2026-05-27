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
│   │   └── gen_data.py                // reduce_accumulate单独的数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── reduce_accumulate.asc          // 累加器方式归约求和样例
│   ├── reduce_iterator.asc            // 迭代器方式归约求和样例
│   ├── reduce_binary.asc              // 二分累加归约求和样例
│   ├── reduce_max.asc                 // 归约求最大值+索引样例
│   ├── reduce_min.asc                 // 归约求最小值+索引样例
│   └── README.md                      // 样例介绍
```

## 样例描述

### 1. reduce_accumulate.asc - 累加方式归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_accumulate`
- 样例实现：每行数据按 repeat 切分加载，每个 repeat 经 Reduce<SUM> 归约后，通过向量加法顺序累加至同一累加器寄存器 (accReg)，最终写出归约结果。
- 累加方式：repeat 内采用硬件 Reduce<SUM> 二叉树归约；repeat 间采用顺序累加，即逐个 repeat 将归约结果 Add 到累加器上
- 实现伪代码：
  ```cpp
  // 串行累加器归约: repeat内硬件二叉树 + repeat间串行Add
  vlSize = 64, mainRepeats = 8, tailR = 16
  for row in 0..dimA-1:
      acc ← Duplicate(0)                              // 累加寄存器初始化为0
      // 处理主块: 8个repeat
      for rep in 0..mainRepeats-1:
          src ← LoadAlign(row + rep*vlSize, 64)
          partial ← Reduce<SUM>(src, FULL_MASK)       // 硬件二叉树: 64个元素→1个归约结果
          acc ← Add(acc, partial, FULL_MASK)          // 归约结果累加到累加器
      // 处理尾块: 16个剩余元素
      if tailR > 0:
          src ← LoadAlign(row + 512, 16 with mask)
          partial ← Reduce<SUM>(src, mask)
          acc ← Add(acc, partial, mask)
      StoreUnalign(dst[row], acc)                     // 写出该行最终归约结果
  ```
  ![alt text](image.png)

### 2. reduce_iterator.asc - 迭代方式归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_iterator`
- 样例实现：采用两级归约策略——第一轮对每个 repeat 执行 Reduce<SUM> 并将中间结果非对齐写出到 UB 中转缓冲区 (workLocal)，第二轮对该缓冲区中的中间标量集合再执行一次 Reduce<SUM>，得到最终归约结果。
- 累加方式：repeat 内为 ReduceSum 二叉树归约；repeat 间同样通过 ReduceSum 完成归约，本质上是两级二叉树归约
- 实现伪代码：
  ```cpp
  // 两级迭代器归约: repeat内硬件二叉树 + repeat间再次硬件二叉树
  vlSize = 64, nRepeats = CeilDiv(528, 64) = 9
  for row in 0..dimA-1:
      // 第一级: repeat内归约, 中间结果写到UB中转缓冲区
      for rep in 0..nRepeats-1:
          src ← LoadAlign(row + rep*vlSize, 64)
          partial ← Reduce<SUM>(src, mask)            // 硬件二叉树: 64→1
          StoreUnalign(workBuf[rep], partial)         // 中间标量写出到UB
      LocalMemBar(STORE→LOAD)                         // 保序: 写完成后才能读
      // 第二级: 加载9个中间归约结果, 再做一次归约
      src ← LoadAlign(workBuf[0], 9 with mask)
      final ← Reduce<SUM>(src, mask)                  // 硬件二叉树: 9→1
      StoreUnalign(dst[row], final)                   // 写出最终结果
  ```
  ![alt text](image-1.png)

### 3. reduce_binary.asc - 二分累加归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_binary`
- 样例实现：使首先将尾部余数元素合并至主块使 repeat 数对齐到 2 的幂次（8），随后将 8 个 repeat 全部加载到寄存器，经 log₂(8)=3 轮两两配对向量加法（8→4→2→1） 折半聚合至单个 repeat，最终通过 Reduce<SUM> 归约并写出
- 累加方式：repeat 内最终由 Reduce<SUM> 二叉树归约完成；repeat 间采用二分折半配对累加
- 实现伪代码：
  ```cpp
  // 二分折半归约: 尾块合并 + 配对Add(log₂N轮) + 仅最后1次Reduce
  vlSize = 64, nRepeat = 8 (对齐到2的幂次), tailR = 16
  for row in 0..dimA-1:
      // Step 0: 加载所有8个repeat到寄存器, 尾块合并到r0
      r0..r7 ← LoadAlign(row, 8*64)                   // 8个向量寄存器各64元素
      if tailR > 0:
          rTail ← LoadAlign(row + 512, 16 with mask)
          r0 ← Add(r0, rTail, mask)                   // 尾部元素并入r0
      // Step 1: 二分折半配对累加
      // L1 - 8→4: 间隔4配对
      r0←Add(r0,r4); r1←Add(r1,r5); r2←Add(r2,r6); r3←Add(r3,r7)
      // L2 - 4→2: 间隔2配对
      r0←Add(r0,r2); r1←Add(r1,r3)
      // L3 - 2→1: 相邻配对, 全行数据已聚合到r0
      r0←Add(r0,r1)
      // Step 2: 寄存器内最终归约
      result ← Reduce<SUM>(r0, FULL_MASK)             // 硬件二叉树: 64→1
      StoreUnalign(dst[row], result)
  ```
  ![alt text](image-2.png)

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
  ./demo_reduce_accumulate    # 执行reduce_accumulate样例
  ./demo_reduce_iterator      # 执行reduce_iterator样例
  ./demo_reduce_binary        # 执行reduce_binary样例
  ./demo_reduce_max           # 执行reduce_max样例
  ./demo_reduce_min           # 执行reduce_min样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　|
| ------------| -----------------------------| ---------------------------------------------------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真　　　　　　　|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

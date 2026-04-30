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
- 样例实现：每行数据按 repeat 切分加载，每个 repeat 经 Reduce<SUM> 二分归约后，通过向量加法顺序累加至同一累加器寄存器 (accReg)，最终写出归约结果。
- 累加方式：repeat 内采用硬件 Reduce<SUM> 二分归约；repeat 间采用顺序累加，即逐个 repeat 将归约结果 Add 到累加器上

### 2. reduce_iterator.asc - 迭代方式归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_iterator`
- 样例实现：采用两级归约策略——第一轮对每个 repeat 执行 Reduce<SUM> 并将中间结果非对齐写出到 UB 中转缓冲区 (workLocal)，第二轮对该缓冲区中的中间标量集合再执行一次 Reduce<SUM>，得到最终归约结果。
- 累加方式：repeat 内为 ReduceSum 二分归约；repeat 间同样通过 ReduceSum 完成归约，本质上是两级二分归约

### 3. reduce_binary.asc - 二分累加归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_binary`
- 样例实现：使首先将尾部余数元素合并至主块使 repeat 数对齐到 2 的幂次（8），随后将 8 个 repeat 全部加载到寄存器，经 log₂(8)=3 轮两两配对向量加法（8→4→2→1） 折半聚合至单个 repeat，最终通过 Reduce<SUM> 二分归约为标量并写出
- 累加方式：repeat 内最终由 Reduce<SUM> 二分归约完成；repeat 间采用二分折半配对累加

### 4. reduce_max.asc - 归约求最大值及索引
- 样例功能：求输入向量的最大值及其对应的索引，向量shape为[1, 1024]，数据类型为float
- 核函数名：`reduce_max`
- 样例实现：使用ReduceMaxWithIndexImpl，同时返回最大值和索引

### 5. reduce_min.asc - 归约求最小值及索引
- 样例功能：求输入向量的最小值及其对应的索引，向量shape为[1, 1024]，数据类型为float
- 核函数名：`reduce_min`
- 样例实现：使用ReduceMinWithIndexImpl，同时返回最小值和索引

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

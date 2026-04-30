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

### 1. reduce_accumulate.asc - 累加器方式归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_accumulate`
- 样例实现：使用累加器寄存器逐步累加多个repeat的结果

### 2. reduce_iterator.asc - 迭代器方式归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_iterator`
- 样例实现：基于ReduceSumImpl实现，支持两级归约

### 3. reduce_binary.asc - 二分累加归约求和
- 样例功能：对输入向量做归约求和运算，向量shape为[20, 528]，数据类型为float
- 核函数名：`reduce_binary`
- 样例实现：使用Binary Halving算法，通过多次两两配对累加实现归约

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

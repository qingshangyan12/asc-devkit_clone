# SIMT编程模式实现Histogram样例

## 概述

本样例展示了如何利用 Ascend C 的SIMT编程方式，高效统计输入字节流中每个字节值的出现频率。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```text
hist-refactor/
├── histogram.asc              # Histogram样例调用
├── CMakeLists.txt             # 样例构建脚本
└── README.md                  # 样例说明文档
```

## 算子描述

- 算子功能：
  Histogram样例对固定大小的输入字节流进行统计，统计各个字节落入256个桶（bin）的频率。输入数据总大小为 `32 * 1024 * 1024` Byte，Host侧使用同一批输入数据计算CPU参考结果，并与NPU输出逐桶（bin）对比。

- 算子规格：

  <table>
  <tr><td align="center">算子类型(OpType)</td><td colspan="4" align="center">histogram</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">32 * 1024 * 1024 Byte</td><td align="center">uint8</td><td align="center">ND</td></tr>
  <tr><td align="center">input_words</td><td align="center">8,388,608</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">actual_histogram</td><td align="center">256</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">核函数名</td><td colspan="4" align="center">accumulate_block_local_histogram</td></tr>
  <tr><td colspan="4" align="center">merge_block_local_histogram</td></tr>
  </table>

- 数据切分：
  * 第一阶段Kernel：`240`个ThreadBlock。
    * 每个ThreadBlock线程数：`192`线程。
    * 第一阶段总线程数：`240 * 192 = 46080`。
    * 单线程处理：按grid-stride遍历`uint32`输入，每次解析4个字节并更新所属Warp的局部Histogram。
  * 第二阶段Kernel：`256`个ThreadBlock，每个ThreadBlock负责一个桶（bin）的频率统计，使用`256`线程对`240`份局部Histogram做归约计算。
  * 结果预期：NPU输出的256个桶（bin）计数应与Host侧`compute_histogram_cpu`的计算结果一致。

- 算子实现：
  - `accumulate_block_local_histogram`在`__ubuf__`中为每个Warp维护一份局部Histogram，线程按grid-stride读取输入并通过`asc_atomic_add`完成输入字节对应桶（bin）计数的增加，在ThreadBlock内合并所有Warp的局部Histogram，结果写入`partialHistogram`。
  - `merge_block_local_histogram`以桶（bin）为粒度读取所有ThreadBlock的局部Histogram，在ThreadBlock内归约后算出最终Histogram。

- 调用实现：
  Host侧先生成固定随机输入并计算CPU参考结果，再依次启动两个Kernel：

  ```cpp
  accumulate_block_local_histogram<<<dim3(PARTIAL_HISTOGRAM_COUNT), dim3(THREADS_PER_BLOCK), 0, device.stream>>>(
      device.partialHistogram,
      device.inputWords,
      wordCount);

  merge_block_local_histogram<<<dim3(HISTOGRAM_BIN_COUNT), dim3(REDUCE_BLOCK_SIZE), 0, device.stream>>>(
      device.finalHistogram,
      device.partialHistogram,
      PARTIAL_HISTOGRAM_COUNT);
  ```

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build      # 创建并进入 build 目录
  cmake ..                        # 配置工程
  make -j                         # 编译样例
  ./histogram                     # 执行样例
  ```

执行结果如下，说明精度对比成功。

```text
Running histogram256 on Ascend C SIMT for fixed 33554432 bytes (16 runs)
histogram256 average time: 0.000599 sec, throughput: 56037.529 MB/s
Validation passed
```

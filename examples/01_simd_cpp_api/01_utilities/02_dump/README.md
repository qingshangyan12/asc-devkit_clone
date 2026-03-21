# MmadCustomDump样例

## 概述

本样例以Mmad算子为示例，详细讲述了算子核函数维测接口（asc_dump系列、printf等）的使用方法：通过在Mmad算子核函数中集成上述维测接口，结合<<<>>>内核调用符完成 NPU 侧部署运行，开发者可参考样例部署快速掌握维测接口的配置与使用，实现 NPU 侧算子运行参数的实时输出、张量数据的完整dump，高效验证算子运行状态。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── 02_dump
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── half.hpp               // 数据类型依赖文件
│   └── mmad_custom_dump.asc   // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：

  本算子以 Mmad 算子为载体，核心演示 asc_dump 系列接口（含asc_dump_gm、asc_dump_ubuf、asc_dump_cbuf和asc_dump_l1buf）在 NPU 侧算子核函数中的使用方法。针对算子运行过程中 GM，UB、L0C、L1等不同物理位置上的张量数据，通过调用 asc_dump 系列接口实现运行态数据可视化。按需输出张量的数值、存储地址等关键信息，解决NPU侧算子运行数据不可见、调试定位难的问题。

  此外，该系列接口兼容 AscendC::DumpTensor 接口。但后续开发中，建议优先使用 asc_dump 系列接口；若需 dump 指定偏移位置的数据，由于 asc_dump 系列暂不支持该能力，可继续使用 DumpAccChkPoint 接口。

- 算子规格：
  - Mmad算子：

    在核函数直调样例中，算子实现支持的shape为：M = 16, N = 16, K = 16。
    <table>
    <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Mmad</td></tr>
    </tr>
    <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
    <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
    <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
    </tr>
    </tr>
    <tr><td rowspan="1" align="center">算子输出</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
    </tr>
    <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_custom</td></tr>
    </table>

- 算子实现：

  1）.本算子以 Mmad 算子为实现载体，在数据搬运与计算的全流程中，通过 asc_dump 系列和 printf 接口对各阶段的张量数据进行完整 Dump 与输出。
  2）.asc_dump系列接口的函数签名一样，仅函数名存在区分，这种区分用以表示要 Dump 的具体位置：
  ```cpp
  __aicore__ inline void asc_dump_gm(__gm__ T* input, uint32_t desc, uint32_t dumpSize);
  __aicore__ inline void asc_dump_ubuf(__ubuf__ T* input, uint32_t desc, uint32_t dumpSize);
  __aicore__ inline void asc_dump_cbuf(__cc__ T* input, uint32_t desc, uint32_t dumpSize);
  __aicore__ inline void asc_dump_l1buf(__cbuf__ T* input, uint32_t desc, uint32_t dumpSize);
  ```
  3）.在数据搬入阶段，通过 asc_dump_gm 接口 Dump 位于全局内存 (GM) 中的输入矩阵 A、矩阵 B 与偏执矩阵 Bias 的原始数据；待数据加载至 L0C 并完成矩阵乘计算后，再通过 asc_dump_cbuf 接口 Dump 并输出 L0C中的最终计算结果，完整呈现数据从 GM 输入到 L0C 计算完成的全流程状态。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake ..;make -j;                # 编译工程
  ./demo                           # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  [Success] Case accuracy is verification passed.
  ```
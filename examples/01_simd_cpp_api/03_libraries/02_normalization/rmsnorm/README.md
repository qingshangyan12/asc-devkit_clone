# RmsNorm样例

## 概述

本样例基于Kernel直调样例工程，介绍了调用RmsNorm高阶API实现rmsnorm单样例，实现对shape大小为[B，S，H]的输入数据的RmsNorm归一化。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── rmsnorm
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── rmsnorm.asc             // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  实现对shape大小为[B，S，H]的输入数据的RmsNorm归一化，其计算公式如下：    
  $$
  y_i = RmsNorm(x_i)\\
  y_i=\frac{x_i}{\sqrt{\frac{1}{N}\sum_{i = 1}^{N}x_i^2+\varepsilon}}\times\gamma
  $$


- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> rmsnorm </td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[B, S, H]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">gamma</td><td align="center">[H]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[B, S, H]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">rmsnorm_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape(src[B, S, H]=[4, 8, 64]，gamma[H]=[64]，dst[B, S, H]=[4, 8, 64])的rmsnorm样例。

  本样例使用 **基本块模式**（isBasicBlock=true），H=64 为 64 的倍数，B*S=32 为 8 的倍数，满足 RmsNorm 基本块要求，性能更优。详细说明请参考RmsNorm API文档。

  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用RmsNorm高阶API接口完成rmsnorm计算，得到最终结果，再搬出到外部存储上。

    rmsnorm样例的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm、gammaGm搬运至LocalMemory，分别存储在srcLocal、gammaLocal中，Compute任务负责对srcLocal、gammaLocal执行rmsnorm计算，计算结果存储在dstLocal中，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGm中。

  - Tiling实现

    rmsnorm样例的tiling实现流程如下：首先获取RmsNorm接口能完成计算所需最大/最小临时空间大小，根据该范围结合实际的内存使用情况设置合适的空间大小，然后根据输入shape、剩余的可供计算的空间大小等信息获取RmsNorm kernel侧接口所需tiling参数。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu ..;make -j; # cpu调试模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

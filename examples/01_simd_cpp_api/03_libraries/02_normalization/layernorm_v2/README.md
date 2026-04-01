# LayerNormV2样例

## 概述

本样例基于Kernel直调算子工程，介绍了在一个核函数中连续调用LayerNorm和Normalize高阶API，实现对输入tensor按行做归一化计算，并对比两种API的输出结果。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
 
```
├── layernorm_v2
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── layernorm_v2.asc        // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  本样例在一个核函数中依次调用LayerNorm和Normalize高阶API，对输入tensor按行做归一化计算，并对比两者的输出结果（验证数学等价性）。LayerNorm的计算公式为 $y = \gamma \cdot \frac{x - \mu}{\sqrt{var + \epsilon}} + \beta$；Normalize在已知均值和方差的情况下，计算归一化结果和标准差倒数rstd。输出包括LayerNorm的归一化结果output、均值outputMean、标准差倒数outputRstd，以及Normalize的归一化结果normalizeOutput和标准差倒数normalizeRstd。

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center"> layernorm_v2 </td></tr>

  <tr><td rowspan="5" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">inputXGm</td><td align="center">[A, R]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">gammaGm</td><td align="center">[R]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">betaGm</td><td align="center">[R]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="7" align="center">算子输出</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">outputGm</td><td align="center">[A, R]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputMeanGm</td><td align="center">[A]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputRstdGm</td><td align="center">[A]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">normalizeOutputGm</td><td align="center">[A, R]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">normalizeRstdGm</td><td align="center">[A]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">layernormv2_custom</td></tr>
  </table>

- 算子实现：  
  本样例中实现的是固定shape(inputX[A, R]=[32, 32], gamma[R]=[32], beta[R]=[32])的融合算子。

  本样例使用 **AR格式**（shape为[A, R]），在一个核函数中依次调用LayerNorm和Normalize高阶API进行归一化计算。A为批处理轴，R为归一化轴。详细说明请参考 [LayerNorm API文档](../../../../../docs/api/context/LayerNorm.md) 和 [Normalize API文档](../../../../../docs/api/context/Normalize.md)。

  - Kernel实现

    本样例在一个核函数内部实现了完整的数据流计算：
    - 1. LayerNorm前向计算：输入inputX、gamma、beta，计算得到输出y、均值mean、标准差的倒数rstd。
    - 2. Normalize计算：使用同一输入计算归一化结果和rstd。
    - 3. 结果对比：对比LayerNorm和Normalize的输出，验证两者数学等价性。

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后依次使用LayerNorm和Normalize高阶API完成计算，得到最终结果，再搬出到外部存储上。

    算子的实现流程分为4个基本任务：CopyIn，LayerNormCompute，NormalizeCompute，CopyOut。

  - Tiling实现

    算子的tiling实现流程如下：
    1. 分别获取LayerNorm和Normalize接口能完成计算所需的最大/最小临时空间大小。
    2. 根据该范围结合实际的内存使用情况设置合适的空间大小。
    3. 根据输入shape、剩余的可供计算的空间大小等信息分别获取两个API kernel侧接口所需tiling参数。
    4. 将两个tiling参数整合到一个融合tiling结构体中。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DNPU_ARCH=dav-2201 ..;make -j;             # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 模式时，添加 `-DRUN_MODE=cpu` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j; # cpu调试模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
# LayerNorm样例

## 概述

本样例基于Kernel直调算子工程，介绍了在一个核函数中连续调用LayerNorm、LayerNormGrad、LayerNormGradBeta三个高阶API实现LayerNorm的完整前向和反向传播。在训练场景中，LayerNorm算子需要同时计算前向输出、输入梯度、参数梯度（gamma和beta的梯度），本样例通过三个API的配合使用，在一个核函数中高效完成这些计算，避免了多个核函数启动的开销和中间数据的多次全局内存访问

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── fused_layernorm
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── layernorm_fused.asc     // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  LayerNorm算子，在一个核函数中连续调用LayerNorm、LayerNormGrad、LayerNormGradBeta三个高阶API，完成LayerNorm的完整前向和反向传播计算。该算子在训练场景中特别有用，可以一次性计算前向输出、输入梯度、参数梯度，提高计算效率。
- 数据流路径：
  本样例在一个核函数内部实现了完整的数据流计算：
  1.LayerNorm前向计算：输入inputX、gamma、beta，计算得到输出y、均值mean、方差var。
  2.LayerNormGrad反向计算：输入dy、inputX、var、mean、gamma，计算得到输入梯度pd_x和中间结果resForGamma
  3.LayerNormGradBeta参数梯度计算：输入dy、resForGamma，计算得到gamma和beta的梯度pd_gamma、pd_beta。所有中间结果都在核内传递，无需写回全局内存，减少了数据搬运开销。
- 算子规格：  
  <table border="2" align="center">
  <caption>表1：融合LayerNorm算子规格表</caption>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center"> fused_layernorm </td></tr>
  <tr><td rowspan="6" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">inputXGm</td><td align="center">[2, 32, 16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">gammaGm</td><td align="center">[16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">betaGm</td><td align="center">[16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">dyGm</td><td align="center">[2, 32, 16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="6" align="center">算子输出</td></tr>
  <tr><td align="center">outputYGm</td><td align="center">[2, 32, 16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputMeanGm</td><td align="center">[2, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputVarGm</td><td align="center">[2, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputPdXGm</td><td align="center">[2, 32, 16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputPdGammaGm</td><td align="center">[16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputPdBetaGm</td><td align="center">[16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">fused_layernorm_custom</td></tr>
  </table>

- 算子实现：  
  本样例实现固定shape(inputX[2, 32, 16]、gamma[16]、beta[16]、dy[2, 32, 16])的融合LayerNorm算子。

  - Kernel实现

    计算逻辑基于Ascend C提供的矢量计算接口，所有操作元素都为LocalTensor。输入数据先搬运进片上存储，然后依次调用三个高阶API完成计算，最后将结果搬出到外部存储。调用流程：  
    LayerNorm（前向）: inputX + gamma + beta → outputY, mean, variance
    LayerNormGrad（反向）: inputDy + inputX + variance + mean + gamma → pd_x, resForGamma
    LayerNormGradBeta（参数梯度）: inputDy + resForGamma → pd_gamma, pd_beta

  - Tiling实现

    layernorm算子的tiling实现流程如下：
    1.分别获取LayerNorm、LayerNormGrad、LayerNormGradBeta接口能完成计算所需的最大/最小临时空间大小。
    2.根据该范围结合实际内存使用情况设置合适的空间大小。
    3.根据输入shape、剩余可供计算的空间大小等信息分别获取三个API kernel侧接口所需tiling参数。
    4.将三个tiling参数整合到一个融合tiling结构体中。
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
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
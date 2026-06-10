# 基于Tensor API实现的Batch MX Mmad样例

## 概述

本样例基于Tensor API展示batch模式下MX Mmad的调用流程。样例使用`fp4x2_e1m2_t`作为矩阵A和矩阵B的输入类型，使用`fp8_e8m0_t`作为ScaleA和ScaleB的scale类型，Mmad在L0C中累加为`float`并通过Tensor API Copy接口写回GM。

样例处理`B=2, M=64, N=64, K=128`的小规模batch矩阵乘，不使用ping/pong和多层分块循环，重点展示batch layout下GM到L1、L1到L0A/L0B、L1到L0ScaleA/L0ScaleB、MX Mmad、L0C到GM的Tensor API调用链。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── mx_batch_mmad_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                  // 输入数据和真值数据生成脚本
│   │   └── verify_result.py             // 输出结果验证脚本
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── data_utils.h                     // 数据读入写出函数
│   ├── mx_batch_mmad_tensor_api.asc     // Ascend C样例实现与调用样例
│   └── README.md                        // 样例说明文档
```

## 样例描述

样例在Kernel中构造GM、L1、L0A、L0B、L0ScaleA、L0ScaleB和L0C上的Tensor对象，并通过显式构造的CopyAtom和MmadAtom执行以下流程：

1. `CopyGM2L1`将batch A/B矩阵从GM搬入L1，将每个batch的ScaleA/ScaleB搬入L1。
2. `CopyL12L0A`和`CopyL12L0B`将batch FP4矩阵搬入L0A/L0B。
3. `CopyL12L0ScaleA`和`CopyL12L0ScaleB`将batch scale数据搬入L0ScaleA/L0ScaleB。
4. 按batch循环调用`Mmad`，使用`MmadType::MX`执行MX矩阵乘。
5. `CopyL0C2GM`将batch L0C中的`float`结果写回GM。

<table border="2">
<caption>表1：样例规格表</caption>
<tr><td align="left">样例类型(OpType)</td><td colspan="4" align="left">Batch MX Mmad</td></tr>
<tr><td rowspan="5" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
<tr><td align="left">a</td><td align="left">[2, 64, 128]</td><td align="left">fp4x2_e1m2_t</td><td align="left">ND</td></tr>
<tr><td align="left">b</td><td align="left">[2, 128, 64]</td><td align="left">fp4x2_e1m2_t</td><td align="left">DN</td></tr>
<tr><td align="left">scaleA</td><td align="left">[2, 64, 4]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleAND</td></tr>
<tr><td align="left">scaleB</td><td align="left">[2, 4, 64]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleBDN</td></tr>
<tr><td align="left">样例输出</td><td align="left">c</td><td align="left">[2, 64, 64]</td><td align="left">float</td><td align="left">ND</td></tr>
<tr><td align="left">核函数名</td><td colspan="4" align="left">mx_batch_mmad_custom</td></tr>
</table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  执行完成后，输出数据会写入`build/output/output.bin`。验证通过时会打印`test pass!`。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :----- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |

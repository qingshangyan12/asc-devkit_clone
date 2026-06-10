# 基于Tensor API实现的Batch MX Mmad样例

## 概述

本样例基于Tensor API展示batch模式下MX Mmad的调用流程，并重点验证MX scale从Global Memory到Local Memory L1的所有分形通路。样例使用`fp4x2_e1m2_t`作为矩阵A和矩阵B的输入类型，使用`fp8_e8m0_t`作为ScaleA和ScaleB的scale类型，Mmad在L0C中累加为`float`并通过Tensor API Copy接口写回GM。

A/B矩阵通路固定（A非转置GM ND->L1 NZ，B转置GM DN->L1 ZN，转置由GM/L1分形表达、L0固定），scale通路则按配对方式覆盖全部6种GM分形（ScaleA的`ScaleANDLayoutPtn`/`ScaleADNLayoutPtn`/`ZZLayoutPtn`，ScaleB的`ScaleBNDLayoutPtn`/`ScaleBDNLayoutPtn`/`NNLayoutPtn`）。样例串行执行3个场景，每种scale pattern各覆盖一次：

| 场景 | ScaleA GM Pattern | ScaleB GM Pattern |
| :--- | :---------------- | :---------------- |
| 0 | `ScaleANDLayoutPtn` | `ScaleBNDLayoutPtn` |
| 1 | `ScaleADNLayoutPtn` | `ScaleBDNLayoutPtn` |
| 2 | `ZZLayoutPtn`       | `NNLayoutPtn`       |

3个场景共享同一份A/B矩阵和逻辑scale数值，仅scale的GM内存排布不同，因此各场景结果相同、分别写入GM输出的不同偏移区，golden将单场景结果重复3次拼接比对。

样例处理`B=2, M=64, N=64, K=128`的小规模batch矩阵乘，不使用ping/pong和多层分块循环，重点展示batch layout下GM到L1、L1到L0A/L0B、L1到L0ScaleA/L0ScaleB、MX Mmad、L0C到GM的Tensor API调用链。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── mx_batch_mmad_fp4_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                  // 输入数据和真值数据生成脚本
│   │   └── verify_result.py             // 输出结果验证脚本
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── data_utils.h                     // 数据读入写出函数
│   ├── mx_batch_mmad_fp4_tensor_api.asc // Ascend C样例实现与调用样例
│   └── README.md                        // 样例说明文档
```

## 样例描述

样例在Kernel中构造GM、L1、L0A、L0B、L0ScaleA、L0ScaleB和L0C上的Tensor对象，并通过显式构造的CopyAtom和MmadAtom执行以下流程：

1. `CopyGM2L1`将batch A/B矩阵从GM搬入L1。
2. `CopyL12L0A`和`CopyL12L0B`将batch FP4矩阵搬入L0A/L0B。
3. `CopyGM2L1`按编译期选定的scale pattern将ScaleA/ScaleB搬入L1（ZZ/NN），再由`CopyL12L0ScaleA`和`CopyL12L0ScaleB`搬入L0ScaleA/L0ScaleB。
4. 按batch循环调用`Mmad`，使用`MmadType::MX`执行MX矩阵乘。
5. `CopyL0C2GM`将batch L0C中的`float`结果写回GM。

每次编译只验证一条scale通路（由`SCENE_ID`宏选择），三种场景通过三次独立编译分别验证，避免单次运行混合多条通路。`run.sh`脚本会依次完成三次编译与验证。

<table border="2">
<caption>表1：样例规格表</caption>
<tr><td align="left">样例类型(OpType)</td><td colspan="4" align="left">Batch MX Mmad (Scale通路验证)</td></tr>
<tr><td rowspan="9" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
<tr><td align="left">a</td><td align="left">[2, 64, 128]</td><td align="left">fp4x2_e1m2_t</td><td align="left">ND</td></tr>
<tr><td align="left">b</td><td align="left">[2, 128, 64]</td><td align="left">fp4x2_e1m2_t</td><td align="left">DN</td></tr>
<tr><td align="left">scaleA (SCENE_ID=0)</td><td align="left">[2, 64, 4]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleAND</td></tr>
<tr><td align="left">scaleA (SCENE_ID=1)</td><td align="left">[2, 64, 4]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleADN</td></tr>
<tr><td align="left">scaleA (SCENE_ID=2)</td><td align="left">[2, 64, 4]</td><td align="left">fp8_e8m0_t</td><td align="left">ZZ</td></tr>
<tr><td align="left">scaleB (SCENE_ID=0)</td><td align="left">[2, 4, 64]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleBND</td></tr>
<tr><td align="left">scaleB (SCENE_ID=1)</td><td align="left">[2, 4, 64]</td><td align="left">fp8_e8m0_t</td><td align="left">ScaleBDN</td></tr>
<tr><td align="left">scaleB (SCENE_ID=2)</td><td align="left">[2, 4, 64]</td><td align="left">fp8_e8m0_t</td><td align="left">NN</td></tr>
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

- 一键验证全部scale通路（推荐）

  ```bash
  bash run.sh sim dav-3510
  ```

  脚本依次以`SCENE_ID=0/1/2`三次编译并验证三条scale通路，全部通过时打印`all scenes pass!`。

- 单条通路手动执行

  ```bash
  mkdir -p build && cd build
  cmake .. -DSCENE_ID=0          # 0: ScaleAND+ScaleBND, 1: ScaleADN+ScaleBDN, 2: ZZ+NN
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
  | `SCENE_ID` | `0`（默认）、`1`、`2` | scale通路场景：0=ScaleAND+ScaleBND，1=ScaleADN+ScaleBDN，2=ZZ+NN |

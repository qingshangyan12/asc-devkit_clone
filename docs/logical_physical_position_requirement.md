# AscendC资料位置描述口径统一需求

## 需求

统一AscendC资料中逻辑位置与物理位置的描述口径，解决同一批资料中位置表达不一致、裸逻辑位置缺少物理位置说明、C API资料出现TPosition逻辑位置等问题。

本需求只调整文档描述，不修改接口原型、参数语义、功能行为和代码示例中的接口调用方式。

目标口径：

- 非C API资料中，描述数据通路、Tensor存储位置、接口支持位置、队列位置、缓存位置时，稳定映射的逻辑位置统一使用“物理位置（逻辑位置）”。
- 对于CO2这类物理存储映射与产品型号相关的逻辑位置，不强行展开为单一物理位置，首次出现时说明其物理存储映射与产品型号相关，后文可继续使用逻辑位置。
- C API资料中，只使用物理位置描述，不使用TPosition逻辑位置补充说明。
- 明确介绍TPosition、逻辑位置映射关系、术语定义或代码真实写法的内容，不作为本次整改对象。

## 背景

本次整改前，资料中存在几类典型问题：

- 已经同时写出逻辑位置和物理位置，但顺序不统一，例如“CO1（L0C Buffer）”和“L0C Buffer（CO1）”并存。
- 正文直接使用逻辑位置描述搬运通路，例如“A1->A2”，读者需要自行映射到实际物理位置。
- 参数说明、代码注释、实践流程中单独出现逻辑位置，例如“输出至VECIN”“在A1中缓存”。
- 部分逻辑位置的物理映射与产品型号相关，例如CO2，直接写成“GM/UB（CO2）”会造成通路表达不清晰。
- C API资料中出现逻辑位置，容易让读者误以为C API接口感知TPosition。

## 整改规则分类

### 1. 已有“逻辑位置（物理位置）”映射时，统一改为“物理位置（逻辑位置）”

适用范围：

- 原文已经同时给出了逻辑位置和物理位置。
- 只是顺序不符合统一口径。
- 修改后保留两个信息，但物理位置放在外层，逻辑位置放在括号内。

判定方式：

```text
逻辑位置（物理位置） -> 物理位置（逻辑位置）
```

典型示例1：LocalTensor支持位置说明

文件：

```text
docs/api/SIMD-API/基础API/Memory矢量计算/归约计算/ReduceMax.md
```

修改前：

```text
类型为LocalTensor，支持的TPosition为VECIN、VECCALC、VECOUT（存储位置为Unified Buffer）。
```

修改后：

```text
类型为LocalTensor，存储位置为Unified Buffer（TPosition为VECIN、VECCALC、VECOUT）。
```

覆盖同类文档：

- `ReduceMin.md`
- `ReduceSum.md`
- `ReduceDataBlock.md`
- `ReducePairElem.md`
- `ReduceRepeat.md`
- `Brcb.md`
- `Gather.md`
- `Gatherb(ISASI).md`
- `Scatter(ISASI).md`

典型示例2：矩阵相关位置映射

文件：

```text
docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过BT-Buffer实现高效的bias计算.md
```

修改前：

```text
C2（Bias Table Buffer）
CO1（L0C Buffer）
```

修改后：

```text
Bias Table Buffer（C2）
L0C Buffer（CO1）
```

典型示例3：Matmul回调说明中的矩阵位置

文件：

```text
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/MatmulCallBackFunc.md
```

修改前：

```text
C矩阵从CO1（L0C Buffer）拷贝
A矩阵从A1（L1 Buffer）拷贝
B矩阵从B1（L1 Buffer）拷贝
```

修改后：

```text
C矩阵从L0C Buffer（CO1）拷贝
A矩阵从L1 Buffer（A1）拷贝
B矩阵从L1 Buffer（B1）拷贝
```

### 2. 裸逻辑位置描述数据通路时，补齐为物理位置通路

适用范围：

- 原文用逻辑位置直接描述数据搬运、转置、加载、取出、放入等通路。
- 原文没有给出物理位置。
- 修改时需要把通路两端都补齐物理位置，避免只补一侧。

判定方式：

```text
A1->A2 -> L1 Buffer（A1）搬运到L0A Buffer（A2）
B1->B2 -> L1 Buffer（B1）搬运到L0B Buffer（B2）
A1/B1 -> A2/B2 -> L1 Buffer（A1/B1）搬运到L0A/L0B Buffer（A2/B2）
```

典型示例1：LoadDataWithTranspose通路说明

文件：

```text
docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithTranspose.md
```

修改前：

```text
a矩阵从A1->A2转置，b矩阵从B1->B2转置。
```

修改后：

```text
a矩阵从L1 Buffer（A1）搬运到L0A Buffer（A2）并转置，b矩阵从L1 Buffer（B1）搬运到L0B Buffer（B2）并转置。
```

典型示例2：耦合模式中的搬运流程

文件：

```text
docs/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md
```

修改前：

```text
使用LoadData接口将LocalTensor从A1/B1中搬运到A2/B2。
```

修改后：

```text
使用LoadData接口将LocalTensor从L1 Buffer（A1/B1）搬运到L0A/L0B Buffer（A2/B2）。
```

典型示例3：基础知识中的搬运阶段

文件：

```text
docs/guide/算子实践参考/SIMD算子实现/矩阵编程（高阶API）/基础知识.md
```

修改前：

```text
数据从A1搬到A2；数据从B1搬到B2。
```

修改后：

```text
数据从L1 Buffer（A1）搬到L0A Buffer（A2）；数据从L1 Buffer（B1）搬到L0B Buffer（B2）。
```

典型示例4：Fixpipe量化参数搬运通路

文件：

```text
docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（DataCopy）.md
```

修改前：

```cpp
// 将C1中的量化数据（quantAlphaTensor）拷贝到C2PIPE2GM（fbTensor）
```

修改后：

```cpp
// 将L1 Buffer（C1）中的量化数据（quantAlphaTensor）拷贝到Fixpipe Buffer（C2PIPE2GM）中的fbTensor
```

`C1`、`C2PIPE2GM`都是逻辑位置。这里描述的是量化参数搬运通路，因此归入“裸逻辑位置描述数据通路时补齐物理位置”。

典型示例5：Tensor编程中的TSCM搬运通路

文件：

```text
docs/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/CPPTensor编程概述.md
docs/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md
```

修改前：

```text
不支持VECIN/VECCALC/VECOUT -> TSCM通路的数据搬运。
```

修改后：

```text
不支持UB（VECIN/VECCALC/VECOUT）->L1 Buffer（TSCM）通路的数据搬运。
```

### 3. 裸逻辑位置描述存储、缓存、队列或输出位置时，补齐物理位置

适用范围：

- 原文不是箭头通路，但逻辑位置用于说明Tensor位置、缓存位置、输出位置、队列位置或Buffer来源。
- 这类描述也会影响读者理解数据实际所在的存储层级，需要补齐物理位置。

判定方式：

```text
输出至VECIN -> 输出至UB（VECIN）
在A1中缓存 -> 在L1 Buffer（A1）中缓存
放入CO1的Queue -> 放入L0C Buffer（CO1）的Queue
```

典型示例1：Matmul输出位置

文件：

```text
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/GetTensorC.md
```

修改前：

```text
获取C矩阵，输出至VECIN
取出C矩阵到VECIN。
```

修改后：

```text
获取C矩阵，输出至UB（VECIN）
取出C矩阵到UB（VECIN）。
```

典型示例2：Matmul配置参数中的缓存位置

文件：

```text
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/MatmulConfig.md
```

修改前：

```text
stepM | 左矩阵在A1中缓存的bufferM方向上baseM的倍数。
stepN | 右矩阵在B1中缓存的bufferN方向上baseN的倍数。
```

修改后：

```text
stepM | 左矩阵在L1 Buffer（A1）中缓存的bufferM方向上baseM的倍数。
stepN | 右矩阵在L1 Buffer（B1）中缓存的bufferN方向上baseN的倍数。
```

典型示例3：TSCM和VECIN组合

文件：

```text
docs/api/SIMD-API/基础API/资源管理/Pipe和Que框架/TSCM/TSCM简介.md
```

修改前：

```text
TSCM从VECIN发起的Buffer块数量与Matmul对象数量之和最大为10个。
DataCopy(scmTensor, ubLocal, 1024); // 将UB数据搬运至TSCM，准备用于Matmul计算
```

修改后：

```text
L1 Buffer（TSCM）从UB（VECIN）发起的Buffer块数量与Matmul对象数量之和最大为10个。
DataCopy(scmTensor, ubLocal, 1024); // 将UB数据搬运至L1 Buffer（TSCM），准备用于Matmul计算
```

典型示例4：队列位置

文件：

```text
docs/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md
```

修改前：

```text
使用DeQue从A1/B1中取出LocalTensor。
使用EnQue将计算结果LocalTensor放入到CO1的Queue中。
```

修改后：

```text
使用DeQue从L1 Buffer（A1/B1）中取出LocalTensor。
使用EnQue将计算结果LocalTensor放入到L0C Buffer（CO1）的Queue中。
```

典型示例5：Matmul计算结果缓存位置

文件：

```text
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Iterate.md
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Kernel侧接口/Matmul使用说明.md
```

修改前：

```text
一次Iterate矩阵乘的结果C矩阵存放在逻辑位置CO1的内存中。
用户提前申请CO1的内存l0cTensor。
```

修改后：

```text
一次Iterate矩阵乘的结果C矩阵存放在L0C Buffer（CO1）中。
用户提前申请L0C Buffer（CO1）的内存l0cTensor。
```

典型示例6：Matmul Tiling缓存参数

文件：

```text
docs/api/SIMD-API/高阶API/矩阵计算/Matmul-Tiling类/TCubeTiling结构体.md
```

修改前：

```text
depthA1为A1中全载baseM * baseK的份数，depthB1为B1中全载baseN * baseK的份数。
stepM为左矩阵在A1中缓存的buffer M方向上baseM的倍数。
```

修改后：

```text
depthA1为L1 Buffer（A1）中全载baseM * baseK的份数，depthB1为L1 Buffer（B1）中全载baseN * baseK的份数。
stepM为左矩阵在L1 Buffer（A1）中缓存的buffer M方向上baseM的倍数。
```

### 4. 物理映射与产品型号相关的逻辑位置，不强行展开为单一物理位置

适用范围：

- 逻辑位置在不同产品型号中可能对应不同物理存储。
- 原文直接使用该逻辑位置描述队列、缓存、搬运目标或搬运来源。
- 如果强行写成“物理位置（逻辑位置）”，会出现“GM/UB（CO2）”这类不清晰表达。

判定方式：

```text
CO2 -> 首次出现时说明为逻辑位置CO2，并说明其物理存储映射与产品型号相关
CO2对应的Queue -> 后文可继续使用CO2对应的Queue
从CO2搬出 -> 后文可写为从逻辑位置CO2搬出
```

典型示例1：耦合模式中的CO2队列和搬运目标

文件：

```text
docs/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md
```

修改前：

```text
使用Ascend C接口拷贝结果矩阵到CO2。
使用EnQue将计算结果LocalTensor放入到CO2的Queue中。
```

修改后：

```text
使用Ascend C接口拷贝结果矩阵到逻辑位置CO2。CO2的物理存储映射与产品型号相关，具体请参考[逻辑位置和物理存储的映射关系](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/通用说明和约束.md#section1359919519819)。
使用EnQue将计算结果LocalTensor放入到CO2对应的Queue中。
```

典型示例2：CO2后续搬运描述

文件：

```text
docs/guide/算子实践参考/SIMD算子实现/矩阵编程（基础API）/耦合模式.md
```

修改前：

```text
将矩阵c的part1和part2分别拷贝到CO2进行合并。
将合并后的输出数据从CO2搬出。
```

修改后：

```text
将矩阵c的part1和part2分别拷贝到逻辑位置CO2进行合并。
将合并后的输出数据从逻辑位置CO2搬出。
```

### 5. C API资料只保留物理位置，不出现TPosition逻辑位置

适用范围：

- C API资料中的接口说明、参数说明、场景说明。
- C API不感知TPosition逻辑位置，因此不采用“物理位置（逻辑位置）”的补充形式。
- 遇到逻辑位置时，直接改成物理位置。

判定方式：

```text
C2 -> Bias Table Buffer
CO1(L0C) -> L0C Buffer
A1 -> L1 Buffer
A2 -> L0A Buffer
B2 -> L0B Buffer
```

典型示例1：asc_mmad参数说明

文件：

```text
docs/api/SIMD-API/C-API/cube_compute/asc_mmad.md
```

修改前：

```text
配置C矩阵初始值是否来源于C2（存放Bias的硬件缓存区）。
true：来源于C2。
false：来源于CO1(L0C)。
```

修改后：

```text
配置C矩阵初始值是否来源于Bias Table Buffer。
true：来源于Bias Table Buffer。
false：来源于L0C Buffer。
```

典型示例2：asc_copy_l12l0a场景说明

文件：

```text
docs/api/SIMD-API/C-API/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md
```

修改前：

```text
输入feature maps 在A1中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在A2 的数据形状为(Wo*Ho, Hk*Wk*Ci)。
```

修改后：

```text
输入feature maps在L1 Buffer中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在L0A Buffer的数据形状为(Wo*Ho, Hk*Wk*Ci)。
```

同类修改覆盖：

- `asc_copy_l12l0a_arch_3510.md`
- `asc_copy_l12l0b_arch_2201.md`
- `asc_copy_l12l0b_arch_3510.md`

典型示例3：asc_copy_l12l0a/l12l0b复制残留场景说明

文件：

```text
docs/api/SIMD-API/C-API/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md
docs/api/SIMD-API/C-API/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md
```

修改前：

```text
对于A2 的feature map来说有两种方案，水平分块和垂直分块。
```

修改后：

```text
对于L0A Buffer中的feature map来说有两种方案，水平分块和垂直分块。
对于L0B Buffer中的feature map来说有两种方案，水平分块和垂直分块。
```

### 6. 不纳入整改的内容

以下内容不按本规则整改，测试核查时不要误报：

- 代码中的真实枚举、模板参数、接口调用，例如`TPosition::VECIN`、`TPosition::A1`。
- 专门介绍TPosition含义、逻辑位置映射关系、术语定义的段落。
- 相邻上下文已经先给出物理位置通路，下一行只补充逻辑位置示意的内容。这里不包括“文档开头说明过物理位置，后文就直接使用逻辑位置”的情况。
- 产品型号、版本号、芯片名称，例如`Atlas A2`、`Atlas A3`。
- 数据格式、shape维度、矩阵块名称，例如`NC1HWC0`、`C1=C/C0`、`A1矩阵`、`A1*B1`。

示例：

```text
- L1 Buffer -> L0A Buffer
  - A1 -> A2
```

这类写法第一行已经说明实际物理通路，第二行只是补充对应的逻辑位置示意，不属于逻辑位置和物理位置混用问题。

如果只是在文档开头统一说明过物理位置映射，后文正文、参数说明、流程说明中再次描述稳定映射的数据位置或搬运通路时，仍应使用“物理位置（逻辑位置）”，不能直接裸写逻辑位置。CO2这类物理映射与产品型号相关的逻辑位置，按第4类规则处理。

## 测试核查建议

测试核查时建议按以下顺序检查：

1. 先确认文档类别。C API资料只允许物理位置；非C API资料中稳定映射的逻辑位置按“物理位置（逻辑位置）”核查。
2. 检查是否存在“逻辑位置（物理位置）”或“逻辑位置(物理位置)”的反向写法。
3. 检查是否存在裸逻辑位置通路，例如`A1->A2`、`B1->B2`、`A1/B1到A2/B2`。
4. 检查是否存在裸逻辑位置描述存储、缓存、输出或队列，例如“输出至VECIN”“在A1中缓存”“放入CO1的Queue”。
5. 检查是否存在CO2这类物理映射与产品型号相关的位置。此类位置不要强行改成“GM/UB（CO2）”，首次出现需要说明为逻辑位置且映射与产品型号相关，后文可继续使用CO2。
6. 对疑似问题先判断是否属于“不纳入整改的内容”，避免把代码真实写法、术语定义、产品型号和数据格式误报为位置混用问题。

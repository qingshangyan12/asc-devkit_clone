# ReduceDataBlock<a name="ZH-CN_TOPIC_0000001938755198"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| --- | :---: |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter> |
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter> |
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | x </cann-filter> |
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter> |
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：basic_api/kernel_operator_vec_reduce_intf.h。

当ReduceType为SUM，将每个DataBlock（32B）中参与计算的元素相加，并将最终的计算结果依次保存在dstReg的最低位。

当ReduceType为MAX，将每个DataBlock（32B）中的最大值依次保存在dstReg中的最低位。

当ReduceType为MIN，将每个DataBlock（32B）中的最小值依次保存在dstReg中的最低位。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <ReduceType type = ReduceType::SUM, typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void ReduceDataBlock(U& dstReg, U srcReg, MaskReg mask)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| type | ReduceType类型，支持SUM、MAX、MIN。<br><pre>enum class ReduceType {<br>    SUM = 0,<br>    MAX,<br>    MIN,<br>};</pre> |
| T | 目的操作数和源操作数的数据类型。 |
| mode | [MaskMergeMode](../数据类型/MaskMergeMode.md#ZH-CN_TOPIC_0000002554608930)，选择MERGING模式或ZEROING模式。<br>&bull; ZEROING，mask未选中的元素在dst中置零。<br>&bull; MERGING，当前不支持。 |
| U | 目的操作数和源操作数的RegTensor类型，例如RegTensor&lt;int32_t&gt;，由编译器自动推导，用户不需要填写。 |

**表 2** 函数参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md#ZH-CN_TOPIC_0000002585048875)。 |
| srcReg | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md#ZH-CN_TOPIC_0000002585048875)。 |
| mask | 输入 | 源操作数元素操作的有效指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md#ZH-CN_TOPIC_0000002554449284)。 |

## 数据类型

**表 3** ReduceType::SUM 数据类型支持情况

| dstReg | srcReg |
| --- | --- |
| half | half |
| int32_t | int32_t |
| uint32_t | uint32_t |
| float | float |

**ReduceType::SUM 输入数据类型为 uint16\_t/int16\_t 的情况待支持。**

**表 4** ReduceType::MAX 或 ReduceType::MIN 数据类型支持情况

| dstReg | srcReg |
| --- | --- |
| half | half |
| int16_t | int16_t |
| uint16_t | uint16_t |
| int32_t | int32_t |
| uint32_t | uint32_t |
| float | float |

## 返回值说明

无

## 约束说明<a name="section177921451558"></a>

- 对于归约求最大值，max\(-0, +0\) = +0。
- 对于归约求最小值，max\(-0, +0\) = -0。
- mask为空场景异常值处理参考[mask为空与srcReg异常值处理规则](Reduce.md#zh-cn_topic_0000002537812548_p1899712172216)。
- 指令内累加顺序采用二叉树累加方式，规则参考[ReduceSum累加顺序](Reduce.md#zh-cn_topic_0000002537812548_p14726121712398)。
- 寄存器的非饱和/饱和模式具体配置方式及说明**待补充**。
- **ReduceType::SUM 输入数据类型为 uint16\_t/int16\_t 的情况待支持。**

## 关键特性<a name="zh-cn_topic_0000002568852247_section18972943153217"></a>

**ReduceType::SUM累加顺序**：

以二叉树累加的方式计算每个DataBlock内的和。

以half类型的数据求和为例，在每个DataBlock内有16个数，通过二叉树的方式，两两相加，计算过程如下图所示：

1. data1和data2相加得到data01，data3和data4相加得到data02，……，data13和data14相加得到data07，data15和data16相加得到data08；
2. data01和data02相加得到data001，data03和data04相加得到data002，……，data07和data08相加得到data004；
3. 以此类推，得到目的操作数为1个half类型的数据sum。

**图 1** ReduceDataBlock示意图<a name="zh-cn_topic_0000002568852247_fig10524085414"></a>  
![ReduceDataBlock示意图](../../../../figures/reg_reduce_datablock_index.png "ReduceDataBlock示意图")

**mask为空与srcReg异常值处理规则：**

- 对于归约求数据总和，当DataBlock中的元素均不参与计算时，将目的操作数数据类型的0写入dstReg。
- 对于归约求最大值，当DataBlock中的元素均不参与计算时，将该数据类型的最小值写入dstReg。
- 对于归约求最小值，当DataBlock中的元素均不参与计算时，将该数据类型的最大值写入dstReg。
- 对于归约求最大值/最小值，如果DataBlock中存在nan，将该数据类型的nan写入dstReg。

## 调用示例<a name="section642mcpsimp"></a>

```cpp
template<typename T>
__simd_vf__ inline void ReduceDataBlockVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, 
 uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        mask = AscendC::Reg::UpdateMask<T>(count);
        // type = ReduceType::SUM
        AscendC::Reg::ReduceDataBlock<AscendC::Reg::ReduceType::SUM>(dstReg, srcReg, mask);
        // type = ReduceType::MAX
        // AscendC::Reg::ReduceDataBlock<AscendC::Reg::ReduceType::MAX>(dstReg, srcReg, mask);
        // type = ReduceType::MIN
        // AscendC::Reg::ReduceDataBlock<AscendC::Reg::ReduceType::MIN>(dstReg, srcReg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask); 
    }
}
```

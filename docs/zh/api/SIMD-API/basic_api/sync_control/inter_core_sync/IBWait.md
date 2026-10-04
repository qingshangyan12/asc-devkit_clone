# IBWait<a name="ZH-CN_TOPIC_0000001487056954"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：支持
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBWait_res.md#id1 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_block_sync_intf.h"`。

等待由`blockIdx`和`eventID`共同指定的同步标志。不同核操作同一块全局内存且存在读后写、写后读或写后写等数据依赖时，等待核在访问依赖数据前调用本接口。本接口持续读取同步标志，直到其值为1时将其清零并返回，以保证被等待核已经完成相关操作，避免因核间执行顺序不确定而产生数据读写错误。

本接口须与[IBSet](IBSet.md)成对使用。被等待核在完成相关操作后调用IBSet，并为`gmWorkspace`、`blockIdx`和`eventID`传入与本接口相同的值。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <bool isAIVOnly = true>
__aicore__ inline void IBWait(const GlobalTensor<int32_t>& gmWorkspace, const LocalTensor<int32_t>& ubWorkspace, int32_t blockIdx, int32_t eventID)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| isAIVOnly | 控制是否为AIVOnly模式，默认为true。 |

**表2**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| gmWorkspace | 输入 | 外部存储核状态的公共缓存，类型为[GlobalTensor](../../data_structures/GlobalTensor/GlobalTensor.md)。 |
| ubWorkspace | 输入 | 存储当前核状态的公共缓存。<br>类型为[LocalTensor](../../data_structures/LocalTensor/LocalTensor.md)，支持的[TPosition](../../aux_data_structures/TPosition.md)为VECIN/VECCALC/VECOUT。 |
| blockIdx | 输入 | 同步标志对应的逻辑核索引。在典型场景下，该参数为被等待核（即调用IBSet的核）的逻辑核索引。取值范围为`[0, 核数 - 1]`。 |
| eventID | 输入 | 同步事件的逻辑编号，与`blockIdx`共同确定`gmWorkspace`中的同步标志。取值为非负整数，建议从0开始连续编号。 |

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

`gmWorkspace`申请的空间须满足以下公式：

$$
\text{gmWorkspace空间大小} \geq (\text{核数} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} + 1) \times 32\text{字节}
$$

- `eventID_max`和`blockIdx_max`分别表示共享同一`gmWorkspace`的所有IBSet和IBWait调用中，`eventID`和`blockIdx`可能取到的最大值。如果参数在运行时确定，应按所有合法执行路径中的取值上界计算。
- AIVOnly模式下，核数为`GetBlockNum()`；MIX模式下，核数为`GetBlockNum() × 2`。`GetBlockNum()`的接口说明请参考[GetBlockNum](../../tool_interface/system_resources_and_variables/GetBlockNum.md)。
- 例如，共享同一`gmWorkspace`的所有调用均传入`blockIdx = 0`和`eventID = 0`时，`blockIdx_max = 0`、`eventID_max = 0`，`gmWorkspace`至少需要32字节。
- ubWorkspace申请的空间最少要求为：32Bytes。
- 使用该接口进行多核控制时，算子调用时指定的逻辑AI Core核数numBlocks必须保证不大于实际运行该算子的AI处理器核数，否则框架进行多轮调度时会插入异常同步，导致核函数（Kernel）“卡死”现象。
- IBSet和IBWait配对使用时，除了ubWorkspace其余所有参数都必须相同，否则程序会在IBWait处卡死。
- [分离模式](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md)下，该接口仅对AIV核生效。在AIC核中调用时，接口直接返回，不等待同步标志，也不执行核间同步。
- IBSet接口的gmWorkspace缓存的值需要初始化为0。

## 调用示例<a name="section177231425115410"></a>

完整样例请参考[调用示例](IBSet.md#section177231425115410)。

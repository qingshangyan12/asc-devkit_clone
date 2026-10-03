# IBSet<a name="ZH-CN_TOPIC_0000001538096733"></a>

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet_res.md#id1 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_block_sync_intf.h"`。

当不同核之间操作同一块全局内存且可能存在读后写、写后读以及写后写等数据依赖问题时，通过调用该函数来插入同步语句来避免上述数据依赖时可能出现的数据读写错误问题。

调用IBSet设置某一个核的标志位，与[IBWait](IBWait.md)成对使用，表示核之间的同步等待指令：被等待核中调用IBSet，通过将值1写入全局内存中某块地址来通知等待核；而等待核中调用IBWait，持续读取全局内存中相应地址块的值，直到其值变为1，从而实现核间同步。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <bool isAIVOnly = true>
__aicore__ inline void IBSet(const GlobalTensor<int32_t>& gmWorkspace, const LocalTensor<int32_t>& ubWorkspace, int32_t blockIdx, int32_t eventID)
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
- 分离模式下，使用该接口进行两个核间的同步时，仅对AIV核生效。
- IBSet接口的gmWorkspace缓存的值需要初始化为0。

## 调用示例<a name="section177231425115410"></a>

本示例实现功能为使用2个核进行数据处理，每个核均是处理256个half类型数据。核0实现x+y的操作，并将结果放入z的前半部分，核1将核0的计算结果放入x，之后与y相加，结果存入z的后半部分，因此多个核之间需要进行数据同步。示例通过[GetBlockIdx](../../tool_interface/system_resources_and_variables/GetBlockIdx.md)获取当前逻辑AI Core的索引。

```cpp
// sync_gm为外部存储核状态的公共缓存，类型为GlobalTensor；sync_buf为存储当前核状态的公共缓存，类型为LocalTensor。
int32_t blockIdx = AscendC::GetBlockIdx(); // 获取当前核。
if (blockIdx == 1) { // 在核1设置IBWait，将阻塞指令执行直到核0操作完成。
    AscendC::IBWait(sync_gm, sync_buf, 0, 0);
}
...
if (blockIdx == 0) { // 在核0设置IBSet，当核0的操作完成后再执行核1的指令。
    AscendC::IBSet(sync_gm, sync_buf, 0, 0);
}
```

完整样例请参考[IBSet与IBWait核间同步样例](../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait)。

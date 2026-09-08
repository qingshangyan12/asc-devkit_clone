# L0C Buffer到UB双目标模式

## 特性说明

本特性仅适用于3510。同一AI Core内有一个Cube Core和两个Vector Core，当启用双目标模式控制时，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的Unified Buffer（UB）中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。

- 双目标模式（M维度）：按M维度拆分成形状为M / 2 \* N的两个矩阵，分别写入两个UB。
- 双目标模式（N维度）：按N维度拆分成形状为M \* N / 2的两个矩阵，分别写入两个UB。

## 特性约束

- 双目标模式仅支持在L0C Buffer到UB通路，普通搬运模式（NZ2NZ）或NZ2ND搬运场景下使用，不支持随路功能场景。
- 按M维度拆分时M必须为2的倍数。
- 按N维度拆分时N须为32的倍数。

## 使用示例

在普通搬运模式下启用双目标模式如下图所示，分为按M维度拆分和按N维度拆分，按M维度拆分M必须为2的倍数，按N维度拆分N必须为32的倍数：

N方向切分：

- n_size = 32，表示源NZ矩阵中待搬运矩阵在N方向上的大小为32个元素。
- m_size = 48，表示源NZ矩阵中待搬运矩阵在M方向上的大小48个元素。
- src_stride = 64，表示源NZ矩阵中待搬运矩阵相邻Z排布的起始地址偏移，即下图中第一个块Z排布矩阵的起始地址与第二个Z排布矩阵的起始地址之间的间隔为64 \* C0\_Size。
- dst_stride = 64 \* C0，表示目的NZ矩阵中相邻Z排布的起始地址偏移，该步长分别应用于每个UB内部，不是两个UB地址空间之间的距离。

M方向切分：

- n_size = 32，表示源NZ矩阵中待搬运矩阵在N方向上的大小为32个元素。
- m_size = 48，表示完整源矩阵M为48；拆分后每个UB收到24行，不能把拆分后行数作为源m_size。
- src_stride = 64，表示源NZ矩阵中待搬运矩阵相邻Z排布的起始地址偏移，即下图中第一个块Z排布矩阵的起始地址与第二个Z排布矩阵的起始地址之间的间隔为64 \* C0\_Size。
- dst_stride = 40 \* C0，表示目的NZ矩阵中相邻Z排布的起始地址偏移，即下图中UB0（或UB1）中第一个Z排布的起始地址与第二个Z排布的起始地址之间的间隔为40 \* 16个元素。

**图1** NZ2NZ双目标搬运模式设置示意图<a name="fig1386816164556"></a>  

![](../../../../figures/nz2nz_l0c2ub_dual_dst.png)

在NZ2ND模式下启用双目标模式如下图所示，分为按M维度拆分和按N维度拆分，按M维度拆分M必须为2的倍数，按N维度拆分N必须为32的倍数：

- nd_num = 2，表示源NZ矩阵的数目为2。图中红框区域为矩阵1，蓝框区域为矩阵2。
- n_size = 32，表示源NZ矩阵（图中红框区域或蓝框区域）在N方向上的大小为32个元素。
- m_size = 48，表示源NZ矩阵在M方向上的大小为48个元素。
- src_stride = 64，表示源NZ矩阵中相邻Z排布的起始地址偏移，即下图红框区域中左侧浅色Z排布矩阵的起始地址与右侧深色Z排布矩阵的起始地址之间的间隔为64 \* C0\_Size。
- dst_stride = 64，表示目的ND矩阵每一行中的元素个数为64。
- nd_num = 2, 表示源NZ矩阵的数目。
- src_nd_stride = 240 ,  表示不同NZ矩阵起始地址之间的间隔为240 \* C0\_Size。
- dst_nd_stride  = 4096，表示目的相邻ND矩阵起始地址之间的偏移为4096个元素。

**图2** NZ2ND参数双搬入模式设置示意图<a name="fig715654093716"></a>  

![](../../../../figures/nz2nd_l0c2ub_dual_dst.png)


## C API枚举与调用

[asc_copy_l0c2ub](../asc_copy_l0c2ub.md)最新重载使用`asc_dual_dst_mode`：

| 枚举 | 模式 | sub_blockid |
| --- | --- | --- |
| `DUAL_DST_DISABLE` | 完整矩阵写入单个UB | 0或1选择目标 |
| `DUAL_DST_SPLIT_M` | 按M拆分，各接收M/2×N | 不用于选择单一UB |
| `DUAL_DST_SPLIT_N` | 按N拆分，各接收M×N/2 | 不用于选择单一UB |

仅在普通NZ搬运或NZ2ND场景启用双目标，关闭额外量化/激活、Channel Split和NZ2DN。片上行宽和步长的32B对齐要求同样适用于拆分后的每个目标，不能只检查拆分前矩阵。

以下为float结果按M拆成两个24×32矩阵并以ND写入的3510片段；`dst`为两个目标UB中约定的相同偏移，数据已就绪。跨核消费者访问前仍需建立AIC/AIV同步。

```cpp
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2ub(dst, src, 32, 48, 32, 64,
    0 /* sub_blockid */, asc_dual_dst_mode::DUAL_DST_SPLIT_M,
    asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant,
    asc_relu_pre_mode::NONE, false, true, false, false);
```

图2中的批次数`ndNum`对应`matrix_num`，源批次步长`srcNdStride`对应`src_nz_matrix_stride`，单位64B；目的批次步长`dstNdStride`对应`dst_matrix_stride`，单位元素。

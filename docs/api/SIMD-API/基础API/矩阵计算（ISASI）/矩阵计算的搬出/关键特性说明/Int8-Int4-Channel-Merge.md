# Int8/Int4 Channel Merge<a name="ZH-CN_TOPIC_0000002538071280"></a>

**特性说明：**

- Int8 Channel Merge

    矩阵计算的搬出过程中对于目标数据类型被转换为 int8\_t、uint8\_t的情况，NZ输出时该特性自动启用。硬件会将分形矩阵从 16×16 扩展为 16×32。具体规则如下：若输出通道数 N 是 16 的偶数倍，则在 N 方向上，每两个相邻的 16×16 分形矩阵将合并为一个单一的 16×32 分形矩阵。若 N 是 16 的奇数倍，则通道 1 至通道 \(N–16\) 将被合并，而最后 16 个通道保持不变，不进行合并。

- Int4 Channel Merge

    矩阵计算的搬出过程中对于目标数据类型被转换为 int4\_t 或 uint4\_t 的情况，NZ输出时该特性自动启用。硬件会将分形矩阵从 16×16 扩展为 16×64。具体规则如下：若输出通道数 N 是 64 的倍数，则在 N 方向上，每 4 个相邻的 16×16 分形矩阵将合并为一个单一的 16×64 分形矩阵。

**特性约束：**

- 硬件自动使能，不能配置。

<cann-filter npu_type = "A3,910b">

- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，当使能Int4 Channel Merge时，N维度必须是64的倍数。

</cann-filter>

**使用示例：**

- Int8 Channel Merge

    例如，当目标数据类型为 s8，M 为 32，N 为 48 时，硬件会先将N方向上前两个 16×16 的分形矩阵合并成一个 16×32 的矩阵，然后将剩余的 16×16 分形矩阵直接搬运至 L1。

    **图 1**  Int8 Channel Merge示意图<a name="zh-cn_topic_0000002547260781_fig1935632714360"></a>  
    ![](../../../../../figures/Fixpipe_Int8_Channel_Merge.png)

- Int4 Channel Merge

    例如，当目标数据类型为 s4，M 为 32，N 为 64 时，系统会先将N方向上前四个 16×16 的分形矩阵合并成一个 16×64 的矩阵，然后也将接下来的四个 16×16 分形矩阵同样合并。

    **图 2**  Int4 Channel Merge示意图<a name="zh-cn_topic_0000002547260781_fig8756192915367"></a>  
    ![](../../../../../figures/Fixpipe_Int4_Channel_Merge.png)

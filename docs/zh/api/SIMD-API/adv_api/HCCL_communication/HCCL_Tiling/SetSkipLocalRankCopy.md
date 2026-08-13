# SetSkipLocalRankCopy

## 功能说明

设置本卡的通信算法的计算结果是否输出到目的数据buffer地址。

## 函数原型

```
uint32_t SetSkipLocalRankCopy(uint8_t skipLocalRankCopy)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| skipLocalRankCopy | 输入 | 本卡的通信算法的计算结果是否输出到recvBuf（目的数据buffer地址）。<!-- npu="910b" id1 --><br><br>针对Atlas A2 训练系列产品/Atlas A2 推理系列产品，仅AllGather算法与AlltoAll算法支持配置该参数。uint8_t类型，参数取值如下：<br>0：输出本卡通信算法的计算结果（未调用本接口时的默认行为）。<br>1：不输出本卡通信算法的计算结果。在无需输出通信结果时，配置参数值为1，此时不会拷贝本卡的通信结果数据，可提升算子性能。例如，在8卡场景下，本卡只取其他卡的部分数据，这时可配置该参数为1。<!-- end id1 --><!-- npu="A3" id2 --><br><br>针对Atlas A3 训练系列产品/Atlas A3 推理系列产品，该参数为预留字段，配置后不生效。<!-- end id2 --> |

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明

无

## 调用示例

本接口的调用示例请见[调用示例](SetOpType.md#调用示例)。

## MatmulReduceScatter融合算子快速路径

### 功能说明

针对2卡（rankSize=2）的ReduceScatter场景，当Matmul计算结果已直接写入通信输出buffer时，可配置`skipLocalRankCopy=1`启用快速路径，跳过本地数据搬运步骤，使用`ReadReduce`原语一步完成读远端数据与归约，显著提升性能。

### 使用条件

- rankSize = 2
- sendBuf与recvBuf必须指向同一buffer（inplace操作），即Matmul输出直接作为通信输入输出。快速路径在recvBuf对应slice位置执行ReadReduce（output := output + remote），若sendBuf != recvBuf则本地数据不在ReadReduce目标位置，会导致结果错误。

### 性能对比

| 指标 | 普通路径 | 快速路径 |
| --- | --- | --- |
| 数据搬运步骤 | Read + LocalCopy + LocalReduce（多步） | 1步（ReadReduce） |
| scratch占用 | rankSize × sliceSize | 0 |

### 调用示例

```cpp
// host tiling 阶段
Mc2CcTilingConfig mc2CcTilingConfig;
mc2CcTilingConfig.SetSkipLocalRankCopy(1);  // 启用ReduceScatter快速路径
mc2CcTilingConfig.SetOpType(HCCL_CMD_REDUCE_SCATTER);
mc2CcTilingConfig.SetReduceType(HCCL_REDUCE_SUM);
mc2CcTilingConfig.SetSrcDataType(HcclDataType::HCCL_DATA_TYPE_FP16);
mc2CcTilingConfig.SetDstDataType(HcclDataType::HCCL_DATA_TYPE_FP16);
// ... 其他tiling配置 ...

// device kernel 阶段
void MatmulReduceScatterProcess() {
    // 1. Matmul计算结果直接写入workspace
    MatmulProcess(workspace, ...);

    // 2. ReduceScatter，sendBuf与recvBuf均为workspace
    //    kernel自动检测skipLocalRankCopy=1，走快速路径
    HcclHandle handle = hccl.ReduceScatter<true>(
        workspace,    // sendBuf
        workspace,    // recvBuf（与sendBuf相同）
        recvCount,
        dataType,
        HCCL_REDUCE_SUM,
        ...);

    // 3. 等待完成
    hccl.Wait(handle);
}
```

### 注意事项

- 仅对2卡ReduceScatter生效，多卡场景下该参数不触发快速路径，走原有逻辑。
- 不配置`SetSkipLocalRankCopy(1)`时（默认值为0），ReduceScatter走原有路径，行为不变。
- 使用快速路径时，Matmul必须已将本卡计算结果写入`sendBuf`对应的slice位置。

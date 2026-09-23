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
| skipLocalRankCopy | 输入 | 本卡的通信算法的计算结果是否输出到recvBuf（目的数据buffer地址）。<!-- npu="910b" id1 --><br><br>针对Atlas A2系列产品，仅AllGather算法与AlltoAll算法支持配置该参数。uint8_t类型，参数取值如下：<br>0：输出本卡通信算法的计算结果（未调用本接口时的默认行为）。<br>1：不输出本卡通信算法的计算结果。在无需输出通信结果时，配置参数值为1，此时不会拷贝本卡的通信结果数据，可提升算子性能。例如，在8卡场景下，本卡只取其他卡的部分数据，这时可配置该参数为1。<!-- end id1 --><!-- npu="A3" id2 --><br><br>针对Atlas A3系列产品，该参数为预留字段，配置后不生效。<!-- end id2 --> |

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明

在 Ascend 950 的 CCU 算法 `CcuSchedReduceScatterSoleMeshPeerOnly`（算法编号 53）中，该参数含义如下：

- `0`（默认）：通信算法先将本卡对应输入分片复制到 `recvBuf`，等待复制完成后再归约对端分片。独立调用 ReduceScatter 不需要预置输出。
- `1`：调用者保证每次通信前，本卡分片的完整结果已经写入 `recvBuf`，并且该写入对 CCU 可见。通信算法跳过本卡复制，仅将对端分片归约到已有输出。例如，配套的 MMRS peer-only Matmul kernel 将本卡结果直接写入最终输出。

仅切换算法名不代表已经满足上述前置条件；不得对未预置本卡数据的独立通信设置为 `1`。该设置随通信 tiling 在 Prepare 时保存，后续修改 tiling 不改变已创建 handle 的设置。其他算法及硬件的原有行为不变。

ASC 与 MMRS 的此项适配需要配套构建和升级，包括使用 ASC 头文件编译的算子 kernel。不支持将此版本 ASC 与尚未设置该参数的旧 peer-only MMRS 二进制混用。

## 调用示例

本接口的调用示例请见[调用示例](SetOpType.md#调用示例)。

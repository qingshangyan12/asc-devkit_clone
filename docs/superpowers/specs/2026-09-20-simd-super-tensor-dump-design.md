# SIMD 超大 Tensor Dump 设计

## 背景与范围

本需求支持 DAV 3510 纯 SIMD Core 场景下的 `AscendC::DumpTensor`：当一个完整 Tensor TLV
超过单核 GM ring buffer 容量时，设备侧将其拆成多条 TLV，Host 侧重新组装成一个逻辑 tensor。

本需求只修改 `__asc_aicore::asc_dump_*` 在 SIMD Core 上的行为，不涉及
`__asc_simd_vf::asc_dump` 及其 2024 字节保留 UB 搬运协议，也不改变 AIC/Cube Core 的行为。

能够完整放入 ring buffer 的 tensor 继续使用现有 `DUMP_TENSOR` 和 `DumpTensorTlv`，其协议及
行为保持不变。只有完整 TLV 大于 `blockInfo->ringBufLen` 时，才使用新协议。

## TLV 协议

在 `__asc_aicore::DumpType` 末尾新增：

- `DUMP_SUPER_TENSOR`：超大 tensor 的首片；
- `DUMP_SUPER_TENSOR_BODY`：超大 tensor 的后续分片。

首片结构命名为 `DumpSuperTensorTlv`，包含与 `DumpTensorTlv` 相同的 tensor 元数据，并新增：

- `tensorLength`：完整 tensor 的有效数据总字节数，类型为 `uint64_t`；
- `tensorOffset`：本片相对完整 tensor 起始地址的字节偏移，首片必须为 0，类型为 `uint64_t`；
- `dumpSize`：本片携带的有效数据字节数，类型为 `uint32_t`。

字段顺序采用以下已确认布局：

```cpp
struct DumpSuperTensorTlv {
    uint32_t type;
    uint32_t length;
    uint32_t tensorAddr;
    uint32_t dataType;
    uint32_t desc;
    uint32_t bufferId;
    uint16_t position;
    uint16_t blockIdx;
    uint32_t dim;
    uint32_t shape[8];
    uint32_t resv;
    uint64_t tensorLength;
    uint64_t tensorOffset;
    uint32_t dumpSize;
};
```

后续分片结构命名为 `DumpSuperTensorBodyTlv`。由于设备保证同一 core 中一个超大 tensor 的全部
分片连续出现，BODY 不重复携带 desc、dtype、shape 等元数据：

```cpp
struct DumpSuperTensorBodyTlv {
    uint32_t type;
    uint32_t length;
    uint32_t resv1;
    uint32_t resv2;
    uint64_t tensorLength;
    uint64_t tensorOffset;
    uint32_t dumpSize;
};
```

两个结构体都不声明变长 `dumpData` 数组。有效数据紧跟固定长度的结构体。`length` 表示 TLV
头之后的固定字段和 32 字节对齐后的数据存储长度；`dumpSize` 不包含对齐 padding。

所有 length 和 offset 均以字节为单位。由于首片的 `resv` 位于 `uint64_t` 字段之前，编译器会
在其后增加 4 字节对齐 padding，并在末尾增加 4 字节 padding；BODY 也会在末尾增加 4 字节
padding。Device 侧增加编译期 `static_assert`，约束首片大小为 96 字节、BODY 大小为 40 字节，
同时约束关键字段偏移。Host 侧使用带显式 `4x` padding 的小端格式解析，防止两端因 ABI 对齐
理解不同而产生协议分歧。

## Device 侧行为

Device 使用 64 位运算计算 `tensorLength = dumpSize * sizeof(T)`，避免乘法溢出。

若以下条件成立，完全复用现有普通 tensor 路径：

```text
sizeof(DumpTensorTlv) + align_up(tensorLength, 32) <= blockInfo->ringBufLen
```

否则进入超大 tensor 路径：

1. 首片使用 `DumpSuperTensorTlv`，后续片使用 `DumpSuperTensorBodyTlv`。
2. 每片数据容量根据当前 header 大小和运行时 `ringBufLen` 分别计算，不硬编码 32 KB。
3. 非尾片尽量填满当前 TLV 可用容量；尾片的 `dumpSize` 填写精确剩余有效字节数。
4. `tensorOffset` 从 0 开始，按每片实际 `dumpSize` 连续递增。
5. 每片的 header 和数据全部写完后才更新 ring buffer 写指针，使 RTS 只能观察到完整 TLV。
6. 每一片都通过现有 `check_ringbuf_space` 完成等待、回绕和覆盖保护。
7. 中途预留 ring buffer 失败时立即停止。此前已经发布的分片留在数据流中，由 Host 判定为不完整
   并丢弃。

本次只在 SIMD Core 支持的 GM、UB 来源上启用上述逻辑。AIC/Cube Core 仍保持原有行为：完整
TLV 放不下时不 dump，不生成新类型。

## Host 侧组包

新版 `show_kernel_debug_data` 同时支持旧 `DUMP_TENSOR` 以及两种新类型。旧工具不要求识别新
类型。

每个输入文件、每个 core 最多维护一个活动的超大 tensor 组包上下文：

1. 收到 `DUMP_SUPER_TENSOR` 时，要求当前没有未完成组包，且 `tensorOffset == 0`。
2. 首片保存 desc、dtype、position、shape 等完整元数据以及第一段数据。
3. 收到 BODY 时，要求存在活动上下文、`tensorLength` 与首片一致，并且
   `tensorOffset == expectedOffset`。
4. 每片必须满足 `dumpSize > 0`、`tensorOffset <= tensorLength`，以及
   `dumpSize <= tensorLength - tensorOffset`。
5. 完成条件严格为 `dumpSize == tensorLength - tensorOffset`。大于剩余长度属于协议错误，不是
   完成状态。
6. Runtime 在 ring buffer 回绕边界可能幂等重放刚消费的完整 TLV。若重复首片或 BODY 的
   `tensorLength`、`tensorOffset`、元数据和数据内容均与已经接收的对应区间完全一致，Host
   忽略该重复记录；相同 offset 但内容不同仍作为重叠错误处理。
7. 完成后只执行一次数据拼接和数值解析，并作为一个普通逻辑 tensor 加入现有
   `dump_tensor_map`，因此最终仍生成一个 `.bin` 和一个 `.txt`。
8. Runtime 落盘文件的最后一个 ring buffer 可能保留未使用尾区。仅当当前没有活动组包、
   `remain_len > 0`，且剩余字节数不超过 `remain_len` 时，长度越界的下一条伪 TLV 被视为尾区并
   停止解析；活动组包未完成时不得使用该规则掩盖缺片。

以下情况均抛出解析错误并丢弃该 tensor，不生成部分文件：

- BODY 前没有首片；
- 上一个 tensor 未完成时又出现不一致的首片或其他 TLV；
- offset 存在缺口、重叠或倒退；
- `tensorLength` 在 BODY 中发生变化；
- 分片越过 `tensorLength`；
- 文件结束时仍有未完成组包。

## 测试方案

### Device 编译验证

通过结构体 `static_assert` 和 DAV 3510 ASC 编译验证：

- 两种新增 TLV 的大小、字段偏移和 type 值固定；
- 普通 tensor 路径仍可编译；
- 超大 tensor 首片/BODY 分片路径在 DAV 3510 纯 SIMD kernel 中可编译。

Device 打屏和 Runtime 落盘不在单元测试中模拟，最终只通过真实样例链路验证。

### Host 单元测试

使用内存 TLV 对象覆盖，不人工构造落盘 `.bin`：

- 旧 `DUMP_TENSOR` 兼容；
- 超大 tensor 正常组包并只生成一个完整 `.bin`；
- BODY 无首片、缺片、乱序、重叠、越界、长度变化和 EOF 未完成时均报错且不落部分文件。

### 样例验证

基于 `asc-tools/examples/01_show_kernel_debug_data`：

- 使用 `dav-3510` 的 `__global__ __vector__` kernel；
- 增加一个超过单 ring buffer 容量的 GlobalTensor dump，避免为构造测试数据而扩大 LocalTensor
  并影响 UB 使用量；
- 运行样例和新版解析工具；
- 校验最终只生成一个对应的 `.bin`，其长度和逐字节内容与输入 tensor 一致；
- 同时确认原有普通 LocalTensor dump、printf 和 timestamp 行为未回归。

当前环境未提供 `npu-smi` 命令和 Ascend 设备节点。Device 编译和 Host 内存 TLV 测试可本地
执行；真实 NPU 端到端 Runtime 落盘和工具解析取决于环境中是否存在可用设备和运行时。

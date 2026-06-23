# AIN使用说明

## 功能说明

AIN提供Device侧主动通信接口，用户可在Ascend C Kernel中调用`ain::device::Ain`对象完成远端写、远端读、远端通知、本地Signal读取和等待等操作。

用户代码包含如下头文件即可使用AIN接口：

```cpp
#include "adv_api/ain/ain.h"
```

## 使用流程

使用AIN接口的基本流程如下：

1. Host侧创建通信资源、对称内存Window和Signal Window，并将Device侧通信资源地址传入Kernel。
2. Kernel侧包含`adv_api/ain/ain.h`头文件。
3. 使用Device侧通信资源地址创建`ain::device::Ain`对象。
4. 了解AIN相关数据结构，并准备Hcomm使用的UB临时工作区描述符`AinDescriptorUbuf`。
5. 根据通信方向调用`put`、`get`或`signal`接口。
6. 若使用延迟提交模式，调用`flush`接口提交并等待通信操作完成。

## 创建AIN对象

### 函数原型

```cpp
AIN_DEVICE Ain(__gm__ void *devComm, int contextIndex)
```

### 参数说明

**表 1**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `devComm` | 输入 | Host侧创建并传入Device侧的通信资源地址。当前实现按Device侧`uint64_t`通信表解析。 |
| `contextIndex` | 输入 | 通信上下文下标，当前为预留参数。 |

### 约束说明

- `devComm`必须为有效通信资源地址。
- 不能使用空`devComm`执行`put`、`get`、`signal`、`readSignal`或`waitSignal`接口。

## 数据结构说明

本节介绍AIN接口中常用的数据结构和枚举类型。

### 通信资源相关类型

```cpp
typedef uint32_t HcclAinSignal_t;
typedef uint64_t HcclDevComm;

typedef struct {
    uint32_t nRanks;
    uint32_t rank;
    uint32_t stride;
    void *ranks;
} HcclTeam;

typedef struct {
    HcclAinSignal_t signal0;
    uint32_t reserved;
} HcclAinBarrierHandle;
```

**表 2**  通信资源相关类型说明

| 类型 | 描述 |
| --- | --- |
| `HcclAinSignal_t` | Signal下标类型，用于标识Signal Window中的Signal位置。 |
| `HcclDevComm` | Device侧通信资源句柄类型。当前实现中该类型保存Device侧通信表地址。 |
| `HcclTeam` | Team描述结构体，当前接口中作为预留参数传入。 |
| `HcclAinBarrierHandle` | Barrier相关句柄结构体，当前接口中保留定义。 |

**表 3**  HcclTeam参数说明

| 参数名 | 描述 |
| --- | --- |
| `nRanks` | Team中的Rank数量。 |
| `rank` | 当前Rank下标。 |
| `stride` | Rank列表访问步长。 |
| `ranks` | Rank列表地址。 |

### AinDescriptorUbuf

`put`、`get`和`signal`接口需要传入`AinDescriptorUbuf`，用于描述Hcomm初始化所需的UB临时工作区。

### 数据结构

```cpp
typedef struct {
    __ubuf__ uint8_t *addr;
    uint32_t bytes;
    uint32_t eventId;
} AinDescriptorUbuf;
```

### 参数说明

**表 4**  AinDescriptorUbuf参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `addr` | 输入 | UB临时Buffer地址。 |
| `bytes` | 输入 | UB临时Buffer字节数。 |
| `eventId` | 输入 | 预留字段，当前未使用。 |

### Signal相关结构体

```cpp
typedef struct {
    HcclAinSignal_t signalId;
    uint64_t value;
} AinSignalAdd;

typedef struct {
    HcclAinSignal_t signalId;
} AinSignalInc;

typedef struct {
    AinSignalType type;
    union {
        struct {
        } vaSignal;

        struct {
            HcclAinSignal_t signalId;
        } indexedSignal;
    };
} AinSignalDescriptor;
```

**表 5**  Signal相关结构体说明

| 结构体 | 描述 |
| --- | --- |
| `AinSignalAdd` | Signal加指定值的动作描述。`signalId`表示Signal下标，`value`表示增加值。 |
| `AinSignalInc` | Signal加1的动作描述。`signalId`表示Signal下标。 |
| `AinSignalDescriptor` | Signal描述结构体，当前实现使用Indexed Signal描述。 |

**表 6**  AinSignalType说明

| 枚举值 | 描述 |
| --- | --- |
| `AIN_SIGNAL_TYPE_NONE` | 无Signal类型。 |
| `AIN_SIGNAL_TYPE_VA` | VA Signal类型，当前仅保留类型定义，未提供运行时实现分支。 |
| `AIN_SIGNAL_TYPE_INDEXED` | Indexed Signal类型，当前`signal`接口使用该类型。 |

## put

### 功能说明

`put`接口用于将本Rank本地Window中的数据写入目标Peer的远端Window。

数据方向如下：

```text
local srcWin + srcOffset -> peer dstWin + dstOffset
```

### 函数原型

```cpp
template <typename RemoteAction = AinRemoteNone,
          typename LocalAction = AinLocalNone,
          typename DescriptorUbuf = AinDescriptorUbuf>
AIN_DEVICE void put(void *team, uint32_t peer,
    __gm__ void *dstWin, uint64_t dstOffset,
    __gm__ void *srcWin, uint64_t srcOffset,
    uint64_t bytes, const DescriptorUbuf &ubuf = DescriptorUbuf{},
    RemoteAction remoteAction = RemoteAction{},
    LocalAction localAction = LocalAction{},
    AinCommitFlags commitFlags = AIN_COMMIT_IMMED)
```

### 参数说明

**表 7**  put接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `team` | 输入 | Team或Communicator参数，当前为预留参数。 |
| `peer` | 输入 | 目标Peer或Rank下标。 |
| `dstWin` | 输入 | 目标Peer的远端Window Index。当前实现将该参数值解释为Window Index。 |
| `dstOffset` | 输入 | 目标Window内的字节偏移。 |
| `srcWin` | 输入 | 本Rank本地Window Index。当前实现将该参数值解释为Window Index。 |
| `srcOffset` | 输入 | 本地Window内的字节偏移。 |
| `bytes` | 输入 | 待写入数据的字节数。 |
| `ubuf` | 输入 | Hcomm使用的UB临时工作区描述符。 |
| `remoteAction` | 输入 | 远端动作参数，当前为预留参数。 |
| `localAction` | 输入 | 本地动作参数，当前为预留参数。 |
| `commitFlags` | 输入 | 提交模式。支持`AIN_COMMIT_IMMED`和`AIN_COMMIT_DELAYED`。 |

## get

### 功能说明

`get`接口用于从目标Peer的远端Window读取数据到本Rank本地Window。

数据方向如下：

```text
peer srcWin + srcOffset -> local dstWin + dstOffset
```

### 函数原型

```cpp
template <typename DescriptorUbuf = AinDescriptorUbuf>
AIN_DEVICE void get(void *team, uint32_t peer,
    __gm__ void *dstWin, uint64_t dstOffset,
    __gm__ void *srcWin, uint64_t srcOffset,
    uint64_t bytes, const DescriptorUbuf &ubuf = DescriptorUbuf{},
    AinCommitFlags commitFlags = AIN_COMMIT_IMMED)
```

### 参数说明

**表 8**  get接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `team` | 输入 | Team或Communicator参数，当前为预留参数。 |
| `peer` | 输入 | 目标Peer或Rank下标。 |
| `dstWin` | 输入 | 本Rank本地Window Index。当前实现将该参数值解释为Window Index。 |
| `dstOffset` | 输入 | 本地Window内的字节偏移。 |
| `srcWin` | 输入 | 目标Peer的远端Window Index。当前实现将该参数值解释为Window Index。 |
| `srcOffset` | 输入 | 目标Peer远端Window内的字节偏移。 |
| `bytes` | 输入 | 待读取数据的字节数。 |
| `ubuf` | 输入 | Hcomm使用的UB临时工作区描述符。 |
| `commitFlags` | 输入 | 提交模式。支持`AIN_COMMIT_IMMED`和`AIN_COMMIT_DELAYED`。 |

## signal

### 功能说明

`signal`接口用于向目标Peer发送Indexed Signal通知。

### 函数原型

```cpp
template <typename RemoteAction, typename DescriptorUbuf = AinDescriptorUbuf>
AIN_DEVICE void signal(HcclTeam team, uint32_t peer, RemoteAction action,
    const DescriptorUbuf &ubuf = DescriptorUbuf{},
    AinCommitFlags commitFlags = AIN_COMMIT_IMMED)
```

### 参数说明

**表 9**  signal接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `team` | 输入 | Team参数，当前为预留参数。 |
| `peer` | 输入 | 目标Peer或Rank下标。 |
| `action` | 输入 | Signal动作，当前支持`AinSignalInc`和`AinSignalAdd`。 |
| `ubuf` | 输入 | Hcomm使用的UB临时工作区描述符。 |
| `commitFlags` | 输入 | 提交模式。支持`AIN_COMMIT_IMMED`和`AIN_COMMIT_DELAYED`。 |

**表 10**  Signal动作说明

| 动作 | 描述 |
| --- | --- |
| `AinSignalInc{signalId}` | 对指定`signalId`增加1。 |
| `AinSignalAdd{signalId, value}` | 对指定`signalId`增加`value`。 |

## readSignal

### 功能说明

`readSignal`接口用于读取本Rank本地Signal Window中的Signal值。

### 函数原型

```cpp
AIN_DEVICE uint64_t readSignal(HcclAinSignal_t signalId, uint32_t bits,
    AinMemoryOrder order = AIN_MEMORY_ORDER_ACQUIRE) const
```

### 参数说明

**表 11**  readSignal接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `signalId` | 输入 | Signal下标。 |
| `bits` | 输入 | 读取低位bit数。大于等于64时，读取完整64位。 |
| `order` | 输入 | Memory Order参数，当前为预留参数。 |

### 返回值说明

返回本地Signal值按`bits`截取后的结果。

## waitSignal

### 功能说明

`waitSignal`接口用于轮询本Rank本地Signal，直到Signal值不小于期望值。

### 函数原型

```cpp
AIN_DEVICE void waitSignal(HcclAinSignal_t signalId, uint64_t least, uint32_t bits,
    AinMemoryOrder order = AIN_MEMORY_ORDER_ACQUIRE) const
```

### 参数说明

**表 12**  waitSignal接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `signalId` | 输入 | Signal下标。 |
| `least` | 输入 | 等待的最小Signal值。 |
| `bits` | 输入 | 参与比较的低位bit数。大于等于64时，比较完整64位。 |
| `order` | 输入 | Memory Order参数，当前为预留参数。 |

## flush

### 功能说明

`flush`接口用于提交并等待当前`Ain`对象记录的延迟通信操作完成。

### 函数原型

```cpp
AIN_DEVICE void flush()
```

### 约束说明

使用`AIN_COMMIT_DELAYED`提交模式调用`put`、`get`或`signal`后，用户必须在依赖通信结果前调用`flush`接口。

## 枚举类型

**表 13**  AinMemoryOrder说明

| 枚举值 | 描述 |
| --- | --- |
| `AIN_MEMORY_ORDER_RELAXED` | Relaxed内存序，当前为预留枚举值。 |
| `AIN_MEMORY_ORDER_ACQUIRE` | Acquire内存序，当前为默认参数。 |
| `AIN_MEMORY_ORDER_RELEASE` | Release内存序，当前为预留枚举值。 |
| `AIN_MEMORY_ORDER_ACQ_REL` | Acquire-Release内存序，当前为预留枚举值。 |

**表 14**  AinCommitFlags说明

| 枚举值 | 描述 |
| --- | --- |
| `AIN_COMMIT_IMMED` | 默认模式，接口调用时立即提交通信操作。 |
| `AIN_COMMIT_DELAYED` | 延迟提交模式，接口先记录Channel，由后续`flush`统一提交并等待完成。 |

## 约束说明

- 使用AIN接口前，需要Host侧提前创建有效Device侧通信资源，并完成对称内存Window和Signal Window注册。
- `dstWin`和`srcWin`当前按Window Index使用，不是普通GM数据地址。
- `put`、`get`和`signal`必须传入有效`AinDescriptorUbuf`。
- `readSignal`和`waitSignal`依赖有效Signal Window，不能在无真实通信资源的编译样例中运行。
- 当前`signal`没有VA Signal运行时分支。
- 当前未提供`getSignalShadowPtr`公开接口。
- 后续Hcomm提供远端`AtomicAdd`接口后，`signal`实现需要切换为对应接口。

## 调用示例

如下示例展示Kernel侧AIN接口调用形态。真实运行前，Host侧必须已完成`devComm`创建，并注册好对称内存Window。

```cpp
#include "adv_api/ain/ain.h"

extern "C" __global__ __aicore__ void AinCustomKernel(GM_ADDR devCommAddr)
{
    auto devComm = reinterpret_cast<__gm__ void *>(devCommAddr);
    ain::device::Ain dev(devComm, 0);

    __ubuf__ uint8_t *ubufAddr = nullptr;
    ain::device::AinDescriptorUbuf ubuf{ubufAddr, 512, 0};

    uint32_t peer = 1;
    auto dstWin = reinterpret_cast<__gm__ void *>(0);
    auto srcWin = reinterpret_cast<__gm__ void *>(1);

    dev.put(nullptr, peer, dstWin, 0, srcWin, 0, 1024, ubuf,
        ain::device::AinRemoteNone{}, ain::device::AinLocalNone{},
        ain::device::AIN_COMMIT_DELAYED);

    dev.get(nullptr, peer, dstWin, 0, srcWin, 0, 1024, ubuf,
        ain::device::AIN_COMMIT_DELAYED);

    ain::device::HcclTeam team{};
    dev.signal(team, peer, ain::device::AinSignalInc{0}, ubuf,
        ain::device::AIN_COMMIT_DELAYED);

    dev.flush();
}
```

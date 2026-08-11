# Level0Shape 拓扑说明

## 1. 概述

`Level0Shape` 是 HCCL 对 Level 0 网络层逻辑拓扑形态的归类，主要用于通信算法选择，并不完全等同于物理布线名称。

定义位于：

```text
impl/adv_api/detail/hccl/cc/src/ops/op_common/inc/alg_param.h
```

```cpp
enum class Level0Shape {
    CLOS = 0,
    MESH_1D = 1,
    MESH_1D_CLOS = 2,
};
```

识别逻辑位于：

```text
impl/adv_api/detail/hccl/cc/src/ops/op_common/topo/topo_host.cc
```

## 2. Level0Shape::MESH_1D

`MESH_1D` 表示 Level 0 中只有一个 `COMM_TOPO_1DMESH` 拓扑实例，并且该实例覆盖当前 Level 0 的全部 rank。

```text
Rank0 ------- Rank1
  |  \         |
  |    \       |
  |      \     |
Rank2 ------- Rank3
```

该示意图强调 rank 之间存在可用于 Mesh 通信的直接或等价高带宽路径，不一定代表物理上每两张卡之间都有独立线缆。

### 2.1 识别条件

代码识别条件为：

```cpp
topoInstNum == 1
COMM_TOPO_1DMESH 实例数 == 1
meshRankSize == level0LocalRankSize
```

参考 `topo_host.cc:597-606`。

### 2.2 特点

- 所有 Level 0 rank 都属于同一个 Mesh 实例。
- 通常可以为每个远端 rank 建立直接 Mesh channel。
- 适合全对全并行收发。
- 通信路径通常较为对称。
- channel 和执行线程需求通常较多。

### 2.3 常用算法

例如 `InsAllReduceMesh1DTwoShot`：

- ReduceScatter 阶段直接与全部 rank 交换数据片。
- AllGather 阶段再次与全部 rank 交换归约结果。
- 通常每个 rank 使用一个执行线程。
- 算法依赖较完整的 Mesh 连接能力。

当前 AllReduce selector 中，单层 `MESH_1D` 默认选择：

```text
InsAllReduceMesh1DTwoShot
```

参考：

```text
impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:30-31
```

## 3. Level0Shape::CLOS

`CLOS` 表示 Level 0 中只有一个 `COMM_TOPO_CLOS` 拓扑实例，并且该 CLOS 实例覆盖全部本地 rank。

```text
Rank0 --+
Rank1 --+--- Switching Fabric ---+-- Rank2
Rank3 --+                        +-- Rank4
```

CLOS 是一种交换网络结构。rank 之间可以通过交换结构互通，但不意味着每一对 rank 都有专用的直接 Mesh 链路。

### 3.1 识别条件

代码识别条件为：

```cpp
topoInstNum == 1
COMM_TOPO_CLOS 实例数 == 1
closRankSize == level0LocalRankSize
```

参考 `topo_host.cc:607-616`。

如果拓扑不符合已知形态，当前代码会在部分 A2 场景默认归类为 CLOS：

```cpp
topoInfo->level0Topo = Level0Shape::CLOS;
```

参考 `topo_host.cc:633-634`。因此代码中的 `CLOS` 有时也具有“非 Mesh 的通用回退拓扑”含义。

### 3.2 特点

- 全部 rank 通过 CLOS 交换结构互联。
- rank 间未必存在完整的直接 Mesh 链路。
- 通常根据算法步骤为部分对端建链。
- 更适合 Ring、NHR、树形或分层通信。
- 相比 Mesh 全连接算法，可以减少同时使用的 channel 和线程。

### 3.3 常用算法

例如 NHR：

```text
step 0：与距离 1 的 rank 通信
step 1：与距离 2 的 rank 通信
step 2：与距离 4 的 rank 通信
...
```

每个阶段大约执行：

```text
ceil(log2(rankSize))
```

个通信 step，不要求每个 rank 同时与所有其他 rank 通信。

当前迁移后的 AllReduce selector 对单层 CLOS 默认选择：

```text
InsAllReduceNHR
```

## 4. Level0Shape::MESH_1D_CLOS

`MESH_1D_CLOS` 表示 Level 0 中同时存在：

- 一个 `COMM_TOPO_1DMESH` 实例。
- 一个 `COMM_TOPO_CLOS` 实例。

逻辑上相当于两种互联能力叠加：

```text
          Mesh 直连域
    Rank0 ------- Rank1
      |             |
    Rank2 ------- Rank3
      \             /
       \           /
         CLOS Fabric
```

### 4.1 识别条件

代码识别条件为：

```cpp
topoInstNum == 2
COMM_TOPO_CLOS 实例数 == 1
COMM_TOPO_1DMESH 实例数 == 1
closRankSize == level0LocalRankSize
```

参考 `topo_host.cc:617-631`。

这里只强制要求 CLOS 覆盖全部 Level 0 rank，Mesh 实例不一定覆盖全部 rank。

### 4.2 Mesh 与 CLOS 覆盖范围相同

```text
meshRankNum == closRankNum
```

全部 rank 既有 Mesh 能力，也有 CLOS 能力。算法可以优先使用 Mesh，或者同时利用两种网络。

### 4.3 CLOS 覆盖多个 Mesh 子域

```text
closRankNum > meshRankNum
closRankNum % meshRankNum == 0
```

示例：

```text
Mesh Group 0          Mesh Group 1
Rank0 -- Rank1        Rank4 -- Rank5
  |       |             |       |
Rank2 -- Rank3        Rank6 -- Rank7
       \                 /
         -- CLOS Fabric --
```

此时：

- Mesh 适合组内通信。
- CLOS 负责跨 Mesh 组通信。
- 常使用 Mesh + NHR、并行或分层算法。

相关判断位于：

```text
impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.cc:267-317
```

## 5. level0PcieMix

`MESH_1D_CLOS` 还经常结合以下字段判断：

```cpp
topoInfo->level0PcieMix
```

它表示 Level 0 的 rank 间链路中是否出现 PCIe 协议。代码会遍历当前 rank 到其他 rank 的 Level 0 链路，只要发现：

```cpp
srcProtocol == COMM_PROTOCOL_PCIE
```

就将 `level0PcieMix` 设置为 `true`。

参考：

```text
impl/adv_api/detail/hccl/cc/src/ops/op_common/topo/topo_host.cc:1003-1038
```

可以进一步区分：

| 场景 | 含义 |
| --- | --- |
| `MESH_1D_CLOS && level0PcieMix` | Mesh/CLOS 混合拓扑中存在 PCIe 链路 |
| `MESH_1D_CLOS && !level0PcieMix` | 没有 PCIe 混合，通常是其他高带宽协议组合 |

`level0PcieMix` 不是第四种 `Level0Shape`，而是对 `MESH_1D_CLOS` 链路协议的补充描述。

## 6. 核心区别

| 维度 | `MESH_1D` | `CLOS` | `MESH_1D_CLOS` |
| --- | --- | --- | --- |
| 拓扑实例 | 只有 Mesh | 只有 CLOS | Mesh 和 CLOS 同时存在 |
| 全 rank 覆盖 | Mesh 覆盖全部 rank | CLOS 覆盖全部 rank | CLOS 必须覆盖全部，Mesh 可能只覆盖部分 |
| rank 间关系 | 更接近直接、对称互联 | 通过交换结构互联 | 组内 Mesh，组间或补充路径走 CLOS |
| 典型 channel | 每个远端 rank 直接建链 | 只为算法步骤涉及的对端建链 | 根据组内、组间路径分别建链 |
| 典型线程数 | 较多，可按 rank 并行 | 较少，可按 NHR step 串行 | 取决于并行或分层算法 |
| 典型算法 | Mesh OneShot/TwoShot | NHR、Ring、Tree | Mesh+NHR、并行、并发 |
| 资源消耗 | channel、thread 较多 | 通常较少 | 介于两者之间或同时利用两类资源 |
| 算法设计重点 | 利用完整直连能力 | 减少对全连接的依赖 | 同时利用组内和组间带宽 |

## 7. 总结

- `MESH_1D`：全部 rank 位于一个 Mesh 直连域。
- `CLOS`：全部 rank 通过 CLOS 交换网络互联。
- `MESH_1D_CLOS`：既有 Mesh 局部或全局直连能力，又有覆盖全部 rank 的 CLOS 网络。

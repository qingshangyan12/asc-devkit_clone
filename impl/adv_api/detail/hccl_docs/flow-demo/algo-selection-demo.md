<!--
@source: impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.cc
@source: impl/adv_api/detail/hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.cc
-->

# 算法选择决策演示

本文档以 AllGather 为例，动态演示算法选择过程。

## 选择入口

```mermaid
flowchart TD
    START["算子入口<br>AllGatherOutPlace()"] --> ES["ExecuteSelector::Run()"]
    ES --> CB{"UseCannBridge?"}
    CB -->|是| CAN["SelectViaCann()"]
    CB -->|否| MC2{"isMc2?"}
    MC2 -->|是| MC2SEL["直接找 priority=18 选择器"]
    MC2 -->|否| REG["SelectorRegistry<br>GetSelectorsByOpType(ALLGATHER)"]
    REG --> ITER["遍历选择器"]
    MC2SEL --> SEL["AllGatherAutoSelector::Select()"]
    ITER --> SEL
    SEL --> RESULT["返回 algName"]
```

## Fallback 链决策

`AutoSelectorBase::Select()` 的完整决策流程：

```mermaid
flowchart TD
    S["Select() 开始<br>读取 opExecuteConfig"] --> D1{"CheckHostDPUOnly?"}
    D1 -->|是| DPU["SelectDPUAlgo()"]
    D1 -->|否| C1

    DPU -->|MATCH OK| DONE["返回 algName"]
    DPU -->|NOT_MATCH NO| C1

    C1{"CCU_MS?"} -->|是| MS["SelectCcuMsAlgo()"]
    C1 -->|否| C2

    MS -->|MATCH OK| DONE
    MS -->|NOT_MATCH NO| SET1["降级 → CCU_SCHED"]
    SET1 --> C2

    C2{"CCU_SCHED?"} -->|是| SCHED["SelectCcuScheduleAlgo()"]
    C2 -->|否| C3

    SCHED -->|MATCH OK| DONE
    SCHED -->|NOT_MATCH NO| SET2["降级 → CCU_FAIL"]
    SET2 --> C3

    C3{"AIV 或 AIV_ONLY?"} -->|是| AIV["SelectAivAlgo()"]
    C3 -->|否| C4

    AIV -->|MATCH OK| DONE
    AIV -->|NOT_MATCH + AIV_ONLY| FAIL["返回 NOT_MATCH"]
    AIV -->|NOT_MATCH + AIV| SET3["降级 → CCU_FAIL"]
    SET3 --> C4

    C4{"IsStarsState?<br>(AICPU_TS / HOSTCPU_TS / CCU_FAIL)"} -->|是| AICPU["SelectAicpuAlgo()"]
    C4 -->|否| FAIL2["返回 NOT_MATCH"]

    AICPU -->|MATCH OK| SET4["设置 AICPU_TS"]
    SET4 --> DONE
    AICPU -->|NOT_MATCH NO| FAIL3["返回 NOT_MATCH"]
```

## AllGather 各模式选择示例

### 示例 1：CCU_MS 模式，单层级 MESH_1D

```mermaid
flowchart TD
    A["opExecuteConfig = CCU_MS"] --> B["SelectCcuMsAlgo()"]
    B --> C{"topoLevelNums > 1?"}
    C -->|否| D{"level0Topo"}
    D -->|MESH_1D| E["✓ CcuAllGatherMesh1D"]
    D -->|MESH_1D_CLOS| F{"dataSize > 512KB?"}
    F -->|否| G["✓ CcuAllGatherMesh1DUBX"]
    F -->|是| H{"MeshNum==ClosNum<br>且 rankSize<=4?"}
    H -->|是| I["✓ CcuAllGatherConcurrentMesh1DNHR"]
    H -->|否| J["✗ NOT_MATCH"]
    D -->|其他| K["✗ NOT_MATCH"]
```

### 示例 2：AICPU 模式，多层级拓扑

```mermaid
flowchart TD
    A["opExecuteConfig = AICPU_TS<br>(CCU 失败降级)"] --> B["SelectAicpuAlgo()"]
    B --> C{"topoLevelNums > 1?"}
    C -->|是| D{"Level1Nhr?"}
    D -->|是| E["✓ InsAllGatherNHR"]
    D -->|否| F{"Level0Nhr?"}
    F -->|是| E
    F -->|否| G{"localNetInsSize == 1?"}
    G -->|是| E
    G -->|否| H{"level0Topo == MESH_1D?"}
    H -->|是| I["✓ InsAllGatherParallelMesh1DNHR"]
    H -->|否| J["✗ NOT_MATCH"]
    C -->|否| K{"level0Topo"}
    K -->|MESH_1D| L["✓ InsAllGatherMesh1D"]
    K -->|MESH_1D_CLOS| M{"参见 UBX 分支..."}
    K -->|CLOS| E
```

### 示例 3：DPU 模式

```mermaid
flowchart TD
    A["CheckHostDPUOnly = true"] --> B["opExecuteConfig → HOSTCPU"]
    B --> C["SelectDPUAlgo()"]
    C --> D{"topoLevelNums > 1?"}
    D -->|是| E{"localNetInsSize==1<br>或 level0Topo==MESH_1D?"}
    E -->|是| F["✓ InsAllGatherMeshNhrDPU"]
    E -->|否| G["✗ NOT_MATCH"]
    D -->|否| G
```

## 场景模拟

### 场景 A：单机 8 卡 AllGather（V310, CCU_MS）

| 参数 | 值 |
|------|-----|
| opExecuteConfig | CCU_MS |
| topoLevelNums | 1 |
| level0Topo | MESH_1D |
| level0MeshType | SINGLE_DIE |
| dataSize | 2MB (> 512KB) |

**选择路径**：`CCU_MS → SelectCcuMsAlgo → SelectMeshAlgo → level0Topo==MESH_1D`

**结果**：`CcuAllGatherMesh1D`

### 场景 B：双机 16 卡 AllGather（AICPU 模式）

| 参数 | 值 |
|------|-----|
| opExecuteConfig | AICPU_TS |
| topoLevelNums | 2 |
| Level1Nhr | true |
| level0Topo | MESH_1D |

**选择路径**：`AICPU_TS → SelectAicpuAlgo → topoLevelNums>1 → Level1Nhr=true`

**结果**：`InsAllGatherNHR`

### 场景 C：UBX 机型，小数据量

| 参数 | 值 |
|------|-----|
| opExecuteConfig | CCU_MS |
| topoLevelNums | 1 |
| level0Topo | MESH_1D_CLOS |
| dataSize | 256KB (< 512KB) |
| MeshNum == ClosNum | true |

**选择路径**：`CCU_MS → SelectCcuMsAlgo → SelectMeshAlgo → MESH_1D_CLOS → dataSize<=512KB`

**结果**：`CcuAllGatherMesh1DUBX`

### 场景 D：CCU 全部失败降级到 AICPU

| 参数 | 值 |
|------|-----|
| 初始 opExecuteConfig | CCU_MS |
| CCU_MS 匹配 | NOT_MATCH |
| CCU_SCHED 匹配 | NOT_MATCH |
| 最终 opExecuteConfig | CCU_FAIL → AICPU_TS |

**选择路径**：`CCU_MS ✗ → CCU_SCHED ✗ → CCU_FAIL → IsStarsState → SelectAicpuAlgo`

**结果**：取决于 AICPU 选择器逻辑

## 算法名到执行器的映射

选择器返回算法名后，通过 `CollAlgExecRegistryV2` 查找执行器：

```mermaid
flowchart LR
    SEL["选择器返回<br>algName"] -->|查找| REG["CollAlgExecRegistryV2"]
    REG -->|键: opType, algName| MAP["注册表查找"]
    MAP --> EXEC["返回执行器实例"]
    EXEC --> TMPL["执行器已绑定模板"]
```

示例映射（AllGather）：

| algName | 执行器 | 模板 |
|---------|--------|------|
| `InsAllGatherMesh1D` | `InsV2AllGatherSoleExecutor` | `InsTempAllGatherMesh1D` |
| `InsAllGatherNHR` | `InsV2AllGatherSoleExecutor` | `InsTempAllGatherNHR` |
| `InsAllGatherParallelMesh1DNHR` | `InsV2AllGatherParallelExecutor` | `Mesh1D` + `NHR` |
| `CcuAllGatherMesh1D` | `InsV2AllGatherSoleExecutor` | `CcuTempAllGatherMesh1D` |

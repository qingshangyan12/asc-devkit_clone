# AicpuAllGatherPipeLineMeshNHR 迁移方案

## 迁移前方案

### repository_baseline

| 仓库 | 路径 | 分支 | commit | 工作区 |
|---|---|---|---|---|
| hccl | ~/gitcode代码/asc-comm/hccl | master | 0564f2cc | clean |
| asc-devkit | ~/gitcode代码/asc-comm/asc-devkit-aicpu-allgather-ubx | aicpu-allgather-ubx | c9b7d4a164 | clean |

### 用户输入与代码实际映射

- 用户输入 algName: `aicpuallgatherpiplineubx`
- hccl 代码实际注册名: **`AicpuAllGatherPipeLineMeshNHR`**（无 "PipeLineUBX" 名）
- opType: `HCCL_CMD_ALLGATHER`
- engine: AICPU (AICPU_TS)
- executor: `InsV2AllGatherOmniPipeExecutor`（devkit 已有）
- TopoMatch: `TopoMatchTwoLevel`（devkit master **缺失**）
- 模板: `InsTempAllGatherOmniPipeMesh1D`(已有) + `InsTempAllGatherOmniPipeNHR`(已有) + `InsTempAllGatherOmniPipeNHRDPU`(**缺失**，L2 级 placeholder 不实际执行）

### 候选/选定算法

- selected_algorithms: [AicpuAllGatherPipeLineMeshNHR]

### HCCL 注册闭包（代码事实）

```cpp
// hccl ins_v2_all_gather_omnipipe_executor.cc:840-853
REGISTER_EXEC_V2_MULTI(
    HCCL_CMD_ALLGATHER, AicpuAllGatherPipeLineMeshNHR, InsV2AllGatherOmniPipeExecutor, TopoMatchTwoLevel,
    InsTempAllGatherOmniPipeMesh1D, InsTempAllGatherOmniPipeNHR, InsTempAllGatherOmniPipeNHRDPU);
REGISTER_ALG_ATTRS(...);  // MESH_1D_CLOS + pcieMix 支持 + topoCustomCheck
```

hccl selector 触发位置（3 处，all_gather_auto_selector.cc）：
- :319 多级对称 Uboe 两级（topoLevelNums==2）
- :374 PCIE 混布非全连 ≥16MB（OMNI_PCIE_AG_DATA_SIZE）
- :396 UBX 矩形 ≥16MB（isClosNumMultipleOfMeshNum && >512KB && ≥16MB 分界）

### asc-devkit 已有 / 缺失

| 组件 | 状态 | 说明 |
|---|---|---|
| InsV2AllGatherOmniPipeExecutor（875 行） | ✅ 已有 | AicpuAllGatherPipeLinePcie 已用 |
| InsTempAllGatherOmniPipeMesh1D / NHR | ✅ 已有 | PipeLinePcie 已用 |
| **InsTempAllGatherOmniPipeNHRDPU** | ❌ 缺 | hccl :176+45 行，L2 placeholder |
| **TopoMatchTwoLevel** | ❌ master 无 | allreduce_test 分支有（PR #5450 未合入） |
| **AicpuAllGatherPipeLineMeshNHR 注册** | ❌ 缺 | |
| **selector 16MB 分界** | ❌ 缺 | devkit UBX 大数据全走 MultiJetty 无分界 |
| AlgoType 枚举 | ✅ 已有 | PR #6735 合入时补全 |

### 计划新增/修改文件

新增（4 文件，~1400 行）：
1. `impl/.../op_common/topo/topo_match_base_v2.{h,cc}` — V2 基础（从 allreduce_test 分支复制）
2. `impl/.../op_common/topo/topo_match_two_level.{h,cc}` — 两级 V2（从 allreduce_test 分支复制）
3. `impl/.../all_gather/template/aicpu/ins_temp_all_gather_omnipipe_nhr_dpu.{h,cc}` — NHRDPU 模板（从 hccl 迁移）

修改（5 文件）：
4. `alg_attrs.h` — 确认 AlgoType/MESH_ALGO_TYPES 已有（PR #6735），如缺补
5. `ins_v2_all_gather_omnipipe_executor.cc` — 新增 REGISTER_EXEC_V2_MULTI + include
6. `all_gather_auto_selector.cc` — UBX 大数据分界（<16MB MultiJetty / ≥16MB PipeLineMeshNHR）
7-9. CMake ×3-4 处 — topo 新文件 + omnipipe 模板接线

### 注册闭环

| 路径 | algName | devkit 本地注册 |
|---|---|---|
| selector UBX ≥16MB | AicpuAllGatherPipeLineMeshNHR | ❌ 待新增 |
| selector UBX <16MB | InsAllGatherParallelMesh1DNHRMultiJetty | ✅ 已有 |
| forced-alg | AicpuAllGatherPipeLineMeshNHR | ❌ 待新增 |

### Host/AICPU 双侧路径

- Host: CalcAlgHierarchyInfo(V1 兼容重载) → TopoMatchTwoLevel::MatchTopo → CalcRes → executor
- AICPU: Orchestrate → KernelRun → 同一 executor（本地注册表查表）
- TopoMatchTwoLevel 需带 V1 兼容重载（MatchTopo(comm, topoInfo, algHierarchyInfo) 内置 {MESH,NHR} AlgAttrs）

### 构建目标矩阵

| 目标 | 新增源文件 |
|---|---|
| mc2_client (Host) | topo_match_base_v2.cc + topo_match_two_level.cc + nhr_dpu 模板 |
| mc2_server (AICPU) | 同上 |
| checker host (mc2_ccu) | topo 新文件 |
| checker AICPU (EXECUTOR+KERNEL) | 同上 |

### 接口适配清单

- namespace: ops_hccl → mc2_ops_hccl
- KernelRun 签名: TemplateResource& (hccl) vs const TemplateResource& (devkit) — 以 devkit 基类为准
- include 路径: hccl algorithm/ → devkit 平铺
- nhr_dpu 模板: 继承 InsTempAllGatherNHRDPU（devkit 已有基类）

### 选择行为约束

- 阈值: OMNI_PCIE_AG_DATA_SIZE = 16MB（devkit 需新增常量）
- UBX 大数据: <16MB → MultiJetty(不变) / ≥16MB → **PipeLineMeshNHR(新增，改变现有行为)**
- PCIE 混布 ≥16MB: 也可选此算法（hccl 同款）
- forced-alg: AicpuAllGatherPipeLineMeshNHR 需精确匹配注册名

### 共享逻辑影响

1. TopoMatchTwoLevel 基建（纯新增，不改既有）
2. selector 仅在 UBX 大数据分支追加 16MB 分界（<16MB 行为不变）
3. **行为变化**: UBX ≥16MB 从 MultiJetty → PipeLineMeshNHR（对齐 hccl）

### 风险与待确认项

- **D-1**: TopoMatchTwoLevel 基建从 allreduce_test 分支复制（~1200 行，已审计通过但 PR #5450 未合入 master）— 是否同意？
- **D-2**: UBX ≥16MB 大数据从 MultiJetty 改选 PipeLineMeshNHR（对齐 hccl 行为）— 是否批准？
- nhr_dpu 模板为 L2 placeholder（hccl 注释"L2 模板不执行"），仍需迁移以匹配 REGISTER_EXEC_V2_MULTI 3 模板参数

### 用户确认记录: （待确认）

scope_status: NEEDS_USER_CONFIRMATION
plan_document: docs/migration/aicpu-allgather-pipeline-mesh-nhr-migration-plan.md

### 用户确认记录

D-1: 确认 — 从 allreduce_test 分支复制 TopoMatchTwoLevel 基建
D-2: 确认 — UBX 矩形 + ≥16MB 从 MultiJetty 改选 PipeLineMeshNHR（本次迁移目的：补齐 hccl 行为）

scope_status: READY_TO_MIGRATE

## 迁移后实施结果

### 实际修改/新增文件（6 新增 + 6 修改）

| 文件 | 操作 | 内容 |
|---|---|---|
| `op_common/topo/topo_match_base_v2.{h,cc}` | 新增（99+383 行） | V2 基础（从 allreduce_test 分支 ac3e18e828 复制） |
| `op_common/topo/topo_match_two_level.{h,cc}` | 新增（41+153 行） | 两级 V2 + V1 兼容重载（同上） |
| `all_gather/template/aicpu/ins_temp_all_gather_omnipipe_nhr_dpu.{h,cc}` | 新增（45+176 行） | NHRDPU 模板（从 hccl 迁移，KernelRun 签名适配 const TemplateResource&） |
| `op_common/selector/alg_attrs.h` | 修改（+40 行） | AlgoType 前向声明→完整枚举（17 值）+ MESH/MESH_CONCUR 集合 |
| `all_gather/executor/ins_v2_all_gather_omnipipe_executor.cc` | 修改（+11 行） | include nhr_dpu + topo_match_two_level；注册 AicpuAllGatherPipeLineMeshNHR + META |
| `all_gather/selector/all_gather_auto_selector.cc` | 修改（+2 行） | UBX 大数据 16MB 分界：<16MB MultiJetty / ≥16MB PipeLineMeshNHR |
| CMake ×3 | 修改 | src（server/AICPU）、topo（mc2_client）、all_gather template 各加新源文件 |

### Host/AICPU 构建目标影响

- mc2_client（Host）: +topo_match_base_v2.cc +topo_match_two_level.cc +nhr_dpu 模板
- mc2_server（AICPU）: 同上（src/CMakeLists.txt 共用）
- checker: mc2_ccu 和 aicpu checker 不含 omnipipe executor（无需接线）

### 迁移前计划与实际差异

- 无（按计划执行）
- 补充：alg_attrs.h 的 AlgoType 补全（计划中列为"确认已有"，实际 master 仍为前向声明，已补）

### 常规检查

- `git diff --check`: 通过
- 注册/选择闭环: AicpuAllGatherPipeLineMeshNHR selector ↔ 注册 精确匹配 ✓
- 悬空 algName: `AivAllGatherMesh1D` 为 master 既有（非本次引入），不阻塞
- nhr_dpu 模板语义对照: 与 hccl 唯一差异为 KernelRun 签名 const 适配（记录为目标接口适配差异）
- include/namespace: mc2_ops_hccl ✓
- CMake 接线: 3 处已覆盖（checker 不需要）
- override 签名: KernelRun(const TemplateResource&) 与 devkit 基类一致 ✓
- 未执行真实编译（本机无 CANN/Linux 工具链）

### 未完成项、遗留风险与 audit 输入

1. 编译验证待环境（无工具链）
2. ST 用例未新增（建议补 PipeLineMeshNHR 在 UBX ≥16MB 的选择断言）
3. TopoMatchTwoLevel 的 V1 兼容重调内置 {MESH,NHR} 固化 AlgAttrs——适用于 AllGather omnipipe 两级场景
4. 建议手动调用 hccl-audit-migration 复审

状态: 迁移完成（阶段二已回填；编译/ST 未执行，待环境）

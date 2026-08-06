<!--
@purpose: Agent 持续更新指南
@audience: AI Agent / 开发者
-->

# Agent 持续更新指南

本指南帮助 AI Agent 或开发者在源码升级后，准确、高效地更新本文档站点。

## 文档站点位置

```
asc-devkit/impl/adv_api/detail/hccl_docs/
```

## 启动文档站点

```bash
# 安装 docsify-cli（一次性）
npm i -g docsify-cli

# 启动
cd asc-devkit/impl/adv_api/detail/hccl_docs
docsify serve
# 访问 http://localhost:3000
```

无需构建步骤，修改 markdown 文件后刷新浏览器即生效。

## 源码 → 文档映射表

当检测到以下源码文件变更时，需要更新对应的文档：

| 源码文件 | 对应文档 | 更新要点 |
|----------|----------|----------|
| `include/adv_api/hccl/hccl.h` | `api-reference/hccl-class.md`, `api-reference/collective-ops.md`, `api-reference/lifecycle.md`, `api-reference/query-utils.md` | API 签名、参数说明、模板参数 |
| `include/adv_api/hccl/hccl_common.h` | `api-reference/hccl-class.md`, `architecture/data-structures.md` | 枚举定义（HcclCMDType, HcclReduceOp, HcclDataType 等） |
| `include/adv_api/hccl/hccl_tiling.h` | `api-reference/tiling-api.md` | Mc2CcTilingConfig 接口 |
| `include/adv_api/hccl/hccl_tilingdata.h` | `api-reference/tiling-api.md`, `architecture/data-structures.md` | Tiling 数据结构定义 |
| `include/adv_api/hccl/internal/hccl.h` | `api-reference/collective-ops.md` | Host C 接口声明 |
| `include/adv_api/hccl/internal/hccl_msg.h` | `architecture/data-structures.md`, `architecture/device-server-comm.md` | HcclMsg/HcclMsgArea 消息结构 |
| `include/adv_api/hccl/internal/hccl_tiling_msg.h` | `architecture/data-structures.md` | Tiling 内部结构 |
| `impl/.../hccl/impl/hccl_impl_def.h` | `implementation/platform-dispatch.md` | 平台分发逻辑 |
| `impl/.../hccl/impl/hccl_impl.h` | `api-reference/hccl-class.md` | Hccl→HcclImpl 转发逻辑 |
| `impl/.../hccl/common/hccl_base.h` | `implementation/device-aicpu.md` | HcclImpl 基类定义 |
| `impl/.../hccl/common/hccl_aicpu_impl.h` | `implementation/device-aicpu.md`, `flow-demo/commit-wait-demo.md` | AICPU 实现：Prepare/Commit/Wait |
| `impl/.../hccl/common/hccl_inner_def.h` | `architecture/data-structures.md` | CommonPrepareParam/HcclCombineOpParam 等 |
| `impl/.../hccl/impl/platform_v310/hccl_aicpu.h` | `implementation/device-aicpu.md` | V310 平台 AICPU 特化 |
| `impl/.../hccl/impl/platform_v310/hccl_ccu_v0.h` | `implementation/device-ccu.md` | V310 平台 CCU 特化 |
| `impl/.../hccl/impl/platform_v220/hccl_aicpu.h` | `implementation/device-aicpu.md` | V220 平台 AICPU 特化 |
| `impl/.../hccl/cc/src/ops/op_common/selector/auto_selector_base.{h,cc}` | `algorithm/selection-mechanism.md` | 算法选择基类逻辑 |
| `impl/.../hccl/cc/src/ops/op_common/selector/selector_registry.{h,cc}` | `algorithm/selection-mechanism.md` | 选择器注册机制 |
| `impl/.../hccl/cc/src/ops/op_common/selector/execute_selector.{h,cc}` | `algorithm/selection-mechanism.md` | 选择器执行入口 |
| `impl/.../hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.{h,cc}` | `operators/allgather.md`, `flow-demo/algo-selection-demo.md` | AllGather 选择逻辑 |
| `impl/.../hccl/cc/src/ops/reduce_scatter/selector/reduce_scatter_auto_selector.{h,cc}` | `operators/reducescatter.md` | ReduceScatter 选择逻辑 |
| `impl/.../hccl/cc/src/ops/op_common/template/alg_template_base.{h,cc}` | `algorithm/template-registry.md` | 算法模板基类 |
| `impl/.../hccl/cc/src/ops/op_common/template/registry/alg_template_register.{h,cc}` | `algorithm/template-registry.md` | 模板注册宏 |
| `impl/.../hccl/cc/src/ops/op_common/executor/executor_base.{h,cc}` | `algorithm/executor-registry.md` | 执行器基类 |
| `impl/.../hccl/cc/src/ops/op_common/executor/executor_v2_base.{h,cc}` | `algorithm/executor-registry.md` | V2 执行器基类 |
| `impl/.../hccl/cc/src/ops/op_common/executor/registry/coll_alg_v2_exec_registry.{h,cc}` | `algorithm/executor-registry.md` | 执行器注册宏 |
| `impl/.../hccl/cc/src/common/alg_type.h` | `algorithm/algo-types.md` | 算法类型枚举 |
| `impl/.../hccl/cc/src/ops/op_common/inc/alg_param.h` | `architecture/data-structures.md` | OpParam/TopoInfo 等核心结构 |
| `impl/.../hccl/cc/src/ops/all_gather/all_gather_op.{h,cc}` | `operators/allgather.md` | AllGather 算子入口 |
| `impl/.../hccl/cc/src/ops/reduce_scatter/reduce_scatter_op.{h,cc}` | `operators/reducescatter.md` | ReduceScatter 算子入口 |
| `impl/.../hccl/cc/src/aicpu_kfc/framework/device/` | `implementation/aicpu-server.md` | AICPU 服务端框架 |

## 文档生成规范

### 1. 元数据头

每个 markdown 文件应以 HTML 注释开头，标注覆盖的源码文件：

```markdown
<!--
@source: path/to/source1.h
@source: path/to/source2.cc
-->
```

### 2. 源码引用格式

引用具体代码位置时使用 `文件相对路径:行号` 格式：

```markdown
> 定义见 `include/adv_api/hccl/hccl_common.h:22`。
```

### 3. Mermaid 图规范

- 架构图使用 `flowchart TD`（从上到下）
- 时序图使用 `sequenceDiagram`
- 类继承使用 `classDiagram`
- 状态转换使用 `stateDiagram-v2`
- 每个图应有简短标题
- 节点名称使用源码中的实际类名/函数名

### 4. 内容准则

- **严肃可信**：所有描述必须基于源码，不得臆测
- **不确定的内容**：标注「待确认」而非编造
- **代码片段**：直接引用源码，注明出处
- **层次分明**：使用标题层级（#, ##, ###）组织内容

## 更新检查清单

当源码升级后，按以下步骤检查：

1. **API 变更**：检查 `include/adv_api/hccl/hccl.h` 是否有新增/修改/废弃接口 → 更新 `api-reference/`
2. **枚举变更**：检查 `hccl_common.h` 中的枚举 → 更新 `api-reference/hccl-class.md` 和 `architecture/data-structures.md`
3. **算法选择变更**：检查 `auto_selector_base.cc` 和各算子 selector → 更新 `algorithm/selection-mechanism.md` 和 `operators/`
4. **新增算子**：检查 `cc/src/ops/` 目录是否有新算子目录 → 新增 `operators/<opname>.md`
5. **平台变更**：检查 `impl/` 下是否有新平台目录 → 更新 `implementation/platform-dispatch.md`
6. **数据结构变更**：检查 `alg_param.h`、`hccl_inner_def.h`、`hccl_msg.h` → 更新 `architecture/data-structures.md`
7. **侧边栏**：新增页面后更新 `_sidebar.md`

# HcclAllocComResourceByTiling / CcuKernelLaunch 调用链分析

## 结论

这份文档先只看两条主线：

1. `HcclAllocComResourceByTiling` 负责“算子资源准备”和“算法选择后的资源序列化”。
2. `CcuKernelLaunch` 负责“把已经准备好的 CCU 资源真正下发出去”。

`HcclExecOp` 这边不要再放进来。`HcclAllocComResourceByTiling` 的 CCU 路径里，真正做 CCU kernel 注册的是更深一层的资源构建流程：

```text
CcuSelectAlg
  -> ProcessCcuTiling
    -> PrepareCcuAlgorithmResource
      -> GetCcuOpParamResCtx
        -> AcquireAlgResources
          -> HcclGetAlgRes
            -> HcclAllocAlgResourceCcu
              -> HcclGetCcuKernel
                -> RegisterCcuKernels
                  -> HcommCcuKernelRegisterStart / HcommCcuKernelRegister / HcommCcuKernelRegisterEnd
```

`CcuKernelLaunch` 不做注册，它只负责把 device 上的 `OpResCtx` 和序列化资源搬回 host，拆出
`ThreadHandle / CcuKernelHandle / kfcServerArgs`，然后逐个调用 `HcommCcuKernelLaunch` 下发。

## 1. 从 HcclAllocComResourceByTiling 开始看

入口在 `impl/adv_api/detail/hccl/cc/src/common/hccl_mc2.cc`。

```text
HcclAllocComResourceByTiling
  -> HcclAllocComResourceByTilingImpl
    -> CheckInputParam
    -> HcclGetTilingList
    -> ObtainCommEngine
    -> AllocComResourceByTilingAicpu / AllocComResourceByTilingCcu
```

CCU 分支里最关键的是：

```text
AllocComResourceByTilingCcu
  -> BuildTagsAndValidate
  -> AllocCcuOpResCtx
  -> CcuSelectAlg
  -> GetOrCreateCcuCtx(tag + "_opResCtx")
  -> aclrtMemcpy(H2D) 写回 OpResCtx
```

这里容易误读成 `CcuSelectAlg` 就是在做注册。不是。它做的是：

1. 根据 `ccType / tiling / comm / topoInfo` 选算法。
2. 计算资源需求。
3. 把结果写进 `OpResCtx` 和序列化上下文。

它本身还没有进入 `HcommCcuKernelRegisterStart/End`。

对应代码：

- `HcclAllocComResourceByTilingImpl`
- `AllocComResourceByTilingCcu`
- `AllocCcuOpResCtx`

## 新 MC2 接口覆盖情况

当前 `hccl_mc2.cc` 里的新接口已经覆盖了旧路径里“资源准备”的大部分逻辑：

- `Mc2GetCcArgsCcu` 负责分配和初始化参数对象。
- `Mc2AcquireCcResCtxCcu` 负责把 `ccArgs` 转成 `OpParam` / `topoInfo` / `algName`，再走 `CalcAlgHierarchyInfo`、`CalcRes`，最后组出 `Mc2CcuBuiltinCtx`。
- `Mc2CcuCcKernel` 负责遍历 `resRequest.ccuKernelInfos` 并触发 kernel 执行。

但它目前还**没有完整包含**旧路径的注册/下发语义：

- 旧路径里的 `HcommCcuKernelRegisterStart / HcommCcuKernelRegister / HcommCcuKernelRegisterEnd` 仍然只在 `RegisterCcuKernels` 里。
- 旧路径里的 `HcommCcuKernelLaunch` 仍然只在 `CcuKernelLaunch` 里。
- `Mc2CcuCcKernel` 现在是直接调用 `kernelFunc(kernelArg)`，没有走 `threadHandle / kernelHandle / kfcServerArgs` 这套 launch 数据结构。
- `Mc2AcquireCcResCtxCcu` 虽然返回了 `schd`，但当前 `Mc2CcuCcKernel` 并没有消费它。

补充一下这层的资源归属：`Mc2AcquireCcResCtxCcu` 产出的 `ccResCtx` 里，已经带上了后续执行需要的 `resRequest.ccuKernelInfos` / `ccuKernelNum` 这类 kernel 描述信息；
如果后面走的是 `Mc2CcuCcKernel<<<schd, insHandle, stream>>>(ccResCtx)` 这种 host launch 形式，那么 `ccResCtx` 会作为 `args` 传入
`asccomm_ccu_host_kernel_launch`，而真正做 `HcommCcuKernelRegisterStart/End` 的是 launch 层，不是 `Mc2AcquireCcResCtxCcu` 本身。

所以结论是：**新接口已经迁移了算子参数、拓扑、算法选择、资源计算这些“准备层”能力，但还没有把旧 aclnn 路径里的 CCU 注册与 launch 能力完整等价地收口进去。**

如果目标是彻底替换旧 aclnn 路径，后续不能只做代码机械搬运，还是要把原流程重新拆成“参数准备 / 资源计算 / 注册 / launch”四段，再映射到新的 `Mc2*` 接口里重组。

## 2. CCU kernel 注册流程

`ProcessCcuTiling` 只负责把单个 tiling 的算法、资源和参数一路算出来；真正的注册动作在 `RegisterCcuKernels`。

```text
ProcessCcuTiling
  -> SelectCcuAlgorithm
  -> PrepareCcuAlgorithmResource
    -> GetCcuOpParamResCtx
      -> AcquireAlgResources
        -> HcclGetAlgRes
          -> HcclAllocAlgResourceCcu
            -> HcclGetThread
            -> HcclGetChannelForCcu
            -> HcclGetCcuKernel
              -> RegisterCcuKernels
```

`AcquireAlgResources` 在 CCU 分支里先补 `kfcServerArgs`，再走 `HcclGetAlgRes`；`HcclGetAlgRes` 最终
进入 `HcclAllocAlgResourceCcu`，然后才到 `RegisterCcuKernels`。

对应代码：
- `CcuSelectAlg`：`impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc:1084-1104`
- `ProcessCcuTiling`：`impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc:1048-1080`
- `PrepareCcuAlgorithmResource`：`impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc:1004-1027`
- `GetCcuOpParamResCtx`：`impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc:841-863`
- `HcclGetAlgRes`：`impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:784-915`
- `HcclAllocAlgResourceCcu`：`impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:1372-1414`
- `HcclGetCcuKernel`：`impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:2123-2154`
- `RegisterCcuKernels`：`impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:1726-1775`

### RegisterCcuKernels 做了什么

`RegisterCcuKernels` 会按 `resGroup` 分组：

1. `HcommCcuKernelRegisterStart(insHandle)`
2. 遍历该组的 `CcuKernelInfo`
3. `HcommCcuKernelRegister(...)`
4. `HcommCcuKernelRegisterEnd(insHandle)`

所以 `register start/end` 是显式的注册边界，不在 `HcclAllocComResourceByTiling` 的外层包装里。

## 3. CcuKernelLaunch 的流程

`CcuKernelLaunch` 做的是 launch，不做注册：

```text
CcuKernelLaunch
  -> CopyOpResCtxToHost
  -> CopyOpParamToHost
  -> LoadResourceCtx
  -> GetLaunchMissionNum
  -> for each mission:
       HcommCcuKernelLaunch(threadHandle, kernelHandle, kfcArgs, argSize)
```

对应代码：`impl/adv_api/detail/hccl/cc/src/common/hccl_mc2.cc:747-789`

### 它依赖的数据结构

`CcuKernelLaunch` 主要依赖下面几类结构：

| 结构 | 作用 |
|---|---|
| `OpResCtx` | device 侧资源句柄容器，launch 时先拷回 host，再继续拆 `OpParam` 和资源上下文 |
| `OpParam` | 从 `OpResCtx.algInfo[0].opParam` 取回，提供 `resCtx` 和 `ctxSize` |
| `AlgResourceCtxSerializable` | 序列化的资源包，launch 核心依赖它的 `threads`、`ccuKernels`、`kfcServerArgs` |
| `ThreadHandle` / `CcuKernelHandle` | 每个 mission 一对 launch 句柄 |

`AlgResourceCtxSerializable` 里和 launch 直接相关的字段是：

- `threads`
- `ccuKernels`
- `kfcServerArgs`
- `kfcServerArgSize`

它的序列化 / 反序列化也把这些字段原样带回 device/host。

对应代码：`impl/adv_api/detail/hccl/cc/src/ops/op_common/inc/alg_param.h:481-589`

### launch 校验逻辑

`GetLaunchMissionNum` 会检查：

1. `threads` 和 `ccuKernels` 不能为空
2. `kfcServerArgs` 的尺寸和 `kfcServerArgSize` 必须匹配
3. `missionNum = kfcServerArgs[0]`
4. 每个 mission 的 `xnAddr / ckeAddr / missionNum / missionIndex` 必须一致
5. `threads.size()` 和 `ccuKernels.size()` 不能小于 `missionNum`

然后按 `missionIndex` 逐条调用：

```text
HcommCcuKernelLaunch(threadHandle, kernelHandle, kfcArgs, resourceCtx.kfcServerArgSize)
```

## 4. 新方案和旧流程的对应

如果把现在的新方案和旧流程一一对上，可以这样理解：

- `Mc2GetCcArgsCcu` 对应“准备 CCU 算子参数对象”。
- `Mc2AcquireCcResCtxCcu` 对应“算法选择 + 资源计算 + 序列化上下文构建”。
- `Mc2CcuCcKernel` 对应“CCU server 的统一 host launch 入口”。

也就是说，新方案不是把旧代码原封不动复制一遍，而是把旧流程拆成：

1. 参数准备
2. 拓扑和算法选择
3. 资源计算和注册信息构造
4. host launch
5. client 侧 AICore 通信调用

然后再映射到新的 `Mc2*` 接口里。

## 5. 补充

`ccu_host_launch.h` 里的 `asccomm_ccu_schd` / `asccomm_launch_kernel_cfg` 是 CCU host launch 侧的配置类型，
但 `CcuKernelLaunch` 这条链路本身没有直接使用它们；这里实际跑的是 `HcommCcuKernelRegister*` 和
`HcommCcuKernelLaunch` 这组接口。

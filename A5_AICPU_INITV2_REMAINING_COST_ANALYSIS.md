# A5 AICPU InitV2 剩余耗时分析与优化方向

## 当前状态

当前 A5 AICPU 路径已经完成：

- 去除 A5 不使用的 `devType_` 读取。
- `handleIdMsgPosition_` 调整为 64 字节对齐，并用 8 次对齐 64 位写完成初始化。
- Init 相关内部校验使用 `ASCENDC_HCCL_API_CHECK`：debug/CPU debug 开启，release 编译期裁剪。
- A5 直接复用原 `platform_v310/hccl_aicpu.h`，不再新增 A5 专用头文件。
- 原 `OpResCtx` 直接裁剪，保留 AICPU 与 CCU 所需字段。
- 修复 AICPU server 遍历 `algInfo` 的越界问题。

当前五类原语的 `client_init` 热态均值约为 **265-270 ticks**。按 A5 文档的 1GHz 换算，约 **265-270 ns**。

## 当前 InitV2 执行路径

以带 tiling 的 AIC 客户端路径为例：

```cpp
debugMode_ = initTilingPtr->debugMode;
tilingBaseAddr_ = reinterpret_cast<uint64_t>(initTiling);
hcclContext_ = reinterpret_cast<__gm__ OpResCtx*>(context);

InitInner(hcclContext_->workspace, CONTEXT_DECOUPLE_VERSION);
```

`InitInner` 内部主要做：

1. release 下裁剪重复初始化和 workspace 空值检查；
2. workspace 512B 对齐计算；
3. 写入 `hcclMsgArea_`；
4. 8 次对齐 64 位写初始化 `handleIdMsgPosition_`；
5. 调用 `InitWorkingFlag()`；
6. 写入 `curVersion_`。

`InitWorkingFlag()` 当前执行：

```cpp
KERNEL_LOG(KERNEL_INFO, "Working core type %u id %u.", ...);

if constexpr (config.type == CoreType::ON_AIC) {
    workingFlag_ = (g_coreType == AscendC::AIC && GetBlockIdx() == config.blockId);
}
```

## 剩余耗时构成判断

### 1. GM 读是主要成本

当前路径至少涉及以下设备侧读：

- `initTilingPtr->debugMode`
- `hcclContext_->workspace`

其中 `workspace` 位于 `OpResCtx` 内部，需要先拿到 context 地址，再读其字段。AIC 上这类 GM/设备侧读的延迟远高于本地寄存器或栈访问。一次读 miss 就可能贡献几十到上百 ticks。

虽然 `OpResCtx` 已从 408B 裁剪到 160B，改善了访问局部性和 D2H 拷贝，但 `InitV2` 仍至少要读 `workspace`。

### 2. 计时边界可能存在重排

当前测试代码：

```cpp
t[2] = CLOCK();
hccl.InitV2(ctx, &localTiling);
t[3] = CLOCK();
```

`GetSystemCycle()` 在 A5 上实现为：

```cpp
uint64_t sysCnt = 0;
asm volatile("MOV %0, SYS_CNT\n" : "+l"(sysCnt));
```

该 asm 是 `volatile`，但没有 `memory` clobber。它能防止系统计数器读取被删除或重复，但不能阻止 `InitV2` 内部写操作被调度到第二个 `CLOCK()` 之后。

因此，265-270 ticks 是当前外部计时口径下的结果，不能完全等价为 `InitV2` 的严格执行时间。

### 3. `InitWorkingFlag()` 仍有系统状态读取

AIC 路径需要读：

- `g_coreType`
- `GetBlockIdx()`

这不是普通本地计算。即使 `config` 是编译期常量，这两个状态读取仍可能走 special/core 信息路径，具有一定固定延迟。

### 4. 8 次宽写仍有成本

`handleIdMsgPosition_` 初始化已经从 63 次逐字节写优化为 8 次对齐 64 位写，但仍是 8 次显式写。如果对象布局或寄存器压力较大，这些写可能落到栈/本地内存。

### 5. `KERNEL_LOG` 需要确认是否编译期裁剪

当前 `InitWorkingFlag()` 内有一条 INFO 日志：

```cpp
KERNEL_LOG(KERNEL_INFO, "Working core type %u id %u.", ...);
```

需要确认 release 下这条日志是否整体编译期裁剪。如果宏只做运行时级别判断，仍可能保留参数求值或分支指令。

### 6. 模板和宏可能影响内联

`InitV2`、`InitInner`、`InitWorkingFlag` 都是 inline，但模板、ASSERT 宏、GM 访问和日志宏组合后，编译器未必能完全内联或达到最优调度。

## 建议先做的验证

### 1. 修复计时边界

在测试框架的 `client.asc` 中给 `InitV2` 前后加内存屏障：

```cpp
t[2] = CLOCK();
asm volatile("" ::: "memory");
hccl.InitV2(ctx, &localTiling);
asm volatile("" ::: "memory");
t[3] = CLOCK();
```

这样先确认当前 265-270 ticks 是否真实。

### 2. 内部分段测量

在 `InitV2` 内部临时增加计数点：

1. 读完 `debugMode_`
2. 读完 `workspace`
3. 完成 `handleIdMsgPosition_` 写
4. 完成 `InitWorkingFlag()`
5. 写完 `curVersion_`

输出每段 ticks，可以定位剩余耗时主要来自哪一步。

### 3. 对比不同路径

建议增加三个实验版本：

1. 完整当前实现；
2. 去掉 `InitWorkingFlag()`；
3. 去掉 8 次 handle 表写。

用差分估算各部分成本。

## 后续优化方向

### 1. 移除或下放 `debugMode_` 读取

当前测试已经把 tiling 从 GM 拷贝到 local 后再传给 `InitV2`。可以在计时外提前读取 `localTiling.debugMode`，或通过模板参数传入，减少 `InitV2` 内一次读。

风险低，收益取决于该读是否命中 cache。

### 2. A5 下删除 `InitWorkingFlag()` 日志

将：

```cpp
KERNEL_LOG(KERNEL_INFO, "Working core type %u id %u.", ...);
```

改为编译期裁剪，或直接删除 A5 热路径日志。

风险低，收益取决于当前日志宏展开方式。

### 3. 缓存 `workingFlag_`

`config` 和核类型在同一个 kernel 内不会变化。可以在 kernel 入口一次性计算：

```cpp
workingFlag_ = (g_coreType == AIC && GetBlockIdx() == blockId);
```

然后 `InitV2` 只读取缓存值，避免在 Init 内做 special/core 状态读取。

### 4. 前置 workspace 对齐

如果 host 侧保证 workspace 已 512B 对齐，A5 设备侧可以删除：

```cpp
if (msgAddr & 0x1ff) { ... }
```

`InitV2` 直接写 `hcclMsgArea_`。

风险低，但需要确认 workspace 分配接口的 Alignment 保证。

### 5. 简化 workspace 访问路径

目前需要：

```cpp
context -> workspace
```

如果 A5 路径将 workspace 作为 `InitV2` 的独立参数传入，或者将其放在更固定的首字段，可以减少依赖链。极端方案是让测试/调用方直接传入已对齐 workspace 地址。

### 6. 初始化范围裁剪

当前初始化 64B handle 表。如果 A5 场景能保证单次 launch 内 Prepare 次数远小于 63，可以只初始化实际使用的范围。但这会影响错误路径语义，风险较高。

### 7. `InitV2` 模板参数化

把运行期不变的配置提升到模板参数：

```cpp
InitV2A5<coreType, blockId, debugMode>(ctx, tiling)
```

让编译器消掉 `workingFlag_`、版本相关分支，甚至部分状态写入。

风险中，需要改调用端。

### 8. 确认设备编译优化选项

检查 A5 设备侧编译是否使用 `-O3`、是否开启内联/LTO。Host `mc2_client` 已有 `-O3`，但设备侧头文件编译选项需要单独确认。

### 9. 减少对象状态

当前 `HcclImpl` 仍维护：

- tiling table
- handle table
- message position
- version
- working flag
- debug mode

如果能将 A5 的状态压缩成少量字段，并把不变状态放到模板/config，可以减少初始化写数量。

## 推荐实施顺序

1. 修测试计时边界，确认真实耗时。
2. 删除 `InitWorkingFlag()` 的 INFO 日志。
3. 缓存或模板化 `workingFlag_`。
4. 前置 workspace 对齐，删除运行时对齐分支。
5. 下放或模板化 `debugMode_`。
6. 做 Init 内部分段 profiling，确认剩余瓶颈。
7. 再考虑协议级或对象布局级改动。

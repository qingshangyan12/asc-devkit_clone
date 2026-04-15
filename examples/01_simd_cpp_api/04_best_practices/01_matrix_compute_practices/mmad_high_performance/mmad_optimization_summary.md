# MMAD 双缓冲优化总结：原版 vs 优化版

## 一、总体架构对比

| 维度 | 原版 (mmad.asc) | 优化版 (mmad_backup.asc) |
|------|-----------------|--------------------------|
| CopyIn 调度方式 | 绑定 kIndex，每轮都调用 CopyInA/B | 独立进度跟踪，按需搬运 |
| L1 双缓冲 flag 数 | 2 个 MTE2_MTE1 flag（A/B 各 1） | 4 个 MTE2_MTE1 flag（A/B 各 2，Ping/Pong 各 1） |
| 预取策略 | 无独立预取，首个 kIndex 隐式触发 | 显式预取 APing + BPing |
| CopyIn 触发时机 | 大包开头（kModStepKa == 0），在 DataLoad 之前 | 大包开头（kModStepKa == 0），WaitFlag 后立即 Preload |
| 指令发射顺序 | CopyIn → DataLoad → Compute | WaitFlag → Preload → DataLoad → Compute（可调整为 Compute → Preload） |
| CopyIn 搬运顺序 | 先 A 后 B | 先 B 后 A（优先满足更紧迫的 B 需求） |

## 二、关键优化点详解

### 优化 1：CopyIn 进度与 kIndex 解耦

**原版问题**：CopyInA/CopyInB 在每个 kIndex 都被调用，虽然内部有 `if (kModStepKa == 0)` 守卫，但函数调用开销和条件判断仍然存在。且搬运进度完全由 kIndex 决定，无法灵活调度。

**优化版改进**：
- 引入 `a1NextKBase`/`b1NextKBase` 独立跟踪 A/B 的 CopyIn 进度
- 引入 `a1CopyInIdx`/`b1CopyInIdx` 独立跟踪 A/B 的 L1 buffer 选择
- CopyIn 仅在真正需要时触发，不受 kIndex 硬绑定

**对应高阶 API**：高阶 API 的 `CubeInBuffer` 模块维护独立的缓存进度（`cacheProc_`、`cacheSize_`），CopyIn 触发由 `Hit()` 判断驱动，而非循环索引。

### 优化 2：4-flag 同步机制（Ping/Pong 独立 flag）

**原版问题**：A 矩阵只用 1 个 MTE2_MTE1 flag（flag 0），B 矩阵只用 1 个 MTE2_MTE1 flag（flag 1）。这意味着 Ping 和 Pong 共享同一个同步标志，无法精确表达"哪个 buffer 数据就绪"或"哪个 buffer 可覆盖"。

```
原版 flag 分配：
MTE2_MTE1 flag 0: A 矩阵数据就绪（不区分 Ping/Pong）
MTE2_MTE1 flag 1: B 矩阵数据就绪（不区分 Ping/Pong）
MTE1_MTE2 flag 0: A 矩阵可覆盖（不区分 Ping/Pong）
MTE1_MTE2 flag 1: B 矩阵可覆盖（不区分 Ping/Pong）
```

**优化版改进**：每个 L1 buffer（APing/APong/BPing/BPong）都有独立的 flag，共 4 对：

```
优化版 flag 分配：
MTE2_MTE1 flag 0: A1Ping 数据就绪    | MTE1_MTE2 flag 0: A1Ping 可覆盖
MTE2_MTE1 flag 1: A1Pong 数据就绪   | MTE1_MTE2 flag 1: A1Pong 可覆盖
MTE2_MTE1 flag 2: B1Ping 数据就绪    | MTE1_MTE2 flag 2: B1Ping 可覆盖
MTE2_MTE1 flag 3: B1Pong 数据就绪   | MTE1_MTE2 flag 3: B1Pong 可覆盖
```

这样实现了：
- DataLoad 读 APing 时，不会影响 APong 的 CopyIn 调度
- CopyIn 写 APong 时，不需要等 APing 被 DataLoad 读完
- 精确的 buffer 级同步，避免不必要的等待

**对应高阶 API**：高阶 API 的 `CubeInBufferDoubleBuffer` 为 Ping/Pong 分别维护 `cachePing_`/`cachePong_` 句柄和 `isCachingPing_`/`isCachingPong_` 状态，实现独立的 buffer 级管理。`Hit()` 通过 `bufferPos & cacheFactor_` 区分 Ping/Pong 位置。

### 优化 3：Preload 机制（搬运与计算流水重叠）

**原版问题**：CopyIn 在 DataLoad 之前执行，意味着必须等 CopyIn 完成后才能开始 DataLoad 和 Compute。MTE2 搬运和 Cube 计算是串行的。

```
原版时序（kIndex=0 为例）：
CopyInA(APing) → [等MTE2完成] → DataLoadA → CopyInB(BPing) → [等MTE2完成] → DataLoadB → Compute
```

**优化版改进**：参照高阶 API 的 `DoPreloadLoad` 机制，在大包开头 WaitFlag MTE2_MTE1（确认当前数据就绪）后，**立即异步发起下一个 L1 buffer 的搬运**。DataCopy 是异步指令，MTE2 搬运可以在后续的 DataLoad + Compute 期间与计算流水重叠。

```
优化版时序（kIndex=0 为例）：
预取: CopyInA(APing) + CopyInB(BPing)    ← K循环前
k=0: WaitFlag(APing) → Preload(APong+BPong) → DataLoad → Compute
k=4: WaitFlag(BPong) → ...                ← BPong已在k=0时预搬，大概率就绪
k=8: WaitFlag(APong) → Preload(...) → ...  ← APong已在k=0时预搬，大概率就绪
```

**关键收益**：MTE2 搬运 APong/BPong 的时间被 k=0~7 的 DataLoad+Compute 覆盖，当 k=4/k=8 需要 BPong/APong 时数据已就绪，消除了等待空白。

**对应高阶 API**：
- `scheduler_mdl_base.h` 的 `CopyIn()` 方法：CopyIn 当前大包后立即调用 `DoPreloadLoad()` 异步搬运下一批
- `DoPreloadLoad()` 支持 `PRELOAD_K` 模式：在 K 方向预加载下一个 stepKb 的 B 矩阵数据
- `DoPreloadAWait()` 在适当时机等待预加载完成

### 优化 4：Preload 放在 Compute 之后 + B 优先搬运

**原版问题**：CopyIn 在 DataLoad 之前执行，CopyIn 的 WaitFlag（MTE1_MTE2）会阻塞后续 DataLoad/Compute 指令的下发。且 A 先于 B 搬运，当 stepKa=8、stepKb=4 时，B 只需 4 轮就需要新数据，但必须等 A 搬完才能开始搬 B。

**优化版改进**：

**(a) Preload 放在 Compute 之后**：

将 Preload 从 DataLoad 之前移到 Compute 之后，使得每个 kIndex 迭代的指令发射顺序变为：

```
WaitFlag(M_MTE1) → WaitFlag(MTE2_MTE1) → DataLoadA → DataLoadB → SetFlag(MTE1_MTE2) → Compute → Preload
```

好处：
- DataLoad 和 Compute 指令优先下发，不被 Preload 的 WaitFlag(MTE1_MTE2) 阻塞
- Preload 的 DataCopy 指令在 Compute 之后异步执行，利用 Cube 计算期间的 MTE2 空闲

**(b) Preload 中先 B 后 A**：

当 stepKa=8、stepKb=4 时，B 矩阵每 4 个 baseK 就需要切换到下一个 buffer（BPong），而 A 矩阵需要 8 个 baseK 才切换。如果先搬 A，MTE2 总线会被 A 的搬运占用，B 的搬运要等 A 搬完才能开始，导致 k=4 时 BPong 数据未就绪。

改为先搬 B 后搬 A，MTE2 优先搬运更紧迫的 B 数据：

```
k=0: Compute → Preload(BPong) → Preload(APong)
                  ↑ MTE2先搬BPong     ↑ 再搬APong
k=4: 需要BPong → 已就绪✓
k=8: 需要APong → 已就绪✓（APong有8轮时间搬运）
```

**对应高阶 API**：高阶 API 的 `DoPreloadLoad()` 中 `PRELOAD_K` 模式专门针对 B 矩阵的 K 方向预加载，因为 B 矩阵的 stepKb 通常小于 stepKa，B 的数据需求更紧迫。高阶 API 会在 `cacheB1Factor_ == 1`（Double Buffer 启用）时优先预加载 B 矩阵。

### 优化 5：显式预取 + 独立搬运函数

**原版问题**：CopyInA/CopyInB 内部混合了搬运逻辑和同步逻辑，每次调用都有条件判断。首个 kIndex 的 CopyIn 和后续的 CopyIn 走同一套代码路径。

**优化版改进**：
- 将搬运逻辑拆分为纯函数 `CopyInABlock`/`CopyInBBlock`，只负责 DataCopy
- 同步逻辑（WaitFlag/SetFlag）由 Process() 统一管理
- 预取阶段独立处理，K 循环前只搬 APing 和 BPing（2 个 CopyIn），不预搬 Pong，避免指令队列堆积

## 三、流水线时序对比

### 原版时序（stepKa=8, stepKb=4）

```
k=0:  CopyInA(APing) → CopyInB(BPing) → DataLoad → Compute
k=1:  CopyInA(skip)  → CopyInB(skip)  → DataLoad → Compute
...
k=4:  CopyInA(skip)  → CopyInB(BPong) → DataLoad → Compute  ← 必须等BPong搬完
k=7:  CopyInA(skip)  → CopyInB(skip)  → DataLoad → Compute
k=8:  CopyInA(APong) → CopyInB(BPing) → DataLoad → Compute  ← 必须等APong搬完
```

**问题**：CopyIn 和 DataLoad 串行，每次大包开头都要等 MTE2 搬运完成。

### 优化版时序（stepKa=8, stepKb=4）

```
预取: CopyInA(APing) + CopyInB(BPing)

k=0:  WaitFlag(APing,BPing) → Preload(BPong,APong) → DataLoad → Compute
      [MTE2: 搬BPong................搬APong................]
k=1:  DataLoad → Compute
k=2:  DataLoad → Compute
k=3:  DataLoad → Compute
k=4:  WaitFlag(BPong已就绪✓) → DataLoad → Compute → Preload(BPing)
      [MTE2: ............搬BPing............]
k=7:  DataLoad → Compute → SetFlag(释放APing,BPong)
k=8:  WaitFlag(APong已就绪✓) → DataLoad → Compute → Preload(APing,BPong)
```

**收益**：MTE2 搬运与 Cube 计算流水重叠，大包开头 WaitFlag 时数据已就绪。

## 四、与高阶 API 的对应关系

| 优化版特性 | 高阶 API 对应机制 | 源文件 |
|-----------|------------------|--------|
| 独立 CopyIn 进度跟踪 | `CubeInBuffer::cacheProc_` | `cube_in_buffer_normal.h` |
| Ping/Pong 独立 flag | `CubeInBufferDoubleBuffer::cachePing_/cachePong_` | `cube_in_buffer_double_buffer.h` |
| 4-flag 精确同步 | `bufferPos & cacheFactor_` 区分 Ping/Pong | `cube_in_buffer_double_buffer.h` |
| Preload 机制 | `DoPreloadLoad()` | `scheduler_mdl_base.h` |
| B 优先搬运 | `PRELOAD_K` 模式 | `scheduler_mdl_base.h` |
| Hit 判断（大包内跳过搬运） | `CubeInBuffer::Hit()` | `cube_in_buffer_intf.h` |
| 大包搬运 | `stepKa/stepKb` tiling 参数 | `copy_cube_in_norm.h` |
| Compute 后 Preload | `CopyIn → DoPreloadLoad → 计算循环 → AwaitLoadData` | `scheduler_mdl_base.h` |

## 五、性能提升总结

| 优化项 | 提升原因 | 预估影响 |
|--------|---------|---------|
| Preload 流水重叠 | MTE2 搬运与 Cube 计算并行，消除大包开头等待 | **高** |
| B 优先搬运 | stepKb < stepKa 时 B 数据更紧迫，优先保证 B 就绪 | **高** |
| Compute 后 Preload | 避免Preload 的 WaitFlag 阻塞 DataLoad/Compute 指令下发 | **中** |
| 4-flag 精确同步 | Ping/Pong 独立同步，避免不必要的串行等待 | **中** |
| 进度解耦 | CopyIn 不绑定 kIndex，灵活调度 | **低**（代码清晰度提升） |

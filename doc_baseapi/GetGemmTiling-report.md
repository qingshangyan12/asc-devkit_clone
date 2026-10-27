# GetGemmTiling 接口跨版本适配报告

> 适配对象：`asc-devkit/include/basic_api/kernel_operator_gemm_intf.h:39` — `template <typename T> __aicore__ inline GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n)`
> 适配依据：用户确认方案（3103 buffer 常量补充；3103/3113 L0A/L0B 修正为 32K；下游 Gemm 问题留待后续；5101 维持现状）

## 适配后支持矩阵

| 版本 | Arch | GetGemmTiling 实现 | TOTAL_L0A/L0B 常量 | tiling k0 计算正确性 | 3103 专用常量 (UB/L1/L0C/ONE_BLK) | 下游 Gemm（V1 LoadData2D / Mmad） |
|------|------|-------------------|--------------------|--------------------|--------------------------------|----------------------------------|
| dav_l300 | 3003 | 共享层完整（**维持**） | 64KB（**维持**） | 正确（**维持**） | — | LoadData2D 完整 + Mmad 完整（**维持**） |
| dav_l310 | 3103 | 共享层完整（**维持**） | ~~64KB（错误）~~ → **32KB（修正）** | ~~高估一倍（kTileBlock 过大→L0A/B 越界）~~ → **正确** | ~~未定义（Gemm CPU debug 编译失败）~~ → **已定义**（UB/TMP_UB_OFFSET 128KB、L1 512KB、L0C 32KB、ONE_BLOCK_SIZE 32） | LoadData2D V1 桩 + Mmad 完整（**维持**，留待后续分析） |
| dav_l311 | 3113 | 共享层完整（**维持**） | ~~64KB（错误）~~ → **32KB（修正）** | ~~高估一倍~~ → **正确** | —（3113 分支已有，**维持**） | LoadData2D V1 桩 + Mmad 完整（**维持**，留待后续分析） |
| dav_5161 | 5101/5161/5165/5163 | 共享层完整（**维持**） | 64KB（**维持**） | 维持（下游不可用，tiling 为死值） | —（5101/5161 分支已有，**维持**） | 全桩（**维持**，按方案等待后续分析，不禁用） |
| dav_3510（参照） | 3510 | 共享层完整（**维持**） | 64KB（**维持**） | 正确（**维持**） | — | LoadData2D 完整 + Mmad 完整（**维持**） |

**适配效果说明（3103/3113 L0A/L0B 修正前后的行为差异）：**

修正前（64K 假设）在 3103/3113 上的连锁错误：
1. `GetGemmTiling` 用 64K 计算 `k0a = TOTAL_L0A_SIZE/2/(roundM*dSize)` → `kTileBlock` **偏大一倍**
2. `GetPingPongBuffer`（gemm_base_impl.h）按 `TOTAL_L0A_SIZE/2 = 32K` 划分 ping/pong → pong 区间（32K~64K）**超出真实 32K 硬件** → L0A/B 越界写
3. `GemmExecNm/Mn` 的容量判断（`needL0Asize > TOTAL_L0A_SIZE`）同样基于错误的 64K，无法拦截上述越界

修正后（32K）：k0/kTileBlock 减半、ping/pong 各 16K、容量判断阈值正确，全链路与硬件一致。

3103 常量补充的修复效果：`Gemm` 的 CPU debug 校验 `CheckParams/CheckOverflow`（gemm_base_impl.h:76/85 引用 `TOTAL_L0C_SIZE`/`TOTAL_L1_SIZE`）此前在 3103 下**无法编译**，现可编译。

## 修改的文件清单

| 文件 | 修改点 | 行号 | 类型 |
|------|--------|------|------|
| `asc-devkit/impl/basic_api/utils/kernel_utils_constants.h` | ① `TOTAL_L0A_SIZE`/`TOTAL_L0B_SIZE` 增加 3103/3113 守卫（32KB），其余版本维持 64KB | L56-62 | 修改 |
| 同上 | ② 新增 3103 常量分支（`TOTAL_UB_SIZE`/`TMP_UB_OFFSET`/`TOTAL_L1_SIZE`/`TOTAL_L0C_SIZE`/`ONE_BLOCK_SIZE`），插入于 3102 与 3510 分支之间 | L177-182 | 新增 |

- `GetGemmTiling`（intf_impl.h）、`CalculateGemmTiling`（gemm_base_impl.h）、`GemmTiling` 结构体（kernel_struct_conv2d.h）：**均未修改** —— 它们通过宏常量自动获得正确行为
- 无需从 dav_3510 移植任何函数代码：`GetGemmTiling` 全版本共享同一实现（analysis 报告已确认无 dav 层分发）
- 本次未新增任何桩函数（不涉及）；5101 分支未做禁用（按方案维持现状）

**修改严格对齐确认方案，未自行假设的项：**
- 3103 分支包含方案指定的 4 个常量（UB 128*1024、L1 512*1024、L0C 32*1024、ONE_BLOCK_SIZE 32）及用户补充的 `TMP_UB_OFFSET = 128*1024`（参照 3113 写法，与 `TOTAL_UB_SIZE` 同值），共 5 个；`TOTAL_VEC_LOCAL_SIZE`、`VECTOR_REG_WIDTH`、`SINGLE_MSG_SIZE`、`CACHE_LINE_SIZE` 方案未指定，**未添加**（引用这些常量的代码在 3103 下仍无法编译，属遗留问题）
- L0A/L0B 的 32K 修正仅覆盖方案指定的 3103 与 3113；3003、5101/5161/5165/5163、3510 等其余版本维持 64K

**验证结果：**
1. 两处修改后的代码已复读核对：`#if/#else/#endif` 配对正确，3103 分支位于 3102（L168-176）与 3510（L182）之间，分支链完整
2. `TOTAL_L0A_SIZE`/`TOTAL_L0B_SIZE` 为 kernel_utils_constants.h 单点定义（namespace 级 const），修改前后结构等价，无 ODR/重定义风险
3. 引用方核查：`GetGemmTiling`（intf_impl.h:53-54）、`GetPingPongBuffer`/`GetSingleThreadBuffer`（gemm_base_impl.h:278-311）均为**引用**（非重定义），在 3103/3113 下自动读到 32KB
4. 交叉影响核查：`TPipe/TBuf/TBufPool` 的容量校验（kernel_tpipe_impl.h:353-361、kernel_tbufpool_impl.h、kernel_tbuf_impl.h）不引用 `TOTAL_L0A_SIZE`（CPU debug 用 `ConstDefiner::bufferInitLen` 表），与本次修改无交叉影响

## 适配后各版本原始代码

### 共享层 — GetGemmTiling（**未修改**，全版本生效，intf_impl.h:30-71）

```cpp
// T should be left matrix dtype
template <typename T>
[[deprecated("NOTICE: GetGemmTiling has been deprecated and will be removed in the next version. "
             "Please do not use it!")]]
__aicore__ inline GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n)
{
    uint32_t c0 = 0;
    uint32_t dSize = 1;
    if (Std::is_same<T, uint8_t>::value || Std::is_same<T, int8_t>::value) {
        c0 = 32;
        dSize = 1;
    } else {
        c0 = 16;
        dSize = 2;
    }
    GemmTiling tiling;
    tiling.c0Size = c0;
    tiling.dtypeSize = dSize;
    tiling.mNum = m;
    tiling.nNum = n;
    tiling.kNum = k;
    tiling.roundM = DivCeil(m, tiling.blockSize) * tiling.blockSize; // blockSize = 16 * 16
    tiling.roundN = DivCeil(n, tiling.blockSize) * tiling.blockSize;
    tiling.roundK = DivCeil(k, tiling.c0Size) * tiling.c0Size; // c0Size = 16 || c0Size = 32
    uint32_t k0a = TOTAL_L0A_SIZE / 2 / (tiling.roundM * dSize);   // ← 3103/3113 下现为 32KB
    uint32_t k0b = TOTAL_L0B_SIZE / 2 / (tiling.roundN * dSize);   // ← 3103/3113 下现为 32KB
    uint32_t k0 = k0a > k0b ? k0b : k0a;
    k0 = k0 > k ? k : k0;

    tiling.kTileBlock = k0 / tiling.c0Size;
    if (tiling.kTileBlock == 0) {
        tiling.kTileBlock = 1;
    }
    tiling.loopMode = LoopMode::MODE_NM;

    tiling.mBlockNum = DivCeil(m, tiling.blockSize);
    tiling.nBlockNum = DivCeil(n, tiling.blockSize);
    tiling.kBlockNum = DivCeil(k, tiling.c0Size);

    CalculateGemmTiling(tiling);

    return tiling;
}
```

### 共享层 — CalculateGemmTiling（**未修改**，gemm_base_impl.h:141-164）

```cpp
__aicore__ inline void CalculateGemmTiling(GemmTiling& tiling)
{
    tiling.mIterNum = 1;
    tiling.nIterNum = 1;
    tiling.kIterNum = DivCeil(tiling.kBlockNum, tiling.kTileBlock);

    tiling.mTileBlock = DivCeil(tiling.mBlockNum, tiling.mIterNum);
    tiling.nTileBlock = DivCeil(tiling.nBlockNum, tiling.nIterNum);

    tiling.kTailBlock = tiling.kBlockNum - (tiling.kIterNum - 1) * tiling.kTileBlock;
    tiling.mTailBlock = tiling.mBlockNum - (tiling.mIterNum - 1) * tiling.mTileBlock; // mTailBlock <= mBlockNum
    tiling.nTailBlock = tiling.nBlockNum - (tiling.nIterNum - 1) * tiling.nTileBlock;

    tiling.kHasTail = tiling.kTailBlock != tiling.kTileBlock;
    tiling.kHasTailEle = tiling.roundK != tiling.kNum;
    tiling.kTailEle = tiling.kNum % (tiling.kTileBlock * tiling.c0Size);

    if (tiling.mNum != tiling.mTileBlock * tiling.blockSize) {
        tiling.mHasTail = true;
    } else {
        tiling.mHasTail = false;
    }
    tiling.nHasTail = tiling.nTileBlock != tiling.nTailBlock;
}
```

### 共享层 — L0A/L0B 常量（**已修改**，kernel_utils_constants.h:56-62）

修改前：

```cpp
const uint32_t TOTAL_L0A_SIZE = 64 * 1024;
const uint32_t TOTAL_L0B_SIZE = 64 * 1024;
```

修改后：

```cpp
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113))
const uint32_t TOTAL_L0A_SIZE = 32 * 1024;
const uint32_t TOTAL_L0B_SIZE = 32 * 1024;
#else
const uint32_t TOTAL_L0A_SIZE = 64 * 1024;
const uint32_t TOTAL_L0B_SIZE = 64 * 1024;
#endif
```

> `__NPU_ARCH__` 未定义时（host 侧等）落入 `#else`，维持原 64KB 行为。

---

### dav_l310（目标版本，3103）— 常量分支（**新增**，kernel_utils_constants.h:177-182）

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3103)
const uint32_t TOTAL_UB_SIZE = 128 * 1024;
const uint32_t TMP_UB_OFFSET = 128 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t TOTAL_L0C_SIZE = 32 * 1024;
const uint32_t ONE_BLOCK_SIZE = 32;
```

> 按确认方案的 4 个常量 + 用户补充的 `TMP_UB_OFFSET`（参照 3113 写法，与 `TOTAL_UB_SIZE` 同值，字段顺序一致）；`TOTAL_VEC_LOCAL_SIZE`/`VECTOR_REG_WIDTH`/`SINGLE_MSG_SIZE`/`CACHE_LINE_SIZE` 未指定故未添加（遗留）。
> 下游 Gemm 依赖（mm_impl.h）：V1 `LoadData2DL12L0ACal/L0BCal` 桩（L285-295）、V2 完整（L53-125）、MmadCal V1/V2 完整（L426-467）——**均维持不变**，留待后续分析。

### dav_l311（目标版本，3113）— 常量分支（**维持**，kernel_utils_constants.h:193-200）

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3113)
const int32_t TOTAL_VEC_LOCAL_SIZE = 118 * 1024;
const uint32_t TOTAL_UB_SIZE = 118 * 1024;
const uint32_t TMP_UB_OFFSET = 118 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t TOTAL_L0C_SIZE = 64 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t ONE_BLOCK_SIZE = 32;
```

> 本分支未修改；3113 的 L0A/L0B 修正通过上方共享常量的 `#if` 守卫生效（64KB → 32KB）。
> 下游 Gemm 依赖（mm_impl.h）：V1 桩（L310-320）、MmadCal 完整 —— **维持不变**。

### dav_l300（目标版本，3003）— 常量分支（**维持**，kernel_utils_constants.h:160-167）

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3003)
const int32_t TOTAL_VEC_LOCAL_SIZE = 118 * 1024;
const uint32_t TOTAL_UB_SIZE = 118 * 1024;
const uint32_t TMP_UB_OFFSET = 118 * 1024;
const uint32_t TOTAL_L1_SIZE = 1024 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t ONE_BLOCK_SIZE = 32;
const uint32_t TOTAL_L0C_SIZE = 128 * 1024;
```

> L0A/L0B 维持 64KB（走 `#else`）。下游 Gemm 依赖（mm_impl.h）：V1 `LoadData2DL12L0ACal/L0BCal` 完整（L62-80/L112-130）、MmadCal 完整 —— **维持不变**。

### dav_5161（目标版本，5101/5161/5165/5163）— 常量分支（**维持**，kernel_utils_constants.h:201-224）

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
const int32_t TOTAL_VEC_LOCAL_SIZE = 120 * 1024;
const uint32_t TOTAL_UB_SIZE = 120 * 1024;
const uint32_t TMP_UB_OFFSET = 120 * 1024;
const uint32_t TOTAL_L1_SIZE = 1536 * 1024;
const uint32_t TOTAL_L0C_SIZE = 0;
const uint32_t VECTOR_REG_WIDTH = 64;
const uint32_t ONE_BLOCK_SIZE = 32;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161)
const int32_t TOTAL_VEC_LOCAL_SIZE = 120 * 1024;
const uint32_t TOTAL_UB_SIZE = 120 * 1024;
const uint32_t TMP_UB_OFFSET = 120 * 1024;
const uint32_t TOTAL_L1_SIZE = 1280 * 1024;
const uint32_t TOTAL_L0C_SIZE = 0;
const uint32_t VECTOR_REG_WIDTH = 64;
const uint32_t ONE_BLOCK_SIZE = 32;
```

> 按方案**维持现状，未禁用**：`GetGemmTiling` 在 5101/5161/5165/5163 下继续可编译运行（tiling 为死值的问题、下游全桩问题等待用户后续分析结果）。

### dav_3510（参照版本）— 常量分支（**维持**，kernel_utils_constants.h:182-192）

```cpp
#elif (__NPU_ARCH__ == 3510)
const int32_t TOTAL_VEC_LOCAL_SIZE = 248 * 1024;
const uint32_t TOTAL_UB_SIZE = 248 * 1024;
const uint32_t TMP_UB_OFFSET = 248 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t SINGLE_MSG_SIZE = 64;
const uint32_t CACHE_LINE_SIZE = 64;
const uint32_t TOTAL_L0C_SIZE = 256 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t VECTOR_REG_WIDTH_2XVL = 512;
const uint32_t ONE_BLOCK_SIZE = 32;
```

> L0A/L0B 维持 64KB（走 `#else`）。全链路维持不变。

### 共享层 — Gemm 下游消费者（**均未修改**，随常量自动适配）

`GetPingPongBuffer` / `GetSingleThreadBuffer` / `GemmExecNm` / `GemmExecMn`（kernel_operator_gemm_base_impl.h:270-313 / 418-430 / 531-543）引用 `TOTAL_L0A_SIZE`/`TOTAL_L0B_SIZE`，在 3103/3113 下自动按 32KB 工作（ping/pong 各 16KB；容量判断阈值同步收紧），无需改动：

```cpp
template <typename T, typename U>
__aicore__ inline void GetPingPongBuffer(
    LocalTensor<T>& l0aPing, LocalTensor<T>& l0aPong, LocalTensor<U>& l0bPing, LocalTensor<U>& l0bPong)
{
    // L0Abuffer
    TBuffAddr tbufaPing;
    tbufaPing.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0aPing.SetAddr(tbufaPing);
    l0aPing.InitBuffer(0, TOTAL_L0A_SIZE / 2 / sizeof(PrimT<T>));      // 3103/3113 下 = 16KB

    TBuffAddr tbufaPong;
    tbufaPong.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0aPong.SetAddr(tbufaPong);
    l0aPong.InitBuffer(TOTAL_L0A_SIZE / 2, TOTAL_L0A_SIZE / 2 / sizeof(PrimT<T>));  // pong 起点 16KB

    // L0Bbuffer
    TBuffAddr tbufbPing;
    tbufbPing.logicPos = static_cast<uint8_t>(TPosition::B2);
    l0bPing.SetAddr(tbufbPing);
    l0bPing.InitBuffer(0, TOTAL_L0B_SIZE / 2 / sizeof(PrimT<U>));

    TBuffAddr tbufbPong;
    tbufbPong.logicPos = static_cast<uint8_t>(TPosition::B2);
    l0bPong.SetAddr(tbufbPong);
    l0bPong.InitBuffer(TOTAL_L0B_SIZE / 2, TOTAL_L0B_SIZE / 2 / sizeof(PrimT<U>));
    return;
}
```

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNm(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t needL0Asize = tiling.roundM * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    uint32_t needL0Bsize = tiling.roundN * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    if (needL0Asize > TOTAL_L0A_SIZE || needL0Bsize > TOTAL_L0B_SIZE) {
        GemmExecNmNopingpong(l0c, src0, src1, tiling, initValue);
        return;
    }
    GemmExecNmPingPong(l0c, src0, src1, tiling, initValue);
}
```

## 遗留问题（不在本次方案范围，待后续分析）

1. **3103 分支常量不全**：`TOTAL_VEC_LOCAL_SIZE`/`VECTOR_REG_WIDTH`/`SINGLE_MSG_SIZE`/`CACHE_LINE_SIZE` 仍未定义（方案未指定；`TMP_UB_OFFSET` 已按用户指示补充）——引用它们的代码（vec 系、UB 池管理等）在 3103 下无法编译，待用户补充数值后完善
2. **下游 Gemm**（按用户方案留待后续）：3103/3113 的 V1 `LoadData2DL12L0A/L0BCal` 桩（硬件具备 L1→L0A/B 能力，V2/BitMode 完整，疑似待补全）；Gemm dst=UB 的 l0c 未初始化问题；Gemm `partialsum` 参数未使用
3. **5101/5161/5165/5163**：GetGemmTiling 维持现状（可编译、tiling 为死值），是否禁用等待后续分析结果
4. **deprecated 标记不对称**（intf.h:39 无属性 vs intf_impl.h:31 有）：本次未动（不属方案范围）

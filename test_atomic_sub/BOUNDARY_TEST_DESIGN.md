# asc_atomic_sub (half / bfloat16_t) 边界值测试设计

本文档给出边界值（零、次正规数、溢出、inf、NaN）测试用例的**实现方案**，供后续实现与执行。

被测接口：`asc_atomic_sub(__gm__ T*, T)` 与 `asc_atomic_sub(__ubuf__ T*, T)`，`T ∈ {half, bfloat16_t}`。

实现均为 `asc_atomic_add(address, -val)`，因此测试同时覆盖"取负 + 累加"这条路径。

---

## 1. 三条必须遵守的实现约束

这些约束来自 `TEST_REPORT.md` 记录的踩坑，违反其中任何一条都会得到**看起来合理但实际无意义**的测试结果。

### 1.1 禁止 host 侧构造 bf16 值

host 编译下 `bfloat16_t` 是不透明 proxy 结构体，`(bfloat16_t)1.0f` **不做任何转换**，恒为 `0x0000`。

**方案**：所有 bf16/half 的目标初值与操作数，一律在 host 侧以 `uint16_t` **原始位模式**给出（本文档第 4 节已算好），经 GM 传入 device，device 侧再做位重解释。不在测试代码里做任何 host 侧浮点→半精度转换。

### 1.2 device 侧用位重解释，不用数值转换

```cpp
uint16_t bits = raw[i];
bfloat16_t v = (bfloat16_t&)bits;   // 位重解释，无数值转换
half      h = (half&)bits;
```

不要用 `__float2bfloat16` / `__float2half` 构造边界值：`inf`/`NaN`/次正规数经 float 中转可能被改写，且 NaN payload 不可控。

### 1.3 校验必须比对原始位，不比对 float

回读一律读 `uint16_t` 原始位：

```cpp
out[i] = ((__gm__ uint16_t*)target)[0];
```

理由：
- `+0.0` 与 `-0.0` 的 float 比较相等，只有位模式能区分（用例 B4）；
- `NaN` 与任何值比较都为 false，`==` 判等会把正确结果误判为失败；
- 位比对能顺带发现"只错高/低若干位"这类问题。

---

## 2. 测试骨架

### 2.1 数据布局

单块 GM 缓冲承载三段，全部 `uint16_t`：

| 段 | 用途 | 建议偏移（uint16_t 索引） |
|----|------|--------------------------|
| `operand[]` | 每个用例的 `val` 位模式 | `0 + i` |
| `target[]` | 每个用例的目标初值，**stride = 4**（即 8 字节） | `64 + 4*i` |
| `result[]` | device 回写：操作后目标原始位 / 返回值原始位 / 邻位 | `256 + 4*i` |

`target` 用 stride 4 而非紧凑排布，出于两点：
- 每个用例必须有**独立目标地址**——原子操作会修改内存，用例之间共用地址会互相污染；
- 目标两侧留出的空位可用于用例 B8（邻位不受损校验）。

### 2.2 kernel 结构

沿用现有测试的混编形态（`__simt_vf__` + `asc_vf_call` + 3-slot launch），4 个独立 `__global__` kernel：

| kernel | 覆盖 |
|--------|------|
| `k_gm_bf16` | GM + bfloat16_t |
| `k_gm_half` | GM + half |
| `k_ub_bf16` | UB + bfloat16_t |
| `k_ub_half` | UB + half |

拆开的原因与现有测试一致：避免 GM/UB 混跑相互影响，且任一组合失败时定位范围明确。

**边界用例一律用单线程（`dim3(1)`，且 `if (threadIdx.x == 0)` 保护）。** 边界语义要的是确定性结果；多线程竞争下浮点累减顺序不定（接口约束说明已明确），会让期望值不唯一。多线程竞争由现有 256 线程用例覆盖，两者职责分离。

### 2.3 UB 初始化

UB 无法从 host 直接 memcpy，需 device 侧从 GM 搬原始位：

```cpp
__simt_vf__ __launch_bounds__(1) inline void vf_ub_init(__ubuf__ uint16_t* ub, __gm__ uint16_t* raw)
{
    if (threadIdx.x == 0) {
        for (int32_t i = 0; i < CASE_NUM; i++) {
            ub[4 * i] = raw[TARGET_OFF + 4 * i];      // 原始位搬运，不经浮点
            ub[4 * i + 1] = NEIGHBOR_MAGIC;           // 邻位哨兵，供 B8 校验
        }
    }
}
```

动态 UB 大小：`CASE_NUM × 8` 字节，取 `DYN_UB_SIZE = 128` 足够。

### 2.4 单个用例的执行体

```cpp
__simt_vf__ __launch_bounds__(1) inline void vf_gm_bf16(__gm__ uint16_t* raw, __gm__ uint16_t* res)
{
    if (threadIdx.x != 0) {
        return;
    }
    for (int32_t i = 0; i < CASE_NUM; i++) {
        uint16_t vb = raw[i];
        bfloat16_t val = (bfloat16_t&)vb;
        __gm__ bfloat16_t* tgt = (__gm__ bfloat16_t*)(raw + TARGET_OFF + 4 * i);

        bfloat16_t ret = asc_atomic_sub(tgt, val);

        res[4 * i]     = ((__gm__ uint16_t*)tgt)[0];    // 操作后内存位
        res[4 * i + 1] = (uint16_t&)ret;                // 返回值位
        res[4 * i + 2] = ((__gm__ uint16_t*)tgt)[1];    // 邻位哨兵
    }
}
```

---

## 3. host 侧校验逻辑

### 3.1 期望值的三种匹配模式

不能一律用位精确相等，需按用例分类：

| 模式 | 判定 | 适用 |
|------|------|------|
| `EXACT` | 位模式完全相等 | 有唯一确定结果的用例（含 ±0 区分） |
| `IS_NAN` | 指数位全 1 **且** 尾数非 0 | 结果为 NaN 的用例 |
| `IS_INF` | 指数位全 1 **且** 尾数为 0，符号位需匹配 | 结果为 ±inf 的用例 |

`IS_NAN` 不用位精确比较，原因有二：NaN payload 由硬件决定，规范未约束；且实现走 `add(-val)`，取负会翻转 NaN 符号位。把 payload 或符号写进期望值会造成虚假失败。

判定辅助函数（host 侧，按类型区分指数/尾数宽度）：

```cpp
// bf16: 1-8-7   half: 1-5-10
bool is_nan_bits(uint16_t b, bool is_bf16)
{
    uint16_t exp_mask  = is_bf16 ? 0x7F80 : 0x7C00;
    uint16_t mant_mask = is_bf16 ? 0x007F : 0x03FF;
    return ((b & exp_mask) == exp_mask) && ((b & mant_mask) != 0);
}

bool is_inf_bits(uint16_t b, bool is_bf16, bool negative)
{
    uint16_t exp_mask  = is_bf16 ? 0x7F80 : 0x7C00;
    uint16_t mant_mask = is_bf16 ? 0x007F : 0x03FF;
    uint16_t sign = negative ? 0x8000 : 0x0000;
    return ((b & exp_mask) == exp_mask) && ((b & mant_mask) == 0) && ((b & 0x8000) == sign);
}
```

### 3.2 自检（必须有）

host 侧位模式常量若写错，测试同样会给出自洽但错误的结论。因此在跑 kernel 前先自检：把每个 bf16/half 位模式**用位运算解回 float** 并与预期数值比对，不一致直接 `FATAL` 退出。

bf16 解码即左移 16 位；half 需按 1-5-10 手工解码，或复用已有的 `bf16_bits_to_float` 同类写法。这一步就是 `TEST_REPORT.md` 修正方案里加的那道防线，边界测试同样需要。

---

## 4. 用例表与期望值

期望值由 IEEE-754 语义推导，并已在 dav-3510 上以探针程序核对过基线行为。

`SUB(t, v)` 表示 `asc_atomic_sub(&target, val)`，`target` 初值位模式 `t`，操作数位模式 `v`。

### 4.1 bfloat16_t（1-8-7，1000.0 处 ULP = 4）

| # | 用例 | target | val | 期望内存位 | 匹配模式 | 考察点 |
|---|------|--------|-----|-----------|----------|--------|
| B1 | 基线 | `0x447A` (1000) | `0x4100` (8) | `0x4478` (992) | EXACT | 正常路径，作为对照 |
| B2 | 自减归零 | `0x447A` (1000) | `0x447A` (1000) | `0x0000` (+0) | EXACT | 结果为 +0 而非 -0 |
| B3 | 低于 ULP 被吸收 | `0x447A` (1000) | `0x3080` (2⁻³⁰) | `0x447A` (1000) | EXACT | 舍入吸收，值不变 |
| B4 | 负零减正零 | `0x8000` (-0) | `0x0000` (+0) | `0x8000` (-0) | EXACT | **符号零保持**，float 比较无法区分 |
| B5 | 溢出至 -inf | `0xFF7F` (-max) | `0x7F7F` (+max) | `0xFF80` | IS_INF(neg) | 有限值相减溢出格式范围 |
| B6 | 有限减 +inf | `0x447A` (1000) | `0x7F80` (+inf) | `0xFF80` | IS_INF(neg) | inf 操作数传播 |
| B7 | +inf 减有限 | `0x7F80` (+inf) | `0x4100` (8) | `0x7F80` | IS_INF(pos) | inf 目标保持 |
| B8 | inf 减 inf | `0x7F80` (+inf) | `0x7F80` (+inf) | — | IS_NAN | 未定义式产生 NaN |
| B9 | NaN 目标 | `0x7FC0` (qNaN) | `0x4100` (8) | — | IS_NAN | NaN 目标传播 |
| B10 | NaN 操作数 | `0x447A` (1000) | `0x7FC0` (qNaN) | — | IS_NAN | NaN 经取负后仍传播 |
| B11 | 最小正规数自减 | `0x0080` (2⁻¹²⁶) | `0x0080` | `0x0000` (+0) | EXACT | 正规数下边界 |

### 4.2 half（1-5-10）

| # | 用例 | target | val | 期望内存位 | 匹配模式 | 考察点 |
|---|------|--------|-----|-----------|----------|--------|
| H1 | 基线 | `0x63D0` (1000) | `0x4800` (8) | `0x63C0` (992) | EXACT | 正常路径 |
| H2 | 自减归零 | `0x63D0` (1000) | `0x63D0` (1000) | `0x0000` (+0) | EXACT | 结果为 +0 |
| H3 | 次正规操作数被吸收 | `0x63D0` (1000) | `0x0001` (2⁻²⁴ 次正规) | `0x63D0` (1000) | EXACT | **次正规数不被当作 0 或异常处理** |
| H4 | 负零减正零 | `0x8000` (-0) | `0x0000` (+0) | `0x8000` (-0) | EXACT | 符号零保持 |
| H5 | 溢出至 -inf | `0xFBFF` (-65504) | `0x7BFF` (65504) | `0xFC00` | IS_INF(neg) | half 动态范围窄，溢出易触发 |
| H6 | 有限减 +inf | `0x63D0` (1000) | `0x7C00` (+inf) | `0xFC00` | IS_INF(neg) | inf 传播 |
| H7 | +inf 减有限 | `0x7C00` (+inf) | `0x4800` (8) | `0x7C00` | IS_INF(pos) | inf 保持 |
| H8 | inf 减 inf | `0x7C00` | `0x7C00` | — | IS_NAN | 未定义式 |
| H9 | NaN 目标 | `0x7E00` (qNaN) | `0x4800` (8) | — | IS_NAN | NaN 传播 |
| H10 | NaN 操作数 | `0x63D0` (1000) | `0x7E00` (qNaN) | — | IS_NAN | NaN 经取负后传播 |
| H11 | 最小正规数自减 | `0x0400` (2⁻¹⁴) | `0x0400` | `0x0000` (+0) | EXACT | 正规数下边界 |

### 4.3 附加校验（贯穿所有用例，无需单列用例）

**邻位不受损**：`half`/`bfloat16_t` 为 2 字节，原子操作不应影响相邻半字。每个 target 的 `+1` 位置预置哨兵 `0xA5A5`，操作后校验其仍为 `0xA5A5`。这一项值得查，因为现有测试的 GM bf16 初始化按 4 字节写入，容易掩盖 2 字节原子越界写的问题。

**返回值语义**：每个用例同时记录返回值位模式。

> 探针实测发现：GM 上 `asc_atomic_sub(1000.0, 8.0)` 的返回值为 `992.0`（**新值**），而 `asc_atomic_sub.md` 的"返回值说明"写的是"Unified Buffer或Global Memory上的**初始数据**"（旧值）。half 与 bf16 均如此。
>
> 这与实现一致：`asc_atomic_add` 的 half/bf16 单类型重载体为 `atomicAdd(address, val); return *address;`，返回的是回读值而非旧值。
>
> 因此**返回值不作为断言项**，仅记录并打印，交由接口负责人确认是文档需更正还是实现需更正。测试代码里为返回值预留输出位，但不判定通过/失败——在语义未定论前断言任一侧都会产生误导性结论。

---

## 5. 输出格式建议

逐用例单行，同时给出位模式与解码值，便于失败时直接定位：

```
[ PASS ] B1  GM bf16 baseline        target=0x447A val=0x4100 -> got=0x4478 (992.00) expect=0x4478 EXACT     neighbor=OK ret=0x4478
[ PASS ] B8  GM bf16 inf-inf         target=0x7F80 val=0x7F80 -> got=0x7FC0 (nan)    expect=IS_NAN          neighbor=OK ret=0x7FC0
[ FAIL ] B4  GM bf16 -0 minus +0     target=0x8000 val=0x0000 -> got=0x0000 (0.00)   expect=0x8000 EXACT     neighbor=OK ret=0x0000
```

末尾按 4 个组合分别汇总，再给总计。

---

## 6. 实现清单

1. 新建 `test_atomic_sub_boundary.asc`，CMakeLists 参照现有 `test_atomic_sub`（`--npu-arch=dav-3510`，混编，不加 `--enable-simt`）。
2. 定义用例表：host 侧 `static const` 数组，字段为 `{name, target_bits, val_bits, expect_bits, match_mode}`，bf16 与 half 各一张。
3. 实现 3.2 的位模式自检，不通过即退出。
4. 实现 4 个 kernel（GM/UB × bf16/half），每个内部循环遍历用例表，单线程执行。
5. 实现 3.1 的三种匹配模式判定 + 邻位哨兵校验。
6. 按第 5 节格式输出，`main` 返回值反映整体通过与否。
7. 记录返回值位模式但不参与判定，输出中标注为待确认项。

## 7. 预期结论形态

半精度浮点原子减的边界行为若与 IEEE-754 一致，B1–B11 与 H1–H11 应全部通过。若某项不符，需按下列顺序自查，再判定为接口/底层问题：

1. 位模式常量是否通过自检（排除测试数据错误）；
2. 是否误用了 host 侧类型转换（排除 1.1 类问题）；
3. 是否用 float 比较代替位比较（排除 ±0、NaN 误判）；
4. UB 初始化是否确实搬入了预期原始位（操作前回读一次确认）。

前三步对应的都是曾经真实发生过的测试缺陷，优先排除它们再下结论。

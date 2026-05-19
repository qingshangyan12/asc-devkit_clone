# C API 参数名一致性检查报告

## 检查范围

- **文档目录**: `docs/api/c_api/`
- **对外接口声明目录**: `include/c_api/`
- **实现目录**: `impl/c_api/instr_impl/npu_arch_2201/`、`impl/c_api/instr_impl/npu_arch_3510/`

---

## 关键术语定义

| 术语 | 定义 |
|:---|:---|
| **gap** | 上一个数据块的**结束地址**与下一个数据块的**起始地址**之间的距离（即：尾到头的间隔，不含数据块自身大小） |
| **stride** | 上一个数据块的**起始地址**与下一个数据块的**起始地址**之间的距离（即：头到头的间隔，包含数据块自身大小） |

---

## 一、gap/stride 参数名语义误用

### 问题 1: `asc_copy_l12l0a_trans` — `dst_stride` / `dst_frac_stride` 名为 stride 实际为 gap 语义

- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md`
  - **第 95 行**: 参数名 `dst_stride`，描述为"目的操作数前一个迭代第一个分形的**结束地址**到下一个迭代第一个分形起始地址的间隔" → **这是 GAP 定义**
  - **第 97 行**: 参数名 `dst_frac_stride`，描述为"每个迭代内目的操作数转置前一个分形**结束地址**与后一个分形起始地址的间隔" → **这是 GAP 定义**
  - **第 22-23 行**: 示例讲解中使用 `dst_gap`、`dst_frac_gap` 但参数名声明为 `dst_stride`、`dst_frac_stride`
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:631` — 参数名为 `dst_stride`、`dst_frac_stride`
- **对比**: 在 `asc_copy_l12l0b_trans_arch_2201.md:93` 中，完全相同的描述对应参数名 `dst_gap`（命名正确）
- **判定**: **参数应命名为 `dst_gap` / `dst_frac_gap` 或描述应改为 stride 定义**

### 问题 2: `asc_copy_l12fb_v2` — `src_stride` / `dst_stride` 名为 stride 实际为 gap 语义

- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12fb_v2.md`
  - **第 41 行**: 参数名 `src_stride`，描述为"前面一个数据块的**尾**与后面一个数据块的头**的间隔**" → **这是 GAP 定义**
  - **第 42 行**: 参数名 `dst_stride`，描述为"前面一个数据块的**尾**与后面一个数据块的头**的间隔**" → **这是 GAP 定义**
- **Header**: `include/c_api/cube_datamove/cube_datamove.h:1888` — 参数名为 `src_stride`、`dst_stride`
- **对比**: 旧版 `asc_copy_l12fb` 使用 `src_gap_size` / `dst_gap_size` 且描述一致（命名与语义一致）
- **判定**: **参数应命名为 `src_gap` / `dst_gap`，或描述应改为 stride 定义。`_v2` 版本将 `gap` 重命名为 `stride` 但描述未更新，可能是命名错误**

---

## 二、gap/stride 参数名跨文件不一致

### 问题 3: `asc_copy_l12l0b_trans` (3510) — Header 用 `dst_frac_stride` / `src_frac_stride`，Doc 与 Impl 用 `dst_frac_gap` / `src_frac_gap`

| 位置 | 参数名 |
|:---|:---|
| **Header** `include/c_api/cube_datamove/cube_datamove.h:1316-1317` | `dst_frac_stride`, `src_frac_stride` |
| **Doc** `docs/api/c_api/cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md:124-125` | `dst_frac_gap`, `src_frac_gap` |
| **Impl dispatch** `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h:938` | `dst_frac_gap`, `src_frac_gap` |
| **Impl body** `impl/.../npu_arch_3510/cube_datamove_impl/asc_copy_l12l0b_trans_impl.h:29` | `dst_frac_gap`, `src_frac_gap` |

- **描述语义**: "前一个分形**结束地址**与后一个分形起始地址的间隔" → GAP 语义
- **判定**: **Header 中的 `dst_frac_stride` / `src_frac_stride` 应改为 `dst_frac_gap` / `src_frac_gap`，与 Doc 和 Impl 保持一致**

### 问题 4: `asc_copy_l12l0a` / `asc_copy_l12l0b` (2201, 2D) — 同一函数签名中混用 `src_stride` 与 `dst_gap`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:397`
  ```c
  uint16_t src_stride, uint16_t dst_gap
  ```
- **Doc** (l12l0a): `...asc_copy_l12l0a_arch_2201.md:76-77`
  - `src_stride`: "相邻迭代间，源操作数前一个分形与后一个分形**起始地址**的间隔" → STRIDE 语义 ✓
  - `dst_gap`: "目的操作数相邻连续数据块的间隔（前面一个数据块的**尾**与后面一个数据块的头**的间隔**）" → GAP 语义 ✓
- **Doc** (l12l0b): `...asc_copy_l12l0b_arch_2201.md:70-71` — 描述相同
- **判定**: **问题在于 src 侧使用 `stride`（头到头），dst 侧使用 `gap`（尾到头）。如果两者语义相同（都是相邻数据块间距离），则命名应统一为 `stride` 或 `gap`**。需要硬件确认实际语义。

### 问题 5: `asc_copy_gm2l0a` (2201) — `src_Stride` 含大写字母 S

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:794-837`
  - 非 sync 版本: `uint16_t src_Stride` （大写 S）
  - sync 版本: bfloat16_t 为 `uint16_t src_stride`（小写 s），其余类型为 `uint16_t src_Stride`（大写 S）
- **整个代码库中其他所有地方均使用 `src_stride`（全小写）**
- **判定**: **笔误，应为 `src_stride`（全小写）**

### 问题 6: `asc_copy_l12l0a_trans` (2201) 内部 — Dispatch 层用 `dst_frac_gap`，Header/Doc/Impl body 用 `dst_frac_stride`

| 位置 | 参数名 |
|:---|:---|
| **Header** `include/c_api/cube_datamove/cube_datamove.h:631` | `dst_frac_stride` |
| **Doc** `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md:65` | `dst_frac_stride` |
| **Impl dispatch** `impl/.../npu_arch_2201/cube_datamove_impl.h:1081-1083` | `dst_frac_gap` |
| **Impl body** `impl/.../npu_arch_2201/cube_datamove_impl/asc_copy_l12l0a_trans_impl.h:28` | `dst_frac_stride` |

- **判定**: **Dispatch 层的 `dst_frac_gap` 应改为 `dst_frac_stride` 以与接口一致（或整体改为 gap）**

### 问题 7: `asc_copy_l12bt` — 使用 `source_gap` 而非 `src_gap`

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:45` — `source_gap`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_2201.md:40` — `source_gap`
- **对比**: 所有其他 API 使用缩写 `src_gap`
- **判定**: **命名风格不一致，应统一为 `src_gap`**

### 问题 8: `asc_copy_l12fb` — 使用 `src_gap_size` / `dst_gap_size` (含 `_size` 后缀)

- **Header**: `include/c_api/cube_datamove/cube_datamove.h:36` — `src_gap_size`, `dst_gap_size`
- **Doc**: `docs/api/c_api/cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_2201.md:40-41` — `src_gap_size`, `dst_gap_size`
- **对比**: `asc_copy_l12fb_v2` 使用 `src_stride` / `dst_stride`（无后缀）
- **判定**: **`_size` 后缀与其他 API 命名风格不一致，应统一为 `src_gap` / `dst_gap`**

---

## 三、参数顺序不一致

### 问题 9: `asc_set_ndim_loop*_stride` 与 `asc_set_*_loop*_stride` 参数顺序相反

| API | 参数顺序 | 文件 |
|:---|:---|:---|
| `asc_set_ub2gm_loop1_stride` | `loop1_src_stride` **先**, `loop1_dst_stride` **后** | `vector_datamove.h:299-301` |
| `asc_set_ub2gm_loop2_stride` | `loop2_src_stride` **先**, `loop2_dst_stride` **后** | `vector_datamove.h:305-307` |
| `asc_set_gm2ub_loop1_stride` | `loop1_src_stride` **先**, `loop1_dst_stride` **后** | `vector_datamove.h:299-301` |
| `asc_set_gm2ub_loop2_stride` | `loop2_src_stride` **先**, `loop2_dst_stride` **后** | `vector_datamove.h:305-307` |
| **`asc_set_ndim_loop0_stride`** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |
| **`asc_set_ndim_loop1_stride`** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |
| **... (loop2-4 同理)** | **`dst_stride` 先, `src_stride` 后** | `vector_datamove.h:310-318` |

- **判定**: **`ndim` 系列接口的 dst/src 参数顺序与其他 loop stride 接口相反，容易引起调用错误**

### 问题 10: `asc_copy_ub2gm_align` (3510) — gap 顺序与 2201 相反

| 架构 | gap 参数顺序 | 文件 |
|:---|:---|:---|
| **2201** | `src_gap` **先**, `dst_gap` **后** | `vector_datamove.h:138` |
| **3510** | `dst_gap` **先**, `src_gap` **后** | `vector_datamove.h:262` |

- 所有其他 API 均使用 src 在前、dst 在后的顺序
- **判定**: **3510 版本应改为 `src_gap` 在前，与 2201 和所有其他 API 保持一致**

---

## 四、参数类型不一致

### 问题 11: `asc_copy_l12l0b_trans` (2201, half 重载) — Doc 与 Header 类型不同

| 位置 | `dst_frac_gap` 类型 |
|:---|:---|
| **Doc** `...asc_copy_l12l0b_trans_arch_2201.md:66` | `uint64_t` |
| **Header** `cube_datamove.h:674-675` | `uint16_t` |
| **其他类型重载** | 均为 `uint64_t` |

- **判定**: **half 重载的 `dst_frac_gap` 在 header 中为 `uint16_t`，与 doc 和其他重载的 `uint64_t` 不一致**

---

## 五、Doc 与 Header 函数声明不一致

### 问题 12: `asc_fill_l0a` / `asc_fill_l0b` — Doc 中有 `half`/`uint32_t` value 重载但 Header 中缺失

| 函数 | Doc 中声明的重载 | Header 中是否存在 | 文件 |
|:---|:---|:---|:---|
| `asc_fill_l0a` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:100` |
| `asc_fill_l0a` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:100` |
| `asc_fill_l0a_sync` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:102` |
| `asc_fill_l0a_sync` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:102` |
| `asc_fill_l0b` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:153` |
| `asc_fill_l0b` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:153` |
| `asc_fill_l0b_sync` | `bfloat16_t* dst, half value` | **缺失** | `cube_datamove.h:155` |
| `asc_fill_l0b_sync` | `bfloat16_t* dst, uint32_t value` | **缺失** | `cube_datamove.h:155` |

- **判定**: **需确认是 Doc 多写了重载，还是 Header 遗漏了重载声明**

---

## 六、同一功能 Doc 内部命名不一致

### 问题 13: `asc_copy_l12l0a_trans.md` — 示例用 `dst_gap` 但参数表用 `dst_stride`

- **文档**: `docs/api/c_api/cube_datamove/asc_copy_l12l0a_trans.md`
  - **第 22 行**: 示例讲解使用 `dst_gap = 1`
  - **第 23 行**: 示例讲解使用 `dst_frac_gap = 0`
  - **第 95 行**: 参数表声明为 `dst_stride`
  - **第 97 行**: 参数表声明为 `dst_frac_stride`
- **判定**: **同一文档内示例和参数表使用不同的命名，应统一**

---

## 总结

| 类别 | 问题数 | 严重程度 |
|:---|:---:|:---|
| gap/stride 语义误用 | 3 | **高** — 影响用户理解 |
| gap/stride 跨文件不一致 | 5 | **高** — 接口声明与实现不同名 |
| 参数顺序不一致 | 2 | **中** — 易导致调用错误 |
| 参数类型不一致 | 1 | **中** — 类型不匹配 |
| Doc/Header 声明不一致 | 1（8 个重载） | **中** — 文档与接口不同步 |
| 命名风格不一致 | 2 | **低** — 风格偏差 |
| 文档内部不一致 | 1 | **中** — 示例与参数表不同名 |

### 建议优先修复

1. **问题 3**: Header 中 `dst_frac_stride` / `src_frac_stride` → 改为 `dst_frac_gap` / `src_frac_gap`（与 impl 和 doc 对齐）
2. **问题 1**: `asc_copy_l12l0a_trans` 的 `dst_stride` / `dst_frac_stride` 参数，如确实描述 gap 语义，应更名为 `dst_gap` / `dst_frac_gap`
3. **问题 2**: `asc_copy_l12fb_v2` 的 `src_stride` / `dst_stride` 参数，如确实描述 gap 语义，应更名为 `src_gap` / `dst_gap`
4. **问题 5**: `asc_copy_gm2l0a` 中 `src_Stride` 改为 `src_stride`
5. **问题 9/10**: 统一参数顺序

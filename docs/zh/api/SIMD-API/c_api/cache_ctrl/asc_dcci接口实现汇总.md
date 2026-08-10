# asc_dcci 接口实现汇总

> 对应文档：`docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md`
> 实现位置：`impl/c_api/instr_impl/npu_arch_{2201,3510}/cache_ctrl_impl/`

---

## 1. 接口原型

| 序号 | 函数原型 | 说明 |
|------|----------|------|
| 1 | `__aicore__ inline void asc_dcci_single(__gm__ void* dst)` | 刷新传入地址所在的单个 Cache Line |
| 2 | `__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)` | 刷新传入的 UB 地址空间地址所在的单个 Cache Line |
| 3 | `__aicore__ inline void asc_dcci_entire_all()` | 刷新所有 Cache Line（ALL） |
| 4 | `__aicore__ inline void asc_dcci_entire_ub()` | 刷新所有 Cache Line（UB） |
| 5 | `__aicore__ inline void asc_dcci_entire_out()` | 刷新所有 Cache Line（OUT） |
| 6 | `__aicore__ inline void asc_dcci_entire_atomic()` | 刷新所有 Cache Line（ATOMIC） |

---

## 2. 调用链

```
asc_dcci_xxx()                  // 公开封装  cache_ctrl_impl.h
  → asc_xxx_impl()              // 底层实现  asc_dcci_impl.h
    → dcci(...)                 // 编译器内建指令（cache_line_t / dcci_dst_t 参数）
```

---

## 3. 公开封装（`cache_ctrl_impl.h`）

### 3.1 npu_arch_2201（`npu_arch_2201/cache_ctrl_impl.h:27-38`）

```cpp
namespace __asc_aicore {
__aicore__ inline void asc_dcci_single(__gm__ void* dst) { asc_dcci_single_impl(dst); }
} // namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst) { asc_ub_dcci_single_impl(dst); }
__aicore__ inline void asc_dcci_entire_all() { asc_dcci_entire_all_impl(); }
__aicore__ inline void asc_dcci_entire_ub() { asc_dcci_entire_ub_impl(); }
__aicore__ inline void asc_dcci_entire_out() { asc_dcci_entire_out_impl(); }
__aicore__ inline void asc_dcci_entire_atomic() { asc_dcci_entire_atomic_impl(); }
```

### 3.2 npu_arch_3510（`npu_arch_3510/cache_ctrl_impl.h:38-48`）

```cpp
namespace __asc_aicore {
__aicore__ inline void asc_dcci_single(__gm__ void* dst) { asc_dcci_single_impl(dst); }
} // namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst) { asc_ub_dcci_single_impl(dst); }
__aicore__ inline void asc_dcci_entire_all() { asc_dcci_entire_all_impl(); }
__aicore__ inline void asc_dcci_entire_out() { asc_dcci_entire_out_impl(); }
__aicore__ inline void asc_dcci_entire_atomic() { asc_dcci_entire_atomic_impl(); }
```

> 注：3510 架构**不提供** `asc_dcci_entire_ub`。

---

## 4. 底层实现（`asc_dcci_impl.h`）

### 4.1 npu_arch_2201（`npu_arch_2201/cache_ctrl_impl/asc_dcci_impl.h:28-52`）

```cpp
namespace __asc_aicore {
__aicore__ inline void asc_dcci_single_impl(__gm__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }
} // namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single_impl(__ubuf__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }

__aicore__ inline void asc_dcci_entire_all_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ALL);
}

__aicore__ inline void asc_dcci_entire_ub_impl()
{
    dcci((__ubuf__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_UB);
}

__aicore__ inline void asc_dcci_entire_out_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_OUT);
}

__aicore__ inline void asc_dcci_entire_atomic_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ATOMIC);
}
```

### 4.2 npu_arch_3510（`npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h:28-47`）

```cpp
namespace __asc_aicore {
__aicore__ inline void asc_dcci_single_impl(__gm__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }
} // namespace __asc_aicore

__aicore__ inline void asc_ub_dcci_single_impl(__ubuf__ void* dst) { dcci(dst, cache_line_t::SINGLE_CACHE_LINE); }

__aicore__ inline void asc_dcci_entire_all_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ALL);
}

__aicore__ inline void asc_dcci_entire_out_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_OUT);
}

__aicore__ inline void asc_dcci_entire_atomic_impl()
{
    dcci((__gm__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_ATOMIC);
}
```

> 注：3510 架构无 `asc_dcci_entire_ub_impl`。

---

## 5. 实现对应关系

| 文档原型 | 公开封装 | 底层实现 | 编译器内建指令 |
|---|---|---|---|
| `asc_dcci_single(__gm__ void*)` | `cache_ctrl_impl.h` | `asc_dcci_single_impl` | `dcci(dst, SINGLE_CACHE_LINE)` |
| `asc_ub_dcci_single(__ubuf__ void*)` | `cache_ctrl_impl.h` | `asc_ub_dcci_single_impl` | `dcci(dst, SINGLE_CACHE_LINE)` |
| `asc_dcci_entire_all()` | `cache_ctrl_impl.h` | `asc_dcci_entire_all_impl` | `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ALL)` |
| `asc_dcci_entire_ub()` | `cache_ctrl_impl.h` | `asc_dcci_entire_ub_impl` | `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_UB)` |
| `asc_dcci_entire_out()` | `cache_ctrl_impl.h` | `asc_dcci_entire_out_impl` | `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_OUT)` |
| `asc_dcci_entire_atomic()` | `cache_ctrl_impl.h` | `asc_dcci_entire_atomic_impl` | `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ATOMIC)` |

---

## 6. 架构差异

| 接口 | 2201 | 3510 |
|------|:----:|:----:|
| `asc_dcci_single` | ✅ | ✅ |
| `asc_ub_dcci_single` | ✅ | ✅ |
| `asc_dcci_entire_all` | ✅ | ✅ |
| `asc_dcci_entire_ub` | ✅ | ❌ |
| `asc_dcci_entire_out` | ✅ | ✅ |
| `asc_dcci_entire_atomic` | ✅ | ✅ |

> **3510 架构没有 `asc_dcci_entire_ub`**——文档第 60 行列出的 `asc_dcci_entire_ub()` 在 3510 架构中无对应实现。

---

## 7. 实现要点

- 所有函数底层最终收敛到编译器内建指令 **`dcci()`**。
- 通过 `cache_line_t` 枚举区分刷新粒度：`SINGLE_CACHE_LINE`（单个 Cache Line）与 `ENTIRE_DATA_CACHE`（整个数据缓存）。
- 通过 `dcci_dst_t` 枚举区分整体刷新范围：`CACHELINE_ALL` / `CACHELINE_UB` / `CACHELINE_OUT` / `CACHELINE_ATOMIC`。
- `asc_dcci_single_impl` 位于 `namespace __asc_aicore`，其余位于全局命名空间。
- `asc_ub_dcci_single` 使用 `__ubuf__` 地址，其余使用 `__gm__` 地址。

---

# DataCacheCleanAndInvalid 接口实现补充

> 对应文档：`docs/zh/api/SIMD-API/basic_api/cache_control/DataCacheCleanAndInvalid.md`
> 实现位置：`impl/basic_api/kernel_operator_cache_intf_impl.h`（公开封装）与 `impl/basic_api/kernel_reg.h`（底层模板）

> **注意**：`DataCacheCleanAndInvalid` 属于 **basic_api 侧**接口（非 C-API 的 `asc_dcci`），但两者底层均收敛到编译器内建 `dcci()` 指令。本接口通过模板 + `dcciDst` 枚举实现与 C-API `asc_dcci` 等价的功能。

## 接口原型（basic_api 侧）

| 序号 | 函数原型 | 说明 |
|------|----------|------|
| 1 | `template <typename T, CacheLine entireType, DcciDst dcciDst> void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)` | 保证 DCache 与 GM 存储一致性（配置 dcciDst） |
| 2 | `template <typename T, CacheLine entireType, DcciDst dcciDst> void DataCacheCleanAndInvalid(const LocalTensor<T>& dst)` | 保证 DCache 与 Local Memory 存储一致性（配置 dcciDst） |
| 3 | `template <typename T, CacheLine entireType> void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)` | 仅保证 DCache 与 GM 一致性（不配置 dcciDst） |

## 调用链

```
DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)   // kernel_operator_cache_intf_impl.h
  → DcciGMImpl<T, entireType, dcciDst>(dst.GetPhyAddr())   // kernel_reg.h
    → dcci(dst, entireType, dcciDst)                       // 编译器内建指令
DataCacheCleanAndInvalid(const LocalTensor<T>& dst)    // kernel_operator_cache_intf_impl.h
  → DcciUBImpl<T, entireType, dcciDst>(dst.GetPhyAddr())   // kernel_reg.h
    → dcci(dst, entireType, dcciDst)                       // 编译器内建指令
```

## 公开封装（`impl/basic_api/kernel_operator_cache_intf_impl.h:59-82`）

```cpp
// 支持 dcciDst 的三参数模板重载
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || \
                              (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)
{
    DcciGMImpl<T, entireType, dcciDst>(const_cast<__gm__ T*>(dst.GetPhyAddr()));
}

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst)
{
    DcciUBImpl<T, entireType, dcciDst>(const_cast<__ubuf__ T*>(dst.GetPhyAddr()));
}
#endif

// 不支持 dcciDst 的两参数模板重载
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst)
{
    DcciGMImpl<T, entireType>(const_cast<__gm__ T*>(dst.GetPhyAddr()));
}
#endif
```

## 底层实现（`impl/basic_api/kernel_reg.h:113-136`）

```cpp
// 枚举定义（kernel_reg.h:109-111）
enum class CacheLine : uint64_t { SINGLE_CACHE_LINE = 0, ENTIRE_DATA_CACHE };
enum class DcciDst : uint64_t { CACHELINE_ALL = 0, CACHELINE_UB, CACHELINE_OUT, CACHELINE_ATOMIC };

// 三参数版本（支持 dcciDst）
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || \
                              (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DcciGMImpl(__gm__ T* dst)
{
    dcci(static_cast<__gm__ void*>(dst), static_cast<uint64_t>(entireType), static_cast<uint64_t>(dcciDst));
}

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DcciUBImpl(__ubuf__ T* dst)
{
    dcci(static_cast<__ubuf__ void*>(dst), static_cast<uint64_t>(entireType), static_cast<uint64_t>(dcciDst));
}
#endif

// 两参数版本（不支持 dcciDst）
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113))
template <typename T, CacheLine entireType>
__aicore__ inline void DcciGMImpl(__gm__ T* dst)
{
    dcci(static_cast<__gm__ void*>(dst), static_cast<uint64_t>(entireType));
}
#endif
```

## 架构对比（2201 vs 3510）

### 共同点

- 2201 与 3510 **均满足**两个条件编译块（`2201 || ... || 3510 || ...`），故**两种重载（三参数/两参数）在两个架构上都提供**。
- `DcciGMImpl` / `DcciUBImpl` 实现体完全一致，最终均调用编译器内建 `dcci()`。
- 枚举 `CacheLine`、`DcciDst` 定义唯一，无架构差异。

### 差异点

| 对比项 | 2201（Atlas A2/A3 等） | 3510（Ascend 950 系列） |
|---|---|---|
| 三参数重载（dcciDst） | ✅ 支持 | ✅ 支持 |
| 两参数重载（无 dcciDst） | ✅ 支持 | ✅ 支持 |
| `DcciGMImpl` / `DcciUBImpl` | 有 | 有 |
| `dcci()` 内建指令 | 调用 | 调用 |
| 底层实现体 | 完全相同 | 完全相同 |

> **结论**：`DataCacheCleanAndInvalid` 在 2201 与 3510 架构上的**底层实现完全一致**，均通过模板 `DcciGMImpl`/`DcciUBImpl` 调用编译器内建 `dcci()` 指令，无架构分支差异。

## 与 C-API asc_dcci 的关系

`DataCacheCleanAndInvalid` 与 C-API 的 `asc_dcci_*` 系列接口**功能等价**（同为 DCache Clean + Invalid），底层都调用 `dcci()` 指令：

| basic_api 接口 | C-API 等价接口 | 说明 |
|---|---|---|
| `DataCacheCleanAndInvalid<T, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(gm)` | `asc_dcci_single(__gm__ void*)` | 刷新单个 GM Cache Line |
| `DataCacheCleanAndInvalid<T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_UB>(ub)` | `asc_dcci_entire_ub()` | 刷新整个 UB Cache Line |
| `DataCacheCleanAndInvalid<T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_OUT>(gm)` | `asc_dcci_entire_out()` | 刷新整个 GM Cache Line（OUT） |
| `DataCacheCleanAndInvalid<T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_ATOMIC>(gm)` | `asc_dcci_entire_atomic()` | 刷新整个 ATOMIC Cache Line |
| `DataCacheCleanAndInvalid<T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_ALL>(gm)` | `asc_dcci_entire_all()` | 刷新所有 Cache Line |

> **架构支持差异**：basic_api 的 `DataCacheCleanAndInvalid` 通过模板参数可覆盖 `CACHELINE_UB`，对应 C-API 的 `asc_dcci_entire_ub`。但 C-API 的 `asc_dcci_entire_ub` **仅在 2201 架构提供**（3510 无），而 basic_api 的模板版在 2201/3510 均可用（是否真正支持 UB 刷新取决于硬件，文档中以 `CACHELINE_UB` 的 `<!-- npu="950" -->` 标注为准）。

# Laneid Functions 示例

本示例演示如何使用 Lane ID 和 Lane Mask 相关 API 获取线程在 Warp 内的标识和掩码信息。

## 功能说明

包含以下 API：

### 1. laneid()

返回当前线程在 Warp 内的 lane ID。一个 Warp 包含 32 个线程，因此 laneid 的取值范围为 [0, 31]。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ int32_t laneid();
```

### 2. lanemask_ge()

返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 大于等于当前线程" 的线程对应位设为1，其余位为0。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_ge();
```

**计算公式**: mask = 0xFFFFFFFFU << laneid

### 3. lanemask_eq()

返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 等于当前线程" 的线程对应位设为1，其余位为0。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_eq();
```

**计算公式**: mask = 1U << laneid

### 4. lanemask_gt()

返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 严格大于当前线程" 的线程对应位设为1，其余位为0。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_gt();
```

**计算公式**: mask = 0xFFFFFFFFU << (laneid + 1)

### 5. lanemask_le()

返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 小于等于当前线程" 的线程对应位设为1，其余位为0。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_le();
```

**计算公式**: mask = 0xFFFFFFFFU >> (31 - laneid)

### 6. lanemask_lt()

返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 严格小于当前线程" 的线程对应位设为1，其余位为0。

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_lt();
```

**计算公式**: mask = (1U << laneid) - 1


## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./laneid_functions
```

## API 对比表

| API | 功能 | 计算公式 |
|-----|------|----------|
| laneid() | 获取当前线程在 Warp 内的 ID | N/A |
| lanemask_ge() | 返回 laneid >= 当前线程的掩码 | 0xFFFFFFFFU << laneid |
| lanemask_eq() | 返回 laneid == 当前线程的掩码 | 1U << laneid |
| lanemask_gt() | 返回 laneid > 当前线程的掩码 | 0xFFFFFFFFU << (laneid + 1) |
| lanemask_le() | 返回 laneid <= 当前线程的掩码 | 0xFFFFFFFFU >> (31 - laneid) |
| lanemask_lt() | 返回 laneid < 当前线程的掩码 | (1U << laneid) - 1 |
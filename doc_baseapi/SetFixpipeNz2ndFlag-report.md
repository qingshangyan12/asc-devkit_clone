# SetFixpipeNz2ndFlag 适配报告

## 适配后支持矩阵

| 版本 | Arch | 声明层（intf.h） | 包装层（intf_impl.h） | 版本实现（dav_XXXX） | 适配前 → 适配后 |
|------|------|------------------|----------------------|---------------------|----------------|
| dav_l300 | 3003 | V2 uint16（维持） | uint16 包装（维持） | **完整**（`set_loop3_para`，维持） | **维持**（声明/包装/实现全链路一致） |
| dav_l310 | 3103 | V2 uint16（维持） | uint16 包装（维持） | **完整**（`set_loop3_para`，维持） | **维持**（全链路一致） |
| dav_l311 | 3113 | V2 uint16（维持） | uint16 包装（维持） | **完整**（`set_loop3_para`，维持） | **维持**（全链路一致） |
| dav_5161 | 5101 | ~~声明 V2 uint16（无定义）~~ → **V1 uint32 声明**（守卫同步，**修改**） | uint32 包装（维持） | 桩（5101 不支持 LOOP_PARA 寄存器，维持打桩，**未修改**——原桩即使用 `ASCENDC_REPORT_NOT_SUPPORT`，按"已存在代码不做替换"约定保持原样） | **修复**：声明/定义不一致消除 |
| dav_3510 | 3510（参照） | V1 uint32（维持） | uint32 包装（维持） | **完整**（AIC 守卫内 `set_loop3_para`，维持） | **维持**（参照版本不动） |

> 5161/5165/5163 与 5101 共享 dav_5161 实现，状态相同。其余非目标 arch（1001/2002/2201/3002/3102/5102）声明/包装均维持原 `#else`/uint32 分支不变。

### 修复效果说明（5101 族）

| 调用方式 | 适配前 | 适配后 |
|---------|--------|--------|
| 第三实参 `uint16_t`（按旧声明签名） | 选中未定义的 uint16 声明 → **链接错误** | 唯一可见 uint32 变体，u16→u32 转换 → 调用桩（CPU debug：报错终止；device：空操作） |
| 第三实参 `int` 字面量 | 两个候选同级转换 → **歧义编译错误** | 唯一可见 uint32 变体 → 调用桩 |
| 第三实参 `uint32_t` | 调用 uint32 包装 → 桩（`ASCENDC_REPORT_NOT_SUPPORT`） | 调用 uint32 包装 → 桩（`ASCENDC_REPORT_NOT_SUPPORT`，未修改） |

> host 编译（`__NPU_ARCH__` 未定义）行为不变：新守卫含 `defined(__NPU_ARCH__) &&` 前缀，仍走 `#else` 声明 uint16 变体（与旧守卫下未定义宏按 0 处理的结果一致）。

## 修改的文件

| 文件 | 修改内容 |
|------|---------|
| `asc-devkit/include/basic_api/kernel_operator_fixpipe_intf.h` | L63-64：声明层守卫由 `#if (__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` 同步为 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 \|\| __NPU_ARCH__ == 5102 \|\| __NPU_ARCH__ == 5101 \|\| __NPU_ARCH__ == 5161 \|\| __NPU_ARCH__ == 5165 \|\| __NPU_ARCH__ == 5163)`（与 intf_impl.h L87-88 公共包装守卫完全一致） |

未修改的文件（维持）：

- `kernel_operator_fixpipe_intf_impl.h`（包装层守卫即同步源，无需改动）
- `dav_l300/kernel_operator_fixpipe_impl.h`、`dav_l310/kernel_operator_fixpipe_impl.h`、`dav_l311/kernel_operator_fixpipe_impl.h`、`dav_3510/kernel_operator_fixpipe_impl.h`
- `dav_5161/kernel_operator_fixpipe_impl.h`（`SetFixpipeNz2ndFlagImpl` 桩函数保持原样——已存在代码不做替换；其原本即使用 `ASCENDC_REPORT_NOT_SUPPORT`，与"新增代码使用 `ASCENDC_REPORT_NOT_SUPPORT`"的约定方向一致）

## 适配后各版本原始代码

### 声明层（kernel_operator_fixpipe_intf.h L63-68，**已修改**）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5101 || \
                              __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride);
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride);
#endif
```

> 修改点：L63-64 守卫条件同步自 intf_impl.h L87-88。效果：5101/5161/5165/5163 由"声明 uint16（无定义）"改为"声明 uint32（与包装/实现签名一致）"。

### 包装层（kernel_operator_fixpipe_intf_impl.h L87-98，未修改）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5101 || \
                              __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#endif
```

---

### dav_3510（参照版本，3510，未修改）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L73-83，`if ASCEND_IS_AIC` 运行时守卫内）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    if ASCEND_IS_AIC {
        // ND_PARA[63:32], dst nd stride in unit of element
        // ND_PARA[31:16], src nd stride in uint of C0_SIZE
        // ND_PARA[15:0], nd number.
        uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                          (static_cast<uint64_t>(ndNum));
        set_loop3_para(config);
    }
}
```

---

### dav_l300（目标版本，3003，未修改）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

---

### dav_l310（目标版本，3103，未修改）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

---

### dav_l311（目标版本，3113，未修改）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

---

### dav_5161（目标版本，5101；5101/5161/5165/5163 共享，未修改）

#### SetFixpipeNz2ndFlagImpl — 桩函数（L63-69，维持原 `ASCENDC_REPORT_NOT_SUPPORT` 格式）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    (void)(ndNum);
    (void)(srcNdStride);
    (void)(dstNdStride);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixpipeNz2ndFlag");
}
```

> 按适配方案 5101 不支持 LOOP_PARA 寄存器，维持打桩；按"已存在代码不做替换"约定，桩函数保持上游原样（原本即使用 `ASCENDC_REPORT_NOT_SUPPORT`）。签名（uint32）不变，与修复后的声明/包装链路一致。

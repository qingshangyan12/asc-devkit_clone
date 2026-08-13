# PR 5039 DAV 3510验证结果

## 验证基线

- PR head：`e01c7406ed3a604e6aee1760f518fa79127cc388`。
- NPU架构：DAV 3510（Ascend 950）。
- CANN：9.2.0。
- 编译器：`bisheng` 15.0.5。
- 每个程序的运行超时：30秒。

`bisheng --asc-aicore-lang`会在用户传入的普通`-I`之前注入CANN安装包的AscendC头文件路径，因此验证PR checkout时使用`-iquote <PR_ROOT>/include -iquote <PR_ROOT>`。通过`-H`确认`c_api/asc_simd.h`、公开声明和3510实现均来自PR checkout。

## 通过项

| 验证项 | 实际结果 |
| --- | --- |
| `asc_duplicate_highest` | 输入最低/最高位为1/64，输出为`64 64 64 64 0 0 0 0 ...`，与golden完全一致。 |
| `asc_duplicate_highest_merge` | 目标初始值为-7，输出为`64 64 64 64 -7 -7 -7 -7 ...`，与golden完全一致。 |
| `asc_load_dev(__gm__ uint32_t*)` | 普通读建立值11的DataCache副本后，`asc_store_dev`将GM直接改为123456789；`asc_load_dev`读取123456789，普通读仍读取11，Host最终读取123456789。 |
| 无符号读取重载 | `uint8_t/uint16_t/uint32_t/uint64_t`的实机读取结果与输入完全一致。 |

以上结果验证了两个highest模式的`POS_HIGHEST`及zero/merge语义，也验证了`asc_load_dev`通过`ld_dev`绕过DataCache读取GM，而普通Scalar访问仍经过DataCache。

三个正式样例均在30秒运行超时下重新编译运行通过。`asc_load_dev`的缓存可见性样例额外连续复验3次，三次均得到`[11, 123456789, 11]`，未出现精度失败、运行错误或超时。

## 发现的问题

### 窄有符号整数未符号扩展

PR文档约定：有符号地址返回`int64_t`。实机向GM写入负数后，PR head的读取结果如下：

可通过`scalar_compute/asc_load_dev_signed_repro.asc`默认编译模式复现。

| 重载 | 输入 | 实际返回 | 期望返回 |
| --- | ---: | ---: | ---: |
| `asc_load_dev(__gm__ int8_t*)` | -1 | 255 | -1 |
| `asc_load_dev(__gm__ int16_t*)` | -2 | 65534 | -2 |
| `asc_load_dev(__gm__ int32_t*)` | -3 | 4294967293 | -3 |

原因是PR最后一个提交删除了原有的`static_cast<int8_t/int16_t/int32_t>`，直接返回`ld_dev`结果；当前CANN 9.2.0编译器头文件将`ld_dev(...)`声明为`uint64_t`，因此窄有符号负数被零扩展。新增UT通过自定义有符号`ld_dev`桩模拟了编译器并不存在的返回重载，未覆盖真实编译器行为。

建议保留按位宽截断后再向`int64_t`符号扩展的实现，例如`int32_t`重载使用：

```cpp
return static_cast<int32_t>(
    ld_dev(reinterpret_cast<__gm__ uint32_t*>(addr), ASC_C_API_LOAD_DEV_DEFAULT_OFFSET));
```

### `int64_t*`重载无法编译

在独立实机矩阵中调用`asc_load_dev(__gm__ int64_t*)`时，device编译报错：

可通过`scalar_compute/asc_load_dev_signed_repro.asc`的`REPRO_INT64_COMPILE_FAILURE`模式复现。

```text
asc_load_dev_impl.h:57:19: error: the 1st parameter maybe need a type 'half *'
return ld_dev(addr, ASC_C_API_LOAD_DEV_DEFAULT_OFFSET);
```

建议与PR前一版实现保持一致，将地址重解释为`__gm__ uint64_t*`后调用真实存在的`ld_dev`重载，再将返回位模式转换为`int64_t`。同时应增加由真实`bisheng --npu-arch=dav-3510`执行的device编译和运行测试；仅使用Host mock无法发现该问题。

上述建议已在临时PR checkout中应用，并对`int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/int64_t/uint64_t`八种重载执行实机矩阵。八种重载均编译运行通过，输出与golden完全一致，证明恢复“按无符号同位宽地址读取，再按原始有符号位宽转换”的实现可以修复这两个问题。

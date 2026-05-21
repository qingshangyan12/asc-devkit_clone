# CANN 版本兼容性说明

## 版本: CANN 9.0.0-beta.2
### 编译状态: ❌ 失败
```
error: unknown type name 'fp4x2_e1m2_t' (3510架构)
error: unknown type name 'int4b_t'        (2201架构)
```

### 根因分析
- CANN 9.0.0-beta.2 的 `asc/impl/utils/common_types.h` 缺少 fp4/fp8/int4 类型的别名定义
- 类型定义存在于 `kernel_utils_macros.h:425-438`，但不在 C API impl 的 include 链中  
- 尝试添加类型定义（#define/using/struct）均导致符号重定义错误
- dav-2201 架构也报 `int4b_t` 缺失，属同一类问题

### 结论
CANN 9.0.0-beta.2 的 C API impl 头文件存在类型定义缺陷，
无法编译任何 dav-3510 或 dav-2201 架构的 Cube C API 代码。
此为 CANN 库版本问题，非测试代码问题。

### 建议
使用 CANN 9.1.0-beta.1 或更新版本。

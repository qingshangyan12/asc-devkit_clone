# 常见问题与解决方案

## 1. 参数结构初始化问题

### 问题现象

```
error: no matching constructor for initialization of 'FixpipeParamsV220'
```

### 根本原因

部分参数结构体（如 `FixpipeParamsV220`, `LoadData2dParams` 等）未定义花括号初始化构造函数。

### 通用解决方案

```cpp
// ❌ 错误：花括号初始化不支持
FixpipeParamsV220 params = {n, m, stride, ...};

// ✅ 正确：使用成员赋值
FixpipeParamsV220 params;
params.nSize = n;
params.mSize = m;
params.srcStride = stride;
params.dstStride = n;
params.quantPre = quantMode;
params.reluEn = reluFlag;
```

**规则：对于所有 Params 结构体，优先使用成员赋值。**

> 详细参数类型说明请参考 [membase AIC API UT 指南](../api-guides/membase-api-aic-ut-guide.md)

---

## 2. Stub 函数参数不匹配

### 问题现象

```
segmentation fault (core dumped)
```

### 原因分析

UT 框架中的 stub 函数（如 `copy_matrix_cc_to_gm`）期望特定数量的参数，但实际 impl 调用传递的参数数量不同。

### 解决方法

1. 使用类封装测试逻辑，避免直接触发有问题的代码路径
2. 参考同架构下已有成功测试的代码模式
3. 简化测试场景，避免复杂配置

> 详细 AIC 测试模式请参考 [membase AIC API UT 指南](../api-guides/membase-api-aic-ut-guide.md)

---

## 3. 核心类型不匹配

### 问题现象

测试执行失败，设备状态异常。

### 解决方法

```cpp
// 确保 Cube 核心 API 在 SetUp 中切换类型
class TEST_FIXPIPE : public testing::Test {
protected:
    void SetUp() {
        g_coreType = AscendC::AIC_TYPE;  // 必须设置
    }
    void TearDown() {
        AscendC::CheckSyncState();       // 检查同步状态
        g_coreType = AscendC::MIX_TYPE;  // 恢复混合模式
    }
};
```

---

## 4. QuantMode 与数据类型组合

### 问题现象

Fixpipe 输出异常或测试失败。

### 快速参考

| QuantMode | 需要 workspace | 量化类型 |
|-----------|----------------|---------|
| NoQuant | 否 | 无量化 |
| F322F16, F322BF16 | 否 | Scalar 量化 |
| DEQF16, REQ8 | 否 | Scalar 量化 |
| **VDEQF16, VQF322B8_PRE, VREQ8** | **是** | **Tensor 量化** |

**关键规则**：
- QuantMode 名称以 `V` 开头的需要 workspace
- workspace 类型必须是 `LocalTensor<uint64_t>`

> 完整对照表请参考 [membase AIC API UT 指南](../api-guides/membase-api-aic-ut-guide.md) 中的 QuantMode 章节

---

## 5. TmpBuffer 大小确定

### 问题

API 签名中包含 sharedTmpBuffer 参数，如何确定大小？

### 解决方法

1. 查看 API 头文件注释
2. 从 impl 文件分析内部计算
3. 参考现有测试文件

### 示例

TmpBuffer 大小不是通用 dtype 事实，必须从目标 API 注释、impl 内部临时空间计算和已有同类 UT 中确认。详细内存申请模式统一参考 [LocalTensor 内存申请指南](../foundations/local-tensor-memory.md#72-需要临时空间的-api)。

---

## 6. 编译报错找不到底层指令

### 问题现象

```
undefined reference to 'vadd'
```

### 解决方法

```cpp
// 检查 cce_stub.h 是否已声明该指令
// 如果没有，需要添加声明
void cce_instruction_name(type1 arg1, type2 arg2);
```

---

## 7. 数据对齐约束

### Count 值计算

```python
# 公式 1: 最小大小约束
count >= ceil(min_tensor_size / sizeof(data_type))

# 公式 2: 32 字节对齐约束
count * sizeof(T) % 32 == 0

# 推荐值：先按 min_tensor_size 推出最小 count，再向上取满足 32B 对齐的 count
# 具体 dtype 的 sizeof 和 32B 对齐元素数见 asc-npu-arch 架构指南
```

### 对齐约束快速计算

基础 dtype 的 `sizeof` 与 32B 对齐元素数统一见 [`asc-npu-arch` 架构指南](../../../asc-npu-arch/references/npu-arch-guide.md#基础-dtype-大小与-32b-对齐)。FAQ 中的 count 推荐值应按该统一表和具体 API 的最小 tensor 大小共同计算。

---

## 8. 测试用例设计建议

### DO（推荐）

```cpp
// 1. 使用类封装，结构清晰
class KernelFixpipe { ... };

// 2. 简单的测试断言
for (int32_t i = 0; i < size; i++) {
    EXPECT_EQ(output[i], expected_value);
}

// 3. 使用 ASCEND_IS_AIV 宏跳过非目标核心
if ASCEND_IS_AIV {
    return;
}
```

### DON'T（避免）

```cpp
// 1. 直接使用 LOCAL_TENSOR_REGISTER 宏（复杂场景易出错）
// 2. 过于复杂的测试配置组合
// 3. 未经验证的 QuantMode 与类型组合
```

---

## 9. 相关文档索引

| 问题类型 | 参考文档 |
|---------|---------|
| AIC 测试模式 | [membase AIC API UT 指南](../api-guides/membase-api-aic-ut-guide.md) |
| AIV 测试模式 | [membase AIV API UT 指南](../api-guides/membase-api-aiv-ut-guide.md) |
| 内存申请 | [LocalTensor 内存申请指南](../foundations/local-tensor-memory.md) |
| 分支覆盖 | [分支覆盖分析指南](../foundations/branch-coverage-guide.md) |
| 自动化流程 | [自动化验证流程](../workflows/automation-guide.md) |

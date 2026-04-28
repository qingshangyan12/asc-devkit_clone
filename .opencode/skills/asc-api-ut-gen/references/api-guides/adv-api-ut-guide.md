# 高阶 API (Advanced API) UT 生成指南

## 1. API 概述

高阶 API (Advanced API) 是 AscendC 提供的高级封装接口，将常用算子封装为易用的高级接口，降低开发复杂度。

### 1.1 目录位置

| 类型 | 路径 |
|------|------|
| **头文件** | `{ASC_DEVKIT_PATH}/include/adv_api/` |
| **实现文件** | `{ASC_DEVKIT_PATH}/impl/adv_api/` |
| **测试目录** | `{ASC_DEVKIT_PATH}/tests/api/adv_api/` |

### 1.2 典型 API 示例

| API 类别 | 典型 API |
|---------|---------|
| 激活函数 | `Softmax`, `LayerNorm`, `Gelu`, `Swish` |
| 数学计算 | `Add`, `Sub`, `Mul`, `Div` |
| 矩阵运算 | `Matmul` |
| 归约操作 | `Reduce` |
| 量化操作 | `Quantize` |

---

## 2. API 特点

### 2.1 模板类封装

高阶 API 通常采用模板类形式封装：

```cpp
template <class A_TYPE, class B_TYPE, class C_TYPE,
          class BIAS_TYPE = half, class C_TYPE_2 = C_TYPE>
class MatmulImpl {
public:
    __aicore__ inline MatmulImpl() {}
    __aicore__ inline void Init(...) { }
    __aicore__ inline void Iterate(...) { }
    __aicore__ inline void GetTensorC(...) { }
    // ...
};
```

### 2.2 架构无关性

高阶 API 通常支持多架构，内部自动适配：
- 单一代码适配多种芯片架构
- 通过模板参数和条件编译实现架构适配

### 2.3 丰富的配置选项

高阶 API 通过枚举类和结构体提供灵活配置：

```cpp
// 常用枚举类
enum class TPosition : int32_t {
    GM, A1, B1, C1, A2, B2, C2, CO1, CO2,
    VECIN, VECOUT, VECCALC, LCM, SPM, SHM, TSCM
};

enum class CubeFormat : int32_t { ... };
enum class TilingPolicy : int32_t { ... };
```

---

## 3. UT 生成注意事项

### 3.1 类测试模式

高阶 API 测试需要实例化模板类：

```cpp
// 定义测试用的类型别名
using A_TYPE = half;
using B_TYPE = half;
using C_TYPE = float;

// 实例化 Kernel 类
class KernelMatmul {
public:
    __aicore__ inline KernelMatmul() {}
    __aicore__ inline void Init(...) {
        matmul.Init(...);
    }
    __aicore__ inline void Process() {
        matmul.Iterate(...);
    }
private:
    MatmulImpl<A_TYPE, B_TYPE, C_TYPE> matmul;
};
```

### 3.2 分支覆盖要点

#### 3.2.1 模板参数组合

高阶 API 的模板参数组合必须从目标 API 声明、impl 分支、设计文档或已有同类 UT 确认。通用基础 dtype 和扩展低精度 dtype 的统一说明回链到 [`asc-npu-arch` 架构指南](../../../asc-npu-arch/references/npu-arch-guide.md#统一数据类型视图)，本 guide 不维护并行的常见 dtype 组合表。

**测试策略**：使用参数化测试覆盖已确认的主要类型组合，不要按通用 dtype 列表推断支持范围。

#### 3.2.2 架构条件编译

检查 impl 文件中的架构分支：

```cpp
#if __NPU_ARCH__ == 2201
    // ascend910b1 特定实现
#elif __NPU_ARCH__ == 3510
    // ascend950pr_9599 特定实现
#endif
```

**测试策略**：不同架构使用对应的测试目录

#### 3.2.3 配置参数组合

```cpp
// 不同 TilingPolicy
TilingPolicy::TILING_POLICY_1
TilingPolicy::TILING_POLICY_2

// 不同 CubeFormat
CubeFormat::ND
CubeFormat::NZ
```

### 3.3 内存管理

高阶 API 通常需要较大的内存空间：

```cpp
// L1 缓冲区初始化
TPipe pipe;
TQue<TPosition::A1, 1> inQueueA1;
TQue<TPosition::B1, 1> inQueueB1;
TQue<TPosition::CO1, 1> outQueueCO1;

pipe.InitBuffer(inQueueA1, 1, m * k * sizeof(half));
pipe.InitBuffer(inQueueB1, 1, k * n * sizeof(half));
pipe.InitBuffer(outQueueCO1, 1, m * n * sizeof(float));
```

### 3.4 测试目录结构

```
tests/api/adv_api/
├── activation/       # 激活函数测试
│   ├── softmax/
│   └── layernorm/
├── math/             # 数学计算测试
├── matmul/           # 矩阵运算测试
└── reduce/           # 归约操作测试
```

---

## 4. 测试模板引用

通用 gtest、参数化测试和精度验证骨架见 [测试模板参考](../foundations/test-templates.md)。高阶 API guide 只维护高阶 API 特有约束：

- 类封装 API 优先按目标 API 的对象生命周期组织 `SetUp`、`Init`、`Process` 和结果校验。
- 模板参数组合必须来自目标 API 声明、impl 分支、设计文档或已有同类 UT。
- 涉及 LocalTensor、TPipe、TQue 的内存申请细节回链 [LocalTensor 内存申请指南](../foundations/local-tensor-memory.md)。

---

## 5. 常见问题

### Q1: 模板参数类型不匹配？

**问题**：编译时模板实例化失败

**解决**：
- 检查类型组合是否在 API 支持列表中
- 参考头文件注释确认支持的数据类型

### Q2: 内存分配失败？

**问题**：Buffer 数量或大小超限

**解决**：
- 检查 InitBuffer 调用总数不超过 64
- 优化内存复用策略

### Q3: 性能问题？

**问题**：测试执行时间过长

**解决**：
- 使用 double buffer 提高流水线效率
- 合理设置测试数据规模

---

## 6. 检查清单

### 6.1 分析阶段

- [ ] 已读取高阶 API 头文件，了解 API 签名
- [ ] 已分析 impl 文件中的所有分支
- [ ] 已确认支持的数据类型组合
- [ ] 已参考同架构下已有类似测试

### 6.2 编写阶段

- [ ] 使用模板类实例化方式测试
- [ ] SetUp/TearDown 正确设置核心类型
- [ ] 内存分配大小正确
- [ ] 使用参数化测试覆盖主要分支

### 6.3 验证阶段

- [ ] 编译通过无错误
- [ ] 测试执行通过
- [ ] 结果验证正确

---

## 7. 常用枚举类

### 7.1 TPosition - 张量位置

**定义文件**: `include/adv_api/matmul/matmul_tiling_base.h`

```cpp
enum class TPosition : int32_t {
    GM,           // Global Memory
    A1, B1, C1,   // L1 缓冲区
    A2, B2, C2,   // L0 缓冲区
    CO1, CO2,     // Cube 输出缓冲区
    VECIN, VECOUT, VECCALC,  // Vector 核心缓冲区
    LCM, SPM, SHM, TSCM,     // 其他存储区域
    C2PIPE2GM, C2PIPE2LOCAL,
    MAX,
};
```

### 7.2 CubeFormat - Cube 格式

**定义文件**: `include/adv_api/matmul/matmul_tiling_base.h`

```cpp
enum class CubeFormat : int32_t {
    ND,   // ND 格式
    NZ,   // NZ 格式
    // ...
};
```

### 7.3 TilingPolicy - Tiling 策略

**定义文件**: `include/adv_api/matmul/matmul_tiling_base.h`

```cpp
enum class TilingPolicy : int32_t {
    // 具体值根据实现确定
};
```

### 7.4 DequantType - 反量化类型

**定义文件**: `include/adv_api/matmul/matmul_tiling_base.h`

```cpp
enum class DequantType : int32_t {
    // 具体值根据实现确定
};
```

### 7.5 ScheduleType - 调度类型

**定义文件**: `include/adv_api/matmul/matmul_tiling_base.h`

```cpp
enum class ScheduleType : int32_t {
    // 具体值根据实现确定
};
```

### 7.6 IterateMode - 迭代模式

**定义文件**: `include/adv_api/matmul/matmul_config.h`

```cpp
enum IterateMode : uint8_t {
    // 具体值根据实现确定
};
```

### 7.7 ReducePattern - 归约模式

**定义文件**: `include/adv_api/reduce/reduce_tiling.h`

```cpp
enum class ReducePattern : uint32_t {
    // 具体值根据实现确定
};
```

### 7.8 QuantizePolicy - 量化策略

**定义文件**: `include/adv_api/quantization/quantize_utils.h`

```cpp
enum class QuantizePolicy : int32_t {
    // 具体值根据实现确定
};
```

### 7.9 ConvFormat - 卷积格式

**定义文件**: `include/adv_api/conv/common/conv_common.h`

```cpp
enum class ConvFormat : uint32_t {
    // 具体值根据实现确定
};
```

---

## 8. 相关参考

| 文档 | 说明 |
|------|------|
| [分支覆盖分析指南](../foundations/branch-coverage-guide.md) | 详细分支分析方法 |
| [测试模板参考](../foundations/test-templates.md) | 通用测试骨架与模板选择索引 |
| [LocalTensor 内存申请指南](../foundations/local-tensor-memory.md) | 内存管理详细说明 |

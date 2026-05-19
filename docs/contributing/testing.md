# 测试规范

## 1. 测试框架概述

### 1.1 框架类型

项目基于 Google Test 构建了单元测试框架，并扩展了参数化测试能力。

### 1.2 测试入口

| 文件 | 用途 |
|------|------|
| `tests/main.cpp` | 测试主入口 |
| `tests/main_global.cpp` | 全局测试入口（部分模块使用） |

### 1.3 测试注册

测试通过 CMake 中 `run_llt_test()` 自定义函数注册到各 product type 的 executable 中。

---

## 2. 测试层次

| 测试类型 | 目的 | 运行环境 |
|---------|------|---------|
| Header Checker | 验证头文件可独立编译 | Host / NPU 编译器 |
| UT（单元测试）| 验证 API 编译正确和参数校验 | Host（CPU Debug）|
| ST（系统测试）| 验证 API 功能正确 | Device（NPU）|
| 单算子测试 | 在真实算子中验证 API 端到端功能 | Device（NPU）|

---

## 3. 测试目录结构

```
tests/
├── CMakeLists.txt
├── main.cpp
├── main_global.cpp
├── api/
│   ├── adv_api/                  # 高阶 API
│   │   ├── CMakeLists.txt
│   │   ├── math/
│   │   │   ├── axpy/
│   │   │   │   └── test_operator_axpy.cpp
│   │   │   ├── exp/
│   │   │   │   ├── test_operator_exp.cpp
│   │   │   │   └── test_operator_exphighprecision.cpp
│   │   │   └── sin/
│   │   │       └── test_operator_sin.cpp
│   │   ├── matmul/
│   │   │   ├── test_operator_matmul_v200.cpp
│   │   │   ├── test_operator_matmul_v220.cpp
│   │   │   └── scheduler/
│   │   │       └── test_scheduler_norm.cpp
│   │   ├── normalization/
│   │   │   └── layernorm/
│   │   │       └── test_operator_layernorm.cpp
│   │   ├── reduce/
│   │   ├── tiling/
│   │   │   └── test_tiling.cpp       # Tiling 侧 UT 集中管理
│   │   └── ...
│   ├── basic_api/                     # 基础 API
│   │   ├── ascendc_header_checker/    # Header 编译检查（独立 project）
│   │   │   ├── CMakeLists.txt
│   │   │   └── check_*.cpp
│   │   ├── ascendc_case_ascend610/    # 按产品类型的功能测试
│   │   ├── ascendc_case_ascend910b1/
│   │   └── ...
│   ├── c_api/
│   └── utils/
└── python/
```

---

## 4. 测试文件命名

| 命名模式 | 示例 | 说明 |
|---------|------|------|
| `test_operator_<api>.cpp` | `test_operator_exp.cpp` | 算子功能测试 |
| `test_operator_<api>_<variant>.cpp` | `test_operator_matmul_v220.cpp` | 特定变体的测试 |
| `test_<component>.cpp` | `test_tiling.cpp`、`test_scheduler_norm.cpp` | 非算子测试 |
| `test_<api>_instr.cpp` | `test_add_instr.cpp` | C API 指令级测试 |

---

## 5. Kernel 侧 UT 编写规范

### 5.1 文件结构（三段式）

```cpp
// 第一部分：引入头文件
#include <gtest/gtest.h>
#include "kernel_operator.h"

// 第二部分：定义测试参数结构体
struct AxpyTestParams {
    int32_t dataSize;
    int32_t dataBitSize;
    void (*calFunc)(uint8_t*, uint8_t*, int32_t);
};

// 第三部分：实现测试类和测试用例
class AxpyTestsuite : public testing::Test,
                      public testing::WithParamInterface<AxpyTestParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(TEST_AXPY, AxpyTestsuite,
    ::testing::Values(
        AxpyTestParams{256, 2, AxpyKernel<half, half>},
        AxpyTestParams{256, 4, AxpyKernel<float, float>}
    ));

TEST_P(AxpyTestsuite, AxpyTestCase)
{
    // 测试逻辑...
}
```

### 5.2 Kernel 函数实现模板

```cpp
template <typename T, typename U>
void AxpyKernel(uint8_t* srcGm, uint8_t* dstGm, int32_t dataSize)
{
    // 1. 初始化变量
    TPipe tpipe;
    TQue<TPosition::VECIN, 1> vecInQue;
    TQue<TPosition::VECOUT, 1> vecOutQue;
    TQue<TPosition::VECCALC, 1> vecTmpQue;

    GlobalTensor<U> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ U*>(srcGm), dataSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    tpipe.InitBuffer(vecInQue, 1, dataSize * sizeof(U));
    tpipe.InitBuffer(vecOutQue, 1, dataSize * sizeof(T));
    tpipe.InitBuffer(vecTmpQue, 1, dataSize * 4 * sizeof(half));

    // 2. 数据从 GlobalMemory 拷贝到 LocalMemory
    LocalTensor<U> inputLocal = vecInQue.AllocTensor<U>();
    LocalTensor<T> outputLocal = vecOutQue.AllocTensor<T>();
    LocalTensor<uint8_t> tmpLocal = vecTmpQue.AllocTensor<uint8_t>();

    DataCopy(inputLocal, inputGlobal, dataSize);

    // 3. 调用被测 API
    SetFlag<HardEvent::MTE2_V>(EVENT_ID0);
    WaitFlag<HardEvent::MTE2_V>(EVENT_ID0);

    Axpy<T, U, false>(outputLocal, inputLocal, scalar, tmpLocal, dataSize);

    // 4. 数据从 LocalMemory 拷贝回 GlobalMemory
    SetFlag<HardEvent::V_MTE3>(EVENT_ID0);
    WaitFlag<HardEvent::V_MTE3>(EVENT_ID0);
    DataCopy(outputGlobal, outputLocal, dataSize);

    // 5. 同步并释放
    PipeBarrier<PIPE_ALL>();
    vecInQue.FreeTensor(inputLocal);
    vecOutQue.FreeTensor(outputLocal);
    vecTmpQue.FreeTensor(tmpLocal);
}
```

### 5.3 测试断言

```cpp
TEST_P(AxpyTestsuite, AxpyTestCase)
{
    auto param = GetParam();
    uint8_t srcGm[param.dataSize * param.dataBitSize] = {0};
    uint8_t dstGm[param.dataSize * param.dataBitSize] = {0};

    param.calFunc(srcGm, dstGm, param.dataSize);

    // 校验结果
    for (int32_t i = 0; i < param.dataSize; i++) {
        EXPECT_EQ(dstGm[i], expectedValue);
    }
}
```

---

## 6. Tiling 侧 UT 编写规范

### 6.1 集中管理

Tiling 接口的 UT 统一在 `tests/api/adv_api/tiling/test_tiling.cpp` 中：

```cpp
TEST_F(TestTiling, TestAxpyTiling)
{
    uint32_t maxVal = 0;
    uint32_t minVal = 0;

    GetAxpyMaxMinTmpSize(ge::Shape({128}), 4, false, maxVal, minVal);
    EXPECT_EQ(maxVal, 0);
    EXPECT_EQ(minVal, 0);

    GetAxpyMaxMinTmpSize(ge::Shape({256}), 2, false, maxVal, minVal);
    EXPECT_EQ(maxVal, 256 * 4 * 2);
    EXPECT_EQ(minVal, 256 * 4);
}
```

### 6.2 编写要求

- 每个 API 一个 `TEST_F(TestTiling, Test<Api>Tiling)` 函数
- 覆盖正常输入和边界条件
- `EXPECT_EQ` 校验 maxValue 和 minValue

---

## 7. Header Checker

### 7.1 目的

验证每个公开头文件可以独立编译，不依赖隐式的 include 顺序。

### 7.2 独立项目

Header Checker 是一个独立的 CMake 项目：

```cmake
project(HeaderChecker)
```

位置：`tests/api/basic_api/ascendc_header_checker/CMakeLists.txt`

### 7.3 四种检查模式

| 模式 | Target 命名 | 编译器 |
|------|------------|--------|
| CPU normal | `check_cpu_${CONFIG_NAME}_${ID}` | Host 编译器 |
| CPU single-header | 同上变体 | Host 编译器 |
| NPU normal | `check_npu_${CONFIG_NAME}_${ID}` | Bisheng 编译器 |
| NPU single-header | 同上变体 | Bisheng 编译器 |

### 7.4 新增要求

每新增一个公开头文件，应添加对应的 Header Checker 文件。

---

## 8. 运行测试

### 8.1 全量测试

```bash
bash build.sh -t
```

### 8.2 仅运行特定测试

修改 `tests/main_global.cpp`，在 `main` 函数中 return 前添加：

```cpp
::testing::GTEST_FLAG(filter) = "*Axpy*";
```

### 8.3 仅编译特定 test target

修改 `build.sh` 中 `build_test()` 函数：

```bash
function build_test() {
    cmake_config
    build ascendc_ut_adv_api_kernel_ascend910B1_AIV     # 替换为目标 target
}
```

然后执行 `bash build.sh -t`。

### 8.4 单算子测试

完成 Kernel 侧代码后，通过搭建自定义算子工程进行端到端测试，详见 [Ascend C 编程指南](https://www.hiascend.com/document/redirect/CannCommunityOpdevAscendC)。

---

## 9. CI/CD 与覆盖率要求

### 9.1 最低要求

| 场景 | 要求 |
|------|------|
| 新增 API | 必须包含 Kernel 侧 UT |
| 新增 API（需要临时空间）| 必须包含 Tiling 侧 UT |
| Bug 修复 | 应补充回归测试用例 |
| 新增公开头文件 | 必须添加 Header Checker |

### 9.2 测试覆盖

- **正常路径**：典型输入下的功能正确性
- **边界条件**：最小/最大输入、空输入、对齐/非对齐
- **错误输入**：参数校验断言触发

### 9.3 Pre-commit 钩子

提交前自动运行：

1. `clang-format` — 代码格式化检查
2. `oat_check.sh` — 许可证合规检查

---

## 10. Mock 测试（C API 专用）

C API 使用 gTest + MockCPP 框架验证指令映射：

```cpp
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include "c_api/stub/cce_stub.h"
#include "c_api/asc_simd.h"
```

验证 C API 是否正确调用了底层硬件指令，但不验证计算结果。

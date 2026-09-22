# C++14语言特性<a name="ZH-CN_TOPIC_000000253823119011"></a>

本文介绍C++14语言特性在Ascend C SIMT Device侧的支持情况。SIMT编程默认按C++17标准编译（CMake的`CMAKE_ASC_STANDARD`默认17），C++14特性自然包含其中。支持情况的判定依据与[C++11语言特性](../cpp11_language_feature/cpp11_language_feature.md)相同：逐特性编写的最小验证用例。完整验证样例请参考[C++14语言特性样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/08_cpp14_features/README.md)。

## C++14语言特性支持列表<a name="section_cpp14_features"></a>

**表1**  C++14语言特性支持列表

| C++14特性 | 支持情况 | 说明 |
| --- | --- | --- |
| 上下文转换调整（Tweak to certain C++ contextual conversions） | ✅ | explicit operator bool在if条件、逻辑非等上下文可用。 |
| 二进制字面量（Binary literals） | ✅ | 支持0b/0B前缀。 |
| 函数返回类型推导（Functions with deduced return type） | ✅ | auto返回类型从return语句推导；Device侧被调用的函数需标记`__aicore__`。 |
| 广义lambda捕获/初始化捕获（Generalized lambda capture） | ✅ | `[sum = expr]`形式可用。 |
| 泛型lambda表达式（Generic lambda expressions） | ✅ | auto参数可用；无捕获泛型lambda特化后可转函数指针。 |
| 变量模板（Variable templates） | ✅ | 支持主模板与偏特化。 |
| 放宽constexpr要求（Relaxing requirements on constexpr functions） | ✅ | constexpr函数内允许局部变量、循环、多返回路径。 |
| 成员初始化器与聚合（Member initializers and aggregates） | ✅ | 含默认成员初始化器的类可作为聚合用花括号初始化。 |
| 内存分配澄清（Clarifying memory allocation） | ❌ | 依赖Device堆动态分配，链接失败（dpp\_alloc未定义）。 |
| sized deallocation | ❌ | 依赖Device堆，`operator delete(void*, size_t)`链接未定义。 |
| \[\[deprecated\]\]属性 | ✅ | C++11属性语法已支持（见[C++11语言特性](../cpp11_language_feature/cpp11_language_feature.md#section_cpp11_core_features)）。 |
| 单引号数字分隔符（Single-quotation-mark as a digit separator） | ✅ | `1'000'000`形式可用。 |

**表2**  C++14库设施支持情况

| 库设施 | 支持情况 | 说明 |
| --- | --- | --- |
| std::integer\_sequence / std::make\_integer\_sequence | ✅ | 编译期序列消解可用（纯模板元编程，无运行时调用）。 |
| std::exchange | ❌ | 被Host标准库头文件标记为`__host__`，Device侧调用报编译错误。语义可用临时变量替代。 |

不支持项的编译期复现方式请参考[C++14/C++17不支持特性样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/10_cpp14_17_unsupported/README.md)。

## 使用注意事项<a name="section_cpp14_notes"></a>

-   **编译标准指定**：默认C++17编译即包含C++14全部特性；显式传`-std=c++14`亦可编译，但SIMT头文件中个别C++17用法会产生扩展告警，属正常现象。
-   **Device侧调用标记**：推导返回类型函数、放宽的constexpr函数被kernel调用时需标记`__aicore__`，规则与C++11一致。
-   **std::exchange替代**：用普通临时变量交换实现同等语义。

    ```cpp
    // 错误示例：std::exchange在Device侧报"call to __host__ function"
    int32_t old = std::exchange(x, 10);
    // 正确示例
    int32_t old = x;
    x = 10;
    ```

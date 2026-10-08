# C++14语言特性<a name="ZH-CN_TOPIC_000000253823119011"></a>

本文介绍C++14语言特性在Ascend C SIMT Device侧的支持情况。SIMT编程默认按C++17标准编译（CMake的`CMAKE_ASC_STANDARD`默认17），C++14特性自然包含其中。支持情况的判定依据与[C++11语言特性](./cpp11_language_feature.md)相同：逐特性编写的最小验证用例。

## C++14语言特性支持列表<a name="section_cpp14_features"></a>

**表1**  C++14语言特性支持列表

| C++14特性 | C++14 Proposal | 支持情况 | 说明 |
| --- | --- | --- | --- |
| 二进制字面量（Binary literals） | [N3472](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3472.pdf) | ✅ | 支持0b/0B前缀。 |
| 广义lambda捕获/初始化捕获（Generalized lambda capture） | [N3648](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3648.html) | ✅ | `[sum = expr]`形式可用。混合编程VF函数内需携带callee标记，见[Lambda表达式](./lambda_expressions.md)。 |
| 泛型lambda表达式（Generic lambda expressions） | [N3649](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3649.html) | ✅ | auto参数可用；无捕获泛型lambda特化后可转函数指针（纯SIMT模式）。混合编程场景见[Lambda表达式](./lambda_expressions.md)。 |
| 放宽constexpr要求（Relaxing requirements on constexpr functions） | [N3652](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html) | ✅ | constexpr函数内允许局部变量、循环、多返回路径。 |
| 成员初始化器与聚合（Member initializers and aggregates） | [N3653](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3653.html) | ✅ | 含默认成员初始化器的类可作为聚合用花括号初始化。 |
| 内存分配澄清（Clarifying memory allocation） | [N3664](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3664.html) | ❌ | 依赖Device堆动态分配，链接失败（dpp\_alloc未定义）。 |
| \[\[deprecated\]\]属性 | [N3760](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3760.html) | ✅ | C++11属性语法已支持（见[C++11语言特性](./cpp11_language_feature.md#section_cpp11_core_features)）。 |
| 单引号数字分隔符（Single-quotation-mark as a digit separator） | [N3781](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3781.pdf) | ✅ | `1'000'000`形式可用。 |


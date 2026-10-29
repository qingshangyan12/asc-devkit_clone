# C++14语言特性

本文介绍C++14语言特性在Ascend C Device侧的支持情况。

**表1**  C++14语言特性支持列表

| C++14特性 | C++14 Proposal | 是否支持 | 说明 |
| --- | --- | --- | --- |
| 二进制字面量 | [N3472](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3472.pdf) | 是 | 支持0b、0B两种前缀。 |
| 广义lambda捕获、初始化捕获 | [N3648](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3648.html) | 是 | `[sum = expr]`形式可用。SIMD与SIMT混合编程场景写法详见[Lambda表达式](./lambda_expressions.md)。 |
| 泛型lambda表达式 | [N3649](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3649.html) | 是 | SIMD与SIMT混合编程场景写法详见[Lambda表达式](./lambda_expressions.md)。 |
| constexpr函数增强 | [N3652](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html) | 是 | constexpr函数内允许局部变量、循环、多返回路径。 |
| 成员初始化器与聚合 | [N3653](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3653.html) | 是 | 含默认成员初始化器的类可作为聚合用花括号初始化。 |
| \[\[deprecated\]\]属性 | [N3760](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3760.html) | 是 | - |
| 单引号数字分隔符 | [N3781](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3781.pdf) | 是 | `1'000'000`形式可用。 |


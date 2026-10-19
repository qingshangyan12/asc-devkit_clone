# C++17语言特性

本文介绍C++17语言特性在Ascend C Device侧的支持情况。

**表1**  C++17语言特性支持列表

| C++17特性 | C++17 Proposal | 是否支持 | 说明 |
| --- | --- | --- | --- |
| 移除trigraphs | [N4086](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4086.html) | 是 | 三字符组不再转译。 |
| u8字符字面量 | [N4267](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4267.html) | 是 | `u8'x'`形式可用。 |
| 命名空间与枚举器属性 | [N4266](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4266.html) | 是 | `namespace [[maybe_unused]] ns {...}`、枚举器携带属性可用。 |
| 嵌套命名空间定义 | [N4230](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4230.html) | 是 | `namespace a::b::c {...}`形式可用。 |
| 非类型模板参数的常量求值 | [N4268](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4268.html) | 是 | — |
| 模板模板参数允许typename | [N4051](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4051.html) | 是 | `template <template <typename> typename TT>`形式可用。 |
| `[[fallthrough]]`属性 | [P0188R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0188r1.pdf) | 是 | — |
| `[[nodiscard]]`属性 | [P0189R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0189r1.pdf) | 是 | 忽略返回值会产生告警。 |
| `[[maybe_unused]]`属性 | [P0212R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0212r1.pdf) | 是 | — |
| constexpr lambda | [P0170R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0170r1.pdf) | 是 | `constexpr auto f = [...] constexpr {...}`编译期调用可用 |
| 一元折叠与空参数包 | [P0036R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0036r0.pdf) | 是 | — |
| lambda按值捕获`*this` | [P0018R3](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0018r3.html) | 是 | `[*this]`捕获当前对象副本，SIMD与SIMT混合编程场景写法见[Lambda表达式](./lambda_expressions.md)。 |
| enum class构造规则 | [P0138R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0138r2.pdf) | 是 | 整型可直接列表初始化enum class。 |
| 十六进制浮点字面量 | [P0245R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0245r1.html) | 是 | `0x1.8p3`形式可用。 |
| `__has_include` | [P0061R1](https://wg21.link/p0061) | 是 | — |

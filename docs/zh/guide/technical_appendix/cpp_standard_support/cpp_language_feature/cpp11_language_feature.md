# C++11语言特性<a name="ZH-CN_TOPIC_000000253823119001"></a>

本文介绍C++11语言特性在Ascend C SIMT Device侧的支持情况。部分特性样例请参考[C++11语言特性支持样例](../../../../../../examples/03_simt_api/02_features/02_language_features/README.md)。

## 核心语言特性支持列表<a name="section_cpp11_core_features"></a>

下表列出C++11核心语言特性在SIMT Device侧的支持情况。其中大部分特性在核函数内直接使用即可；个别特性的注意事项见[使用注意事项](#section_cpp11_notes)。

**表1**  C++11核心语言特性支持列表

| C++11特性 | C++11 Proposal | 支持情况 | 说明 |
| --- | --- | --- | --- |
| \*this的右值引用（Rvalue references for \*this） | [N2439](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2439.htm)| ✅ | 支持`func() &`、`func() &&`、`func() const &`引用限定成员函数重载。 |
| 右值初始化类对象（Initialization of class objects by rvalues） | [N1610](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1610.html) | ✅ | 工厂函数返回右值时移动构造或返回值优化（RVO）均生效。 |
| 变参模板（Variadic templates） | [N2242](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf) | ✅ | 支持参数包、`sizeof...`、包展开进数组初始化/函数实参/基类列表、递归继承展开。 |
| 静态断言（Static assertions） | [N1720](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1720.html) | ✅ | 支持命名空间级与函数内`static_assert`，可在模板中使用。 |
| auto类型变量（auto-typed variables） | [N1984](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf) | ✅ | 支持从初始化表达式、lambda表达式推导类型。 |
| 多声明符auto（Multi-declarator auto） | [N1737](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1737.pdf) | ✅ | 一条语句声明多个auto变量，要求各声明符推导类型一致。 |
| Lambda表达式（Lambda expressions） | [N2927](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2927.pdf) | ✅ | 纯SIMT核函数内直接使用；SIMD/SIMT混合编程场景需显式标记函数属性（标记选择规则与逐项验证见[Lambda表达式](./lambda_expressions.md)）。 |
| 表达式的声明类型（Declared type of an expression，decltype） | [N2343](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2343.pdf) | ✅ | 值类别语义与标准一致：具名实体推导声明类型，表达式按左值/右值推导引用。 |
| 右尖括号（Right angle brackets） | [N1757](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1757.html) | ✅ | 嵌套模板关闭符`>>`无需空格分隔。 |
| 别名模板（Alias templates） | [N2258](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2258.pdf) | ✅ | 支持`template <typename T> using Alias = ...`及元函数级联。 |
| 空指针常量（Null pointer constant，nullptr） | [N2431](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf) | ✅ | 支持nullptr初始化、比较与函数重载决议。 |
| 强类型枚举（Strongly-typed enums） | [N2347](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2347.pdf) | ✅ | 支持enum class及底层类型指定。 |
| 对齐支持（Alignment support） | [N2341](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf) | ✅ | 支持alignas/alignof，可作用于类、全局与局部变量。 |
| 条件支持行为（Conditionally-supported behavior） | [N1627](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1627.pdf) | ✅ | `__int128`扩展整型在Device侧可用。 |
| 委托构造函数（Delegating constructors） | [N1986](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1986.pdf) | ✅ | 支持构造函数委托链。 |
| 显式转换运算符（Explicit conversion operators） | [N2437](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2437.pdf) | ✅ | 支持`explicit operator T`。 |
| 新字符类型（New character types） | [N2249](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2249.html) | ✅ | 支持char16_t/char32_t作为算术类型参与运算。 |
| 字面量中的通用字符名（Universal character names in literals） | [N2170](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2170.html) | ✅ | 支持UCN的UTF-8落地形式。 |
| 标准布局类型（Standard Layout Types） | [N2342](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2342.htm) | ✅ | `std::is_standard_layout`判定与标准一致，`<type_traits>`可在Device侧使用。 |
| defaulted函数（Defaulted functions） | [N2346](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm) | ✅ | 构造/拷贝/移动/赋值/析构均支持`= default`。 |
| deleted函数（Deleted functions） | [N2346](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm) | ✅ | 支持`= delete`，可构造move-only类型。 |
| 扩展friend声明（Extended friend declarations） | [N1791](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1791.pdf) | ✅ | 支持模板友元不写`<>`的扩展形式。 |
| 扩展sizeof（Extending sizeof） | [N2253](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2253.html)、[DR850](https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#850) | ✅ | 支持类成员sizeof（无需实例）与`sizeof...`参数包。 |
| 定义移动特殊成员函数（Defining move special member functions） | [N3053](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3053.html) | ✅ | 支持移动构造/移动赋值的定义与使用。 |

## C++11收录的C99特性支持列表<a name="section_cpp11_c99_features"></a>

**表2**  C99特性支持列表

| C99特性 | C++11 Proposal | 支持情况 | 说明 |
| --- | --- | --- | --- |
| \_\_func\_\_预定义标识符 | [N2340](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2340.htm) | ✅ | 函数内隐式静态字符串数组可用。 |
| C99预处理器（C99 preprocessor） | [N1653](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1653.htm) | ✅ | 支持可变参数宏`__VA_ARGS__`、记号粘贴`##`、字符串化`#`。GNU扩展`##__VA_ARGS__`空参吞并在可变参数为空时不可用，需以函数重载等方式替代。 |
| long long | [N1811](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1811.pdf) | ✅ | 至少64位，支持LL/ULL后缀与宽位数算术。 |
| 扩展整型（Extended integral types） | [N1988](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1988.pdf) | ✅ | `<stdint.h>`/`<cinttypes>`的intmax\_t/uintmax\_t/INT64\_C等在Device侧可用。 |


# C++11语言特性

本文介绍C++11语言特性在Ascend C Device侧的支持情况。下表列出C++11核心语言特性在Device侧的支持情况。

**表1**  C++11核心语言特性列表

| C++11特性 | C++11 Proposal | 是否支持 | 说明 |
| --- | --- | --- | --- |
| \*this的右值引用（Rvalue references for \*this） | [N2439](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2439.htm)| 是 | 支持`func() &`、`func() &&`、`func() const &`引用限定成员函数重载。 |
| 右值初始化类对象 | [N1610](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1610.html) | 是 | 工厂函数返回右值时移动构造或返回值优化均生效。 |
| 变参模板 | [N2242](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2242.pdf) | 是 | 支持参数包、`sizeof...`、包展开进数组初始化、函数实参、基类列表，递归继承展开。 |
| 静态断言 | [N1720](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1720.html) | 是 | 支持命名空间级与函数内`static_assert`，可在模板中使用。 |
| auto类型变量 | [N1984](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1984.pdf) | 是 | 支持从初始化表达式、lambda表达式推导类型。注意Host侧`auto dev = nullptr;`声明的设备指针经`aclrtMalloc`传入核函数时Device地址不生效，设备指针应以显式指针类型声明。 |
| 多声明符auto | [N1737](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1737.pdf) | 是 | 一条语句声明多个auto变量，要求各声明符推导类型一致，不一致时编译报错。 |
| Lambda表达式 | [N2927](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2927.pdf) | 是 | SIMD与SIMT混合编程场景需显式标记函数属性，详见[Lambda表达式](./lambda_expressions.md)。 |
| 表达式的声明类型（Declared type of an expression，decltype） | [N2343](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2343.pdf) | 是 | 值类别语义与标准一致：具名实体推导声明类型，表达式按左值、右值推导引用。 |
| 右尖括号 | [N1757](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1757.html) | 是 | 嵌套模板关闭符`>>`无需空格分隔。 |
| 别名模板 | [N2258](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2258.pdf) | 是 | 支持`template <typename T> using Alias = ...`及元函数级联。|
| 空指针常量（Null pointer constant，nullptr） | [N2431](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf) | 是 | 支持nullptr初始化、比较与函数重载决议。 |
| 强类型枚举 | [N2347](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2347.pdf) | 是 | 支持enum class及底层类型指定。 |
| 对齐支持 | [N2341](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2341.pdf) | 是 | 支持alignas、alignof，可作用于类、全局与局部变量。 |
| 条件支持行为 | [N1627](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1627.pdf) | 是 | `__int128`扩展整型在Device侧可用。 |
| 委托构造函数 | [N1986](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1986.pdf) | 是 | 支持构造函数委托链。 |
| 显式转换运算符 | [N2437](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2437.pdf) | 是 | 支持`explicit operator T`。 |
| 新字符类型 | [N2249](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2249.html) | 是 | 支持char16_t、char32_t作为算术类型参与运算。 |
| 字面量中的通用字符名 | [N2170](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2170.html) | 是 | 支持UCN的UTF-8落地形式。 |
| 标准布局类型 | [N2342](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2342.htm) | 是 | `std::is_standard_layout`判定与标准一致，`<type_traits>`可在Device侧使用。 |
| defaulted函数 | [N2346](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm) | 是 | 构造、拷贝、移动、赋值、析构均支持`= default`。 |
| deleted函数 | [N2346](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm) | 是 | 支持`= delete`，可构造move-only类型。 |
| 扩展friend声明 | [N1791](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1791.pdf) | 是 | 支持模板友元不写`<>`的扩展形式。 |
| 扩展sizeof | [N2253](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2253.html)、[DR850](https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#850) | 是 | 支持类成员sizeof且无需实例，以及`sizeof...`参数包。 |
| 定义移动特殊成员函数 | [N3053](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3053.html) | 是 | 支持移动构造与移动赋值的定义、使用。 |

下表汇总了Device侧对C++11收录的C99特性的支持情况

**表2**  C99特性列表

| C99特性 | C++11 Proposal | 支持情况 | 说明 |
| --- | --- | --- | --- |
| \_\_func\_\_预定义标识符 | [N2340](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2340.htm) | 是 | 函数内隐式静态字符串数组可用。 |
| C99预处理器 | [N1653](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1653.htm) | 是 | 支持可变参数宏`__VA_ARGS__`、记号粘贴`##`、字符串化`#`。GNU扩展`##__VA_ARGS__`空参吞并在可变参数为空时不可用，需以函数重载等方式替代。 |
| long long | [N1811](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1811.pdf) | 是 | 至少64位，支持LL、ULL后缀与宽位数算术。 |
| 扩展整型 | [N1988](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1988.pdf) | 是 | `<stdint.h>`与`<cinttypes>`的intmax\_t、uintmax\_t、INT64\_C等在Device侧可用。 |


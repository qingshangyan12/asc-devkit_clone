# C++11语言特性<a name="ZH-CN_TOPIC_000000253823119001"></a>

本文介绍C++11语言特性在Ascend C SIMT Device侧的支持情况。支持情况的判定依据为逐特性编写的最小验证用例：每个特性在核函数（Kernel）中实际使用并参与计算，Host侧比对期望值；不支持项以编译期报错为证据。完整验证样例请参考[C++11语言特性支持样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/README.md)。

## 核心语言特性支持列表<a name="section_cpp11_core_features"></a>

下表列出C++11核心语言特性在SIMT Device侧的支持情况。其中大部分特性在核函数内直接使用即可；个别特性的注意事项见[使用注意事项](#section_cpp11_notes)。

**表1**  C++11核心语言特性支持列表

| C++11特性 | 支持情况 | 说明 |
| --- | --- | --- |
| 右值引用（Rvalue references） | ✅ | 支持右值引用绑定、移动语义完整链路。`std::move`不可用，可用`static_cast<T&&>(x)`替代，见[使用注意事项](#section_cpp11_notes)。 |
| \*this的右值引用（Rvalue references for \*this） | ✅ | 支持`func() &`、`func() &&`、`func() const &`引用限定成员函数重载。 |
| 右值初始化类对象（Initialization of class objects by rvalues） | ✅ | 工厂函数返回右值时移动构造或返回值优化（RVO）均生效。 |
| 非静态成员初始化器（Non-static data member initializers） | ✅ | 成员缺省值在默认构造时生效，可与委托构造、继承构造共存。 |
| 变参模板（Variadic templates） | ✅ | 支持参数包、`sizeof...`、包展开进数组初始化/函数实参/基类列表、递归继承展开。 |
| 扩展变参模板模板参数（Extending variadic template template parameters） | ✅ | 支持`template <template <typename...> class TT, typename... Ts>`形式。 |
| 初始化列表（Initializer lists） | ⚠️ | 花括号`{}`统一初始化、聚合初始化、`std::initializer_list`构造函数均可用；但`std::initializer_list`的`begin()/end()/size()`成员被Host标准库头文件标记为`__host__`，Device侧无法直接调用，见[使用注意事项](#section_cpp11_notes)。 |
| 静态断言（Static assertions） | ✅ | 支持命名空间级与函数内`static_assert`，可在模板中使用。 |
| auto类型变量（auto-typed variables） | ✅ | 支持从初始化表达式、lambda表达式推导类型。 |
| 多声明符auto（Multi-declarator auto） | ✅ | 一条语句声明多个auto变量，要求各声明符推导类型一致。 |
| 移除auto存储类说明符（Removal of auto as a storage-class specifier） | ✅ | C++11前的`auto int x;`旧写法仅产生告警，语义已移除。 |
| 新函数声明语法（New function declarator syntax） | ✅ | 支持后置返回类型`auto f(...) -> T`，可与decltype组合。 |
| Lambda表达式（Lambda expressions） | ✅ | 纯SIMT核函数内直接使用；SIMD/SIMT混合编程场景需显式标记函数属性，详见[Lambda表达式](../lamda_expressions/lamda_expressions.md)。 |
| 表达式的声明类型（Declared type of an expression，decltype） | ✅ | 值类别语义与标准一致：具名实体推导声明类型，表达式按左值/右值推导引用。 |
| 不完整返回类型（Incomplete return types） | ✅ | 支持声明返回不完整类型及其指针、引用的函数。 |
| 右尖括号（Right angle brackets） | ✅ | 嵌套模板关闭符`>>`无需空格分隔。 |
| 函数模板默认实参（Default template arguments for function templates） | ✅ | 支持函数模板指定默认模板实参。 |
| 表达式SFINAE（Solving the SFINAE problem for expressions） | ✅ | 支持`decltype`参与模板形参推导与重载决议。 |
| 别名模板（Alias templates） | ✅ | 支持`template <typename T> using Alias = ...`及元函数级联。 |
| extern模板（Extern templates） | ✅ | 支持显式实例化声明与定义。 |
| 空指针常量（Null pointer constant，nullptr） | ✅ | 支持nullptr初始化、比较与函数重载决议。 |
| 强类型枚举（Strongly-typed enums） | ✅ | 支持enum class及底层类型指定。 |
| 枚举前向声明（Forward declarations for enums） | ✅ | scoped枚举可不指定底层类型前向声明；unscoped枚举需指定底层类型。 |
| 标准属性语法（Standardized attribute syntax） | ✅ | 支持`[[deprecated]]`、`[[noreturn]]`等。 |
| 广义常量表达式（Generalized constant expressions，constexpr） | ✅ | 支持constexpr递归函数、constexpr构造/成员函数、编译期与运行期双态求值、constexpr全局只读表运行期索引。 |
| 对齐支持（Alignment support） | ✅ | 支持alignas/alignof，可作用于类、全局与局部变量。 |
| 条件支持行为（Conditionally-supported behavior） | ✅ | `__int128`扩展整型在Device侧可用。 |
| 未定义行为转为可诊断错误（Changing undefined behavior into diagnosable errors） | ✅ | static_assert等诊断路径可用；未序列化修改等场景的诊断策略与Host侧clang一致。 |
| 委托构造函数（Delegating constructors） | ✅ | 支持构造函数委托链。 |
| 继承构造函数（Inheriting constructors） | ✅ | 支持`using Base::Base;`；遵循标准语义，被继承构造初始化其余基类时要求其可默认构造。 |
| 显式转换运算符（Explicit conversion operators） | ✅ | 支持`explicit operator T`。 |
| 新字符类型（New character types） | ✅ | 支持char16_t/char32_t作为算术类型参与运算。 |
| Unicode字符串字面量（Unicode string literals） | ✅ | 支持u""、U""、u8""前缀。 |
| 原始字符串字面量（Raw string literals） | ✅ | 支持R"(...)"，内容原样保留。 |
| 字面量中的通用字符名（Universal character names in literals） | ✅ | 支持UCN的UTF-8落地形式。 |
| 用户自定义字面量（User-defined literals） | ✅ | 支持整型、浮点、字符串字面量运算符。 |
| 标准布局类型（Standard Layout Types） | ✅ | `std::is_standard_layout`判定与标准一致，`<type_traits>`可在Device侧使用。 |
| defaulted函数（Defaulted functions） | ✅ | 构造/拷贝/移动/赋值/析构均支持`= default`。 |
| deleted函数（Deleted functions） | ✅ | 支持`= delete`，可构造move-only类型。 |
| 扩展friend声明（Extended friend declarations） | ✅ | 支持模板友元不写`<>`的扩展形式。 |
| 扩展sizeof（Extending sizeof） | ✅ | 支持类成员sizeof（无需实例）与`sizeof...`参数包。 |
| 内联命名空间（Inline namespaces） | ✅ | 支持inline namespace及版本化访问语义。 |
| 非受限union（Unrestricted unions） | ✅ | 支持含非平凡成员的union（需用户定义构造/析构）及匿名union。 |
| 局部与匿名类型作为模板实参（Local and unnamed types as template arguments） | ✅ | 核函数内定义的局部类型可实例化模板。 |
| 基于范围的for（Range-based for） | ⚠️ | 数组与自定义begin()/end()区间类型可用；花括号列表形式`for (auto v : {1, 2})`因依赖`std::initializer_list::begin()`不可用。 |
| 显式virtual覆盖（Explicit virtual overrides） | ✅ | 支持override与final。虚函数使用注意事项见[使用注意事项](#section_cpp11_notes)。 |
| 垃圾回收最小支持（Minimal support for garbage collection） | ❌ | `std::declare_no_pointers`等接口被标记为`__host__`；Device侧无GC模型，无需该机制。 |
| 允许移动构造抛出（Allowing move constructors to throw \[noexcept\]） | ✅ | 支持`noexcept(false)`移动构造与noexcept运算符编译期判定。 |
| 定义移动特殊成员函数（Defining move special member functions） | ✅ | 支持移动构造/移动赋值的定义与使用。 |

## C++11收录的C99特性支持列表<a name="section_cpp11_c99_features"></a>

**表2**  C99特性支持列表

| C99特性 | 支持情况 | 说明 |
| --- | --- | --- |
| \_\_func\_\_预定义标识符 | ✅ | 函数内隐式静态字符串数组可用。 |
| C99预处理器（C99 preprocessor） | ✅ | 支持可变参数宏`__VA_ARGS__`、记号粘贴`##`、字符串化`#`。GNU扩展`##__VA_ARGS__`空参吞并在可变参数为空时不可用，需以函数重载等方式替代。 |
| long long | ✅ | 至少64位，支持LL/ULL后缀与宽位数算术。 |
| 扩展整型（Extended integral types） | ✅ | `<stdint.h>`/`<cinttypes>`的intmax\_t/uintmax\_t/INT64\_C等在Device侧可用。 |

## 使用注意事项<a name="section_cpp11_notes"></a>

以下注意事项来自逐特性实测，均可在[验证样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/README.md)中找到对应的最小复现。

-   **Device侧函数需标记`__aicore__`**：未标记的普通函数（含constexpr函数、字面量运算符、namespace-scope静态变量）会被视为Host函数，从核函数调用时报"call to \_\_host\_\_ function"或"reference to \_\_host\_\_ variable"编译错误。标记示例：

    ```cpp
    __aicore__ constexpr uint32_t power(uint32_t base, uint32_t exp)
    {
        return exp == 0 ? 1u : base * power(base, exp - 1u);
    }
    ```

-   **`std::move`/`std::forward`不可用**：被Host标准库头文件标记为`__host__`。语言层的右值引用完全可用，用`static_cast<T&&>(x)`替代即可。

    ```cpp
    // 错误示例：std::move在Device侧报"call to __host__ function"
    float&& r = std::move(x);
    // 正确示例：直接使用右值引用转换
    float&& r = static_cast<float&&>(x);
    ```

-   **`std::initializer_list`成员函数不可直接调用**：`begin()/end()/size()`被标记为`__host__`（源自其`noexcept`函数属性）。使用`std::initializer_list`构造函数时，可按其ABI布局（存储为"首元素指针、元素个数"两个字段）直接访问元素：

    ```cpp
    struct VecInit {
        float data[4];
        uint32_t n;
        __aicore__ VecInit(std::initializer_list<float> l) : n(0)
        {
            const float* p = reinterpret_cast<const float**>(const_cast<std::initializer_list<float>*>(&l))[0];
            size_t len = reinterpret_cast<const size_t*>(const_cast<std::initializer_list<float>*>(&l))[1];
            const float* e = p + len;
            for (; p != e && n < 4; ++p) {
                data[n++] = *p;
            }
        }
    };
    ```

-   **虚函数与虚析构**：SIMT Device侧的虚函数调用、虚继承实测可用。但virtual析构函数会引入`operator delete`依赖，Device侧无动态释放机制，导致链接失败（`dpp_dealloc`未定义）。编写含多态的Device侧类型时应避免virtual析构，改用非虚析构加显式管理。

-   **namespace-scope的constexpr对象**：编译期初始化的POD constexpr全局数组可在Device侧运行期索引；但构造函数标记`__aicore__`的对象不可作全局动态初始化，会报"dynamic initialization is not supported for \_\_device\_\_ ... variables"。需要全局常量时使用constexpr POD，或在核函数内构造。

-   **用户自定义字面量匹配规则**：整型后缀匹配`unsigned long long`、浮点后缀匹配`long double`、字符串后缀匹配`(const char*, size_t)`。注意`90_fdeg`会因字面量形式为整型而匹配失败，浮点UDL须写作`90.0_fdeg`。

-   **核函数参数支持POD结构体**：基础类型组成的结构体（支持嵌套）可按值传入核函数，结构体内的NSDMI等C++11特性不影响其作为参数的合法性，具体约束见[核函数（Kernel）参数列表限制](../../syntax_restrictions/functions.md#section_kernel_function_argument_list_constraint)。

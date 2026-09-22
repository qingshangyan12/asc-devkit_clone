# C++17语言特性<a name="ZH-CN_TOPIC_000000253823119012"></a>

本文介绍C++17语言特性在Ascend C SIMT Device侧的支持情况。SIMT编程默认按C++17标准编译（CMake的`CMAKE_ASC_STANDARD`默认17），C++17语言特性在Device侧整体支持良好。支持情况的判定依据与[C++11语言特性](../cpp11_language_feature/cpp11_language_feature.md)相同。完整验证样例请参考[C++17语言特性样例](../../../../../../../examples/03_simt_api/02_features/02_language_features/09_cpp17_features/README.md)。

## C++17语言特性支持列表<a name="section_cpp17_features"></a>

**表1**  C++17语言特性支持列表

| C++17特性 | 支持情况 | 说明 |
| --- | --- | --- |
| 移除trigraphs | ✅ | 三字符组不再转译。 |
| u8字符字面量 | ✅ | `u8'x'`形式可用。 |
| 折叠表达式（Folding expressions） | ✅ | 一元/二元折叠、空包折叠（空包&&折叠为true）、逗号折叠均可用。 |
| 命名空间与枚举器属性 | ✅ | `namespace [[maybe_unused]] ns {...}`、枚举器携带属性可用。 |
| 嵌套命名空间定义 | ✅ | `namespace a::b::c {...}`形式可用。 |
| 非类型模板参数的常量求值 | ✅ | — |
| 扩展static\_assert | ✅ | 无消息形式可用。 |
| auto从花括号初始化列表推导新规则 | ✅ | `auto x{1}`推导为int（C++17前为initializer\_list）。 |
| 模板模板参数允许typename | ✅ | `template <template <typename> typename TT>`形式可用。 |
| \[\[fallthrough\]\]属性 | ✅ | — |
| \[\[nodiscard\]\]属性 | ✅ | 忽略返回值会产生告警。 |
| \[\[maybe\_unused\]\]属性 | ✅ | — |
| 聚合初始化扩展（带基类） | ✅ | 含基类的聚合可直接花括号初始化。 |
| constexpr lambda | ✅ | `constexpr auto f = [...] constexpr {...}`编译期调用可用。 |
| 一元折叠与空参数包 | ✅ | — |
| 泛化range-based for | ✅ | begin/end成员形式可用（与C++11结论一致）。 |
| lambda按值捕获\*this | ✅ | `[*this]`捕获当前对象副本。 |
| enum class构造规则 | ✅ | 整型可直接列表初始化enum class。 |
| 十六进制浮点字面量 | ✅ | `0x1.8p3`形式可用。 |
| 过度对齐数据的动态分配 | ❌ | 依赖Device堆动态分配，链接失败（operator new/align\_val\_t未定义）。 |
| 保证拷贝消除（Guaranteed copy elision） | ✅ | 删除拷贝/移动构造的类型经工厂函数返回可用。 |
| 求值顺序细化 | ✅ | — |
| constexpr if | ✅ | 编译期分支裁剪可用，与折叠表达式组合良好。 |
| 带初始化的选择语句 | ✅ | `if (init; cond)` / `switch (init; cond)`可用。 |
| 类模板参数推导（CTAD） | ⚠️ | 从构造函数实参推导可用；推导指引（deduction guide）无法标记`__aicore__`而被视为Host函数，Device侧不可用；纯聚合的CTAD为C++20特性。 |
| auto非类型模板参数声明 | ✅ | `template <auto N>`可用。 |
| 属性命名空间免重复 | ✅ | — |
| 忽略不支持的非标准属性 | ✅ | — |
| 结构化绑定 | ✅ | 值绑定/引用绑定/嵌套成员/数组形式均可用。 |
| 移除register关键字 | ✅ | — |
| 移除operator++(bool) | ✅ | — |
| 异常规格作为类型系统一部分 | ⚠️ | 语法可用；Device侧无异常机制，try/throw仍不可用（见[C++11限制](../cpp11_restrictions/cpp11_restrictions.md#section_cpp11_concurrency_restrictions)）。 |
| \_\_has\_include | ✅ | — |
| 继承构造措辞修订（P0136） | ✅ | 继承构造函数可用（见[C++11语言特性](../cpp11_language_feature/cpp11_language_feature.md#section_cpp11_core_features)）。 |
| 内联变量（Inline variables） | ⚠️ | `inline constexpr` POD常量可用；普通inline变量被kernel读取时需标记`__aicore__`（与C++11全局变量规则一致）。 |
| DR150模板模板实参匹配 | ❌ | 实测本编译器仍要求模板模板实参与形参的模板参数严格一致，宽松匹配报"different template parameters"编译错误。 |
| 移除动态异常规格 | ✅ | — |
| using声明中的包展开 | ✅ | `using Ts::operator()...`形式可用。 |
| byte类型定义 | ⚠️ | std::byte类型可用；`std::to_integer`被标记`__host__`，需用底层类型转换替代。 |
| DR727类内显式实例化 | ✅ | — |

**表2**  C++17库设施支持情况

| 库设施 | 支持情况 | 说明 |
| --- | --- | --- |
| \<type\_traits\>新增trait（is\_aggregate、has\_unique\_object\_representations等） | ✅ | C++14/C++17新增trait的编译期判定可用。 |
| std::byte | ⚠️ | 类型可用；std::to\_integer被标记`__host__`。 |

## 使用注意事项<a name="section_cpp17_notes"></a>

-   **CTAD的编写方式**：为类型提供带`__aicore__`标记的构造函数，从构造实参隐式推导：

    ```cpp
    template <typename T>
    struct Box {
        T v;
        __aicore__ Box(T pv) : v(pv) {}   // 提供构造函数：Box bx{5}; 推导Box<int>
    };
    ```

    推导指引`template <typename T> Box(T) -> Box<T>;`无法标记`__aicore__`，会被拒绝用于Device代码，因此纯聚合类型的CTAD不可用。

-   **内联变量标记**：namespace-scope的`inline`变量若被kernel读取，需标记`__aicore__`；`inline constexpr` POD常量无需标记：

    ```cpp
    __aicore__ inline int32_t g_inlineVar = 3;   // 被kernel读取的inline变量
    inline constexpr uint32_t INLINE_V = 77;      // POD常量无需标记
    ```

-   **std::byte取值**：用底层类型转换替代std::to\_integer：

    ```cpp
    std::byte b{10};
    // 错误示例：std::to_integer在Device侧报"call to __host__ function"
    int v = std::to_integer<int>(b);
    // 正确示例
    int v = static_cast<unsigned char>(b);
    ```

-   **模板模板参数严格匹配**：编写模板模板形参时，传入的模板实参的模板参数列表需与形参完全一致（参数个数与默认实参），不要依赖P0522的宽松匹配规则。

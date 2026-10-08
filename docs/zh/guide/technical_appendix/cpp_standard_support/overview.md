# 概述<a name="ZH-CN_TOPIC_0000002509891888"></a>

本文介绍C/C++语言标准在Ascend C中的支持情况，包括语言标准的选择方式、Device侧的能力支持边界，以及本章节的组织结构。Ascend C源文件由bisheng编译器编译，Host侧与Device侧代码在同一编译单元内统一处理，整体支持情况如下：

-   Host侧与clang15一致，支持完整的C/C++标准。
-   Device侧支持C++11、C++14、C++17与C++20四档语言标准，默认按C++17编译。

## 语言标准的选择<a name="section_selecting_cpp_standard"></a>

通过`-std=c++<version>`（或`--std=c++<version>`）编译选项为编译单元指定C++语言标准，SIMD与SIMT编程场景均适用。该选项启用所选标准的全部语言特性，并同时作用于Host侧与Device侧的编译过程。Device侧可选择的语言标准及对应编译选项如下：

-   C++11（ISO/IEC 14882:2011）：`-std=c++11`
-   C++14（ISO/IEC 14882:2014）：`-std=c++14`
-   C++17（ISO/IEC 14882:2017）：`-std=c++17`，默认值
-   C++20（ISO/IEC 14882:2020）：`-std=c++20`

Host侧与clang15一致，支持C++98至C++20及后续各档标准。低于C++11的标准（如C++98/03）Device侧不支持。

**表1**  语言标准的指定方式

| 指定方式 | 用法 | 说明 |
| --- | --- | --- |
| bisheng命令行编译 | 在编译命令中追加`-std=c++<version>` | 例如`bisheng <source_file>.asc -o <output_file> --npu-arch=dav-<npu architecture> -std=c++20`，SIMT编程场景另需追加`--enable-simt`。完整编译命令说明参考[AI Core编译基本用法](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md)。 |
| CMake工程 | 设置`CMAKE_ASC_STANDARD`变量 | 编译时自动注入`-std=c++<version>`，默认值为17；设置为低于17的值时CMake会发出警告。 |

> [!NOTE]说明
> 指定低于C++17的标准（如`-std=c++11`）时，部分高版本语言特性会以clang扩展告警的形式继续可用，而非全部变为编译错误；需要严格限制语言版本时，可配合`-Werror`编译选项将此类告警升级为错误。

## 语言特性与限制导航<a name="section_reading_navigation"></a>

-   **[C++语言支持](./cpp_language_feature/cpp_language_feature.md)**：C++语言特性在SIMT Device侧的逐项支持情况，各项结论以逐特性编写的最小验证用例为依据。
    -   [C++11语言特性](./cpp_language_feature/cpp11_language_feature.md)：C++11核心语言特性与C99特性在Device侧的逐项支持列表及使用注意事项。
    -   [C++14语言特性](./cpp_language_feature/cpp14_language_feature.md)：C++14语言特性在Device侧的逐项支持列表及使用注意事项。
    -   [C++17语言特性](./cpp_language_feature/cpp17_language_feature.md)：C++17语言特性在Device侧的逐项支持列表及使用注意事项。
    -   [Lambda表达式](./cpp_language_feature/lambda_expressions.md)：纯SIMT与SIMD/SIMT混合编程两种场景下Lambda表达式的使用方式与标记要求。
-   **[Device侧C/C++语法限制](./syntax_restrictions/syntax_restrictions.md)**：Device侧通用的语法与能力限制。
    -   [不支持的特性](./syntax_restrictions/unsupport_features.md)：虚函数、动态内存、STL、运行时typeid、文件系统IO等能力限制总览。
    -   [函数](./syntax_restrictions/functions.md)：核函数（Kernel）参数列表限制、SIMT VF函数限制、SIMD与SIMT混合编程结构体使用限制。
    -   [C++11限制](./syntax_restrictions/cpp11_restrictions.md)：并发类与运行时类特性的限制、根因分类，以及并发能力的SIMT API替代方案。


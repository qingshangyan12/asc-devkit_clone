# 概述<a name="ZH-CN_TOPIC_0000002509891888"></a>

Host侧与clang15一致，支持完整的C/C++标准。

Device侧，默认支持C++11标准，支持指定C++14、C++17、C++20。由于硬件限制，部分C++运行时能力无法支持。

SIMT编程场景下C++语言特性的逐项支持情况、限制及Lambda表达式的使用方式，请参考[SIMT C++语言支持](./simt_cpp_language_support/simt_cpp_language_support.md)；通用语法限制请参考[Device侧C/C++语法限制](./syntax_restrictions/syntax_restrictions.md)。


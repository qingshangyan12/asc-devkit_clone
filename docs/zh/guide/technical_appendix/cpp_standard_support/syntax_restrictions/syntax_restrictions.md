# Device侧C/C++语法限制

本文介绍Device侧通用的语法与能力限制，包含三部分内容：

-   **[不支持的特性](./unsupport_features.md)**：虚函数、动态内存、STL、运行时typeid、文件系统IO等能力限制总览。
-   **[函数](./functions.md)**：核函数（Kernel）参数列表限制、SIMT VF函数限制、SIMD与SIMT混合编程结构体使用限制。
-   **[C++11限制](./cpp11_restrictions.md)**：并发类与运行时类特性的限制、根因分类，以及并发能力的SIMT API替代方案。

# Device侧C/C++语法限制

本文介绍Device侧通用的语法与能力限制，包含四部分内容：

-   **[不支持的特性](./unsupport_features.md)**：虚函数、动态内存、STL、运行时typeid、文件系统IO等能力限制总览。
-   **[函数](./functions.md)**：核函数（Kernel）参数列表限制、SIMT VF函数限制、SIMD与SIMT混合编程结构体使用限制。
-   **[C++ 11语法约束限制](./cpp11_restrictions.md)**：auto推导的设备指针传入核函数的限制、多声明符auto类型一致性要求、auto存储类说明符旧写法；核函数模板不支持以依赖模板参数的别名模板作形参。

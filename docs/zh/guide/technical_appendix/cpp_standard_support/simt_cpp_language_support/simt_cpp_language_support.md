# SIMT C++语言支持<a name="ZH-CN_TOPIC_000000253823119004"></a>

本文介绍C++语言特性在Ascend C SIMT Device侧的支持情况，包含五部分内容：

-   **[C++11语言特性](./cpp11_language_feature/cpp11_language_feature.md)**：C++11核心语言特性与C99特性在Device侧的逐项支持列表及使用注意事项。
-   **[C++11限制](./cpp11_restrictions/cpp11_restrictions.md)**：并发类与运行时类特性的限制、根因分类，以及并发能力的SIMT API替代方案。
-   **[C++14语言特性](./cpp14_language_feature/cpp14_language_feature.md)**：C++14语言特性在Device侧的支持列表及使用注意事项。
-   **[C++17语言特性](./cpp17_language_feature/cpp17_language_feature.md)**：C++17语言特性在Device侧的支持列表及使用注意事项。
-   **[Lambda表达式](./lambda_expressions/lambda_expressions.md)**：纯SIMT与SIMD/SIMT混合编程两种场景下Lambda表达式的使用方式与标记要求。

各项支持结论均以逐特性编写的最小验证用例为依据：特性在核函数中实际使用并参与计算、Host侧比对期望值，不支持项以编译期报错为证据。验证样例位于[examples/03_simt_api/02_features/02_language_features](../../../../../../examples/03_simt_api/02_features/02_language_features/README.md)，样例的编译运行方式与验证方法说明见其README。

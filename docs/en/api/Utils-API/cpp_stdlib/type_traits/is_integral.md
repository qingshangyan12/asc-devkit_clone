# is\_integral

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:35:01.449Z pushedAt=2026-10-23T02:53:03.800Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
- Atlas inference products Vector Core: Not supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Not supported
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/Utils-API/cpp_stdlib/type_traits/is_integral_res.md#id1 -->

## Description

Detects whether a type is an integral type at compile time, which can be used for compile-time type checking and conditional processing.

## Prototype

```
template <typename T>
struct is_integral;
```

## Parameters

**Table 1** Template parameters

| Parameter | Description |
| --- | --- |
| T | Type to be detected, including basic data types, modified types, and so on. |

## Constraints

None

## Return Value

The static constant member value of is\_integral is used to obtain the returned boolean value. is\_integral<T\>::value takes the following values:

-   true: Tp is an array type.
-   false: Tp is not an array type.

## Examples

```
// Custom test type.
struct MyStruct{};
using FuncType = int(int);

// legal integral type
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<bool>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<char>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<int>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<long long>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<unsigned int>::value);

// Integer type limited by CV
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<const int>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<volatile long>::value);

// non-integral type
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<float>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<double>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<int*>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<int&>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<int[5]>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<FuncType>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<MyStruct>::value);
AscendC::printf("AscendC::Std::is_integral::value:%d\n", AscendC::Std::is_integral<void>::value);
```

```
// Execution result:
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:1
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
AscendC::Std::is_integral::value:0
```

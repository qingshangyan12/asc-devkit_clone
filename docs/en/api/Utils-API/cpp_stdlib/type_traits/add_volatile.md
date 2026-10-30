# add\_volatile

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:27:18.213Z pushedAt=2026-10-23T02:50:00.014Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/Utils-API/cpp_stdlib/type_traits/add_volatile_res.md#id1 -->

## Description

At compile time, adds the volatile qualifier to a specified type, which can be used for type conversion during compilation.

## Prototype

```
template <typename Tp>
struct add_volatile;
```

## Parameters

**Table 1** Template parameters

| Parameter | Description |
| --- | --- |
| Tp | Type to be processed, including basic types (such as int and float), compound types (such as arrays, pointers, and references), user-defined types (such as classes and structs), and types with the volatile qualifier. |

## Constraints

None

## Return Value

add\_volatile is a struct that provides a nested type named type, which represents the type after the volatile qualifier is added. Access this type through add\_volatile<Tp\>::type.

## Examples

```
// Test non-volatile type
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<int>::type, volatile int>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<double>::type, volatile double>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<char>::type, volatile char>));
// Test volatile type
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<volatile int>::type, volatile int>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<volatile double>::type, volatile double>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile<volatile char>::type, volatile char>));

ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile_t<int>, volatile int>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile_t<double>, volatile double>));

ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile_t<volatile int>, volatile int>));
ascendc_assert((AscendC::Std::is_same_v<AscendC::Std::add_volatile_t<volatile double>, volatile double>));
```

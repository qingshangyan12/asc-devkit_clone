# asc_position_mode

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:03:26.210Z pushedAt=2026-10-28T12:47:03.101Z nl=2 -->

`asc_position_mode` indicates the even/odd position of a source or destination operand in a register, for use by Reg type conversion and fused computation APIs to select the data position. The constants `ASC_POSITION_EVEN` and `ASC_POSITION_ODD` can be directly used as the position parameter of the corresponding APIs.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_position_mode {
    EVEN = 0,
    ODD
};

constexpr std::integral_constant<asc_position_mode, asc_position_mode::EVEN> ASC_POSITION_EVEN;
constexpr std::integral_constant<asc_position_mode, asc_position_mode::ODD> ASC_POSITION_ODD;
```

## Enumeration Value Details

**Table 1** Enumeration value description

| Enumeration Value | Value | Description | Corresponding Constant |
| --- | --- | --- | --- |
| EVEN | 0 | Selects the even position. | ASC_POSITION_EVEN |
| ODD | 1 | Selects the odd position. | ASC_POSITION_ODD |

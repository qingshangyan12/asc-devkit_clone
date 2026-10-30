# stride

<!-- md-trans-meta sourceCommit=46a50d69bd092786e44495d519df57743eefbfc6 translatedAt=2026-10-27T10:17:35.991Z pushedAt=2026-10-27T13:04:46.111Z nl=2 -->

stride is used to define the stride of each tensor dimension in memory, describing the interval between adjacent elements of the same dimension in memory.

The header file path is `tensor_api/tensor.h`.

## Prototype

```cpp
template <typename... Strides>
using stride = Std::tuple<Strides...>;
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
|--------|----------|------|
| Strides... | Input | Variadic template representing the stride value of each dimension, or a nested stride substructure. |

## API Mapping

A stride is usually created through the [make_stride](make_stride.md) function and obtained through the [get_stride](get_stride.md) function and the [stride](layout.md) member function of a layout.

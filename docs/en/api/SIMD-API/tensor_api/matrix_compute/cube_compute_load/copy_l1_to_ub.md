# copy (Movement from L1 Buffer to Unified Buffer)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-27T10:29:47.731Z pushedAt=2026-10-27T13:04:46.126Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- AI Core in Atlas inference products: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Vector Core in Atlas inference products: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `tensor_api/tensor.h`.

This API moves the source tensor data from the L1 Buffer to the Unified Buffer (UB). The API selects the corresponding movement implementation based on the layouts of the source tensor and the destination tensor.

The API supports moving a complete tensor, and also supports regional movement by specifying the start coordinate in the destination tensor, the start coordinate in the source tensor, and the shape of the movement region through `dst_coord`, `src_coord`, and `copy_shape`.

## Function Prototype

- Performs L1 Buffer to Unified Buffer movement.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom,
        const DstTensor& dst, const SrcTensor& src)
    ```

- Automatically derives the movement path based on the storage locations of the source tensor and destination tensor, and performs L1 Buffer to Unified Buffer movement using the default trait.

    ```cpp
    template <typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src)
    ```

- Performs L1 Buffer to Unified Buffer movement according to the specified source coordinates, destination coordinates, and movement shape.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- Automatically derives the movement path based on the storage locations of the source tensor and destination tensor, and uses the default trait to perform L1 Buffer to Unified Buffer movement according to the specified source coordinates, destination coordinates, and movement shape.

    ```cpp
    template <typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- Constructs a movement atomic object using the default trait.

    ```cpp
    template <typename CopyOperation>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation)
    ```

- Constructs a movement atomic object using the specified trait.

    ```cpp
    template <typename CopyOperation, typename CopyTrait>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation,
        const CopyTrait& trait)
    ```

## Parameters

**Table 1** Template parameter description

| Parameter | Description |
| :--- | :--- |
| Atom | Template parameter of `copy_atom`, derived by the `make_copy` API. |
| DstTensor | Destination tensor type, which must be a Tensor API Tensor type. |
| SrcTensor | Source tensor type, which must be a Tensor API Tensor type. |
| DstCoord | Start coordinate type of the destination tensor, which can also be `zero_coord`. |
| SrcCoord | Start coordinate type of the source tensor, which can also be `zero_coord`. |
| CopyShape | Type of the movement region shape. |
| CopyOperation | Type of the movement operation object. |
| CopyTrait | Type of the movement trait object. |

**Table 2**  `copy` API parameter description

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| atom | Input | Movement atom object, which can be constructed by the `make_copy` API. |
| dst | Output | Destination tensor, whose storage location must be `location::ub`. |
| src | Input | Source tensor, whose storage location must be `location::l1`. |
| dst_coord | Input | Start coordinate of the movement region in the destination tensor. |
| src_coord | Input | Start coordinate of the movement region in the source tensor. |
| copy_shape | Input | Shape of the movement region, used to specify the size of the movement region. |

**Table 3**  `make_copy` API parameter description

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| operation | Input | Movement operation object. For L1 Buffer to Unified Buffer movement, use `copy_l1_to_ub{}`. |
| trait | Input | Movement trait object. For L1 Buffer to Unified Buffer movement, use `l1_to_ub_trait_default{}` by default. |

### copy_l1_to_ub Description

`copy_l1_to_ub` identifies the L1 Buffer to Unified Buffer movement path. Only its public members are listed:

```cpp
struct copy_l1_to_ub {
    template <typename Trait, const Trait& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};
```

The `copy` static member function receives the Trait and movement parameters, and dispatches the L1 Buffer to Unified Buffer movement.

### l1_to_ub_trait Description

`l1_to_ub_trait` represents the static configuration of this path. It currently has no configurable members and is defined as follows:

```cpp
struct l1_to_ub_trait {};
```

`l1_to_ub_trait_default` provides the default trait type and default value for this path:

```cpp
struct l1_to_ub_trait_default {
    using trait_type = l1_to_ub_trait;
    static constexpr const trait_type value = {};
};
```


## Return Value

`copy` has no return value. `make_copy` returns a `copy_atom` object.

## Constraints

- `dst` must be located in the Unified Buffer, and `src` must be located in the L1 Buffer.
- The layout combination of the source tensor and the destination tensor must belong to the data layouts supported by this path.
- When coordinate-based movement is used, `coord` and `copy_shape` must match the shape structure of the corresponding tensors, and the movement region must not exceed the bounds.

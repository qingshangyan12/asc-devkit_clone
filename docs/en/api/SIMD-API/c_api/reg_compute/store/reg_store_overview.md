# Reg Data Store Overview

<!-- md-trans-meta sourceCommit=10589b6e0347a8628628193466fcae403c924d16 translatedAt=2026-10-23T03:54:34.120506+00:00 pushedAt=2026-10-27T02:26:33.321Z nl=2 -->

Reg data store APIs are used to write data from vector data registers, mask registers, or unaligned registers into the Unified Buffer (UB). These APIs are used within Vector Functions (functions marked with `__simd_vf__`) and take effect only on AIV.

## API Overview

### Vector Data Register Store

#### Aligned Store

The destination address alignment requirement of aligned store APIs varies by API, and not all require 32-byte alignment. When using APIs with an offset, the actual access address after applying the offset must also meet the alignment requirement in the table.

**Table 1** Vector data register aligned store APIs

| API Name | Mode | Function Description | Destination Address Alignment Requirement |
| --- | --- | --- | --- |
| [asc_storealign](asc_storealign.md) | Contiguous aligned store | Contiguously writes data of one VL (256 bytes) from the vector data register to the UB according to `mask`. | 32-byte aligned. |
| [asc_storealign](asc_storealign.md) | Immediate offset store | Writes data of one VL to `dst + offset` according to `mask`, where `offset` is in elements. | 32-byte aligned. |
| [asc_storealign](asc_storealign.md) | Address register offset store | Writes data of one VL to the offset address specified by `addr_reg` according to `mask`. | 32-byte aligned. |
| [asc_storealign](asc_storealign.md) | Non-contiguous aligned store | Configures the write positions of 8 DataBlocks according to `block_stride` and `repeat_stride`. | 32-byte aligned. |
| [asc_storealign_postupdate](asc_storealign_postupdate.md) | Immediate offset Post Update store | Contiguously stores one VL according to `mask` and automatically updates the destination address pointer by element offset. | 32-byte aligned. |
| [asc_storealign_postupdate](asc_storealign_postupdate.md) | Non-contiguous aligned Post Update store | Configures the write positions of 8 DataBlocks by stride and automatically updates the destination address pointer in 32-byte units. | 32-byte aligned. |
| [asc_storealign_1st](asc_storealign_1st.md) | First element store | Writes the first element of the vector data register to the UB. | `dtype` aligned. |
| [asc_storealign_1st](asc_storealign_1st.md) | First element immediate offset store | Writes the first element to `dst + offset`, where `offset` is in elements. | `dtype` aligned. |
| [asc_storealign_1st](asc_storealign_1st.md) | First element address register offset store | Writes the first element to the offset address specified by `addr_reg`. | `dtype` aligned. |
| [asc_storealign_1st_postupdate](asc_storealign_1st_postupdate.md) | First element Post Update store | After storing the first element, this API automatically updates the destination address pointer by element offset. | `dtype` aligned. |
| [asc_storealign_intlv](asc_storealign_intlv.md) | Interleaved contiguous store | Stores two vector data registers after interleaving them by element, with a single store size of `2 x VL` (512 bytes). | 32-byte aligned. |
| [asc_storealign_intlv](asc_storealign_intlv.md) | Interleaved immediate offset store | Writes two registers to `dst + offset` after interleaving, where `offset` is in elements. | 32-byte aligned. |
| [asc_storealign_intlv](asc_storealign_intlv.md) | Interleaved address register offset store | Interleaves two registers and writes the result to the offset address specified by `addr_reg`. | 32-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed contiguous store | Compresses the low half of the valid elements according to `mask` and writes the result to the UB. | 32-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed immediate offset store | Writes the compressed result to `dst + offset`, where `offset` is in elements. | 32-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed address register offset store | Writes the compressed result to the offset address specified by `addr_reg`. | 32-byte aligned. |
| [asc_storealign_pack_postupdate](asc_storealign_pack_postupdate.md) | Compressed Post Update store | After the compressed store, this API automatically updates the destination address pointer by the element offset. | 32-byte aligned. |
| [asc_storealign_pack_quarter](asc_storealign_pack_quarter.md) | Quarter-Pack contiguous store | Compresses the low 8 bits of valid 32-bit elements according to `mask` and writes the result to the UB. | 32-byte aligned. |
| [asc_storealign_pack_quarter](asc_storealign_pack_quarter.md) | Quarter-Pack immediate offset store | Writes the quarter-pack compressed result to `dst + offset`, where `offset` is in elements. | 32-byte aligned. |
| [asc_storealign_pack_quarter](asc_storealign_pack_quarter.md) | Quarter-Pack address register offset store | Writes the quarter-pack compressed result to the offset address specified by `addr_reg`. | 32-byte aligned. |
| [asc_storealign_pack_quarter_postupdate](asc_storealign_pack_quarter_postupdate.md) | Quarter-Pack Post Update store | After the quarter-pack compressed store, this API automatically updates the destination address pointer by the element offset. | 32-byte aligned. |
| [asc_store](asc_store.md) | Contiguous store | Stores the entire vector data register, with a single store amount of one VL. For 32-byte aligned scenarios, `asc_storealign` is recommended; for non-32-byte aligned scenarios, `asc_storeunalign` is recommended. | `dtype` aligned. |
| [asc_store](asc_store.md) | Store with count | Stores the first `count` elements of the vector data register. For 32-byte aligned scenarios, `asc_storealign` is recommended; for non-32-byte aligned scenarios, `asc_storeunalign` is recommended. | `dtype` aligned. |

#### Unaligned Store

The `asc_storeunalign*` APIs temporarily store the tail block through the unaligned register `vector_store_unalign` and store the temporarily stored tail block through a post-processing API. In consecutive calls, the same unaligned register must be reused; the post-processing API must also reuse this register with the preceding main store API.

**Table 2** Vector data register unaligned store APIs

| API Name | Mode | Function Description | Destination Address Alignment Requirement | Matching Post-Processing API |
| --- | --- | --- | --- | --- |
| [asc_storeunalign](asc_storeunalign.md) | Store with count | Stores the first `count` elements; writes the main block to the UB and temporarily stores the tail block in the unaligned register. The destination address is updated manually by the user. | `dtype` aligned; 32-byte alignment is not required. | The immediate offset store mode of [asc_storeunalign_post](asc_storeunalign_post.md). |
| [asc_storeunalign_postupdate](asc_storeunalign_postupdate.md) | Immediate offset Post Update store | Stores the first `count` elements and automatically updates the destination address pointer by `count` elements; temporarily stores the tail block in the unaligned register. | `dtype` aligned; 32-byte alignment is not required. | [asc_storeunalign_post_postupdate](asc_storeunalign_post_postupdate.md). |
| [asc_storeunalign_postupdate](asc_storeunalign_postupdate.md) | Address register offset Post Update store | Stores a fixed VL and uses and automatically updates the destination address offset indicated by `addr_reg& count`; the tail block is temporarily stored in the unaligned register. | `dtype` aligned; 32-byte alignment is not required. | Address register offset store mode of [asc_storeunalign_post](asc_storeunalign_post.md). |

### Mask Register Store

#### Aligned Store

Aligned mask register store does not support configuring an additional mask. Direct store writes 32-byte mask data; compressed store discards every other bit and contiguously writes the retained bits to the UB.

**Table 3** Aligned mask register store APIs

| API Name | Mode | Function Description | Destination Address Alignment Requirement |
| --- | --- | --- | --- |
| [asc_storealign](asc_storealign.md) | Contiguous aligned store | Directly writes 32-byte mask register data to the UB. | 32-byte aligned. |
| [asc_storealign](asc_storealign.md) | Immediate offset store | Writes the mask data to `dst + offset`, where `offset` is in bytes. | 32-byte aligned. |
| [asc_storealign](asc_storealign.md) | Address register offset store | Writes the mask data to the offset address specified by `addr_reg`. | 32-byte aligned. |
| [asc_storealign_postupdate](asc_storealign_postupdate.md) | Immediate offset Post Update store | After directly storing the mask data, this API automatically updates the destination address pointer by byte offset. | 32-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed contiguous store | Discards every other bit and writes the retained bits to UB after compression. | 16-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed immediate offset store | Writes the compression result to `dst + offset`, where `offset` is in bytes. | 16-byte aligned. |
| [asc_storealign_pack](asc_storealign_pack.md) | Compressed address register offset store | Writes the compressed result to the offset address specified by `addr_reg`. | 16-byte aligned. |
| [asc_storealign_pack_postupdate](asc_storealign_pack_postupdate.md) | Compressed Post Update store | After a compressed store, this API automatically updates the destination address pointer by byte offset. | 16-byte aligned. |

#### Unaligned Store

Unaligned mask register store also requires `vector_store_unalign` to assist in saving the tail block. The temporarily stored tail block is stored through a post-processing API, and the register is reused during post-processing.

**Table 4** Unaligned mask register store APIs

| API Name | Mode | Function Description | Destination Address Alignment Requirement | Matching Post-Processing API |
| --- | --- | --- | --- | --- |
| [asc_storeunalign_postupdate](asc_storeunalign_postupdate.md) | Mask register Post Update store | For a `uint16_t` destination address, this API extracts the LSB from each 2-bit group and packs the extracted bits into 16 bytes; for a `uint32_t` destination address, this API extracts the LSB from each 4-bit group and packs the extracted bits into 8 bytes. After storing, this API automatically updates the destination address pointer, and the tail block is temporarily stored in the unaligned register. | For a `uint16_t` destination address, 16-byte aligned; for a `uint32_t` destination address, 8-byte aligned. | The immediate offset store mode of [asc_storeunalign_post](asc_storeunalign_post.md). |

## Key Features

### Comparison of Aligned Contiguous Store Modes

The following presents four address offset modes, each illustrated by an example that adds two inputs of length 1024 and data type `half` element-wise and writes the results contiguously to the UB. A single iteration processes 128 elements, with 8 iterations in total, and the final result is `dst[i] = src0[i] + src1[i]`.

**Table 5** Comparison of aligned contiguous store modes

| Scenario | Store Mode | Destination Address Maintenance Mode | Applicable Scenario |
| --- | --- | --- | --- |
| Scenario 1 | User manual offset address store | The caller calculates the destination address for each store through pointer arithmetic. | The destination address offset rule is simple, and the address needs to be explicitly controlled by the user. |
| Scenario 2 | Store through API immediate offset | Passes the fixed element offset relative to the destination base address as an `int32_t` parameter. | The offset is determined at compile time, and the destination address pointer does not need to be modified. |
| Scenario 3 | Address register offset store | Generates `addr_reg` through [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md), and the hardware updates the offset during iterations. | The offset within a Hardware Loop changes with iterations. |
| Scenario 4 | Post Update store | `asc_storealign_postupdate` automatically updates the destination address pointer after each store. | Contiguous store without requiring the user to manually maintain the destination address. |

#### User Manual Offset Address Store

Calls the contiguous aligned store mode of `asc_storealign`. The destination address is calculated as `dst + i * one_repeat_size`, where `one_repeat_size` is 128 `half` elements.

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
for (uint16_t i = 0; i < repeat_times; ++i) {
    asc_loadalign(src0_reg, src0 + i * one_repeat_size);
    asc_loadalign(src1_reg, src1 + i * one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst + i * one_repeat_size, dst_reg, mask);
}
```

#### Store Through API Immediate Offset

Calls the immediate offset store mode of `asc_storealign`. `offset` is relative to the `dst` base address, in elements. This example calculates the offset for 8 stores using `i * 128`, covering 1024 `half` elements.

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
constexpr uint16_t REPEAT_TIMES = 8;
for (uint16_t i = 0; i < REPEAT_TIMES; ++i) {
    asc_loadalign(src0_reg, src0 + i * one_repeat_size);
    asc_loadalign(src1_reg, src1 + i * one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, i * 128, mask);
}
```

#### Address Register Offset Store

Calls the address register offset store mode of `asc_storealign`. `addr_reg` must be assigned and used in the innermost loop, with the offset in elements.

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask;
addr_reg offset;
for (uint16_t i = 0; i < repeat_times; ++i) {
    offset = asc_update_addr_reg_b16(one_repeat_size);
    mask = asc_update_mask_b16(output_length);
    asc_loadalign(src0_reg, src0, offset);
    asc_loadalign(src1_reg, src1, offset);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, offset, mask);
}
```

#### Post Update Store

Calls the immediate offset store mode of `asc_storealign_postupdate`. The destination address is passed by reference, and the API updates it by `one_repeat_size` elements after each store.

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask;
for (uint16_t i = 0; i < repeat_times; ++i) {
    mask = asc_update_mask_b16(output_length);
    asc_loadalign_postupdate(src0_reg, src0, one_repeat_size);
    asc_loadalign_postupdate(src1_reg, src1, one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign_postupdate(dst, dst_reg, one_repeat_size, mask);
}
```

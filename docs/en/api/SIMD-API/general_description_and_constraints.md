# General Description and Constraints<a name="ZH-CN_TOPIC_0000001714258422"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-27T08:09:32.515270+00:00 pushedAt=2026-10-27T08:22:24.828Z nl=2 -->

## Required Header and Library Files<a name="section1299152225418"></a>

> [!NOTE]Note
> The header file directories for Ascend C APIs are as follows:
> - Basic APIs: `${INSTALL_DIR}/asc/include/basic_api`
> - Advanced APIs: (Note that APIs in the header files in the following directories that are not documented are called indirectly and do not require developers' attention.)
>   - `${INSTALL_DIR}/asc/include/adv_api`
>   - `${INSTALL_DIR}/asc/include/tiling`
> 
> Replace `${INSTALL_DIR}` with the directory where the CANN software files are stored after installation. For installation as the root user, the default directory is `/usr/local/Ascend/cann`.
> 
> The library files required by Ascend C APIs are as follows:
> - Basic APIs: Not required
> - Advanced APIs: Link `libtiling_api.a` because advanced APIs have accompanying Host Tiling APIs.

For convenience, both the Ascend C basic APIs and advanced APIs can be called by including kernel\_operator.h. Unless otherwise specified, including this header file is sufficient to call the APIs. Follow any API-specific instructions in the corresponding API documentation.

```
#include "kernel_operator.h"
```

## Mapping Between Logical Positions and Physical Storage<a name="section1359919519819"></a>

The operands of Ascend C APIs are typically [GlobalTensor](basic_api/data_structures/GlobalTensor/GlobalTensor.md) and [LocalTensor](basic_api/data_structures/LocalTensor/LocalTensor.md). Tensor data storage locations are expressed as logical positions ([TPosition](basic_api/aux_data_structures/TPosition.md)), hiding differences between hardware architectures. TPosition values include VECIN, VECOUT, VECCALC, A1, A2, B1, B2, CO1, and CO2. Their mappings to physical memory are shown in the following table.

**Table 1**  Mapping between TPosition and physical memory

<a name="table07372185712"></a>

| TPosition | Physical Memory |
| --- | --- |
| GM | Global Memory |
| VECIN | UB |
| VECCALC | UB |
| VECOUT | UB |
| A1 | L1 Buffer |
| A2 | L0A Buffer |
| B1 | L1 Buffer |
| B2 | L0B Buffer |
| C1 | <!-- npu="950" id1 -->Ascend 950PR&950DT products: L1 Buffer.<br><!-- end id1 --><!-- npu="910" id2 -->Atlas training products: Unified Buffer (UB).<br><!-- end id2 --><!-- npu="310p" id3 -->Atlas inference products AI Core: UB.<br><!-- end id3 --><!-- npu="910b" id4 -->Atlas A2 products: L1 Buffer.<br><!-- end id4 --><!-- npu="A3" id5 -->Atlas A3 products: L1 Buffer.<br><!-- end id5 --><!-- npu="310b" id6 -->Atlas 200I/500 A2 inference products: UB.<!-- end id6 --> |
| C2 | <!-- npu="950" id7 -->Ascend 950PR&950DT products: BiasTable Buffer.<br><!-- end id7 --><!-- npu="910" id8 -->Atlas training products: L0C Buffer.<br><!-- end id8 --><!-- npu="310p" id9 -->Atlas inference products AI Core: L0C Buffer.<br><!-- end id9 --><!-- npu="910b" id10 -->Atlas A2 products: BiasTable Buffer.<br><!-- end id10 --><!-- npu="A3" id11 -->Atlas A3 products: BiasTable Buffer.<br><!-- end id11 --><!-- npu="310b" id12 -->Atlas 200I/500 A2 inference products: BiasTable Buffer.<!-- end id12 --> |
| CO1 | L0C Buffer |
| CO2 | <!-- npu="950" id13 -->Ascend 950PR&950DT products: Global Memory.<br><!-- end id13 --><!-- npu="910" id14 -->Atlas training products: UB.<br><!-- end id14 --><!-- npu="310p" id15 -->Atlas inference products AI Core: UB.<br><!-- end id15 --><!-- npu="910b" id16 -->Atlas A2 products: Global Memory.<br><!-- end id16 --><!-- npu="A3" id17 -->Atlas A3 products: Global Memory.<br><!-- end id17 --><!-- npu="310b" id18 -->Atlas 200I/500 A2 inference products: Global Memory.<!-- end id18 --> |
| TSCM | L1 Buffer |
| SPM | <!-- npu="950" id19 -->Ascend 950PR&950DT products: Global Memory.<br><!-- end id19 --><!-- npu="910" id20 -->Atlas training products: L1 Buffer.<br><!-- end id20 --><!-- npu="310p" id21 -->Atlas inference products AI Core: L1 Buffer.<br><!-- end id21 --><!-- npu="910b" id22 -->Atlas A2 products: Global Memory.<br><!-- end id22 --><!-- npu="A3" id23 -->Atlas A3 products: Global Memory.<!-- end id23 --> |
| C2PIPE2GM | <!-- npu="950" id24 -->Ascend 950PR&950DT products: Fixpipe Buffer.<br><!-- end id24 --><!-- npu="910b" id25 -->Atlas A2 products: Fixpipe Buffer.<br><!-- end id25 --><!-- npu="A3" id26 -->Atlas A3 products: Fixpipe Buffer.<!-- end id26 --> |

## General Address Alignment Constraints<a name="section796754519912"></a>

The storage units on the AI Core store the source and destination operands for vector computation and Cube computation. The alignment requirements of these storage units are listed in [Table 2](#table16278354141117). The start addresses of Ascend C API operands must therefore meet the alignment requirements of the corresponding storage units. **If an API explicitly specifies start address alignment requirements for its operands, those API-specific requirements take precedence.**

**Table 2**  Alignment requirements of different storage units

<a name="table16278354141117"></a>

| Storage Unit | Alignment Requirement |
| --- | --- |
| Global Memory | No alignment requirement. |
| UB | 32-byte aligned. |
| L1 Buffer | 32-byte aligned. |
| L0A Buffer/L0B Buffer | 512-byte aligned. |
| L0C Buffer | 64-byte aligned. |
| BiasTable Buffer | 64-byte aligned. |
| Fixpipe Buffer | 64-byte aligned. |

## General Address Overlap Constraints<a name="section668772811100"></a>

When using the Tensor high-dimensional slicing computation APIs in the basic APIs, developers can define a single Tensor as both the source and destination operands to save address space (that is, address overlap). The following constraints apply:

-   Within a single iteration: The source and destination operands must overlap completely (100%). Partial overlap is not supported.
-   Across multiple iterations: Overlap between the destination operand of an earlier iteration and the source operand of a later iteration is not supported. For example, the destination operand of iteration N is the source operand of iteration N+1, as shown in the following figure. In this case, iteration N may overwrite source operand values, preventing the expected result. For certain binary computation APIs (Add, Sub, Mul, Max, Min, AddRelu, and SubRelu), this overlap is supported when the data type is half, int32\_t, or float, but only between the destination operand and the second source operand, and src1RepStride or dstRepStride must be 0.
-   Across multiple iterations: Overlap between the destination operands of an earlier iteration and a later iteration is not supported. For example, the first iteration writes to DataBlocks numbered 0, 2, 4, 6, 8, 10, 12, and 14, and the second iteration writes to DataBlocks numbered 8, 10, 12, 14, 16, 18, 20, and 22. As a result, the last 4 DataBlocks of the earlier iteration are overwritten, preventing the expected result.

**Figure 1**  Address overlap example (unsupported)<a name="fig12654416181216"></a>  
![](../figures/address_overlap_unsupported.png "Address overlap example (unsupported)")

> [!NOTE]Note 
>The general address overlap constraints in this section apply to general cases. Any additional API-specific instructions in the API reference take precedence.
>If an API does not describe address overlap constraints, address overlap is considered unsupported for Tensor high-dimensional slicing computation. Address overlap may produce unexpected computation results.

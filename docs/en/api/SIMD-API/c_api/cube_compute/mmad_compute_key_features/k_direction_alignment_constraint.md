# K Direction Alignment Constraints

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:06:51.906Z pushedAt=2026-10-28T12:21:43.287Z nl=2 -->

**Feature Description:**

<!-- npu="910b,A3" id1 -->
The asc_mmad API supports enabling this feature on Atlas A2 products/Atlas A3 products.
<!-- end id1 -->

<!-- npu="950" id2 -->
On Ascend 950PR&950DT products, the L0A Buffer uses Nz layout instead of Zz layout. In the scenario where the A cube is transposed and the data type is non-aligned float, the L0A Buffer is 8-aligned in the K direction, so this feature does not need to be enabled to ensure computation correctness.
<!-- end id2 -->

<!-- npu="910b,A3" id3 -->
The core function of K direction alignment is to control, through the k_direction_align parameter, the alignment of the L0A Buffer and L0B Buffer cubes in the K direction when the float data type is used (other data types do not need to consider this feature).

When k_direction_align = true is set, the cubes in the L0A Buffer/L0B Buffer align the K direction to `ceil\(K/16\)\*16`.

![K direction alignment schematic diagram](../../../../figures/mmad_kdirectionalign_c_api.png)

For example, when K=44 and K=36, both are aligned to 48. In this case, for K=44, all 12 fractal blocks in the L0A Buffer/L0B Buffer are fully read into the CUBE; for K=36, although the aligned value is also 48, only 10 of the fractal blocks are read into the CUBE.

![Schematic diagram of K direction alignment to 48](../../../../figures/kdirectionalign_k_is36.png)

When `k_direction_align = false` is set, the cubes in the L0A Buffer/L0B Buffer align the K direction to `ceil\(K/8\)\*8`.

For example, when K = 44, it is aligned to 48, and when K = 36, it is aligned to 40. In this case, for K = 44, all 12 fractal blocks in the L0A Buffer/L0B Buffer are fully read into the CUBE; for K = 36, all 10 fractal blocks in the L0A Buffer/L0B Buffer are read into the CUBE.

![Schematic diagram of K direction alignment to 40](../../../../figures/kdirectionalign_k_is36_false.png)

1. When the A cube is not transposed, k_direction_align does not need to be enabled.

2. When the A cube is transposed, k_direction_align needs to be enabled.

When the input requires a transpose operation, for example, the original cube A has dimensions K×M. After being loaded into the L1 Buffer, its storage format changes to Nz layout (essentially still K×M). In this case, call the cube load API that supports transposition. The internal implementation of this API is shown in the following figure: 16-byte alignment is performed in the K direction (as shown in the left figure). After the transpose is complete, the data is stored in the L0A Buffer in the Zz fractal form shown in the right figure. To avoid reading invalid data, enable the k_direction_align parameter to read only the valid fractal data.

![Schematic diagram of K direction alignment transpose](../../../../figures/kdirectionalign_demo.png)

**Examples:**

```cpp
asc_mmad(c, a, b, m, k, n, asc_unit_flag_mode::DISABLE,
         true /* k_direction_align */, false, true);
```
<!-- end id3 -->

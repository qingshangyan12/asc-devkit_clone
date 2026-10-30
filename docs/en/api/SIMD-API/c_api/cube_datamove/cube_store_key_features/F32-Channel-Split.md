# F32 Channel Split

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T12:56:01.268Z pushedAt=2026-10-28T12:45:45.426Z nl=2 -->

## Description

During the cube move-out process, when the target type is f32 and Channel Split is enabled, the hardware can convert a 16×16 fractal cube into 16×8 small-z fractal cubes. In this case, each 16×16 fractal cube is split into two independent 16×8 small-z fractal cubes.

## Constraints

<!-- npu="A3,910b" id3 -->
- For the following product models:

    <!-- npu="A3" id4 -->
    Atlas A3 products
    <!-- end id4 -->

    <!-- npu="910b" id5 -->
    Atlas A2 products
    <!-- end id5 -->

    Channel Split can be enabled only when the input and output data types of the Fixpipe instruction are both float, Nz is output to GM, and Nz2ND and unitFlag are not enabled.
<!-- end id3 -->

<!-- npu="950" id6 -->
- For Ascend 950PR&950DT products, Channel Split can be enabled only when the input and output data types of the Fixpipe instruction are both float, Nz is output to GM, and Nz2ND, Nz2DN, and unitFlag are not enabled. This feature is currently not supported when moving data out to the L1 Buffer and Unified Buffer (UB).
<!-- end id6 -->

## Exampless

As shown in the following figure, when Channel Split is enabled, the source operand with shape \[64,32\] is split into 16 independent 16x8 Fractal Matrices.

**Figure 1** F32 Channel Split diagram (no invalid data)

![](../../../../figures/Fixpipe_Channel_Split.png)

When Channel Split is enabled, n_size (the size of the source Nz cube in the N direction) must be a multiple of 8. As shown in the following figure, the source operand with shape \[64,24\] is split into 3 columns of 16x8 Fractal Matrices.

**Figure 2** F32 Channel Split diagram (with invalid data)

![](../../../../figures/Fixpipe_Channel_Split_with_dirty.png)

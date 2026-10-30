# Int8 Channel Merge

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:56:06.130Z pushedAt=2026-10-28T12:45:45.427Z nl=2 -->

## Feature Description

During the data movement out process of cube computation, when the target data type is converted to int8\_t or uint8\_t, this feature is automatically enabled for Nz output. The hardware expands the Fractal Matrix from 16×16 to 16×32. The specific rules are as follows: if the number of output channels N is an even multiple of 16, then in the N direction, every two adjacent 16×16 Fractal Matrices are merged into a single 16×32 Fractal Matrix. If N is an odd multiple of 16, channels 1 through channel (N–16) are merged, while the last 16 channels remain unchanged and are not merged.

## Constraints

The hardware enables this feature automatically and it cannot be configured.

## Exampless

For example, when the target data type is s8, M is 32, and N is 48, the hardware first merges the first two 16×16 Fractal Matrices in the N direction into a 16×32 cube, and then moves the remaining 16×16 Fractal Matrix directly to the L1 Buffer.

**Figure 1** Int8 Channel Merge diagram

![](../../../../figures/Fixpipe_Int8_Channel_Merge.png)

# GEMV

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:04:10.555Z pushedAt=2026-10-28T12:21:43.284Z nl=2 -->

**Feature Description:**

The core function of GEMV is as follows: when the dimension M of cube A is 1, the API automatically enables the GEMV function, and the operation degenerates into a multiplication between a 1×K row vector and a K×N cube.

<!-- npu="950" id1 -->
For Ascend 950PR&950DT products, you can disable this function by setting the **disable_gemv** parameter of asc_mmad to true.
<!-- end id1 -->

**Constraints:**

1. The 1×K cube A must be 512B-aligned, and the K data elements must be stored contiguously. Taking the half type as an example, when K=256, the software side can treat it as a 16\*16 block. After m=1 is configured, the hardware parses it as a 1\*256 vector. You can use the asc_copy_l12l0a API to move 256 half data elements from the L1 Buffer to the L0A Buffer.

2. Cube C is a (1\*N) vector. When a 1×N vector is divided into multiple 1×16 sub-vectors, each sub-vector occupies 1024B in the L0C Buffer, but only the lowest 64B contains valid data. For example, when N=50, the vector is divided into four 1\*16 sub-vectors, occupying 4\*1024B = 4096B, where each sub-vector can hold 64B and the four blocks can hold 256B in total. When N=50, the logically valid data is 50×4B=200B.

**Figure 1** GEMV mode, cube multiplication illustration

![GEMV mode - cube multiplication illustration](../../../../figures/mmad_gemv.png "GEMV mode - cube multiplication illustration")

**Advantages:**

When GEMV mode is enabled, cube multiplication processes the M dimension as a non-aligned scenario. Compared with the non-aligned processing method, GEMV mode moves less data, reducing the move overhead. The following uses a cube multiplication example with M=1, K=256, N=32, and half as the data type of both the left and right matrices for illustration.

- GEMV mode

    When moving cube A from the L1 Buffer to the L0A Buffer, the 1\*256 vector is processed as a 16\*16 cube, and the asc_copy_l12l0a API is called once to complete the cube movement of 16\*16 fractal size. The movement of cube B and the cube multiplication computation are the same as in the basic scenario, as shown in the following figure.

    **Figure 2** Cube multiplication computation illustration in GEMV mode with M=1

    ![Cube multiplication computation illustration in GEMV mode with M=1](../../../../figures/mmad_gemv_compare.png "Cube multiplication computation illustration in GEMV mode with M=1")

- Non-GEMV mode

    When moving cube A from the L1 Buffer to the L0A Buffer, the 1\*256 vector is processed as non-aligned cube data, and the M dimension is aligned to 32 bytes before movement. In the figure, K/16=16 input fractals need to be covered, and the amount of moved data increases accordingly. The number of fractals is not equal to the number of C API calls; the number of calls must be determined based on the stride and repeat parameters of the selected movement API, as shown in the following figure.

    **Figure 3** Cube multiplication computation illustration in non-GEMV mode with M=1

    ![Cube multiplication computation illustration in non-GEMV mode with M=1](../../../../figures/mmad_nongemv_compare.png "Cube multiplication computation illustration in non-GEMV mode with M=1")

**Examples:**

**Table 1** Example configuration

| Cube | Dimension | Data Type |
| --- | --- | --- |
| A | 1 * 4096 | half |
| B | 4096 * 256 | half |
| C | 1 * 256 | float |

Cube A is a 1\*4096 vector, moved to the L0A Buffer via asc_copy_l12l0a; cube C is a 1\* 256 vector, which can be divided into 16 sub-vectors of 1 \* 16, occupying 16 \* 1024B = 16384B\(f162f32\), of which the valid data is only 16 \* 64B = 1024B.

**Figure 4** GEMV mode cube multiplication illustration

![GEMV mode-cube multiplication illustration-demo](../../../../figures/mmad_gemv_demo.png "GEMV mode-cube multiplication illustration-demo")

```cpp
// m = 1, indicating enabling the GEMV capability.
asc_mmad(c, a, b, 1, k, n, asc_unit_flag_mode::DISABLE,
         false, false, true);
```

<!-- npu="950" id3 -->
For Ascend 950PR&950DT products, you can disable this feature by setting the disable_gemv parameter of asc_mmad to true, as shown in the following example:
```cpp
asc_mmad(c, a, b, 1, k, n, asc_unit_flag_mode::DISABLE,
         true /* disable_gemv */, false, true);
```
<!-- end id3 -->

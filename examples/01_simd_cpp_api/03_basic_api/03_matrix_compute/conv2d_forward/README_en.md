# conv2d_forward Sample

## Overview

This sample shows how to implement Conv2D forward computation with basic APIs. It demonstrates the complete flow of moving the FeatureMap from GM to L1, performing img2col spatial expansion through LoadData3DV2, accumulating the matrix product with Mmad, and moving the result back to GM through Fixpipe.
This sample uses a 3x3 convolution kernel, padding, stride=2, dilation=2, and a multi-C1 channel combination to demonstrate common convolution scenarios and the related API usage.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

```
├── conv2d_forward
│   ├── scripts
│   │   ├── gen_data.py                // Script that generates input data and golden data
│   │   └── verify_result.py           // Script that verifies whether the output data matches the golden data
│   ├── CMakeLists.txt                 // Build project file
│   ├── data_utils.h                   // Data read and write functions
│   ├── conv2d_forward.asc             // Ascend C sample implementation and invocation
│   └── README.md                      // Sample description document
```

## Sample Specification

This sample uses the following fixed Conv2D specification. The kernel function name is `conv2d_forward_custom`. Each AI Core processes one batch, and 4 AI Cores are used in total.

<a name="Table1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 1: Sample Specification</span></caption>
<tr><td rowspan="1" align="center">Parameter</td><td align="center">Value</td><td align="center">Description</td></tr>
<tr><td align="center">FeatureMap</td><td align="center">[N, H, W, C] = [4, 9, 9, 32]</td><td>The GM input is of the half type and uses the NHWC or NCHW layout depending on the scenario</td></tr>
<tr><td align="center">Weight</td><td align="center">[Cout, C, Kh, Kw] = [16, 32, 3, 3]</td><td>The GM input is of the half type</td></tr>
<tr><td align="center">Convolution parameters</td><td align="center">stride = [2, 2], dilation = [2, 2], padding = [2, 2, 2, 2]</td><td>The padding order is [top, bottom, left, right]</td></tr>
<tr><td align="center">Output</td><td align="center">[N, HOut, WOut, Cout] = [4, 5, 5, 16]</td><td>The GM output is of the half type and uses the NHWC layout</td></tr>
</table>

`C1` and `C0` are the channel split dimensions used when the FeatureMap adopts the NC1HWC0 layout on L1. `C0` is the number of elements in each channel block, which is 16 in this sample. `C1` is the number of channel blocks. Because `C = 32`, `C1 = C / C0 = 2` and `C0 = 16`.

The effective convolution kernel size is calculated as follows:

$$
K_{\text{effective}} = dilation \times (K_h - 1) + 1 = 2 \times (3 - 1) + 1 = 5
$$

The output H/W is calculated as follows:

$$
\begin{aligned}
H_{out} &= \frac{H + pad_{top} + pad_{bottom} - K_{\text{effective}}}{stride_h} + 1 = \frac{9 + 2 + 2 - 5}{2} + 1 = 5 \\
W_{out} &= \frac{W + pad_{left} + pad_{right} - K_{\text{effective}}}{stride_w} + 1 = \frac{9 + 2 + 2 - 5}{2} + 1 = 5
\end{aligned}
$$

The matrix multiplication specification is `M = HOut * WOut = 25`, `K = C * Kh * Kw = 288`, and `N = Cout = 16`.

## Scenario Details

This sample uses the `SCENARIO_NUM` build parameter to select different data movement and padding scenarios. The following table describes the meaning of each `SCENARIO_NUM` value.

<a name="Table2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 2: Meaning of Each scenarioNum Value</span></caption>
<tr><td rowspan="1" align="center">scenarioNum</td><td align="center">FeatureMap input layout</td><td align="center">Weight input layout</td><td align="center">GM-to-L1 conversion</td><td align="center">img2col movement</td><td align="center">Padding value</td><td align="center">Supported architectures</td></tr>
<tr><td align="center">1</td><td align="center">NHWC [4, 9, 9, 32]</td><td align="center">[16, 32, 3, 3]</td><td align="center">Nd2Nz</td><td align="center">Moves the entire M in a single pass, repeatTime = 1</td><td align="center">0</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">2</td><td align="center">NHWC [4, 9, 9, 32]</td><td align="center">[16, 32, 3, 3]</td><td align="center">Nd2Nz</td><td align="center">Moves the entire M in a single pass, repeatTime = 1 (FeatureMap attributes are set explicitly by SetFmatrix)</td><td align="center">1 (explicit SetLoadDataPaddingValue)</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">3</td><td align="center">NHWC [4, 9, 9, 32]</td><td align="center">[16, 32, 3, 3]</td><td align="center">Nd2Nz</td><td align="center">Repeats along the K direction in 2 iterations, repeatTime = 2</td><td align="center">0</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">4</td><td align="center">NCHW [4, 32, 9, 9]</td><td align="center">[16, 32, 3, 3]</td><td align="center">Dn2Nz</td><td align="center">Moves the entire M in a single pass, repeatTime = 1</td><td align="center">0</td><td align="center">dav-3510</td></tr>
</table>

**Scenario 1: Nd2Nz movement with single-pass img2col**
- Input: FeatureMap [4, 9, 9, 32] of the half type in the NHWC layout; Weight [16, 32, 3, 3] of the half type in the [Cout, C, Kh, Kw] layout, expanded to the 2D [K, Cout] layout on GM
- Output: Output [4, 5, 5, 16] of the half type in the NHWC layout
- Implementation: Use `Nd2NzParams` of `DataCopy` to move the FeatureMap and Weight from GM to L1, and use `LoadData3DV2` to move the entire M direction in a single pass

**Scenario 2: Set FeatureMap attributes and the padding value explicitly through SetFmatrix and SetLoadDataPaddingValue**
- Input: FeatureMap [4, 9, 9, 32] of the half type in the NHWC layout; Weight [16, 32, 3, 3] of the half type in the [Cout, C, Kh, Kw] layout, expanded to the 2D [K, Cout] layout on GM
- Output: Output [4, 5, 5, 16] of the half type in the NHWC layout
- Implementation: Set the Load3D template parameter `IsResetLoad3dConfig` to `{false, false}` so that the `l1H`, `l1W`, `padList`, and `padValue` fields of the structure do not take effect. Before Load3D, call `SetFmatrix(H, W, padList, FmatrixMode::FMATRIX_LEFT)` explicitly to set the FeatureMap attributes (`fmatrixMode` must match `loadDataParams.fMatrixCtrl`), and call `SetLoadDataPaddingValue` explicitly to set the padding value to 1

**Scenario 3: Block-wise repeat movement along the K direction**
- Input: FeatureMap [4, 9, 9, 32] of the half type in the NHWC layout; Weight [16, 32, 3, 3] of the half type in the [Cout, C, Kh, Kw] layout, expanded to the 2D [K, Cout] layout on GM
- Output: Output [4, 5, 5, 16] of the half type in the NHWC layout
- Implementation: Move only half of K in a single iteration (`kExtension = K_ALIGN / 2`), and configure `repeatTime = 2`, `repeatMode = 1` (along the K direction), and `repeatStride` through `SetLoadDataRepeat` (dav-2201) or `SetLoadDataRepeatWithStride` (dav-3510) so that one Load3D call completes 2 iterations along the K direction

**Scenario 4: Dn2Nz movement with single-pass img2col**
- Input: FeatureMap [4, 32, 9, 9] of the half type in the NCHW layout; Weight [16, 32, 3, 3] of the half type in the [Cout, C, Kh, Kw] layout, expanded to the 2D [Cout, K] layout on GM (the transpose of the [K, Cout] layout in Scenario 1)
- Output: Output [4, 5, 5, 16] of the half type in the NHWC layout
- Implementation: Use `Dn2NzParams` of `DataCopy` to move the FeatureMap and Weight from GM to L1, and use `LoadData3DV2` to move the entire M direction in a single pass
- Description: This scenario demonstrates the Dn2Nz on-the-fly conversion capability, supports the NCHW FeatureMap input, and runs only on `dav-3510`

The core flow is as follows:

1. Move the single-batch FeatureMap from GM to L1. The C0 channel blocks are arranged tightly by H * W to form the NC1HWC0 layout that img2col can use. Scenarios 1, 2, and 3 use `Nd2NzParams`, and Scenario 4 uses `Dn2NzParams`.
2. Move the pre-arranged Weight from GM to L1 to obtain the `[K, Cout]` Nz fractal layout, and then move it to L0B through `LoadData2D` with transpose.
3. Use `LoadData3DV2` to run img2col on the FeatureMap and move it to L0A, completing the spatial expansion and padding of the 3x3 convolution kernel under stride=2 and dilation=2.
4. Use `Mmad` to complete the matrix multiplication.
5. Use `Fixpipe` to move the L0C result back to GM in the NHWC layout.

## Build and Run

Perform the following steps in the sample root directory to build and run the sample.
- Set environment variables.
  Set environment variables based on the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit package in your environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **NOTE:** `${install_path}` is the CANN package installation directory. If you do not specify the installation directory, the package is installed in `/usr/local/Ascend` by default.
- Run the sample.

  Run the following commands in the sample directory.
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project (dav-3510 NPU mode by default)
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input data
  ./demo                           # Run the generated executable to run the sample
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # Verify whether the output is correct
  ```

  To use the NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # NPU simulation mode
  ```
  > **NOTE:** Clear the cmake cache before switching the build mode. Run `rm CMakeCache.txt` in the build directory and then run cmake again.

- Build option description

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU run or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default), `dav-2201` | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT, and dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products |
  | `SCENARIO_NUM` | 1-4 | Scenario number. Scenario 4 runs only on `dav-3510` |


  The following result indicates that the accuracy comparison is successful.

  ```bash
  test pass!
  ```

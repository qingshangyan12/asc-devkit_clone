# Custom Operator Project Build, Package, and Deployment Sample

## Overview

This sample demonstrates the workflow of building, packaging into a custom operator package, and deploying to a CANN environment based on a simple custom operator.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas 200I/500 A2 Inference Products
- Atlas Inference Series Products

> Note: This sample involves multiple operator examples. Please refer to the actual supported product models for each operator example.

## Directory Structure

```
├── custom_op
│   ├── CMakeLists.txt               // Top-level build project file
│   ├── framework                    // Framework adapter plugin directory
│   │   ├── CMakeLists.txt           // Framework adapter build project file
│   │   ├── onnx_plugin              // ONNX framework adapter plugin
│   │   └── tf_plugin                // TensorFlow framework adapter plugin
│   ├── op_host                      // Host-side operator implementation directory
│   │   ├── CMakeLists.txt           // Host-side build project file
│   │   ├── add_custom               // Add operator Host implementation
│   │   ├── add_custom_template      // Add template operator Host implementation
│   │   ├── add_custom_tiling_sink   // Add Tiling sink operator Host implementation
│   │   └── leaky_relu_custom        // LeakyRelu operator Host implementation
│   └── op_kernel                    // Kernel-side operator implementation directory
│       ├── CMakeLists.txt           // Kernel-side build project file
│       ├── add_custom               // Add operator Kernel implementation
│       ├── add_custom_template      // Add template operator Kernel implementation
│       ├── add_custom_tiling_sink   // Add Tiling sink operator Kernel implementation
│       └── leaky_relu_custom        // LeakyRelu operator Kernel implementation
```

## Sample Description

Add calculation formula:

```
z = x + y
```

AddCustomTilingSink and AddCustomTemplate have the same kernel function as Add. Specifically:
  - AddCustomTemplate demonstrates Tiling template programming. Added template parameters include input data type, shape, and so on. Based on template parameters, the sample implementation logic is simplified or unified. Developers can define required information in template parameters, such as input and output data types and other extension parameters.
  - AddCustomTilingSink demonstrates the Tiling sink scenario. It registers the Tiling function to both host and device for execution through `DEVICE_IMPL_OP_OPTILING`.

LeakyRelu calculation formula:

$$
y=
\begin{cases}
x, \quad x\geq 0\\
a*x, \quad x<0
\end{cases}
$$
Where a is a scalar value.

## Sample Specifications

- Add
  <table border="2" align="center">
  <caption>Table 1: Add Sample Specifications</caption>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">add_custom, add_custom_tiling_sink</td></tr>
  </table>

- AddCustomTemplate

  <table border="2" align="center">
  <caption>Table 2: AddCustomTemplate Sample Specifications</caption>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">add_custom_template</td></tr>
  <tr><td rowspan="6" align="center">Template Parameters</td><td colspan="4" align="center">template&lt;typename D_T_X, typename D_T_Y, typename D_T_Z, int TILE_NUM, int IS_SPLIT&gt;</td>
      <tr><td>D_T_X</td><td colspan="1">typename</td><td colspan="2">Data type (half, float)</td></tr>
      <tr><td>D_T_Y</td><td colspan="1">typename</td><td colspan="2">Data type (half, float)</td></tr>
      <tr><td>D_T_Z</td><td colspan="1">typename</td><td colspan="2">Data type (half, float)</td></tr>
      <tr><td>TILE_NUM</td><td colspan="1">int</td><td colspan="2">Number of tiles</td></tr>
      <tr><td>IS_SPLIT</td><td colspan="1">int</td><td colspan="2">Whether to split</td></tr>
  </tr>
  </table>

- LeakyRelu

  <table border="2" align="center">
  <caption>Table 3: LeakyRelu Sample Specifications</caption>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">LeakyRelu</td></tr>
  <tr><td rowspan="3" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 200, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">negative_slope</td><td align="center">0.0</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">y</td><td align="center">[8, 200, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">leaky_relu_custom</td></tr>
  </table>

## Code Implementation Introduction

- Add</br>
  - Kernel Implementation:</br>
    Implemented based on the Ascend C vector computation interface `Add`.</br>

  - Tiling Implementation:</br>
    TilingData parameter design. The `AddCustomTilingData` parameters are essentially related to parallel data splitting. This sample uses two tiling parameters: `totalLength` and `tileNum`. `totalLength` indicates the data size to compute, and `tileNum` indicates the number of data blocks per core.</br>

- AddCustomTemplate</br>
  - Kernel Implementation:</br>
    Same as Add.
  - Tiling Template Design:</br>
    This sample uses five template parameters. `D_T_X`, `D_T_Y`, and `D_T_Z` are the data types of input x, input y, and output z respectively. `TILE_NUM` is the number of data blocks per core. `IS_SPLIT` indicates whether to enable data block computation. When `IS_SPLIT` is 0, `TILE_NUM` is invalid. Template parameter combination replaces traditional TilingKey.

  - TilingData Parameter Design:</br>
    This sample uses one tiling parameter. `totalLength` indicates the total data size to compute across all cores.</br>


- AddCustomTilingSink</br>
  - Kernel Implementation:</br>
    Same functionality as Add. The kernel forces the sample to run in AIC and AIV mixed scenario through the `KERNEL_TASK_TYPE_DEFAULT` interface, satisfying the Tiling sink operator condition. All Tiling function logic is implemented separately in `add_custom_tiling_sink_tiling.cpp` and registered as a sink Tiling function through the `DEVICE_IMPL_OP_OPTILING` interface.</br>

- LeakyRelu</br>
  - Kernel Implementation:</br>
    Implemented based on the high-level API interface `LeakyRelu`.</br>

  - Tiling Implementation:</br>
    TilingData parameter design. The `LeakyReluCustomTilingData` parameters are essentially related to parallel data splitting. This sample uses three tiling parameters: `totalLength`, `tileNum`, and `negativeSlope`. `totalLength` and `tileNum` are similar to the Add sample. `negativeSlope` represents the negative half-axis slope coefficient of LeakyRelu, passed as a computation parameter to the kernel side.</br>


## Build and Run

Execute the following steps in the sample root directory to build, package, and deploy the custom sample package.

- Configure Environment Variables

  Select the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
  - Default path, root user installed CANN package

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN package

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, installed CANN package

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Build, Package, and Deploy Sample

  ```bash
  mkdir -p build && cd build
  cmake .. && make -j binary package
  ./custom_opp_*.run
  ```

  The following output indicates successful execution:

  ```log
  SUCCESS
  ```

## Cache Build Acceleration (Optional)

This sample supports accelerating repeated builds through ccache, providing both local cache and distributed cache modes.

### Prerequisites

- `ccache` installed, recommend using `ccache >= 4.6.1`

  ```bash
  apt install ccache
  ```

- For distributed cache scenarios, confirm that the current version supports Redis storage

  ```bash
  ccache --version
  # Expected: Features include redis-storage
  ```

### Local Cache

After enabling `-DENABLE_CCACHE=ON`, `cmake` connects `ccache` to the build process. When source code, build commands, compiler, and build directory path are identical, repeated builds can directly reuse local cache results.

Enable through cmake parameters without modifying CMakeLists.txt:

```bash
mkdir -p build && cd build
cmake -DENABLE_CCACHE=ON .. && make -j binary package
```

Verify whether local cache is effective as follows:

```bash
ccache -Cz
# First build: write to cache
rm -rf build && mkdir -p build && cd build
cmake -DENABLE_CCACHE=ON .. && make -j binary package
cd ..
ccache -z
# Second build: cache hit
rm -rf build && mkdir -p build && cd build
cmake -DENABLE_CCACHE=ON .. && make -j binary package
```

After each build, view cache hit statistics through `ccache --show-stats -v`. You can see that the second build's local cache hit rate `Local storage Hits` increases significantly.

To clear the local cache, operate as follows:

```bash
# Clear statistics only, do not delete cache content
ccache -z

# Clear local cache content
ccache -C

# Clear both local cache content and statistics
ccache -Cz
```

### Distributed Cache (ccache + Redis)

Suitable for multi-machine shared cache scenarios: Machine A compiles and pushes results to Redis, Machine B can hit cache from Redis under the same source code, same compilation options, and same toolchain version, reducing repeated compilation.

In distributed scenarios, `ccache` uses each machine's local cache as the primary cache and Redis as the shared secondary cache. When Machine A compiles for the first time, it calls the actual compiler and writes results to both local cache and Redis. When Machine B compiles again under the same source code, same build commands, same compiler, and same build directory path, it can directly hit the shared cache from Redis, reducing repeated execution of build actions. If the compiler paths of two machines differ but content is identical, recommend setting `compiler_check=content`.


For more `ccache` configuration and cache behavior details, refer to the [ccache official documentation](https://ccache.dev/documentation.html).

Network Requirements:

- Machine A: First compilation machine, writes cache to Redis, IP is `<A_IP>`
- Machine B: Second compilation machine, verifies shared cache hit from Redis, IP is `<B_IP>`
- Machine C: Redis server, stores shared cache data, IP is `<C_IP>`
- A, B, C machines must be on the same network. Both A and B machines must be able to access `C_IP:6379`

Recommend that Machine A and Machine B use the same source code content, same build commands, same version of compiler, and maintain consistent source code path and build directory path. Otherwise, cache may not be hit.

**1. Machine C: Deploy Redis Service**
```bash
apt install redis-server
# Start Redis service
redis-server --daemonize yes --bind 0.0.0.0 --port 6379 --requirepass <PASSWORD>
# Verify Redis connection
redis-cli -h <C_IP> -p 6379 -a <PASSWORD> ping
```

> Note: The above configuration is for controlled test environments only. For shared or production environments, recommend enabling access control, authentication, and network isolation.

**2. Machine A / Machine B: Configure ccache**
```bash
apt install redis-tools
# Verify Redis connection
redis-cli -h <C_IP> -p 6379 -a <PASSWORD> ping
# Configure Redis as secondary storage with password authentication format
# Format: redis://default:<PASSWORD>@<C_IP>:6379
ccache --set-config=secondary_storage=redis://default:<PASSWORD>@<C_IP>:6379
# Configure compiler content verification to avoid cache misses due to path differences
ccache --set-config=compiler_check=content
```

**3. Machine A / Machine B: Execute Build**
```bash
ccache -Cz
rm -rf build && mkdir -p build && cd build
cmake -DENABLE_CCACHE=ON .. && make -j binary package
```

Machine A build writes to local cache and Redis. Machine B local cache is empty and can read shared cache from Redis. After Machine A builds, execute build on Machine B. Compare the two `ccache --show-stats -v` results to see that the remote cache hit rate `Remote storage Hits` on Machine B increases significantly.
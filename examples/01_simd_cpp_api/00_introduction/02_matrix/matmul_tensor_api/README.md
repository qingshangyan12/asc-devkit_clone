# 基于Tensor_API实现的静态Matmul算子样例

## 概述

本样例基于Tensor_API编程范式实现多核矩阵乘计算，对标matmul_memory_allocator样例的功能规格，使用tensor_api的高层次抽象接口实现相同的计算逻辑。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── matmul_tensor_api
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── matmul.asc              // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：  
  Matmul计算公式：
  $$
  C = A * B
  $$
- 样例规格：  
  本样例参数M = 512, N = 1024, K = 512，调用4个核完成计算，输入规格如下表所示：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">matmul_tensor_api</td></tr>
  </table>

- 样例实现：
  - 实现流程：
    <table>
    <tr><th align="left">步骤</th><th align="left">Tensor_API操作</th><th align="left">功能</th><th align="left">布局转换</th></tr>
    <tr><td align="left">1</td><td align="left">MakeTensor + Slice</td><td align="left">创建GM张量并切片获取当前核处理的数据块</td><td align="left">ND格式</td></tr>
    <tr><td align="left">2</td><td align="left">Copy(CopyGM2L1)</td><td align="left">将A矩阵和B矩阵数据从GM搬运到L1</td><td align="left">A: ND->NZ<br>B: ND->ZN</td></tr>
    <tr><td align="left">3</td><td align="left">Copy(CopyL12L0A/B)</td><td align="left">将数据从L1搬运到L0A和L0B</td><td align="left">保持NZ/ZN格式</td></tr>
    <tr><td align="left">4</td><td align="left">Mmad</td><td align="left">完成矩阵乘加计算</td><td align="left">矩阵乘结果为NZ格式</td></tr>
    <tr><td align="left">5</td><td align="left">Copy(CopyL0C2GM)</td><td align="left">将L0C中的计算结果搬运到GM</td><td align="left">NZ->ND格式转换</td></tr>
    </table>
  
  - Tensor_API核心接口：
    1. **张量创建接口**：使用MakeTensor + MakeMemPtr + MakeFrameLayout创建各级张量
       - GM张量：NDExtLayoutPtn布局（ND格式扩展）
       - L1/L0张量：NZLayoutPtn/ZNLayoutPtn布局（适配cube计算单元）
    
    2. **数据搬运接口**：使用Copy + CopyAtom抽象
       - CopyGM2L1：GM到L1的数据搬运，自动处理ND→NZ/ZN格式转换
       - CopyL12L0A/B：L1到L0A/L0B的数据搬运
       - CopyL0C2GM：L0C到GM的数据搬运，自动处理NZ→ND格式转换
    
    3. **矩阵乘接口**：使用Mmad + MmadAtom抽象
       - 自动管理累加控制（cmatrixInitVal参数）
       - 内部使用float计算单元
    
    4. **切片接口**：使用Slice + MakeCoord + MakeShape获取张量的子区域
       - 实现分核逻辑：每个核处理不同的数据块
       - 实现分块逻辑：baseM/baseK/baseN的基础块切分
  
  - 约束条件：
    1. baseM/baseK/baseN满足16对齐
    2. baseM/baseK/baseN能被singleCoreM/singleCoreK/singleCoreN整除
    3. singleCoreM/singleCoreK/singleCoreN能被M/K/N整除，不支持非整切场景

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## Tensor_API编程范式对比

与matmul_memory_allocator样例相比，tensor_api样例具有以下优势：

| 特性 | matmul_memory_allocator | matmul_tensor_api |
|------|------------------------|-------------------|
| 内存管理 | 手动地址计算和偏移量管理 | MakeMemPtr自动抽象 |
| 张量表示 | LocalTensor + 手动配置 | MakeTensor + Layout自动管理 |
| 数据搬运 | DataCopy + Nd2NzParams手动配置 | Copy + CopyTraits自动处理 |
| 格式转换 | 手动配置参数（dstNzC0Stride等） | 布局模式自动转换（NZ/ZN） |
| 矩阵乘 | Mmad + MmadParams | Mmad + MmadTraits |
| 分核逻辑 | 手动计算GM偏移量 | Slice自动切片 |
| 代码抽象层次 | 低层次API，细节控制多 | 高层次API，代码更简洁 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 软连接配置  
  由于当前tensor_api样例只用于功能展示，相关源码未随标准CANN软件包发布。
  
  如需编译运行该样例，需要先将tensor_api相关源码通过软连接挂在到CANN安装路径下。
  
  **对于aarch64架构环境**：
  ```bash
  cd ${install_path}/cann/aarch64-linux/asc/impl
  ln -s ${code_path}/impl/tensor_api  # ${code_path}表示本地代码下载路径，例如/mnt/workspace/gitCode/chenma/asc-devkit/asc-devkit
  cd ../include
  ln -s ${code_path}/include/tensor_api
  ```
  
  **对于x86_64架构环境**：
  ```bash
  cd ${install_path}/cann/x86-64-linux/asc/impl
  ln -s ${code_path}/impl/tensor_api
  cd ../include
  ln -s ${code_path}/include/tensor_api
  ```
  
  > **注意：** 如果无法创建软连接（权限问题），可以修改CMakeLists.txt添加本地tensor_api路径：
  > ```cmake
  > # 在target_compile_options中添加-isystem参数指向项目根目录
  > # 例如：-isystem/mnt/workspace/gitCode/chenma/asc-devkit/asc-devkit
  > ```

- 样例执行
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake ..;make -j;                                                         # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu ..;make -j;   # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明
  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  
  > **说明：** 本样例仅支持 dav-3510 架构（对应 Ascend 950PR/Ascend 950DT），架构参数已硬编码在 CMakeLists.txt 中。

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 关键技术点

### 1. 张量布局管理

tensor_api通过Layout模式自动管理数据布局：
- **NDExtLayoutPtn**：ND格式的扩展布局，用于GM数据
- **NZLayoutPtn**：NZ格式布局，用于L0A和L1A数据
- **ZNLayoutPtn**：ZN格式布局，用于L0B和L1B数据（cube单元要求）

### 2. 自动格式转换

Copy接口根据源和目的张量的布局自动进行格式转换：
- GM(ND) → L1(NZ)：CopyGM2L1自动处理
- L1(NZ) → L0A(NZ)：CopyL12L0A保持格式
- L1(ZN) → L0B(ZN)：CopyL12L0B保持格式
- L0C(NZ) → GM(ND)：CopyL0C2GM自动处理

### 3. 分核和分块逻辑

- **分核**：通过Slice + MakeCoord实现，每个核处理不同的数据块
- **分块**：通过三重循环(mLoop/nLoop/kLoop)实现baseM/baseK/baseN的切分

### 4. 事件同步

保持与传统实现相同的事件同步机制，确保数据搬运和计算的正确流水执行。
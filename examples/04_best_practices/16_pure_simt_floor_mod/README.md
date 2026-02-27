# 纯SIMT编程实现floor_mod算子样例

## 概述

本样例以基于Ascend C纯SIMT编程方式floor_mod功能的算子为例，展示了纯SIMT编程的算子开发方式。


## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── 16_simt_and_simd_floor_mod
│   ├── floor_mod.asc    # Ascend C算子实现 & 调用样例
|   └── README.md
```

## 算子描述

- 算子功能：  
  输入self和other两个tensor，将self每个元素除以other的对应元素得到的余数，结果与除数other同符号，并且绝对值小于other的绝对值。算子输出output第i个数据计算公式为：

  ```
  output[i] = self[i] - floor(self[i]/other[i]) * other[i]
  ```

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">floor_mod</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">self</td><td align="center">6400</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td align="center">other</td><td align="center">6400</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">6400</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">floor_mod</td></tr>
  </table>

- 基础知识：  
  在计算过程中，需要多次调用微指令API完成单核数据的处理。在SIMT编程中，可以直接读取和使用Global Memory上的数据。在Vector Core中，SIMT单元和SIMD单元共享片上存储，因此可以使用片上存储完成SIMT和SIMD的混合编程。

- 数据切分：  
  本例中算子输入总元素个数为6400，每个核处理数据量为1024，因此核数为7。
  - SIMT场景，设置每个核实际启动的线程数THREAD_COUNT为1024，每个线程负责处理一个元素。每个线程处理的数据位置是通过threadIdx进行索引，后续按照总线程数进行步进索引。
    ```cpp
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    ```

- 算子实现：
  floor_mod_simt负责整型场景的元素计算。
  ```cpp
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    auto rem = self[index] % other[index];
    bool signs_differ = ((rem < 0) != (other[index] < 0));
    if (signs_differ && (rem != 0)) {
        out[index] = rem + other[index];
    } else {
        out[index] = rem;
    }
  ```

- 调用实现：    
  使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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


- 样例执行
```bash
CANN_PATH=$(printenv ASCEND_HOME_PATH)  # 获取CANN包安装路径
OUTPUT="floor_mod_simt"               #用户自定义编译生成的二进制文件名称

bisheng \
  -x dpp --cce-aicore-arch=dav-c310-vec \
  -std=c++17 \
  floor_mod.asc \
  -I${CANN_PATH}/include \
  -I${CANN_PATH}/include/ascendc/host_api \
  -I${CANN_PATH}/compiler/ascendc/include/highlevel_api \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/lib \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/lib/matmul \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/impl \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/interface \
  -L${CANN_PATH}/lib64 \
  -lascendc_runtime \
  -lascendcl \
  -lruntime \
  -lregister \
  -lerror_manager \
  -lprofapi \
  -lunified_dlog \
  -lmmpa \
  -lascend_dump \
  -ltiling_api \
  -lplatform \
  -ldl \
  -lc_sec \
  -lstdc++ \
  -o ${OUTPUT}

  ./${OUTPUT}                        # 执行样例

  ```
执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```

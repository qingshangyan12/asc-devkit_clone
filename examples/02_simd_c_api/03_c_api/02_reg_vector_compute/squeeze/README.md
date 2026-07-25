# squeeze样例

## 概述
本样例基于C API编程接口实现Squeeze运算，主要调用asc_squeeze接口和asc_storeunalign/asc_storeunalign_post接口。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍
```
├── squeeze
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── squeeze.asc                    // AscendC样例实现 & 调用样例
│   └── README.md                      // 样例介绍
```

## 样例描述
- 样例功能：
  本样例以mask设置为MaskPattern::M4为例，每轮迭代选择输入向量矢量数据寄存器x中索引为4的倍数元素依次复制到输出向量矢量数据寄存器y中连续排列，且输出向量中剩余位置元素置为0。
  - asc_squeeze接口的模板参数store配置为STORE_REG时，可以记录有效元素个数并存入AR特殊寄存器中，配合asc_storeunalign接口使用
  - asc_storeunalign接口可以使用AR特殊寄存器中记录的值作为搬运的元素个数，实现连续非对齐搬运，将Squeeze的结果连续搬出
- 约束说明：
  - 当asc_squeeze接口的模板参数store配置为STORE_REG时，必须保证asc_squeeze接口和asc_storeunalign接口交替使用，以保证AR特殊寄存器的正常使能
  - 在计算前需要调用asc_clear_ar_spr接口，将AR寄存器进行清零，否则可能会有数据残留导致精度问题
- 样例规格：
  | 项目 | name | shape | data type |
  | --- | --- | --- | --- |
  | 样例类型(OpType) | AIV样例 |  |  |
  | 样例输入 | x | [1, 256] | float |
  | 样例输出 | y | [1, 64] | float |
  | 核函数名 | kernel_squeeze |  |  |
- 样例实现：  
  - 输入向量的shape为[1, 256]，数据类型为float，进行4次迭代，每轮迭代处理64个元素
  - asc_squeeze：每次调用选择矢量数据寄存器x中索引为4的倍数的元素，即矢量数据寄存器x[i * 4]依次连续写入矢量数据寄存器y[i]中，结果y[0:16]为16个有效元素，剩余的y[16:64]置0，AR特殊寄存器的值置为16
  - asc_storeunalign：根据AR寄存器中的值作为搬运元素个数，搬出矢量数据寄存器y中前16个元素至非对齐寄存器ureg或者输出UB地址yAddr
  - asc_storeunalign_post：4次迭代结束后，搬出ureg中剩余的数据
  - 调用实现：使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

  使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

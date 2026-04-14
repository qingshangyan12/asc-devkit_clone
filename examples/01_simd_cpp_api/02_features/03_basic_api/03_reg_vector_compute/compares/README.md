# compares样例

## 概述
本样例基于RegBase编程范式实现Compares运算，主要调用Compares接口。
- 用于逐元素比较一个向量RegTensor和一个标量Scalar的大小，结果输出至MaskReg。如果比较后的结果为真，则输出MaskReg的对应比特位为1，否则为0.
- 支持的比较模式：LT（小于）、GT（大于）、GE（大于或等于）、EQ（等于）、NE（不等于）、LE（小于或不等于）

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── compares
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── compares.asc                   // AscendC样例实现 & 调用样例
│   └── README.md                      // 样例介绍
```

## 样例描述
- 样例功能：  
  本样例以比较float元素，且比较模式为GE（大于或等于）为例，compare输出结果用uint8_t的数据类型保存。  
  - 输入：向量：float数据类型，shape为[1, 256]；标量：float数据类型
  - 输出：uint8_t数据类型，shape为[1, 256]
  - 说明：Compare指令每次读取64个float32，写入256bit，具体流程和示意图如下：
    - 搬入：调用LoadAlign接口，常规搬入
    - 计算：调用compare接口，float数据类型的MaskReg格式为每4bits保存一个mask，所以compare从xReg依次读取数据，比较后依次写入至MaskReg的4*N位置
    - 搬出：调用StoreAlign接口，以uint8_t数据类型保存MaskReg的数据  
    <img src="figure/compares.png">
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
  <tr><td align="center">scalar</td><td align="center">[1]</td><td align="center">float</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[1, 256]</td><td align="center">uint8_t</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">compares</td></tr>
  </table>
- 样例实现：  
   ComparesVF函数内调用Compares接口进行计算，结果写回UB
  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                                     # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

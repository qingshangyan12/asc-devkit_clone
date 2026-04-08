# cast样例

## 概述
本样例基于RegBase编程范式实现Cast运算，主要调用Cast接口，用于向量RegTensor的数据类型转换。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── cast
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   └── cast.asc                       // AscendC样例实现 & 调用样例
```

## 样例描述
- 样例功能：
  本样例对输入向量做数据类型转换，由256个half间隔转换成128个int32_t，由Cast接口中的AscendC::Reg::CastTrait配置转换规则，具体规则如下：
  - AscendC::Reg::RegLayout::ZERO：在本样例输入输出位宽比值为1:2的场景下，表示从输入的2*N位置读取数据（N为索引值）。
  - AscendC::Reg::SatMode::NO_SAT：在本样例浮点数转整数的场景下，表示数据溢出时采用截断模式，溢出值按32位截断，保留低32位，舍弃高位。
  - AscendC::Reg::MaskMergeMode::ZEROING：表示mask未选择的元素在输出中置零。
  - AscendC::RoundMode::CAST_RINT：表示就近舍入，距离相同时向偶数进位。

  AscendC::Reg::CastTrait配置的可选项与输入输出类型有关，具体可参考[Cast](../../../../../../docs/api/context/Cast-46.md)。

- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  </tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 256]</td><td align="center">half</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[1, 128]</td><td align="center">int32_t</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">cast</td></tr>
  </table>
- 样例实现：
   CastVF函数内调用Cast接口进行计算，结果写回UB
  - 调用实现
    使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake ..;make -j;                                                         # 编译工程
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
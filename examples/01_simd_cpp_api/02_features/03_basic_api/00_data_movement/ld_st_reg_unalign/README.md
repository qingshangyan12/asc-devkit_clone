# ld_st_reg_unalign样例

## 概述
本样例基于RegBase编程范式实现UB(Unified Buffer)对RegTensor(Reg矢量计算基本单元)的非对齐数据搬运操作，支持多种场景，通过编译参数选择场景。
<table>
	<tr>
		<td>scenarioNum</td>
		<td>场景类型</td>
	</tr>
	<tr>
		<td>1</td>
		<td>单核非对齐数据搬运：使用LoadUnAlign和StoreUnAlign进行非对齐地址的数据搬运</td>
	</tr>
	<tr>
		<td>2</td>
		<td>多核ReduceDataBlock计算：使用LoadUnAlign处理非对齐输入，ReduceDataBlock进行块内求和</td>
	</tr>
</table>

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── ld_st_reg_unalign
│   ├── CMakeLists.txt                 // 编译工程文件
│   └── README.md                      // 样例介绍
│   └── ld_st_reg_unalign.asc          // AscendC样例实现 & 调用样例
```

## 样例描述

### 场景1：单核非对齐数据搬运
- 样例功能：  
  输入一个数据类型为float，数据量为1024的向量，在源操作数和目的操作数地址均非32B对齐的情况下，搬运128个数据。
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  </tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 1024]</td><td align="center">float</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[1, 128]</td><td align="center">float</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">ld_st_reg_unalign_kernel</td></tr>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">1</td></tr>
  </table>
- 样例实现：  
   将UB上源操作数和目的操作数地址偏移一个元素（4 Byte）长度，使得起始地址非32B对齐，调用CopyVF从该地址开始搬运128个float数，写回UB(Unified Buffer)。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

### 场景2：多核ReduceDataBlock计算
- 样例功能：  
  输入shape为[14, 255]的half类型矩阵，展示LoadUnAlign处理非对齐输入的自然使用场景。使用ReduceDataBlock对每个DataBlock（16个half元素）求和，多核并行处理，StoreUnAlign连续存储结果。
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  </tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[14, 255]</td><td align="center">half</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[14, 16]</td><td align="center">half</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">ld_st_reg_unalign_kernel</td></tr>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">4（动态调整，避免尾核空闲）</td></tr>
  </table>
- 样例实现：
  1. **数据搬入（整块连续）**：
     - 14×255=3570个half作为连续整块搬入UB
     - UB紧密排列，每行间隔255个元素
     - 行1-3起始地址非32B对齐（510字节、1020字节、1530字节）

  2. **LoadUnAlign自然使用**：
     - 所有行统一使用LoadUnAlignPre + LoadUnAlign
     - LoadUnAlign可处理对齐地址（行0）和非对齐地址（行1-3）
     - 无需区分地址是否对齐，代码统一简洁

  3. **ReduceDataBlock计算**：
     - 每行255元素分2个VL块处理（128 + 127）
     - 每个VL块产生8个结果（每DataBlock求和）
     - 使用真实Mask处理第2次VL块（127个元素）

  4. **StoreUnAlign连续存储**：
     - 使用带postUpdateStride的StoreUnAlign版本
     - 每次存储偏移8个元素（resultsPerRepeat=8）
     - 循环2次后自动偏移到dstAddr+16
     - StoreUnAlignPost处理最后的边界数据

  5. **LocalMemBar屏障机制**：
     - VF内屏障：确保第1次存储完成，避免第2次StoreUnAlign覆盖有效数据
     - Kernel内屏障：确保当前行存储完成，避免下一行覆盖当前行的有效数据

  6. **多核切分公式**：
     - 动态调整核数：`usedCoreNum = min(4, 14) = 4`
     - 向上取整分配：`formerRowsPerCore = (14+3)/4 = 4`
     - 核0-2各处理4行，核3处理2行（避免尾核空闲）

  7. **输出紧凑布局**：
     - 每行16个结果，紧凑连续排列
     - 核0输出64个数（GM[0-63]），核3输出32个数（GM[192-223]）
  
  - 调用实现  
    使用内核调用符<<<>>>调用核函数，启动4个核。

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
    
- 样例执行（场景1）
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

- 样例执行（场景2）
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DSCENARIO_NUM=2 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;     # 编译工程，选择场景2
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  # 场景1 CPU调试模式
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  
  # 场景2 CPU调试模式
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DSCENARIO_NUM=2 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  
  # 场景2 NPU仿真模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=2 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　|
| ------------| -----------------------------| ---------------------------------------------------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真　　　　　　　|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |
| `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号：场景1单核数据搬运、场景2多核ReduceDataBlock |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
# reg_storealign_mask_preserve样例

## 概述

本样例使用`half`类型的`asc_storealign`验证`mask`的保持语义：目的Unified Buffer（UB）预先填充固定值`-1.0`，源数据为`2.0`，`mask`仅选择偶数下标元素。执行`asc_storealign`后，偶数下标元素应更新为`2.0`，未被`mask`选中的奇数下标元素应保留目的UB中的原值`-1.0`。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构

```text
reg_storealign_mask_preserve/
├── CMakeLists.txt                        // 编译配置
├── reg_storealign_mask_preserve.asc      // Ascend C C API样例
└── README.md                             // 样例说明
```

## 实现说明

样例处理一个完整矢量，即128个`half`元素（256B）：

1. 将值为`2.0`的源数据从GM（Global Memory）搬入`src_ub`。
2. 将值为`-1.0`的初始数据从GM搬入`dst_ub`，完成目的UB预填充。
3. 将32B的`mask`数据搬入`mask_ub`，再通过`asc_loadalign`加载到掩码寄存器。
4. 调用`asc_storealign(dst_ub, src_reg, even_mask)`执行带`mask`的对齐写回。
5. 将`dst_ub`搬回GM并逐元素校验。

`b16`模式下，每2个`mask`位对应一个`half`元素，且仅组内最低位决定该元素是否有效。每个`mask`字节设置为`0x11`（二进制`00010001`），对应下标0、2、4、6等偶数位置有效。

预期的前16个输出元素如下：

```text
2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0
```

这说明`mask`为1的偶数位置写入源数据，而`mask`为0的奇数位置保留目的UB地址中的原值。

## 编译与运行

### 配置环境变量

请根据CANN开发套件包的安装路径配置环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

`${install_path}`为CANN包安装目录。root用户默认安装目录为`/usr/local/Ascend`，非root用户默认安装目录为`${HOME}/Ascend`。

### NPU模式

在本样例目录执行：

```bash
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

### NPU仿真模式

```bash
mkdir -p build
cd build
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

## 运行结果

校验成功时输出：

```text
output[0..15]: 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0 2.0 -1.0
test pass!
```

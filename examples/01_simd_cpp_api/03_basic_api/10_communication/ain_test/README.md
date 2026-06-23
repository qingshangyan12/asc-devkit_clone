# Ain Test样例

## 概述

本样例用于验证 `adv_api/ain/ain.h` 头文件和设备侧 Ain 类在 Ascend C 工程中可编译。样例不依赖输入输出数据，仅完成 ACL 初始化、Kernel 拉起和同步。

## 目录结构

```
ain_test
|-- CMakeLists.txt
|-- README.md
`-- ain_test.asc
```

## 编译

在本样例目录下执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

生成可执行文件 `demo`。

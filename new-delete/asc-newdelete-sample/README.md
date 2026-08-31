# newdelete 样例

## 概述

本样例从 CUDA Samples `cpp/3_CUDA_Features/newdelete` 改写而来，演示 Ascend C SIMT kernel 中 C++ `new/delete`、`new[]/delete[]` 以及 placement new 的使用方式。

样例包含三组测试：

- `Vector test`：在 Device 侧通过 `new Vector<int32_t>` 创建对象，对象内部通过 `new T[]` 申请数据区，多个线程块填充和消费同一个对象，最后通过 `delete` 释放对象。
- `Vector, using placement new on UB buffer test`：在 UB 上预留对象 buffer 和数据 buffer，通过 placement new 构造 `Vector<int32_t>`。
- `Vector, with user defined datatype test`：在 UB 上通过 placement new 构造 `Vector<ComplexType>`，验证用户自定义类型。

> 说明：本样例在 Host 侧申请一块 GM workspace，并在样例内提供最小 `dpp_alloc/dpp_dealloc` 实现，供 Device 侧 non-placement `new/delete` 使用。CUDA 原样例还通过 `Container` 基类虚函数调用 `Vector`；Ascend C Device 侧不支持虚函数，因此本样例聚焦 `new/delete`，直接通过 `Vector` 调用。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- >= CANN 9.2.0

## 目录结构介绍

```text
├── CMakeLists.txt
├── README.md
├── container.hpp
└── newdelete.asc
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  ```bash
  source /home/zhangshunwei/cann/20260819/cann-9.2.0/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  ./demo
  ```

- 执行结果

  ```text
  newdelete Starting...

   > Vector test OK

   > Vector, using placement new on UB buffer test OK

   > Vector, with user defined datatype test OK

  Test Summary: 3/3 successfully run
  ```

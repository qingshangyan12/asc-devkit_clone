---
okftype: reference.index
title: 卷积格式转换
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/cube_datamove/cube_store_key_features/convolution_format_conversion.md
navigation:
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/cube_store_key_features/NZ2ND.md
    title: Nz2ND
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/cube_store_key_features/NZ2DN.md
    title: Nz2DN
timestamp: '2026-09-28'
---
# 卷积格式转换

在卷积运算中，L0C Buffer上存放的数据格式为NC1HWC0格式，Fixpipe指令通过[Nz2ND](NZ2ND.md)能力，支持将L0C Buffer上数据转为NHWC格式输出。转换示意图如下所示：

**图1** NC1HWC0转换为NHWC格式

![](../../../../figures/Fixpipe_NC1HWC0_NHWC.png)

<!-- npu="950" id1 -->
特别的，针对Ascend 950PR&950DT系列产品产品，还支持Fixpipe指令通过[Nz2DN](NZ2DN.md)能力，将L0C Buffer上数据转为NCHW格式输出。转换示意图如下所示：

**图2** NC1HWC0转换为NCHW格式

![](../../../../figures/Fixpipe_NC1HWC0_NCHW.png)
<!-- end id1 -->

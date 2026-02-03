
# 数据搬运类api样例介绍

## 概述

本路径下包含了与数据搬运相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。

## 算子开发样例

|  目录名称                                                  |  功能描述                                             |
| ----------------------------------------------------------- | --------------------------------------------------- |
| [copy](./copy) | 本样例基于Copy实现数据搬运，适用于VECIN，VECCALC，VECOUT之间的数据搬运，支持mask操作和DataBlock间隔操作 |
| [data_copy_pad](./data_copy_pad) | 本样例基于DataCopyPad实现数据非对齐搬运，其中从Global Memory搬运数据至Local Memory时，可以根据开发者的需要自行填充数据 |
| [nd2nz_during_data_movement](./nd2nz_during_data_movement) | 本样例基于DataCopy实现数据搬运，可用于在数据搬运时进行ND到NZ格式的转换|
| [nz2nd_during_data_movement](./nz2nd_during_data_movement) | 本样例基于DataCopy实现数据搬运，可用于在数据搬运时进行NZ到ND格式的转换|
| [set_pad_value](./set_pad_value) | 本样例基于SetPadValue为非对齐搬运DataCopyPad接口设置需要填充的数值 |
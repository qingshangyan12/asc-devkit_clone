# 07_data_movement样例介绍

## 概述

本样例路径以copy、data_copy_pad、data_copy_ub2l1等算子为示例，展示了数据搬运接口的使用。

## 算子开发样例

| 目录名称  | 功能描述 |
| --------- | --------- |
| [copy](./copy) | 本样例基于Copy实现数据搬运，适用于VECIN，VECCALC，VECOUT之间的数据搬运，支持mask操作和DataBlock间隔操作。 |
| [data_copy_pad](./data_copy_pad) | 本样例基于DataCopyPad实现数据非对齐搬运，其中从Global Memory搬运数据至Local Memory时，可以根据开发者的需要自行填充数据。 |
| [data_copy_ub2l1](./data_copy_ub2l1) | 本样例基于DataCopy实现UB->L1 Buffer的数据搬运，其中从UB搬运数据至L1 Buffer时，通过硬通道进行搬运。 |
| [data_copy_ub2l1_kfc](./data_copy_ub2l1_kfc) | 本样例通过Matmul高阶API注册KFC，将数据从UB搬运到L1 Buffer，然后进行mmad计算，不使用UB->L1 Buffer的硬件通道能力，底层是UB->GM->L1 Buffer的搬运，对比硬通道搬运性能较差。 |
| [nd2nz_during_data_movement](./nd2nz_during_data_movement) | 本样例基于DataCopy实现数据搬运，可用于在数据搬运时进行ND到NZ格式的转换。 |
| [nz2nd_during_data_movement](./nz2nd_during_data_movement) | 本样例基于DataCopy实现数据搬运，可用于在数据搬运时进行NZ到ND格式的转换。 |
| [scalar_quantized_activation_during](./scalar_quantized_activation_during) | 本样例基于DataCopy实现数据搬运，支持在数据搬运过程中进行scalar量化和Relu激活等操作。 |
| [set_pad_value](./set_pad_value) | 本样例基于SetPadValue为非对齐搬运DataCopyPad接口设置需要填充的数值。 |
| [slice_data_movement](./slice_data_movement) | 本样例通过Ascend C编程语言实现了DataCopy数据切片算子，支持数据的切片搬运，提取多维Tensor数据的子集进行搬运。使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。 |
| [tensor_quantized_activation_during](./tensor_quantized_activation_during) | 本样例基于DataCopy实现数据搬运，支持在数据搬运过程中进行tensor量化和Relu激活等操作。 |
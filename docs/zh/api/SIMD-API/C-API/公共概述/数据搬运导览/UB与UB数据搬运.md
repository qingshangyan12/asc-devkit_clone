# UB与UB数据搬运

| 基础API场景 | C API落点 |
|---|---|
| UB->UB连续/高维DataCopy | [asc_copy_ub2ub](../../vector_datamove/asc_copy_ub2ub.md) |
| UB->UB连续Copy | [asc_copy](../../vector_compute/asc_copy.md) |
| UB->UB掩码式高维Copy | [asc_copy](../../vector_compute/asc_copy.md)与[掩码配置接口](../../vector_compute/Memory矢量计算.md) |

前者属于数据搬运接口，后者属于Vector计算接口；掩码、repeat和stride的单位及覆盖范围应分别按C API文档确认。

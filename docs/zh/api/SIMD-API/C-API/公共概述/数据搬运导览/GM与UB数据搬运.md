# GM与UB数据搬运

| 基础API场景 | C API落点 | 状态 |
|---|---|---|
| GM->UB、UB->GM连续搬运 | [asc_copy_gm2ub](../../vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md)、[asc_copy_ub2gm](../../vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md) | 同一CCE搬运通路。 |
| GM->UB、UB->GM高维切分 | 两接口的burst/stride重载 | 参数模型不同，逐接口核对。 |
| GM->UB、UB->GM切片 | 无直接C API | 基础API软仿能力。 |
| GM->UB ND2NZ、UB->GM NZ2ND | 无同语义通用C API | 基础API软仿能力；C API格式转换主要见GM->L1和L0C搬出。 |
| GM->UB NDDMA | [asc_ndim_copy_gm2ub](../../vector_datamove/asc_ndim_copy_gm2ub.md) | 支持，需配置stride、Padding。 |
| GM->UB、UB->GM非对齐/Padding | [asc_copy_gm2ub_align](../../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)、[asc_copy_ub2gm_align](../../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md) | 支持，使用对应setter。 |

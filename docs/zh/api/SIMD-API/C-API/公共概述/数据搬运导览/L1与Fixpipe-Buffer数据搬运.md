# L1与Fixpipe Buffer数据搬运

基础API的L1 Buffer->Fixpipe Buffer连续和高维切分搬运，在C API中对应[asc_copy_l12fb](../../cube_datamove/asc_copy_l12fb/asc_copy_l12fb.md)。该接口用于将量化/激活等参数送至Fixpipe Buffer；地址单位、长度、对齐和后续[asc_set_l0c2gm_config](../../cube_compute/asc_set_l0c2gm_config.md)配置必须一起核对。

需求：Ascend 950新增支持asc_dcci_entire_ub.md，需要将impl和include做修改。


注意：本机环境是Ascend 950（架构3510），NUP。
由于实际执行的代码是cann包的代码，cann包的路径是/usr/local/Ascend/cann-9.2.0-beta.1/。我需要你修改cann包中的软链接，预期的目标是执行用例时,实际执行的代码是在本项目中新增的asc_dcci_entire_ub接口，因为当前cann包中并不支持3510架构下调用本接口。
为了验证接口的功能，我需要你在examples/02_simd_c_api/03_c_api/04_cache_ctrl新增针对本接口功能的测试用例，确保编译和功能没问题。注意为了测试功能，需要使ub对dcache是cacheable的，需要将ctrl[49]改为=1后再测试，因为如果不设置的话，就不需要考虑ub与dcache的一致性，那么无论是否调用本接口都没有差别。注意样例的readme中需要对设计的用例进行说明，以及给出接口是否可以合入的判断依据和结论。

给出你的计划，以及你需要向我确认的问题。

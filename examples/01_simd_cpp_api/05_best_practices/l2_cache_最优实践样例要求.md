背景：需要从已有的最优实践样例中涉及到l2 cache的内容，但是比较分散（只是其中一个优化点），我希望单独整理出一个样例只向读者介绍l2 cache mode的配置。


样例各个case的说明：
1. case1和case2目的是为了说明针对需要复用的数据，使能l2 cache能够优化性能。case1来源于样例examples\01_simd_cpp_api\05_best_practices\04_memory_access\data_copy的case6，其中COPY_DST为UB。
由于global tensor是默认使能l2 cache的，因此case2相较于case1的区别是调用了SetL2CacheHint接口将l2 cache关闭，可参考：docs\api\SIMD-API\基础数据结构\GlobalTensor\SetL2CacheHint.md。


2.case3来源于样例examples\01_simd_cpp_api\05_best_practices\04_memory_access\data_copy的case5，其中COPY_DST为UB。通过case1和case3的对比，希望说明由于L2 cache的容量有限，因此分片重复搬运较整块重复搬运，由于能够减少每次的搬运量，因此能够提高l2 cache的命中率，从而提升性能。

3.case4和case5则来源于样例examples\01_simd_cpp_api\05_best_practices\00_vector_compute\add_high_performance的case4和case5，目的是说明对于不需要复用的流式数据，关闭默认的l2 cache能够优化性能。

3.根据examples\01_simd_cpp_api\05_best_practices\00_vector_compute\add_high_performance\流式数据开启L2Cache时MTE2性能劣化根因分析.md，case6和case7是为了说明针对流式数据开启l2 cache还可能会影响其他算子的性能：
    流式数据写入 L2 Cache 后，会：
    - 占据宝贵的 L2 Cache 空间
    - 可能逐出其他热点数据（如卷积权重、后续可复用的特征图）
    - 逐出时还需将脏数据写回 HBM，进一步消耗带宽
    这是从**全局视角**看的负面效应，特别是在多算子流水线场景中。
因此case6是将case4和case1的两个算子合并到一起，来展示多算子流水线场景下，流式数据占据l2 cache空间的后果。case7相较于case6，仅仅将两个流式数据SetL2CacheHint接口将l2 cache关闭。

4.样例的展现格式参考：examples\01_simd_cpp_api\05_best_practices\04_memory_access\data_copy


5.希望读者能够通过这个样例，理解到l2 cache mode的配置，以及在不同场景下的影响。因此，需要以上两个已有的样例中readme的展现形式来完成一个新的readme文件。注意，md中可以预先将性能数据表格准备好，数据可以先空着，等后续测试完成后再填写。

6.生成的代码中，针对每一个case需要注释说明其设计的含义，针对核心代码需要有详细的注释。

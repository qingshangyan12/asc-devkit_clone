mkdir -p build && cd build;   # 创建并进入build目录
cmake ..; make -j;            # 编译工程
msprof ./simt_data_cache_hint_baseline                        # 执行样例
msprof ./simt_data_cache_hint_optimized                        # 执行样例
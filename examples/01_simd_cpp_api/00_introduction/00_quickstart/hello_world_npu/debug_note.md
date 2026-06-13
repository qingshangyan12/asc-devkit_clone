dfx_region存在硬件bug,使用前需插入pipe_barrier<>, 对实际流水有点影响, MTE2,MTE3启动，一次latency 300 cycles.

需要在算子尾巴压入NOP，保证数据能流出来

biu打印有1M限制，当前算子内容变多会导致整体数据太大，以至于打不出来。

每个AI core block会有一个channel，采样

- hello_world.asc
    - 增加 profiling sampler 独立线程
    - 使用 channel 53 CHANNEL_FFTS_PROFILE_BUFFER_TASK
    - 使用 88B StarsAccProfileConfig 启动 task-based profiling
    - 通过 rtProfSetProSwitch(PROF_TASK_TIME_MASK) 开关 runtime
    profiling
    - 采样数据写入 hello_world_prof.bin
    - kernel workload 改为真实 GM/UB DataCopy，并保留
    PROF_MARK_STAMP_COUNT=30
    - kernel launch 次数已恢复为 PROF_KERNEL_LAUNCH_COUNT=1
- CMakeLists.txt
    - 增加 CANN include/pkg_inc
    - 链接 libascend_hal.so、libascendcl.so、libruntime.so
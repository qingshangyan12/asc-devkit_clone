/**
 * xpuops_exception_dump - 初始化入口
 */
#include "xpuops_exception_dump.h"

#include "exception_callback.h"
#include "utils.h"

extern "C" int xpuopsExceptionDumpInit(const char *kernels[], size_t count)
{
    using namespace xpuops;

    // 1. 注册 Runtime exception callback（幂等）
    if (RegisterExceptionCallback() != 0) {
        return -1;
    }

    // 2. 设置 kernel name 前缀启用列表（count==0 对所有 kernel 生效）
    SetEnabledKernels(kernels, count);

    if (kernels == nullptr || count == 0) {
        LogInfo("exception dump enabled for ALL kernels");
    } else {
        for (size_t i = 0; i < count; i++) {
            LogInfo("exception dump enabled for kernel prefix: %s", kernels[i] == nullptr ? "(null)" : kernels[i]);
        }
    }
    LogInfo("dump dir: %s (XPUOPS_DUMP_DIR), level: %d (XPUOPS_DUMP_LEVEL)", GetDumpDir().c_str(), GetDumpLevel());
    return 0;
}

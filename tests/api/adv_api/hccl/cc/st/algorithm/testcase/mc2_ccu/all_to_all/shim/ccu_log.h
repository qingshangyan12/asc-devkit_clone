#ifndef CCU_ST_LOG_STUB_H
#define CCU_ST_LOG_STUB_H

#undef HCCL_INFO
#undef HCCL_WARNING
#undef HCCL_ERROR
#undef HCCL_DEBUG

#define HCCL_INFO(...)
#define HCCL_WARNING(...)
#define HCCL_ERROR(...)
#define HCCL_DEBUG(...)

#define CCU_CHK_RET(call) \
    do { \
        auto _ret = (call); \
        if (_ret != CcuResult::CCU_SUCCESS) { \
            return _ret; \
        } \
    } while (0)

#endif

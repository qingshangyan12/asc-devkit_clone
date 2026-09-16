#ifndef ASCENDC_MODULE_UTILS_PTR_H
#define ASCENDC_MODULE_UTILS_PTR_H
namespace AscendC {
template <typename U>
__aicore__ inline auto GetUnderlyingPtr(const U& val)
{
    if constexpr (Std::is_pointer_v<U>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}
} // namespace AscendC
#endif

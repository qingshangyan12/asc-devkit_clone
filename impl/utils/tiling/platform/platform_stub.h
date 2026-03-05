#ifndef PLATFORM_STUB_H
#define PLATFORM_STUB_H
namespace platform_ascendc {
bool GetPlatformResWithLock(const std::string &label, const std::string &key, std::string  &val)
{
    return false;
}
}
#endif
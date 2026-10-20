#include "basic_api/kernel_tpipe.h"
#include <cstdio>

int main()
{
    AscendC::TPipe pipe;
    AscendC::TBuf<AscendC::TPosition::VECCALC> ub;
    AscendC::TBuf<AscendC::TPosition::A1> l1;
    pipe.InitBuffer(ub, 256);
    pipe.InitBuffer(l1, 256);
    auto u = ub.Get<float>();
    auto c = l1.Get<half>();
    for (int i : {0, 1, 7, 31}) {
        auto ut = u[i];
        auto ct = c[i];
        auto up = reinterpret_cast<float*>(ut.GetPhyAddr());
        auto cp = reinterpret_cast<half*>(ct.GetPhyAddr());
        if (pipe.GetAbsAddr(up) != pipe.GetAbsAddr(ut) ||
            AscendC::GetAbsAddr(&pipe, up) != AscendC::GetAbsAddr(&pipe, ut) ||
            pipe.GetAbsAddr<half, AscendC::Hardware::L1>(cp) != pipe.GetAbsAddr(ct) ||
            AscendC::GetAbsAddr<half, AscendC::Hardware::L1>(&pipe, cp) != AscendC::GetAbsAddr(&pipe, ct)) {
            std::printf("FAIL GetAbsAddr offset=%d\n", i);
            return 1;
        }
    }
    std::puts("PASS GetAbsAddr CPU_DEBUG member/free UB/L1 offsets=0,1,7,31");
}

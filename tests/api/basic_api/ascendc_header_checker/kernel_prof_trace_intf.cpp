#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_prof_trace_intf.h"
#endif

// need #define ASCENDC_TRACE_ON

// // __aicore__ inline void TRACE_START(TraceId apid)
// extern "C" __global__ __aicore__ void KernelTestTRACE_START1() {
//     AscendC::TraceId apid;
//     AscendC::TRACE_START(apid);
// }

// // __aicore__ inline void TRACE_STOP(TraceId apid)
// extern "C" __global__ __aicore__ void KernelTestTRACE_STOP1() {
//     AscendC::TraceId apid;
//     AscendC::TRACE_STOP(apid);
// }

// // __aicore__ inline void MarkStamp();
// extern "C" __global__ __aicore__ void KernelTestMarkStamp1() {
//     MarkStamp();
// }

// // __aicore__ inline void MetricsProfStart();
// extern "C" __global__ __aicore__ void KernelTestMetricsProfStart1() {
//     MetricsProfStart();
// }

// // __aicore__ inline void MetricsProfStop();
// extern "C" __global__ __aicore__ void KernelTestMetricsProfStop1() {
//     MetricsProfStop();
// }

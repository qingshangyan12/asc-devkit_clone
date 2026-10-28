"""Host-only buffered probe contract test, without CANN libraries or a device."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class ProbeTest(unittest.TestCase):
    @unittest.skipUnless(shutil.which('g++'), 'requires g++')
    def test_runtime_switch_buffering_failures_and_cleanup(self):
        header = Path(__file__).resolve().parents[2] / 'impl/adv_api/detail/hccl/cc/src/common/mc2_ccu_perf_probe.h'
        source = r'''
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <initializer_list>
#include <type_traits>
static int clocks = 0;
static int logs = 0;
static bool fail = false;
static bool reverse = false;
static int FakeClock(clockid_t, timespec* ts) {
    ++clocks;
    if (fail) return -1;
    ts->tv_sec = 0;
    ts->tv_nsec = reverse ? 1000000 - clocks * 10 : clocks * 10;
    return 0;
}
static void FakeLog(const char* format, ...) {
    ++logs;
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    puts("");
}
#define clock_gettime FakeClock
#include "mc2_ccu_perf_probe.h"
#undef clock_gettime
using namespace Mc2CcuPerfProbe;
static_assert(!std::is_copy_constructible<KernelScope>::value, "scope must not double dump");
static bool Requested() {
    const char* value = std::getenv("MC2_PERF_ACCEPTANCE");
    return value != nullptr && std::strcmp(value, "1") == 0;
}
static void Run(bool complete, bool overflow = false, int status = 0) {
    const int before = logs;
    {
        KernelScope scope(Requested());
        PollBegin();
        if (complete) {
            Parsed();
            if (Enabled()) assert(Get().records[1].end == Get().parsed);
            Submitted(status);
            if (Enabled()) assert(Get().records[2].begin == Get().records[1].end);
        }
        if (overflow) { PollBegin(); }
        assert(logs == before); // no output inside measured operations
    }
    assert(!Enabled());
}
int main() {
    unsetenv("MC2_PERF_ACCEPTANCE"); Run(true);
    for (const char* value : {"", "0", "true", "01", "1 "}) {
        setenv("MC2_PERF_ACCEPTANCE", value, 1); Run(true);
    }
    assert(clocks == 0 && logs == 0);
    setenv("MC2_PERF_ACCEPTANCE", "1", 1); Run(true);
    assert(clocks == 4 && logs == 4);
    setenv("MC2_PERF_ACCEPTANCE", "0", 1); Run(true);
    assert(clocks == 4 && logs == 4);
    setenv("MC2_PERF_ACCEPTANCE", "1", 1); Run(false);
    fail = true; Run(true); fail = false;
    reverse = true; Run(true); reverse = false;
    Run(true, true);
    Run(true, false, 7);
    {
        KernelScope scope(true);
        const int before = clocks;
        Parsed(); // Missing PollBegin must not create a parameter interval.
        Submitted(0); // Missing Parsed must never become a success.
        assert(clocks == before && Get().status == -1);
    }
    {
        KernelScope scope(true);
        PollBegin();
        Parsed();
        // Model parameter loading failure: return without submission.
        assert(Get().valid == 3 && Get().status == -1);
    }
    assert(!Enabled());
}
'''
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'probe.cc').write_text(source)
            (root / header.name).write_text(header.read_text())
            (root / 'log.h').write_text('#define HCCL_RUN_INFO(...) FakeLog(__VA_ARGS__)\n')
            subprocess.run(['g++', '-std=c++14', '-Wall', '-Wextra', '-Werror', '-I', str(root), str(root / 'probe.cc'), '-o', str(root / 'probe')],
                           check=True)
            lines = subprocess.check_output([str(root / 'probe')], text=True).splitlines()
        self.assertEqual(len(lines), 26)
        self.assertIn('launch=1 seq=0 metric=host_ctx_load begin_ns=10 end_ns=20 cache=-1 status=0', lines[0])
        self.assertIn('metric=host_param_load begin_ns=20 end_ns=30', lines[1])
        self.assertIn('metric=host_kernel_submit begin_ns=30 end_ns=40', lines[2])
        self.assertIn('launch=1 records=3 dropped=0 valid=7 status=0', lines[3])
        self.assertIn('launch=2 records=1 dropped=0 valid=1 status=-1', lines[5])
        self.assertIn('launch=3 records=3 dropped=0 valid=7 status=-2', lines[9])
        self.assertIn('launch=4 records=3 dropped=0 valid=7 status=-2', lines[13])
        self.assertIn('launch=5 records=3 dropped=1 valid=7 status=0', lines[17])
        self.assertIn('launch=6 records=3 dropped=0 valid=7 status=7', lines[21])
        self.assertIn('launch=7 records=0 dropped=0 valid=0 status=-1', lines[22])
        self.assertIn('launch=8 records=2 dropped=0 valid=3 status=-1', lines[25])


if __name__ == '__main__':
    unittest.main()

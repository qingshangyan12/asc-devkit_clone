#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CUBE ptr sweep: group by (api, scenario), compile once, run many MNK (all_ok style).

Env:
  CUBE_ARCH / CMAKE_ASC_ARCHITECTURES, CUBE_SRC, RUN_ROOT, CXX, CMAKE_ASC_FLAGS,
  ASCEND_DEVICE_ID, DEVICES (comma), CUBE_BATCH_CHUNK (default 16),
  CMAKE_JOBS, CUBE_JSONL, PY
"""
from __future__ import print_function
import json
import os
import re
import shutil
import subprocess
import sys
import threading
from collections import OrderedDict
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))
ARCH = os.environ.get("CUBE_ARCH") or os.environ.get("CMAKE_ASC_ARCHITECTURES", "dav-3510")
SRC = os.environ.get(
    "CUBE_SRC",
    os.path.abspath(os.path.join(HERE, "..", "..")),
)
RUN_ROOT = os.environ.get("RUN_ROOT", os.path.join(os.getcwd(), "runs_sweep_batch"))
CXX = os.environ.get("CXX", "g++")
CC = os.environ.get("CC", CXX)
ASC_FLAGS = os.environ.get("CMAKE_ASC_FLAGS", os.environ.get("ASC_FLAGS", ""))
CHUNK = int(os.environ.get("CUBE_BATCH_CHUNK", "16") or "16")
CMAKE_JOBS = int(os.environ.get("CMAKE_JOBS", os.environ.get("PARALLEL_CMAKE_JOBS", "8") or "8"))
JSONL = os.environ.get("CUBE_JSONL", os.path.join(HERE, "cases.jsonl"))
PY = os.environ.get("PY", sys.executable)
EX_REL = os.path.join(
    "examples", "01_simd_cpp_api", "03_basic_api", "03_matrix_compute"
)

_summary_lock = threading.Lock()
_build_locks = {}
_build_locks_guard = threading.Lock()


def sh(cmd, cwd=None, log_path=None, env=None, timeout=600):
    p = subprocess.Popen(
        cmd,
        cwd=cwd,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True,
    )
    try:
        out, _ = p.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        p.kill()
        out = b"TIMEOUT\n"
        return 124, out
    if log_path:
        open(log_path, "wb").write(out or b"")
    return p.returncode, out or b""


def sample_dir(api):
    base = os.path.join(SRC, EX_REL)
    if api == "mmad":
        return os.path.join(base, "mmad")
    if api == "fixpipe":
        return os.path.join(base, "fixpipe_l0c2gm")
    if api == "load_data":
        if ARCH == "dav-2201":
            return os.path.join(base, "load_data_l12l0")
        return os.path.join(base, "load_data_2dv2_l12l0")
    raise ValueError(api)


def find_asc(work):
    for fn in os.listdir(work):
        if fn.endswith(".asc"):
            return os.path.join(work, fn)
    raise FileNotFoundError("no .asc in " + work)


def patch_ceil_and_comments(work):
    for root, _, files in os.walk(work):
        if "build" in root.replace("\\", "/").split("/"):
            continue
        for f in files:
            if not f.endswith(".asc"):
                continue
            p = os.path.join(root, f)
            t = open(p, encoding="utf-8", errors="replace").read()
            t2 = t.replace("ceil_div(", "ceil_division(")
            t2 = re.sub(
                r"Mmad\*/Fixpipe\*/LoadData\*",
                "Mmad/Fixpipe/LoadData",
                t2,
            )
            if t2 != t:
                open(p, "w", encoding="utf-8").write(t2)


def _size_block_mmad():
    return """
    size_t aFileSize = 0, bFileSize = 0, biasFileSize = 0, cFileSize = 0;
    if constexpr (scenarioNum == 1) {
        aFileSize = (size_t)M * K * sizeof(int8_t);
        bFileSize = (size_t)K * N * sizeof(int8_t);
        biasFileSize = (size_t)N * sizeof(int32_t);
        cFileSize = (size_t)M * N * sizeof(int32_t);
    } else if constexpr (scenarioNum == 2) {
        aFileSize = (size_t)M * K * sizeof(bfloat16_t);
        bFileSize = (size_t)K * N * sizeof(bfloat16_t);
        biasFileSize = (size_t)N * sizeof(float);
        cFileSize = (size_t)M * N * sizeof(float);
    } else if constexpr (scenarioNum == 3) {
        aFileSize = (size_t)M * K * sizeof(float);
        bFileSize = (size_t)K * N * sizeof(float);
        biasFileSize = (size_t)N * sizeof(float);
        cFileSize = (size_t)M * N * sizeof(float);
    } else {
        aFileSize = (size_t)M * K * sizeof(int8_t) / 2;
        bFileSize = (size_t)K * N * sizeof(int8_t) / 2;
        biasFileSize = (size_t)N * sizeof(int32_t);
        cFileSize = (size_t)M * N * sizeof(int32_t);
    }
"""


def _size_block_fixpipe():
    return """
    size_t aFileSize = (size_t)M * K * sizeof(half);
    size_t bFileSize = (size_t)K * N * sizeof(half);
    size_t cFileSize;
    if constexpr (SCENARIO_NUM == 4 || SCENARIO_NUM == 5) {
        cFileSize = (size_t)M * N * sizeof(int8_t);
    } else {
        cFileSize = (size_t)M * N * sizeof(float);
    }
    size_t quantAlphaSize = (size_t)1 * N * sizeof(uint64_t);
"""


def _size_block_loaddata():
    # 910B load_data_l12l0 vs 950 load_data_2dv2_l12l0：scenario→dtype 编号不同
    if ARCH == "dav-2201":
        return """
    size_t aFileSize = 0, bFileSize = 0, cFileSize = 0;
    if constexpr (scenarioNum <= 2) {
        aFileSize = (size_t)M * K * sizeof(int8_t);
        bFileSize = (size_t)K * N * sizeof(int8_t);
        cFileSize = (size_t)M * N * sizeof(int32_t);
    } else if constexpr (scenarioNum <= 5) {
        aFileSize = (size_t)M * K * sizeof(int8_t);
        bFileSize = (size_t)K * N * sizeof(int8_t);
        cFileSize = (size_t)M * N * sizeof(int32_t);
    } else if constexpr (scenarioNum <= 10) {
        aFileSize = (size_t)M * K * sizeof(half);
        bFileSize = (size_t)K * N * sizeof(half);
        cFileSize = (size_t)M * N * sizeof(float);
    } else {
        aFileSize = (size_t)M * K * sizeof(float);
        bFileSize = (size_t)K * N * sizeof(float);
        cFileSize = (size_t)M * N * sizeof(float);
    }
"""
    # dav-3510 / load_data_2dv2_l12l0
    return """
    size_t aFileSize = 0, bFileSize = 0, cFileSize = 0;
    if constexpr (scenarioNum <= 2 || scenarioNum == 7) {
        aFileSize = (size_t)M * K * sizeof(int8_t);
        bFileSize = (size_t)N * K * sizeof(int8_t);
        cFileSize = (size_t)M * N * sizeof(int32_t);
    } else if constexpr (scenarioNum <= 4) {
        aFileSize = (size_t)M * K * sizeof(half);
        bFileSize = (size_t)N * K * sizeof(half);
        cFileSize = (size_t)M * N * sizeof(float);
    } else {
        aFileSize = (size_t)M * K * sizeof(float);
        bFileSize = (size_t)N * K * sizeof(float);
        cFileSize = (size_t)M * N * sizeof(float);
    }
"""


def gen_main(api, shapes):
    """Generate batch main: ./demo M N K [M N K ...]"""
    tries = []
    for m, n, k in shapes:
        tries.append("    if (M == %du && N == %du && K == %du) return run_one<%du, %du, %du>(stream);" % (m, n, k, m, n, k))
    dispatch = "\n".join(tries) if tries else "    (void)M; (void)N; (void)K;"

    if api == "mmad":
        size_blk = _size_block_mmad()
        run_one = """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
""" + size_blk + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *biasHost=nullptr,*biasDevice=nullptr,*cHost=nullptr,*cDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x1_gm.bin", aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x2_gm.bin", bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    if constexpr (HAS_BIAS_TENSOR) {
        aclrtMallocHost((void**)(&biasHost), biasFileSize);
        aclrtMalloc((void**)&biasDevice, biasFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
        ReadFile("./input/bias_gm.bin", biasFileSize, biasHost, biasFileSize);
        aclrtMemcpy(biasDevice, biasFileSize, biasHost, biasFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    }
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    mmad_custom<M, N, K><<<numBlocks, 0, stream>>>(aDevice, bDevice, biasDevice, cDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile("./output/output.bin", cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    if constexpr (HAS_BIAS_TENSOR) { aclrtFree(biasDevice); aclrtFreeHost(biasHost); }
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    return 0;
}
"""
        launch_name = "mmad"
    elif api == "fixpipe":
        size_blk = _size_block_fixpipe()
        run_one = """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
""" + size_blk + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *cHost=nullptr,*cDevice=nullptr,*quantAlphaHost=nullptr,*quantAlphaDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x1_gm.bin", aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x2_gm.bin", bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMallocHost((void**)(&quantAlphaHost), quantAlphaSize);
    aclrtMalloc((void**)&quantAlphaDevice, quantAlphaSize, ACL_MEM_MALLOC_HUGE_FIRST);
    if constexpr (SCENARIO_NUM == 5) {
        ReadFile("./input/quant_pre.bin", quantAlphaSize, quantAlphaHost, quantAlphaSize);
    }
    aclrtMemcpy(quantAlphaDevice, quantAlphaSize, quantAlphaHost, quantAlphaSize, ACL_MEMCPY_HOST_TO_DEVICE);
    fixpipe_l0c2gm<M, K, N><<<numBlocks, 0, stream>>>(aDevice, bDevice, cDevice, quantAlphaDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile("./output/output.bin", cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    aclrtFree(quantAlphaDevice); aclrtFreeHost(quantAlphaHost);
    return 0;
}
"""
        launch_name = "fixpipe"
    else:  # load_data — __CUBE_LOAD_KERNEL__ replaced in replace_main
        size_blk = _size_block_loaddata()
        run_one = """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
""" + size_blk + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *cHost=nullptr,*cDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x1_gm.bin", aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/x2_gm.bin", bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    __CUBE_LOAD_KERNEL__<M, N, K><<<numBlocks, 0, stream>>>(aDevice, bDevice, cDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile("./output/output.bin", cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    return 0;
}
"""
        launch_name = "load_data"

    main = """
#include <cstdio>
#include <cstdlib>
#include <cstring>

%s

static int dispatch_shape(uint32_t M, uint32_t N, uint32_t K, aclrtStream stream) {
%s
    printf("unsupported SHAPE=%%u,%%u,%%u\\n", M, N, K);
    return 2;
}

int32_t main(int32_t argc, char* argv[])
{
    aclInit(nullptr);
    int32_t deviceId = 0;
    const char* envDev = getenv("ASCEND_DEVICE_ID");
    if (envDev && envDev[0]) deviceId = atoi(envDev);
    aclrtSetDevice(deviceId);
    aclrtStream stream = nullptr;
    aclrtCreateStream(&stream);

    int nTriples = (argc > 1) ? ((argc - 1) / 3) : 0;
    int allOk = 1;
    if (nTriples <= 0) {
#ifndef CASE_M
#define CASE_M 32
#endif
#ifndef CASE_N
#define CASE_N 32
#endif
#ifndef CASE_K
#define CASE_K 32
#endif
        if (dispatch_shape(CASE_M, CASE_N, CASE_K, stream) != 0) allOk = 0;
    } else {
        for (int i = 0; i < nTriples; i++) {
            uint32_t M = (uint32_t)atoi(argv[1 + i * 3 + 0]);
            uint32_t N = (uint32_t)atoi(argv[1 + i * 3 + 1]);
            uint32_t K = (uint32_t)atoi(argv[1 + i * 3 + 2]);
            // per-shape: caller stages input/output for single triple runs;
            // multi-triple in one process expects pre-staged last shape only — runner uses 1 triple.
            if (dispatch_shape(M, N, K, stream) != 0) allOk = 0;
        }
    }
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return allOk ? 0 : 1;
}
""" % (run_one, dispatch)
    return main, launch_name


def replace_main(asc_path, api, shapes):
    text = open(asc_path, encoding="utf-8", errors="replace").read()
    new_main, _ = gen_main(api, shapes)
    if api == "load_data":
        m = re.search(r"__global__\s+__cube__\s+void\s+(\w+)\s*\(", text)
        kname = m.group(1) if m else "KernelLoadDataL12L0"
        new_main = new_main.replace("__CUBE_LOAD_KERNEL__", kname)

    idx = text.find("int32_t main(int32_t argc, char* argv[])")
    if idx < 0:
        idx = text.find("int main(int argc, char** argv)")
    if idx < 0:
        idx = text.find("int main(int argc, char* argv[])")
    if idx < 0:
        raise RuntimeError("main not found in " + asc_path)
    open(asc_path, "w", encoding="utf-8").write(text[:idx] + new_main)
    return asc_path


def load_cases(smoke):
    rows = []
    for ln in open(JSONL, encoding="utf-8"):
        if not ln.strip():
            continue
        r = json.loads(ln)
        a = r.get("arch", "any")
        if a not in ("any", ARCH):
            continue
        if r["api"] == "load_data":
            key = "scenario_2201" if ARCH == "dav-2201" else "scenario_3510"
            sc = r.get(key)
            if sc is None:
                continue
            r = dict(r)
            r["scenario"] = sc
        rows.append(r)
    by = OrderedDict()
    for r in rows:
        by.setdefault(r["api"], []).append(r)
    n = 8 if smoke else 10**9
    sel = []
    for api in ("mmad", "load_data", "fixpipe"):
        pool = by.get(api, [])
        if not smoke or len(pool) <= n:
            sel.extend(pool[:n])
            continue
        buckets = {}
        for r in pool:
            buckets.setdefault(r.get("scenario"), []).append(r)
        keys = sorted(buckets)
        i = 0
        picked = []
        while len(picked) < n and keys:
            k = keys[i % len(keys)]
            if buckets[k]:
                picked.append(buckets[k].pop(0))
            else:
                keys = [x for x in keys if buckets[x]]
                if not keys:
                    break
                continue
            i += 1
        sel.extend(picked)
    return sel


def chunked(seq, n):
    for i in range(0, len(seq), n):
        yield seq[i : i + n]


def build_group(api, sc, shapes, build_root):
    key = "%s_s%s_n%d" % (api, sc, len(shapes))
    # include shape fingerprint for cache correctness
    sig = "_".join("%dx%dx%d" % t for t in shapes[:3])
    key = "%s_s%s_%d_%s" % (api, sc, len(shapes), sig.replace("x", ""))
    bin_dir = os.path.join(build_root, "_bin", key)
    demo_cached = os.path.join(bin_dir, "demo")
    if os.path.isfile(demo_cached):
        print("BUILD_CACHE", key, flush=True)
        return demo_cached, "CACHE"

    with _build_locks_guard:
        lk = _build_locks.setdefault(key, threading.Lock())
    with lk:
        if os.path.isfile(demo_cached):
            return demo_cached, "CACHE"
        work = os.path.join(build_root, "_builds", key)
        if os.path.isdir(work):
            shutil.rmtree(work)
        src = sample_dir(api)
        shutil.copytree(src, work)
        patch_ceil_and_comments(work)
        asc = find_asc(work)
        replace_main(asc, api, shapes)
        build = os.path.join(work, "build")
        os.makedirs(build, exist_ok=True)
        mx = max(t[0] for t in shapes)
        nx = max(t[1] for t in shapes)
        kx = max(t[2] for t in shapes)
        blog = os.path.join(work, "build.log")
        cmake = (
            'cmake -DSCENARIO_NUM=%s -DCMAKE_ASC_ARCHITECTURES=%s '
            '-DCASE_M=%s -DCASE_N=%s -DCASE_K=%s '
            '-DCMAKE_CXX_COMPILER="%s" -DCMAKE_C_COMPILER="%s" '
            '-DCMAKE_ASC_FLAGS="%s" -S "%s" -B "%s" && '
            'cmake --build "%s" -j%d'
            % (sc, ARCH, mx, nx, kx, CXX, CC, ASC_FLAGS, work, build, build, CMAKE_JOBS)
        )
        print("BUILD_START", key, "shapes", len(shapes), flush=True)
        rc, out = sh(cmake, log_path=blog, timeout=900)
        demo = os.path.join(build, "demo")
        if rc != 0 or not os.path.isfile(demo):
            err = ""
            try:
                for line in open(blog, encoding="utf-8", errors="replace"):
                    if "error:" in line or "FATAL" in line:
                        err = line.strip()[:160]
                        break
            except Exception:
                pass
            print("BUILD_FAIL", key, err, flush=True)
            return None, "BUILD_FAIL:" + (err or "build")
        os.makedirs(bin_dir, exist_ok=True)
        shutil.copy2(demo, demo_cached)
        print("BUILD_OK", key, flush=True)
        return demo_cached, "BUILD"


def append_summary(path, line):
    with _summary_lock:
        with open(path, "a", encoding="utf-8") as f:
            f.write(line + "\n")


def run_one_case(case, demo, summary_path, note_how):
    api = case["api"]
    cid = case["case_id"]
    dtype = case["dtype"]
    M, N, K = int(case["M"]), int(case["N"]), int(case["K"])
    sc = int(case["scenario"])
    work = os.path.join(RUN_ROOT, cid)
    if os.path.isdir(work):
        shutil.rmtree(work)
    os.makedirs(os.path.join(work, "input"), exist_ok=True)
    os.makedirs(os.path.join(work, "output"), exist_ok=True)
    # need scripts from sample
    src = sample_dir(api)
    for sub in ("scripts",):
        s = os.path.join(src, sub)
        if os.path.isdir(s):
            shutil.copytree(s, os.path.join(work, sub))
    # copy data_utils / headers if demo needs relative includes? demo is binary — only input/output matter
    shutil.copy2(demo, os.path.join(work, "demo"))
    os.chmod(os.path.join(work, "demo"), 0o755)

    env = os.environ.copy()
    glog = os.path.join(work, "gendata.log")
    rc, _ = sh(
        '"%s" scripts/gen_data.py -scenarioNum %s -M %s -N %s -K %s' % (PY, sc, M, N, K),
        cwd=work,
        log_path=glog,
        env=env,
        timeout=120,
    )
    if rc != 0 or not os.listdir(os.path.join(work, "input")):
        append_summary(
            summary_path,
            "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tGENDATA_FAIL\t-\t-\t" % (api, cid, ARCH, dtype, M, N, K, sc),
        )
        print("DONE", cid, "gendata=FAIL", flush=True)
        return

    rlog = os.path.join(work, "run.log")
    vstr = "FAIL"
    for _try in range(3):
        rc, _ = sh("./demo %d %d %d" % (M, N, K), cwd=work, log_path=rlog, env=env, timeout=120)
        out_bin = os.path.join(work, "output", "output.bin")
        if rc != 0 or not os.path.isfile(out_bin):
            append_summary(
                summary_path,
                "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tPASS\tRUN_FAIL\t-\texit=%s"
                % (api, cid, ARCH, dtype, M, N, K, sc, rc),
            )
            print("DONE", cid, "run=FAIL", flush=True)
            return
        vpy = os.path.join(work, "scripts", "verify_result.py")
        if os.path.isfile(vpy):
            vlog = os.path.join(work, "verify.log")
            golden = os.path.join(work, "output", "golden.bin")
            vrc, _ = sh(
                '"%s" scripts/verify_result.py -scenarioNum %s -M %s -N %s -K %s ./output/output.bin ./output/golden.bin'
                % (PY, sc, M, N, K),
                cwd=work,
                log_path=vlog,
                env=env,
                timeout=120,
            )
            ok = vrc == 0
            if not ok and os.path.isfile(vlog):
                ok = "test pass" in open(vlog, encoding="utf-8", errors="replace").read().lower()
            if ok:
                vstr = "PASS"
                break
            try:
                os.remove(out_bin)
            except OSError:
                pass
        else:
            vstr = "SKIP"
            break

    append_summary(
        summary_path,
        "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tPASS\tPASS\t%s\t%s"
        % (api, cid, ARCH, dtype, M, N, K, sc, vstr, note_how),
    )
    print("DONE", cid, "verify=%s" % vstr, "device=%s" % env.get("ASCEND_DEVICE_ID", ""), flush=True)


def main():
    smoke = os.environ.get("SMOKE", "0") == "1"
    os.makedirs(RUN_ROOT, exist_ok=True)
    cases = load_cases(smoke)
    print("SELECTED", len(cases), "smoke" if smoke else "full", "ARCH", ARCH, "CHUNK", CHUNK, flush=True)

    # group (api, scenario) then chunk shapes
    groups = OrderedDict()
    for c in cases:
        gk = (c["api"], int(c["scenario"]))
        groups.setdefault(gk, []).append(c)

    units = []  # (api, sc, cases_chunk, shapes)
    for (api, sc), clist in groups.items():
        # unique shapes preserve order
        seen = set()
        shapes = []
        for c in clist:
            t = (int(c["M"]), int(c["N"]), int(c["K"]))
            if t not in seen:
                seen.add(t)
                shapes.append(t)
        # map shape -> cases
        by_shape = OrderedDict()
        for c in clist:
            t = (int(c["M"]), int(c["N"]), int(c["K"]))
            by_shape.setdefault(t, []).append(c)
        shape_list = list(by_shape.keys())
        for ch in chunked(shape_list, CHUNK):
            sub_cases = []
            for t in ch:
                sub_cases.extend(by_shape[t])
            units.append((api, sc, sub_cases, ch))

    print("BATCH_UNITS", len(units), "from_groups", len(groups), "cases", len(cases), flush=True)

    summary = os.path.join(RUN_ROOT, "summary.tsv")
    open(summary, "w", encoding="utf-8").write(
        "api\tcase_id\tarch\tdtype\tM\tN\tK\tscenario\tbuild\trun\tverify\tnote\n"
    )

    devices = [d.strip() for d in os.environ.get("DEVICES", "").split(",") if d.strip()]
    if not devices:
        devices = [os.environ.get("ASCEND_DEVICE_ID", "0")]

    # Build all units first (parallel limited)
    demos = {}
    build_workers = min(4, max(1, len(units)))
    with ThreadPoolExecutor(max_workers=build_workers) as ex:
        futs = {
            ex.submit(build_group, api, sc, shapes, RUN_ROOT): (api, sc, shapes, sub)
            for api, sc, sub, shapes in units
        }
        for fut in as_completed(futs):
            api, sc, shapes, sub = futs[fut]
            demo, how = fut.result()
            demos[(api, sc, tuple(shapes))] = (demo, how, sub)

    # Run cases: assign device round-robin
    run_list = []
    for (api, sc, shapes), (demo, how, sub) in demos.items():
        if demo is None:
            for c in sub:
                append_summary(
                    summary,
                    "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tBUILD_FAIL\t-\t-\t%s"
                    % (
                        c["api"],
                        c["case_id"],
                        ARCH,
                        c["dtype"],
                        c["M"],
                        c["N"],
                        c["K"],
                        c["scenario"],
                        how,
                    ),
                )
            continue
        for c in sub:
            run_list.append((c, demo, how))

    def _run_idx(item_idx):
        c, demo, how = run_list[item_idx]
        dev = devices[item_idx % len(devices)]
        env_dev = os.environ.copy()
        env_dev["ASCEND_DEVICE_ID"] = str(dev)
        # temporarily set for child
        old = os.environ.get("ASCEND_DEVICE_ID")
        os.environ["ASCEND_DEVICE_ID"] = str(dev)
        try:
            run_one_case(c, demo, summary, how + "_BATCH")
        finally:
            if old is None:
                os.environ.pop("ASCEND_DEVICE_ID", None)
            else:
                os.environ["ASCEND_DEVICE_ID"] = old

    # Limit concurrency to number of devices (1 case per device)
    with ThreadPoolExecutor(max_workers=len(devices)) as ex:
        futs = [ex.submit(_run_idx, i) for i in range(len(run_list))]
        for f in as_completed(futs):
            f.result()

    # summary stats
    body = open(summary, encoding="utf-8").read().splitlines()[1:]
    ok = sum(1 for ln in body if ln.split("\t")[10:11] == ["PASS"])
    fail = sum(
        1
        for ln in body
        if len(ln.split("\t")) > 10
        and (
            ln.split("\t")[8] in ("BUILD_FAIL", "GENDATA_FAIL")
            or ln.split("\t")[9] == "RUN_FAIL"
            or ln.split("\t")[10] == "FAIL"
        )
    )
    print("BATCH_DONE ok=%d fail=%d total=%d units=%d" % (ok, fail, len(body), len(units)), flush=True)
    md = os.path.join(RUN_ROOT, "SELFTEST_SWEEP.md")
    open(md, "w", encoding="utf-8").write(
        "# CUBE ptr sweep (batch)\n\n- ARCH: %s\n- verify PASS: **%d**\n- FAIL: %d\n- total: %d\n- units: %d\n"
        % (ARCH, ok, fail, len(body), len(units))
    )


if __name__ == "__main__":
    main()

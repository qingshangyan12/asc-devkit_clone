#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CUBE ptr sweep: group by (api, scenario), compile once, continuous multi-MNK.

One ./demo process per unit runs many shapes (I/O under m{M}n{N}k{K}/).
gendata is once per shape and reused across retries / re-runs if present.

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


def _io_paths_decl():
    return """
    char p_x1[320], p_x2[320], p_out[320], p_bias[320], p_quant[320];
    snprintf(p_x1, sizeof(p_x1), "m%un%uk%u/input/x1_gm.bin", M, N, K);
    snprintf(p_x2, sizeof(p_x2), "m%un%uk%u/input/x2_gm.bin", M, N, K);
    snprintf(p_out, sizeof(p_out), "m%un%uk%u/output/output.bin", M, N, K);
    snprintf(p_bias, sizeof(p_bias), "m%un%uk%u/input/bias_gm.bin", M, N, K);
    snprintf(p_quant, sizeof(p_quant), "m%un%uk%u/input/quant_pre.bin", M, N, K);
"""


def gen_main(api, shapes):
    """Generate batch main: one process, many MNK (subdir m{M}n{N}k{K}/)."""
    tries = []
    for m, n, k in shapes:
        tries.append(
            "    if (M == %du && N == %du && K == %du) return run_one<%du, %du, %du>(stream);"
            % (m, n, k, m, n, k)
        )
    dispatch = "\n".join(tries) if tries else "    (void)M; (void)N; (void)K;"
    paths = _io_paths_decl()

    if api == "mmad":
        size_blk = _size_block_mmad()
        run_one = (
            """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
"""
            + size_blk
            + paths
            + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *biasHost=nullptr,*biasDevice=nullptr,*cHost=nullptr,*cDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x1, aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x2, bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    if constexpr (HAS_BIAS_TENSOR) {
        aclrtMallocHost((void**)(&biasHost), biasFileSize);
        aclrtMalloc((void**)&biasDevice, biasFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
        ReadFile(p_bias, biasFileSize, biasHost, biasFileSize);
        aclrtMemcpy(biasDevice, biasFileSize, biasHost, biasFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    }
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    mmad_custom<M, N, K><<<numBlocks, 0, stream>>>(aDevice, bDevice, biasDevice, cDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile(p_out, cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    if constexpr (HAS_BIAS_TENSOR) { aclrtFree(biasDevice); aclrtFreeHost(biasHost); }
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    return 0;
}
"""
        )
    elif api == "fixpipe":
        size_blk = _size_block_fixpipe()
        run_one = (
            """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
"""
            + size_blk
            + paths
            + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *cHost=nullptr,*cDevice=nullptr,*quantAlphaHost=nullptr,*quantAlphaDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x1, aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x2, bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMallocHost((void**)(&quantAlphaHost), quantAlphaSize);
    aclrtMalloc((void**)&quantAlphaDevice, quantAlphaSize, ACL_MEM_MALLOC_HUGE_FIRST);
    if constexpr (SCENARIO_NUM == 5) {
        ReadFile(p_quant, quantAlphaSize, quantAlphaHost, quantAlphaSize);
    }
    aclrtMemcpy(quantAlphaDevice, quantAlphaSize, quantAlphaHost, quantAlphaSize, ACL_MEMCPY_HOST_TO_DEVICE);
    fixpipe_l0c2gm<M, K, N><<<numBlocks, 0, stream>>>(aDevice, bDevice, cDevice, quantAlphaDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile(p_out, cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    aclrtFree(quantAlphaDevice); aclrtFreeHost(quantAlphaHost);
    return 0;
}
"""
        )
    else:
        size_blk = _size_block_loaddata()
        run_one = (
            """
template <uint32_t M, uint32_t N, uint32_t K>
static int run_one(aclrtStream stream) {
"""
            + size_blk
            + paths
            + """
    uint32_t numBlocks = 1;
    uint8_t *aHost=nullptr,*aDevice=nullptr,*bHost=nullptr,*bDevice=nullptr;
    uint8_t *cHost=nullptr,*cDevice=nullptr;
    aclrtMallocHost((void**)(&aHost), aFileSize);
    aclrtMalloc((void**)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x1, aFileSize, aHost, aFileSize);
    aclrtMemcpy(aDevice, aFileSize, aHost, aFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&bHost), bFileSize);
    aclrtMalloc((void**)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile(p_x2, bFileSize, bHost, bFileSize);
    aclrtMemcpy(bDevice, bFileSize, bHost, bFileSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMallocHost((void**)(&cHost), cFileSize);
    aclrtMalloc((void**)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    __CUBE_LOAD_KERNEL__<M, N, K><<<numBlocks, 0, stream>>>(aDevice, bDevice, cDevice);
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, cFileSize, cDevice, cFileSize, ACL_MEMCPY_DEVICE_TO_HOST);
    WriteFile(p_out, cHost, cFileSize);
    printf("SHAPE=%u,%u,%u done\\n", M, N, K);
    aclrtFree(aDevice); aclrtFreeHost(aHost);
    aclrtFree(bDevice); aclrtFreeHost(bHost);
    aclrtFree(cDevice); aclrtFreeHost(cHost);
    return 0;
}
"""
        )

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
        printf("usage: ./demo M N K [M N K ...]\\n");
        allOk = 0;
    } else {
        for (int i = 0; i < nTriples; i++) {
            uint32_t M = (uint32_t)atoi(argv[1 + i * 3 + 0]);
            uint32_t N = (uint32_t)atoi(argv[1 + i * 3 + 1]);
            uint32_t K = (uint32_t)atoi(argv[1 + i * 3 + 2]);
            if (dispatch_shape(M, N, K, stream) != 0) allOk = 0;
        }
    }
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return allOk ? 0 : 1;
}
""" % (
        run_one,
        dispatch,
    )
    return main, api


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


def shape_dir_name(M, N, K):
    return "m%un%uk%u" % (int(M), int(N), int(K))


def build_group(api, sc, shapes, build_root):
    # iov2: multi-shape I/O under m{M}n{N}k{K}/ — bust old flat-input demo cache
    sig = "_".join("%dx%dx%d" % t for t in shapes[:3])
    key = "iov2_%s_s%s_%d_%s" % (api, sc, len(shapes), sig.replace("x", ""))
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


def _gen_shape_data(api, sc, M, N, K, shape_work, scripts_dir, env):
    """Generate input/golden once under shape_work; skip if already present."""
    inp = os.path.join(shape_work, "input")
    outp = os.path.join(shape_work, "output")
    os.makedirs(inp, exist_ok=True)
    os.makedirs(outp, exist_ok=True)
    marker = os.path.join(inp, "x1_gm.bin")
    golden = os.path.join(outp, "golden.bin")
    if os.path.isfile(marker) and os.path.isfile(golden):
        return True, "REUSE"
    gen_py = os.path.join(scripts_dir, "gen_data.py")
    glog = os.path.join(shape_work, "gendata.log")
    rc, _ = sh(
        '"%s" "%s" -scenarioNum %s -M %s -N %s -K %s' % (PY, gen_py, sc, M, N, K),
        cwd=shape_work,
        log_path=glog,
        env=env,
        timeout=120,
    )
    if rc != 0 or not os.path.isfile(marker):
        return False, "GENDATA_FAIL"
    return True, "GEN"


def _verify_shape(sc, M, N, K, shape_work, scripts_dir, env):
    vpy = os.path.join(scripts_dir, "verify_result.py")
    out_bin = os.path.join(shape_work, "output", "output.bin")
    if not os.path.isfile(out_bin):
        return "FAIL"
    if not os.path.isfile(vpy):
        return "SKIP"
    vlog = os.path.join(shape_work, "verify.log")
    vrc, _ = sh(
        '"%s" "%s" -scenarioNum %s -M %s -N %s -K %s ./output/output.bin ./output/golden.bin'
        % (PY, vpy, sc, M, N, K),
        cwd=shape_work,
        log_path=vlog,
        env=env,
        timeout=120,
    )
    ok = vrc == 0
    if not ok and os.path.isfile(vlog):
        ok = "test pass" in open(vlog, encoding="utf-8", errors="replace").read().lower()
    return "PASS" if ok else "FAIL"


def run_unit(api, sc, cases, shapes, demo, summary_path, note_how, unit_idx):
    """One process: gen each shape once (reuse if present), ./demo continuous, verify."""
    work = os.path.join(RUN_ROOT, "_run", "u%d_%s_s%s" % (unit_idx, api, sc))
    os.makedirs(work, exist_ok=True)
    src = sample_dir(api)
    scripts_dst = os.path.join(work, "scripts")
    if not os.path.isdir(scripts_dst):
        s = os.path.join(src, "scripts")
        if os.path.isdir(s):
            shutil.copytree(s, scripts_dst)
    shutil.copy2(demo, os.path.join(work, "demo"))
    os.chmod(os.path.join(work, "demo"), 0o755)

    env = os.environ.copy()
    by_shape = OrderedDict()
    for c in cases:
        t = (int(c["M"]), int(c["N"]), int(c["K"]))
        by_shape.setdefault(t, []).append(c)

    ready_shapes = []
    for t in shapes:
        M, N, K = t
        sdir = os.path.join(work, shape_dir_name(M, N, K))
        ok, how = _gen_shape_data(api, sc, M, N, K, sdir, scripts_dst, env)
        if not ok:
            for c in by_shape.get(t, []):
                append_summary(
                    summary_path,
                    "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tGENDATA_FAIL\t-\t-\t"
                    % (api, c["case_id"], ARCH, c["dtype"], M, N, K, sc),
                )
                print("DONE", c["case_id"], "gendata=FAIL", flush=True)
            continue
        ready_shapes.append(t)
        if how == "REUSE":
            print("GENDATA_REUSE", shape_dir_name(M, N, K), flush=True)

    if not ready_shapes:
        return

    args = " ".join("%d %d %d" % t for t in ready_shapes)
    rlog = os.path.join(work, "run.log")
    timeout = max(120, 30 * len(ready_shapes))
    print("RUN_CONT", api, "s%s" % sc, "shapes", len(ready_shapes), flush=True)
    rc, _ = sh("./demo %s" % args, cwd=work, log_path=rlog, env=env, timeout=timeout)
    run_ok = rc == 0

    for t in ready_shapes:
        M, N, K = t
        sdir = os.path.join(work, shape_dir_name(M, N, K))
        out_bin = os.path.join(sdir, "output", "output.bin")
        clist = by_shape.get(t, [])
        if not run_ok and not os.path.isfile(out_bin):
            for c in clist:
                append_summary(
                    summary_path,
                    "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tPASS\tRUN_FAIL\t-\texit=%s"
                    % (api, c["case_id"], ARCH, c["dtype"], M, N, K, sc, rc),
                )
                print("DONE", c["case_id"], "run=FAIL", flush=True)
            continue

        vstr = "FAIL"
        for _try in range(3):
            if _try > 0:
                try:
                    if os.path.isfile(out_bin):
                        os.remove(out_bin)
                except OSError:
                    pass
                trc, _ = sh(
                    "./demo %d %d %d" % (M, N, K),
                    cwd=work,
                    log_path=os.path.join(sdir, "run_retry.log"),
                    env=env,
                    timeout=120,
                )
                if trc != 0 or not os.path.isfile(out_bin):
                    vstr = "FAIL"
                    break
            vstr = _verify_shape(sc, M, N, K, sdir, scripts_dst, env)
            if vstr in ("PASS", "SKIP"):
                break

        if not os.path.isfile(out_bin) and vstr == "FAIL":
            for c in clist:
                append_summary(
                    summary_path,
                    "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tPASS\tRUN_FAIL\t-\tretry"
                    % (api, c["case_id"], ARCH, c["dtype"], M, N, K, sc),
                )
                print("DONE", c["case_id"], "run=FAIL", flush=True)
            continue

        for i, c in enumerate(clist):
            note = note_how if i == 0 else (note_how + "_SHAREdata")
            append_summary(
                summary_path,
                "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tPASS\tPASS\t%s\t%s"
                % (api, c["case_id"], ARCH, c["dtype"], M, N, K, sc, vstr, note),
            )
            print(
                "DONE",
                c["case_id"],
                "verify=%s" % vstr,
                "device=%s" % env.get("ASCEND_DEVICE_ID", ""),
                flush=True,
            )


def main():
    smoke = os.environ.get("SMOKE", "0") == "1"
    os.makedirs(RUN_ROOT, exist_ok=True)
    cases = load_cases(smoke)
    print("SELECTED", len(cases), "smoke" if smoke else "full", "ARCH", ARCH, "CHUNK", CHUNK, flush=True)

    groups = OrderedDict()
    for c in cases:
        gk = (c["api"], int(c["scenario"]))
        groups.setdefault(gk, []).append(c)

    units = []
    for (api, sc), clist in groups.items():
        by_shape = OrderedDict()
        for c in clist:
            t = (int(c["M"]), int(c["N"]), int(c["K"]))
            by_shape.setdefault(t, []).append(c)
        shape_list = list(by_shape.keys())
        for ch in chunked(shape_list, CHUNK):
            sub_cases = []
            for t in ch:
                sub_cases.extend(by_shape[t])
            units.append((api, sc, sub_cases, list(ch)))

    print("BATCH_UNITS", len(units), "from_groups", len(groups), "cases", len(cases), flush=True)

    summary = os.path.join(RUN_ROOT, "summary.tsv")
    open(summary, "w", encoding="utf-8").write(
        "api\tcase_id\tarch\tdtype\tM\tN\tK\tscenario\tbuild\trun\tverify\tnote\n"
    )

    devices = [d.strip() for d in os.environ.get("DEVICES", "").split(",") if d.strip()]
    if not devices:
        devices = [os.environ.get("ASCEND_DEVICE_ID", "0")]

    demos = {}
    build_workers = min(4, max(1, len(units)))
    with ThreadPoolExecutor(max_workers=build_workers) as ex:
        futs = {
            ex.submit(build_group, api, sc, shapes, RUN_ROOT): (ui, api, sc, shapes, sub)
            for ui, (api, sc, sub, shapes) in enumerate(units)
        }
        for fut in as_completed(futs):
            ui, api, sc, shapes, sub = futs[fut]
            demo, how = fut.result()
            demos[ui] = (demo, how, api, sc, shapes, sub)

    run_units = []
    for ui in sorted(demos.keys()):
        demo, how, api, sc, shapes, sub = demos[ui]
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
        run_units.append((ui, api, sc, sub, shapes, demo, how))

    def _run_unit_idx(item_idx):
        ui, api, sc, sub, shapes, demo, how = run_units[item_idx]
        dev = devices[item_idx % len(devices)]
        old = os.environ.get("ASCEND_DEVICE_ID")
        os.environ["ASCEND_DEVICE_ID"] = str(dev)
        try:
            run_unit(api, sc, sub, shapes, demo, summary, how + "_CONT", ui)
        finally:
            if old is None:
                os.environ.pop("ASCEND_DEVICE_ID", None)
            else:
                os.environ["ASCEND_DEVICE_ID"] = old

    with ThreadPoolExecutor(max_workers=len(devices)) as ex:
        futs = [ex.submit(_run_unit_idx, i) for i in range(len(run_units))]
        for f in as_completed(futs):
            f.result()

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
        "# CUBE ptr sweep (batch continuous)\n\n- ARCH: %s\n- verify PASS: **%d**\n- FAIL: %d\n- total: %d\n- units: %d\n"
        % (ARCH, ok, fail, len(body), len(units))
    )


if __name__ == "__main__":
    main()

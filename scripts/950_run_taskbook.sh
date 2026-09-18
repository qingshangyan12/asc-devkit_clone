#!/usr/bin/env bash
# Ascend950 task-book / MX gate for CUBE pointerization (dav-3510)
# Modes:
#   CUBE_MATRIX_MODE=mx_only   → only mmad_mx + load_data_2dmx
#   CUBE_MATRIX_MODE=taskbook  → main + shapes + dual + MX (strict)
set -euo pipefail

WS="${CUBE_WS:-/home/Temp_room/wj_cub_cube_ptr_950}"
CANN_ENV="${CANN_ENV:-/usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh}"
PTA="${GITCODE_PTA:?set GITCODE_PTA}"
REPO_PATH="${CUBE_REPO_PATH:-baibai_Uu/asc-devkit}"
BRANCH="${CUBE_BRANCH:-feature/cube-basic-api-pointerization}"
ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"
MODE="${CUBE_MATRIX_MODE:-taskbook}"
DEVICE_ID="${ASCEND_RT_VISIBLE_DEVICES:-0}"
DEMO_TIMEOUT="${CUBE_DEMO_TIMEOUT:-180}"

HELPER="${HELPER:-$WS/helper_repo}"
SRC="${SRC:-$WS/src/asc-devkit}"
STAMP=$(date +%Y%m%d_%H%M%S)
LOGDIR="$WS/logs"
LOG="$LOGDIR/cube_taskbook_950_${STAMP}.log"
DEST="$HELPER/artifacts/950_taskbook_${STAMP}"
AUTH="https://oauth2:${PTA}@gitcode.com/${REPO_PATH}.git"
SAFE="https://gitcode.com/${REPO_PATH}.git"

export GIT_TERMINAL_PROMPT=0 GIT_ASKPASS=true SSH_ASKPASS=true GCM_INTERACTIVE=never
mkdir -p "$LOGDIR" "$WS/runs_950" "$WS/tmp"
exec > >(tee -a "$LOG") 2>&1
echo "=== CUBE 950 TASKBOOK START $(date -Iseconds) MODE=$MODE ==="

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-}" PYTHONPATH="${PYTHONPATH:-}" CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH:-}"
set +u
# shellcheck disable=SC1091
if [ -f "$CANN_ENV" ]; then source "$CANN_ENV"
elif [ -f /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh ]; then source /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh
elif [ -f /usr/local/Ascend/ascend-toolkit/set_env.sh ]; then source /usr/local/Ascend/ascend-toolkit/set_env.sh
else echo "FATAL: no CANN set_env"; exit 2; fi
set +u
export PATH="/usr/local/Ascend/cann-9.1.0-beta.3/tools/ccec_compiler/bin:/usr/local/Ascend/cann-9.1.0-beta.3/bin:${ASCEND_HOME_PATH:+$ASCEND_HOME_PATH/bin:}${PATH:-}"
ASC_FLAGS=""
if command -v g++ >/dev/null 2>&1; then
  export CXX="${CXX:-$(command -v g++)}"; export CC="${CC:-$(command -v gcc)}"
else echo "FATAL: no g++"; exit 2; fi
export ASCEND_RT_VISIBLE_DEVICES="$DEVICE_ID"
export HOME="$WS" TMPDIR="$WS/tmp" TEMP="$WS/tmp" TMP="$WS/tmp"
PY=/usr/bin/python3
echo "ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-} CXX=$CXX DEVICE=$DEVICE_ID"

# ensure helper synced
if [ ! -d "$HELPER/.git" ]; then
  git -c credential.helper= clone --branch "$BRANCH" --single-branch "$AUTH" "$HELPER"
fi
cd "$HELPER"
git remote set-url origin "$AUTH"
git -c credential.helper= fetch origin "$BRANCH" || true
git checkout "$BRANCH" 2>/dev/null || true
git -c credential.helper= reset --hard "origin/$BRANCH" || true
git remote set-url origin "$SAFE"
HEAD=$(git rev-parse --short HEAD)
ln -sfn "$HELPER" "$SRC" 2>/dev/null || true
echo "TEST_HEAD=$HEAD"

# overlay ptr headers into ASCEND_HOME if present
# CANN layout may be $ASCEND_HOME_PATH/{asc,tikcpp} or .../x86_64-linux/{asc,tikcpp}
ASC_ROOT="$ASCEND_HOME_PATH"
[ -d "$ASCEND_HOME_PATH/x86_64-linux/asc" ] && ASC_ROOT="$ASCEND_HOME_PATH/x86_64-linux"
ACT_INC="${ASC_ROOT}/asc/include/basic_api"
ACT_IMPL="${ASC_ROOT}/asc/impl/basic_api"
TIK_IMPL="${ASC_ROOT}/tikcpp/tikcfw/impl/basic_api"
TIK_DAV="${ASC_ROOT}/tikcpp/tikcfw/impl/dav_3510"
if [ -d "$ACT_INC" ] && [ -d "$ACT_IMPL" ]; then
  cp -a "$HELPER/include/basic_api/kernel_operator_ptr_utils.h" "$ACT_INC/" 2>/dev/null || true
  for f in kernel_operator_mm_ptr_impl.h kernel_operator_fixpipe_ptr_impl.h \
           kernel_operator_dump_tensor_ptr_impl.h kernel_operator_common_ptr_impl.h \
           kernel_operator_block_sync_ptr_impl.h kernel_operator_determine_compute_sync_ptr_impl.h; do
    [ -f "$HELPER/impl/basic_api/$f" ] && cp -a "$HELPER/impl/basic_api/$f" "$ACT_IMPL/" || true
  done
  # beta.3: LoadData2DMx assert misses plain fp8; Tensor MmadMx needs MmadMxImpl;
  # Tensor LoadData+Mx needs intf_impl + load2d_impl bodies (else undefined-inline dual link fail).
  # Overlay base/load2d/dav + intf_impl (scrub NEAREST_AWAY below) — not a raw unscrubbed intf drop.
  for f in kernel_operator_mm_base_impl.h kernel_operator_mm_load2d_impl.h \
           kernel_operator_mm_intf_impl.h; do
    if [ -f "$HELPER/impl/basic_api/$f" ]; then
      cp -a "$HELPER/impl/basic_api/$f" "$ACT_IMPL/"
      echo "OVERLAY_ASC $f"
      # tikcfw dual path: interface/../../impl/basic_api — create dir if absent
      mkdir -p "$TIK_IMPL" 2>/dev/null || true
      if [ -d "$TIK_IMPL" ]; then
        cp -a "$HELPER/impl/basic_api/$f" "$TIK_IMPL/"
        echo "OVERLAY_TIK $f -> $TIK_IMPL"
      else
        echo "OVERLAY_TIK_SKIP $f missing=$TIK_IMPL"
      fi
    fi
  done
  if [ -f "$HELPER/include/basic_api/kernel_operator_mm_intf.h" ]; then
    TIK_INTF_DIR="${ASC_ROOT}/tikcpp/tikcfw/interface"
    mkdir -p "$TIK_INTF_DIR" 2>/dev/null || true
    if [ -d "$TIK_INTF_DIR" ]; then
      cp -a "$HELPER/include/basic_api/kernel_operator_mm_intf.h" "$TIK_INTF_DIR/"
      echo "OVERLAY_INTF $TIK_INTF_DIR/kernel_operator_mm_intf.h"
    else
      echo "OVERLAY_INTF_SKIP missing=$TIK_INTF_DIR"
    fi
  fi
  if [ -f "$HELPER/impl/basic_api/dav_3510/kernel_operator_mm_impl.h" ]; then
    mkdir -p "$ACT_IMPL/dav_3510" 2>/dev/null || true
    cp -a "$HELPER/impl/basic_api/dav_3510/kernel_operator_mm_impl.h" "$ACT_IMPL/dav_3510/" 2>/dev/null || true
    echo "OVERLAY_DAV_ASC dav_3510/kernel_operator_mm_impl.h"
    if [ -d "$TIK_DAV" ]; then
      cp -a "$HELPER/impl/basic_api/dav_3510/kernel_operator_mm_impl.h" "$TIK_DAV/"
      echo "OVERLAY_DAV_TIK tikcfw/impl/dav_3510/kernel_operator_mm_impl.h"
    else
      echo "OVERLAY_DAV_TIK_SKIP missing=$TIK_DAV"
    fi
  fi
  for pair in \
    "kernel_operator_mm_intf_impl.h:kernel_operator_mm_ptr_impl.h" \
    "kernel_operator_fixpipe_intf_impl.h:kernel_operator_fixpipe_ptr_impl.h"; do
    host=${pair%%:*}; inc=${pair##*:}; tgt="$ACT_IMPL/$host"
    [ -f "$tgt" ] || continue
    grep -q "$inc" "$tgt" 2>/dev/null && continue
    python3 - "$tgt" "$inc" <<'PY'
from pathlib import Path
import sys
p=Path(sys.argv[1]); inc=sys.argv[2]
t=p.read_text(encoding='utf-8', errors='replace')
line='#include "%s"\n'%inc
m='} // namespace AscendC'
i=t.rfind(m)
t=(t[:i+len(m)]+'\n'+line+t[i+len(m):]) if i!=-1 else t+line
p.write_text(t, encoding='utf-8')
print('patched', p)
PY
  done
  # Scrub accidental prior overlay of NEW intf onto beta.3 CANN (NEAREST_AWAY / CubePtrMxDetail).
  python3 - "$ASCEND_HOME_PATH" <<'PY'
from pathlib import Path
import re, sys
root = Path(sys.argv[1])
changed = 0
for p in root.rglob("kernel_operator_mm_intf_impl.h"):
    t = p.read_text(encoding="utf-8", errors="replace")
    orig = t
    t = t.replace(
        "SetHF32TransModeImpl(mode == AscendC::HF32TransMode::NEAREST_AWAY);",
        "SetHF32TransModeImpl(static_cast<uint32_t>(mode) == 0u);",
    )
    # drop pointer-MmadMx block inserted in feature intf (keep Tensor MmadMx only)
    t2 = re.sub(
        r"\n// Pointer overloads: bind then reuse MmadMxImpl.*?^#endif\n",
        "\n#endif\n",
        t,
        count=1,
        flags=re.S | re.M,
    )
    if t2 != t:
        t = t2
    # if CubePtrMxDetail still present without the comment anchor, strip namespace+following 4 MmadMx ptr templates roughly
    if "namespace CubePtrMxDetail" in t:
        t = re.sub(
            r"\nnamespace CubePtrMxDetail \{.*?\n\} // namespace CubePtrMxDetail\n"
            r"(?:\ntemplate <typename T, typename U, typename S>.*?\{.*?\n\}\n)+",
            "\n",
            t,
            count=1,
            flags=re.S,
        )
    if t != orig:
        p.write_text(t, encoding="utf-8")
        changed += 1
        print("scrubbed_intf", p)
print("SCRUB_INTF_FILES", changed)
PY
  echo "OVERLAY_OK"
fi

# MX golden needs real fp4/fp8 dtypes (host may lack pip — bootstrap first)
echo "=== ensure MX python deps ==="
ensure_pip() {
  if $PY -m pip --version >/dev/null 2>&1; then echo "PIP_OK"; return 0; fi
  $PY -m ensurepip --upgrade >/dev/null 2>&1 || true
  if command -v apt-get >/dev/null 2>&1; then
    apt-get update -qq >/dev/null 2>&1 || true
    apt-get install -y -qq python3-pip >/dev/null 2>&1 || true
  fi
  if $PY -m pip --version >/dev/null 2>&1; then echo "PIP_OK after bootstrap"; return 0; fi
  local gp="$WS/tmp/get-pip.py"
  mkdir -p "$WS/tmp"
  if command -v curl >/dev/null 2>&1; then
    curl -fsSL https://bootstrap.pypa.io/get-pip.py -o "$gp" || true
  elif command -v wget >/dev/null 2>&1; then
    wget -q -O "$gp" https://bootstrap.pypa.io/get-pip.py || true
  fi
  [ -f "$gp" ] && $PY "$gp" || true
  $PY -m pip --version >/dev/null 2>&1
}
ensure_pip || { echo "FATAL: cannot bootstrap pip"; exit 2; }
$PY -m pip install --user -q --force-reinstall 'numpy==1.26.4' 'ml_dtypes==0.2.0' 'en_dtypes==0.0.4' 2>&1 | tail -n 40 || true
$PY - <<'PY'
import sys
ok=True
try:
    import ml_dtypes
    print("ml_dtypes", getattr(ml_dtypes, "__version__", "?"), "fp8", ml_dtypes.float8_e4m3fn)
except Exception as e:
    print("ml_dtypes FAIL", e); ok=False
try:
    import en_dtypes
    print("en_dtypes", getattr(en_dtypes, "__version__", "?"), "fp4", en_dtypes.float4_e2m1)
except Exception as e:
    print("en_dtypes FAIL", e); ok=False
sys.exit(0 if ok else 2)
PY
MXDEP_RC=$?
if [ "$MXDEP_RC" -ne 0 ]; then
  echo "FATAL: MX deps missing after pip; cannot produce correct golden"
  exit 2
fi
echo "MX_DEPS_OK"

EX="$HELPER/examples/01_simd_cpp_api/03_basic_api/03_matrix_compute"
RUNROOT="$WS/runs_950/taskbook_${STAMP}"
mkdir -p "$RUNROOT"
SUMMARY="$RUNROOT/summary.tsv"
echo -e "case\tscenario\tarch\tbuild\trun\tverify\tnote" > "$SUMMARY"

run_case() {
  local name="$1" scenario="$2"
  local M="${3:-}" N="${4:-}" K="${5:-}"
  local tag="s${scenario}"
  [ -n "$M" ] && [ -n "$N" ] && [ -n "$K" ] && tag="s${scenario}_m${M}n${N}k${K}"
  local srcdir="$EX/$name"
  local work="$RUNROOT/${name}_${tag}"
  local blog="$RUNROOT/${name}_${tag}_build.log"
  local rlog="$RUNROOT/${name}_${tag}_run.log"
  local vlog="$RUNROOT/${name}_${tag}_verify.log"
  local glog="$RUNROOT/${name}_${tag}_gendata.log"
  echo "==== RUN $name $tag ===="
  if [ ! -d "$srcdir" ]; then
    echo -e "${name}\t${scenario}\t${ARCH}\tMISSING\t-\t-\t${tag}" >> "$SUMMARY"; return
  fi
  rm -rf "$work"; mkdir -p "$work/build" "$work/input" "$work/output"
  cp -a "$srcdir"/. "$work/"
  find "$work" -name '*.asc' -print0 | xargs -0 sed -i \
    -e 's/ceil_div(/ceil_division(/g' 2>/dev/null || true
  cd "$work"
  set +e
  if [ -n "$M" ]; then
    $PY scripts/gen_data.py -scenarioNum "$scenario" -M "$M" -N "$N" -K "$K" >"$glog" 2>&1 || \
      $PY scripts/gen_data.py -scenarioNum "$scenario" >"$glog" 2>&1
  else
    $PY scripts/gen_data.py -scenarioNum "$scenario" >"$glog" 2>&1 || $PY scripts/gen_data.py >>"$glog" 2>&1
  fi
  set -e
  cd "$work/build"
  set +e
  local cmake_extra=()
  [ -n "$M" ] && cmake_extra=(-DCASE_M="$M" -DCASE_N="$N" -DCASE_K="$K")
  cmake -DSCENARIO_NUM="$scenario" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
    -DCMAKE_CXX_COMPILER="$CXX" -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_ASC_FLAGS="$ASC_FLAGS" "${cmake_extra[@]}" .. >"$blog" 2>&1
  cmake --build . -j8 >>"$blog" 2>&1
  set -e
  if [ ! -x ./demo ]; then
    err=$(grep -m1 'error:' "$blog" | tr '\t' ' ' | cut -c1-120 || true)
    echo -e "${name}\t${scenario}\t${ARCH}\tBUILD_FAIL\t-\t-\t${tag}|${err}" >> "$SUMMARY"
    echo "DONE $name $tag build=BUILD_FAIL"
    echo "---- build FAIL $name $tag (tail) ----"
    tail -n 40 "$blog" 2>/dev/null || true
    echo "---- gendata tail ----"
    tail -n 20 "$glog" 2>/dev/null || true
    return
  fi
  cd "$work"; rm -f ./output/output.bin
  set +e
  timeout --signal=KILL "$DEMO_TIMEOUT" ./build/demo >"$rlog" 2>&1; rrc=$?
  set -e
  if [ "$rrc" -eq 124 ] || [ "$rrc" -eq 137 ]; then
    echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tTIMEOUT\t-\t${tag}" >> "$SUMMARY"
    echo "DONE $name $tag run=TIMEOUT"
    return
  fi
  if [ "$rrc" -ne 0 ] || [ ! -f ./output/output.bin ]; then
    echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tRUN_FAIL\t-\t${tag}|exit=$rrc" >> "$SUMMARY"
    echo "DONE $name $tag run=RUN_FAIL exit=$rrc"
    echo "---- run FAIL $name $tag (tail) ----"
    tail -n 40 "$rlog" 2>/dev/null || true
    return
  fi
  vstr=SKIP
  if [ -f scripts/verify_result.py ]; then
    set +e
    local mnk_args=()
    [ -n "$M" ] && mnk_args=(-M "$M" -N "$N" -K "$K")
    if grep -qE 'add_argument\(.*scenarioNum|"\-scenarioNum"|'\''\-scenarioNum'\''' scripts/verify_result.py 2>/dev/null; then
      $PY scripts/verify_result.py -scenarioNum "$scenario" "${mnk_args[@]}" ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    elif grep -q 'sys.argv\[3\]' scripts/verify_result.py 2>/dev/null; then
      $PY scripts/verify_result.py ./output/output.bin ./output/golden.bin "$scenario" >"$vlog" 2>&1
    else
      $PY scripts/verify_result.py ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    fi
    vrc=$?
    set -e
    if [ "$vrc" -eq 0 ] || grep -qi 'test pass' "$vlog"; then
      vstr=PASS
    else
      vstr=FAIL
      echo "---- verify FAIL $name $tag (tail) ----"
      tail -n 40 "$vlog" 2>/dev/null || true
      echo "---- gendata tail ----"
      tail -n 20 "$glog" 2>/dev/null || true
    fi
  fi
  echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tPASS\t${vstr}\t${tag}" >> "$SUMMARY"
  echo "DONE $name $tag verify=$vstr"
}

MX_CASES=(
  "mmad_mx:1" "mmad_mx:2" "mmad_mx:3"
  "load_data_2dmx_l12l0:1" "load_data_2dmx_l12l0:2" "load_data_2dmx_l12l0:3"
)
TASKBOOK_CASES=(
  "mmad:1" "mmad:2" "mmad:3"
  "fixpipe_l0c2gm:1" "fixpipe_l0c2gm:2"
  "load_data_2dv2_l12l0:1" "load_data_2dv2_l12l0:3"
  "batch_matmul:1" "mmad_gemv:1" "mmad_unitflag:1"
  "mmad:1:1:1:16" "mmad:1:32:32:32" "mmad:1:1024:16:16" "mmad:1:16:16:2048"
  "mmad_mx:1" "mmad_mx:2"
  "load_data_2dmx_l12l0:1" "load_data_2dmx_l12l0:2"
)

if [ "$MODE" = "mx_only" ]; then
  CASES=("${MX_CASES[@]}")
else
  CASES=("${TASKBOOK_CASES[@]}")
fi

echo "CASE_COUNT=${#CASES[@]} MODE=$MODE"
for item in "${CASES[@]}"; do
  IFS=':' read -r name sc M N K <<< "$item"
  if [ -n "${M:-}" ] && [ -n "${N:-}" ] && [ -n "${K:-}" ]; then
    run_case "$name" "$sc" "$M" "$N" "$K"
  else
    run_case "$name" "$sc"
  fi
done

# dual-path: Tensor vs Pointer (taskbook + mx_only)
to_tensor_asc() {
  python3 - "$1" <<'PY'
from pathlib import Path
import re, sys
p = Path(sys.argv[1])
t = p.read_text(encoding="utf-8", errors="replace")
# Strip (__xx__ Type*)tensor.GetPhyAddr() -> tensor  (incl. template Fixpipe/LoadData/MmadMx)
cast_get = re.compile(
    r'\(__\w+__\s+[^)]+?\)\s*((?:[A-Za-z_][\w.]*(?:\[[^\]]+\])?)+)\.GetPhyAddr\(\)'
)
t2 = cast_get.sub(r'\1', t)
p.write_text(t2, encoding="utf-8")
PY
}

dump_bin_stats() {
  local bin="$1" label="$2"
  python3 - "$bin" "$label" <<'PY' || true
import sys, numpy as np
from pathlib import Path
p, label = Path(sys.argv[1]), sys.argv[2]
if not p.is_file():
    print(f"STATS {label}: MISSING")
    raise SystemExit(0)
a = np.fromfile(p, dtype=np.float32)
print(
    f"STATS {label}: n={a.size} min={float(a.min()) if a.size else 'nan':.6g} "
    f"max={float(a.max()) if a.size else 'nan':.6g} mean={float(a.mean()) if a.size else 'nan':.6g} "
    f"nan={int(np.isnan(a).sum())} zero={(a==0).sum() if a.size else 0}"
)
PY
}

if [ "$MODE" = "taskbook" ] || [ "$MODE" = "mx_only" ]; then
  if [ "$MODE" = "mx_only" ]; then
    DUALS=("mmad_mx:1" "load_data_2dmx_l12l0:1")
  else
    DUALS=("mmad:1" "fixpipe_l0c2gm:1" "mmad_mx:1")
  fi
  for duo in "${DUALS[@]}"; do
    IFS=':' read -r name sc <<< "$duo"
    tag="s${sc}"
    ptr_work="$RUNROOT/${name}_${tag}"
    work="$RUNROOT/dual_tensor_${name}_${tag}"
    echo "==== DUAL $name $tag ===="
    if [ ! -f "$ptr_work/output/output.bin" ]; then
      echo -e "dual/${name}\t${sc}\t${ARCH}\tSKIP\t-\t-\tno_ptr_out" >> "$SUMMARY"; continue
    fi
    dump_bin_stats "$ptr_work/output/output.bin" "ptr_${name}_${tag}"
    dump_bin_stats "$ptr_work/output/golden.bin" "golden_${name}_${tag}"
    rm -rf "$work"; mkdir -p "$work/build"
    cp -a "$ptr_work"/. "$work/"; rm -rf "$work/build"; mkdir -p "$work/build" "$work/output"
    # reuse same inputs/golden; only rebuild Tensor-call kernel
    find "$work" -name '*.asc' -print0 | while IFS= read -r -d '' f; do to_tensor_asc "$f"; done
    cd "$work/build"
    set +e
    cmake -DSCENARIO_NUM="$sc" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
      -DCMAKE_CXX_COMPILER="$CXX" -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_ASC_FLAGS="$ASC_FLAGS" .. >"$RUNROOT/dual_${name}_${tag}_build.log" 2>&1
    cmake --build . -j8 >>"$RUNROOT/dual_${name}_${tag}_build.log" 2>&1
    set -e
    if [ ! -x ./demo ]; then
      echo -e "dual/${name}\t${sc}\t${ARCH}\tBUILD_FAIL\t-\t-\t" >> "$SUMMARY"
      echo "---- dual build FAIL $name $tag (tail) ----"
      tail -n 40 "$RUNROOT/dual_${name}_${tag}_build.log" 2>/dev/null || true
      continue
    fi
    cd "$work"; rm -f ./output/output.bin
    set +e; timeout --signal=KILL "$DEMO_TIMEOUT" ./build/demo >"$RUNROOT/dual_${name}_${tag}_run.log" 2>&1; rrc=$?; set -e
    if [ "$rrc" -ne 0 ] || [ ! -f ./output/output.bin ]; then
      echo -e "dual/${name}\t${sc}\t${ARCH}\tPASS\tRUN_FAIL\t-\texit=$rrc" >> "$SUMMARY"
      echo "---- dual run FAIL $name $tag (tail) ----"
      tail -n 40 "$RUNROOT/dual_${name}_${tag}_run.log" 2>/dev/null || true
      continue
    fi
    dump_bin_stats "$work/output/output.bin" "tensor_${name}_${tag}"
    # compare ptr vs tensor; also tensor vs golden
    if cmp -s "$ptr_work/output/output.bin" "$work/output/output.bin"; then
      ptr_eq=PASS
    else
      ptr_eq=FAIL
    fi
    set +e
    $PY "$work/scripts/verify_result.py" "$work/output/output.bin" "$work/output/golden.bin" \
      >"$RUNROOT/dual_${name}_${tag}_verify.log" 2>&1
    tv=$?
    set -e
    if [ "$tv" -eq 0 ] || grep -qi 'test pass' "$RUNROOT/dual_${name}_${tag}_verify.log"; then
      ten_v=PASS
    else
      ten_v=FAIL
      echo "---- dual tensor verify FAIL $name $tag ----"
      tail -n 20 "$RUNROOT/dual_${name}_${tag}_verify.log" 2>/dev/null || true
    fi
    echo -e "dual/${name}\t${sc}\t${ARCH}\tPASS\tPASS\t${ptr_eq}\tptr_eq_tensor=${ptr_eq};tensor_vs_golden=${ten_v}" >> "$SUMMARY"
    echo "DONE dual/$name $tag ptr_eq_tensor=$ptr_eq tensor_vs_golden=$ten_v"
  done
fi

# documented skips for non-pointerable APIs
{
  echo -e "PopStackBuffer\t0\t${ARCH}\tSKIP\t-\t-\tfriend_stack_meta"
  echo -e "SetAddrWithOffset\t0\t${ARCH}\tSKIP\t-\t-\tdeprecated_member"
} >> "$SUMMARY"

# dump summary into main log for outbox visibility
echo "=== SUMMARY.tsv ==="
cat "$SUMMARY" || true

python3 - <<PY
from pathlib import Path
p=Path("$SUMMARY")
rows=[ln.strip().split("\\t") for ln in p.read_text().splitlines() if ln.strip()]
hdr,body=rows[0],rows[1:]
ok=sum(1 for r in body if len(r)>=6 and r[3]=="PASS" and r[4]=="PASS" and r[5]=="PASS")
fail=sum(1 for r in body if len(r)>=6 and r[3] not in ("SKIP","MISSING") and (r[3]!="PASS" or r[4]!="PASS" or r[5] not in ("PASS","SKIP")))
md=["# Ascend950 CUBE task-book self-test","","- HEAD: $HEAD","- ARCH: $ARCH","- MODE: $MODE","- STAMP: $STAMP","",
    f"- PASS: **{ok}**", f"- FAIL: {fail}","",
    "| "+" | ".join(hdr)+" |","|"+"|".join(["---"]*len(hdr))+"|"]
for r in body:
    while len(r)<len(hdr): r.append("")
    md.append("| "+" | ".join(r)+" |")
Path("$RUNROOT/SELFTEST_950_TASKBOOK.md").write_text("\\n".join(md),encoding="utf-8")
print(f"PASS={ok} FAIL={fail} rows={len(body)}")
raise SystemExit(0 if fail==0 else 1)
PY
GATE_RC=$?

mkdir -p "$DEST"
cp -a "$LOG" "$SUMMARY" "$RUNROOT/SELFTEST_950_TASKBOOK.md" "$DEST/" 2>/dev/null || true
# ship per-case logs so Agent can read max_diff / build errors from feature branch
find "$RUNROOT" -maxdepth 1 -type f \( -name '*_verify.log' -o -name '*_build.log' -o -name '*_run.log' -o -name '*_gendata.log' -o -name 'dual_*' \) \
  -exec cp -a {} "$DEST/" \; 2>/dev/null || true
mkdir -p "$HELPER/artifacts/950_taskbook"
cp -a "$DEST/." "$HELPER/artifacts/950_taskbook/" 2>/dev/null || true
cp -a "$DEST/SELFTEST_950_TASKBOOK.md" "$HELPER/artifacts/950_taskbook/" 2>/dev/null || true

cd "$HELPER"
git remote set-url origin "$AUTH"
set +e
git -c credential.helper= pull --rebase origin "$BRANCH" || true
git add "$DEST" artifacts/950_taskbook 2>/dev/null || git add "$DEST"
MSG="Add Ascend950 CUBE taskbook ${STAMP} MODE=${MODE} $([ "$GATE_RC" -eq 0 ] && echo GREEN || echo INCOMPLETE)"
git -c user.email="baibai_Uu@users.noreply.gitcode.com" -c user.name="baibai_Uu" commit -m "$MSG" || true
git -c credential.helper= push origin "HEAD:${BRANCH}"
set -e
git remote set-url origin "$SAFE"

echo "=== DONE_CUBE_TASKBOOK_950 GATE_OK=$([ "$GATE_RC" -eq 0 ] && echo 1 || echo 0) ==="
exit "$GATE_RC"

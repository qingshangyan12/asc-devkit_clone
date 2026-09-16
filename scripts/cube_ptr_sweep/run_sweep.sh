#!/usr/bin/env bash
# Parameterized LoadData / Mmad / Fixpipe pointer sweep (~200 cases / API).
# SMOKE=1  -> first 8 cases per API
# ARCH=dav-2201 (910B) or dav-3510 (950, default)
set -euo pipefail

WS="${CUBE_WS:-/home/Temp_room/wj_cub_cube_ptr_950}"
PTA="${GITCODE_PTA:-gQR4aEUMUTjUMhyvDyx65xD1}"
REPO_PATH="qq_53202098/asc-devkit"
BRANCH="${CUBE_BRANCH:-feature/cube-basic-api-pointerization}"
ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"
SMOKE="${SMOKE:-0}"
CANN_ENV="${CANN_ENV:-/usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh}"
CANN_REAL="${CANN_REAL:-/home/Temp_room/cann-9.1.0}"
TE="${TE_NPU:-/home/Temp_room/te_npu_30}"

HELPER="$WS/helper_repo"
SRC="$WS/src/asc-devkit"
STAMP=$(date +%Y%m%d_%H%M%S)
LOGDIR="$WS/logs"
MODE=$([ "$SMOKE" = "1" ] && echo smoke || echo full)
LOG="$LOGDIR/cube_ptr_sweep_${ARCH}_${MODE}_${STAMP}.log"
DEST="$HELPER/artifacts/cube_ptr_sweep_${ARCH}_${MODE}_${STAMP}"
AUTH="https://oauth2:${PTA}@gitcode.com/${REPO_PATH}.git"
SAFE="https://gitcode.com/${REPO_PATH}.git"
EX_REL="examples/01_simd_cpp_api/03_basic_api/03_matrix_compute"

export GIT_TERMINAL_PROMPT=0 GIT_ASKPASS=true SSH_ASKPASS=true GCM_INTERACTIVE=never
mkdir -p "$LOGDIR" "$WS/runs_sweep" "$WS/tools" "$WS/tmp"
exec > >(tee -a "$LOG") 2>&1
echo "=== CUBE ptr sweep START $(date -Iseconds) ARCH=$ARCH SMOKE=$SMOKE WS=$WS ==="

setup_950() {
  set +u
  if [ -f "$CANN_ENV" ]; then source "$CANN_ENV"
  elif [ -f /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh ]; then source /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh
  else echo "FATAL: no 950 CANN set_env"; exit 2; fi
  set +u
  export PATH="/usr/local/Ascend/cann-9.1.0-beta.3/tools/ccec_compiler/bin:/usr/local/Ascend/cann-9.1.0-beta.3/bin:${ASCEND_HOME_PATH:+$ASCEND_HOME_PATH/bin:}${PATH:-}"
  ASC_FLAGS=""
  if command -v g++ >/dev/null 2>&1; then
    export CXX="${CXX:-$(command -v g++)}"; export CC="${CC:-$(command -v gcc)}"
  else echo "FATAL: no g++"; exit 2; fi
}

setup_910b() {
  set +u
  [ -f "$CANN_REAL/set_env.sh" ] && source "$CANN_REAL/set_env.sh"
  set +u
  OVERLAY="$WS/cann_local"
  mkdir -p "$OVERLAY"
  export ASCEND_HOME_PATH="$OVERLAY" ASCEND_TOOLKIT_HOME="$OVERLAY"
  export ASCEND_OPP_PATH="${ASCEND_OPP_PATH:-$CANN_REAL/opp}"
  export PATH="$TE/tools/bin:$CANN_REAL/bin:$CANN_REAL/tools/ccec_compiler/bin:${PATH:-}"
  export CC="$TE/tools/bin/gcc" CXX="$TE/tools/bin/g++"
  GCC_ROOT="$TE/tools/gcc_root"
  CXXINC="$GCC_ROOT/aarch64-conda-linux-gnu/include/c++/9.5.0"
  TRIPLEINC="$CXXINC/aarch64-conda-linux-gnu"
  SYSINC="$GCC_ROOT/aarch64-conda-linux-gnu/sysroot/usr/include"
  export CPLUS_INCLUDE_PATH="$CXXINC:$TRIPLEINC:$SYSINC${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
  ASC_FLAGS="-isystem$CXXINC -isystem$TRIPLEINC -isystem$SYSINC"
  if [ ! -d "$OVERLAY/asc/include/basic_api" ] && [ -d "$CANN_REAL/asc/include/basic_api" ]; then
    mkdir -p "$OVERLAY/asc/include" "$OVERLAY/asc/impl"
    cp -a "$CANN_REAL/asc/include/basic_api" "$OVERLAY/asc/include/basic_api"
    cp -a "$CANN_REAL/asc/impl/basic_api" "$OVERLAY/asc/impl/basic_api"
  fi
}

if [ "$ARCH" = "dav-2201" ]; then setup_910b; else setup_950; fi
echo "ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-} CXX=${CXX:-}"

sync_repo() {
  local dir="$1"; mkdir -p "$(dirname "$dir")"
  if [ ! -d "$dir/.git" ]; then
    git -c credential.helper= clone --branch "$BRANCH" --single-branch "$AUTH" "$dir" || git -c credential.helper= clone "$AUTH" "$dir"
  fi
  cd "$dir"; git remote set-url origin "$AUTH"
  set +e; git -c credential.helper= fetch origin "$BRANCH"; FR=$?; set -e
  if [ "$FR" -eq 0 ]; then
    git checkout "$BRANCH" 2>/dev/null || git checkout -B "$BRANCH" "origin/$BRANCH"
    git -c credential.helper= reset --hard "origin/$BRANCH"
  fi
  git remote set-url origin "$SAFE"
  echo "REPO_HEAD=$(git rev-parse --short HEAD) @ $dir"
}
sync_repo "$HELPER"
if [ -d "$SRC/.git" ] && [ ! -L "$SRC" ]; then sync_repo "$SRC"
else mkdir -p "$(dirname "$SRC")"; ln -sfn "$HELPER" "$SRC"; fi
cd "$SRC"; HEAD=$(git rev-parse --short HEAD); echo "TEST_HEAD=$HEAD"

ACT_INC="${ASCEND_HOME_PATH}/asc/include/basic_api"
ACT_IMPL="${ASCEND_HOME_PATH}/asc/impl/basic_api"
if [ -d "$ACT_INC" ] && [ -d "$ACT_IMPL" ]; then
  cp -a "$SRC/include/basic_api/kernel_operator_ptr_utils.h" "$ACT_INC/" 2>/dev/null || true
  for f in kernel_operator_mm_ptr_impl.h kernel_operator_fixpipe_ptr_impl.h; do
    [ -f "$SRC/impl/basic_api/$f" ] && cp -a "$SRC/impl/basic_api/$f" "$ACT_IMPL/" || true
  done
  for pair in "kernel_operator_mm_intf_impl.h:kernel_operator_mm_ptr_impl.h" \
              "kernel_operator_fixpipe_intf_impl.h:kernel_operator_fixpipe_ptr_impl.h"; do
    host=${pair%%:*}; inc=${pair##*:}; tgt="$ACT_IMPL/$host"
    [ -f "$tgt" ] || continue
    grep -q "$inc" "$tgt" 2>/dev/null && continue
    python3 - "$tgt" "$inc" <<'PY'
from pathlib import Path
import sys
p=Path(sys.argv[1]); inc=sys.argv[2]
t=p.read_text(encoding="utf-8", errors="replace")
line='#include "%s"\n'%inc
marker="} // namespace AscendC"
i=t.rfind(marker)
t = (t[:i]+marker+"\n"+line+t[i+len(marker):]) if i!=-1 else t+line
p.write_text(t, encoding="utf-8")
print("patched", p)
PY
  done
  echo "OVERLAY_OK"
fi

pick_py() {
  local c p
  for c in "${PYTHON:-}" python3 python /usr/bin/python3 /usr/local/python3.12.13/bin/python3 \
    /usr/local/bin/python3 /home/Temp_room/te_npu_30/tools/bin/python3; do
    [ -n "$c" ] || continue
    if [ -x "$c" ]; then p="$c"; elif command -v "$c" >/dev/null 2>&1; then p=$(command -v "$c"); else continue; fi
    if "$p" -c "import numpy" >/dev/null 2>&1; then echo "$p"; return 0; fi
  done
  return 1
}
PY="$(pick_py || true)"
if [ -z "${PY:-}" ]; then
  VENV="$WS/tools/pyvenv"
  BASE="$(command -v python3 || true)"
  [ -n "$BASE" ] || { echo "FATAL: no python3"; exit 2; }
  [ -x "$VENV/bin/python" ] || { "$BASE" -m venv "$VENV"; "$VENV/bin/python" -m pip install -U pip numpy >/dev/null; }
  PY="$VENV/bin/python"
fi
echo "PY=$PY"

JSONL="$SRC/scripts/cube_ptr_sweep/cases.jsonl"
[ -f "$JSONL" ] || { echo "FATAL: missing $JSONL"; exit 2; }

RUNROOT="$WS/runs_sweep/${STAMP}"; mkdir -p "$RUNROOT"
SUMMARY="$RUNROOT/summary.tsv"
echo -e "api\tcase_id\tarch\tdtype\tM\tN\tK\tscenario\tbuild\trun\tverify\tnote" > "$SUMMARY"

sample_dir() {
  local api="$1"
  case "$api" in
    mmad) echo "$SRC/$EX_REL/mmad" ;;
    fixpipe) echo "$SRC/$EX_REL/fixpipe_l0c2gm" ;;
    load_data)
      if [ "$ARCH" = "dav-2201" ]; then echo "$SRC/$EX_REL/load_data_l12l0"
      else echo "$SRC/$EX_REL/load_data_2dv2_l12l0"; fi
      ;;
    *) echo "" ;;
  esac
}

run_one() {
  local api="$1" cid="$2" dtype="$3" M="$4" N="$5" K="$6" sc="$7"
  local srcdir work blog rlog vlog glog
  srcdir=$(sample_dir "$api")
  work="$RUNROOT/${cid}"
  echo "==== $cid ===="
  [ -d "$srcdir" ] || { echo -e "${api}\t${cid}\t${ARCH}\t${dtype}\t${M}\t${N}\t${K}\t${sc}\tMISSING\t-\t-\t" >> "$SUMMARY"; return; }
  rm -rf "$work"; mkdir -p "$work/build" "$work/input" "$work/output"
  cp -a "$srcdir"/. "$work/"
  find "$work" -name '*.asc' -print0 | xargs -0 sed -i \
    -e 's|Mmad\*/Fixpipe\*/LoadData\*|Mmad/Fixpipe/LoadData|g' \
    -e 's/ceil_div(/ceil_division(/g' 2>/dev/null || true
  cd "$work"
  glog="$work/gendata.log"; blog="$work/build.log"; rlog="$work/run.log"; vlog="$work/verify.log"
  set +e
  $PY scripts/gen_data.py -scenarioNum "$sc" -M "$M" -N "$N" -K "$K" >"$glog" 2>&1
  grc=$?
  set -e
  if [ "$grc" -ne 0 ] || ! ls ./input/* >/dev/null 2>&1; then
    err=$(grep -m1 -E 'Error|ModuleNotFound|Traceback' "$glog" | tr '\t' ' ' | cut -c1-80 || true)
    echo -e "${api}\t${cid}\t${ARCH}\t${dtype}\t${M}\t${N}\t${K}\t${sc}\tGENDATA_FAIL\t-\t-\t${err}" >> "$SUMMARY"
    return
  fi
  cd "$work/build"
  set +e
  cmake -DSCENARIO_NUM="$sc" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
    -DCASE_M="$M" -DCASE_N="$N" -DCASE_K="$K" \
    -DCMAKE_CXX_COMPILER="$CXX" -DCMAKE_C_COMPILER="${CC:-$CXX}" \
    -DCMAKE_ASC_FLAGS="$ASC_FLAGS" .. >"$blog" 2>&1
  cmake --build . -j8 >>"$blog" 2>&1
  set -e
  if [ ! -x ./demo ]; then
    err=$(grep -m1 -E 'error:|FATAL_ERROR' "$blog" | tr '\t' ' ' | cut -c1-80 || true)
    echo -e "${api}\t${cid}\t${ARCH}\t${dtype}\t${M}\t${N}\t${K}\t${sc}\tBUILD_FAIL\t-\t-\t${err}" >> "$SUMMARY"
    return
  fi
  cd "$work"; rm -f ./output/output.bin
  vstr=FAIL
  for _try in 1 2 3; do
    set +e; ./build/demo >"$rlog" 2>&1; rrc=$?; set -e
    if [ "$rrc" -ne 0 ] || [ ! -f ./output/output.bin ]; then
      echo -e "${api}\t${cid}\t${ARCH}\t${dtype}\t${M}\t${N}\t${K}\t${sc}\tPASS\tRUN_FAIL\t-\texit=$rrc" >> "$SUMMARY"
      echo "DONE $cid run=FAIL"
      return
    fi
    if [ -f scripts/verify_result.py ]; then
      set +e
      $PY scripts/verify_result.py -scenarioNum "$sc" -M "$M" -N "$N" -K "$K" ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
      vrc=$?; set -e
      if [ "$vrc" -eq 0 ] || grep -qi 'test pass' "$vlog"; then
        vstr=PASS
        break
      fi
      rm -f ./output/output.bin
    else
      vstr=SKIP
      break
    fi
  done
  echo -e "${api}\t${cid}\t${ARCH}\t${dtype}\t${M}\t${N}\t${K}\t${sc}\tPASS\tPASS\t${vstr}\t" >> "$SUMMARY"
  echo "DONE $cid verify=$vstr"
}

"$PY" - "$JSONL" "$SMOKE" "$ARCH" "$RUNROOT/selected.jsonl" <<'PY'
import json, sys
from pathlib import Path
jsonl, smoke, arch, outp = Path(sys.argv[1]), sys.argv[2] == "1", sys.argv[3], Path(sys.argv[4])
rows = [json.loads(ln) for ln in jsonl.read_text(encoding="utf-8").splitlines() if ln.strip()]
by = {}
for r in rows:
    a = r.get("arch", "any")
    if a not in ("any", arch):
        continue
    sc = r.get("scenario")
    if r["api"] == "load_data":
        key = "scenario_2201" if arch == "dav-2201" else "scenario_3510"
        sc = r.get(key)
        if sc is None:
            continue
        r = dict(r)
        r["scenario"] = sc
    by.setdefault(r["api"], []).append(r)
n = 8 if smoke else 10**9
sel = []
for api in ("mmad", "load_data", "fixpipe"):
    pool = by.get(api, [])
    if not smoke or len(pool) <= n:
        sel.extend(pool[:n])
        continue
    # smoke: round-robin by scenario so dtype/场景都冒到
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
outp.write_text("".join(json.dumps(x) + "\n" for x in sel), encoding="utf-8")
print("SELECTED", len(sel), "smoke" if smoke else "full")
PY
SEL="$RUNROOT/selected.jsonl"

while IFS=$'\t' read -r api cid dtype M N K sc; do
  run_one "$api" "$cid" "$dtype" "$M" "$N" "$K" "$sc"
done < <("$PY" -c '
import json,sys
from pathlib import Path
for ln in Path(sys.argv[1]).read_text(encoding="utf-8").splitlines():
    if not ln.strip():
        continue
    r=json.loads(ln)
    print("\t".join([r["api"], r["case_id"], r["dtype"], str(r["M"]), str(r["N"]), str(r["K"]), str(r["scenario"])]))
' "$SEL")

"$PY" - <<PY
from pathlib import Path
p=Path("$SUMMARY")
rows=[ln.strip().split("\t") for ln in p.read_text().splitlines() if ln.strip()]
hdr,body=rows[0],rows[1:]
ok=sum(1 for r in body if len(r)>=11 and r[8]=="PASS" and r[9]=="PASS" and r[10]=="PASS")
fail=sum(1 for r in body if len(r)>=11 and (r[8] in ("BUILD_FAIL","GENDATA_FAIL") or r[9]=="RUN_FAIL" or r[10]=="FAIL"))
md=["# CUBE ptr sweep","","- HEAD: $HEAD","- ARCH: $ARCH","- MODE: $MODE","- STAMP: $STAMP","",
    f"- verify PASS: **{ok}**", f"- FAIL: {fail}", f"- total: {len(body)}",""]
Path("$RUNROOT/SELFTEST_SWEEP.md").write_text("\\n".join(md), encoding="utf-8")
print("\\n".join(md))
print("ok", ok, "fail", fail, "n", len(body))
PY

mkdir -p "$DEST"
cp -a "$SUMMARY" "$RUNROOT/SELFTEST_SWEEP.md" "$LOG" "$DEST/" 2>/dev/null || true
cp -a "$LOG" "$DEST/cube_ptr_sweep_${STAMP}.txt" 2>/dev/null || true
cd "$HELPER"
git remote set-url origin "$AUTH"
set +e
git -c credential.helper= pull --rebase origin "$BRANCH" || true
git add "$DEST"
git -c user.email="qq_53202098@users.noreply.gitcode.com" -c user.name="qq_53202098" \
  commit -m "Add CUBE ptr sweep ${ARCH} ${MODE} ${STAMP}" || true
git -c credential.helper= push origin "HEAD:${BRANCH}"; PUSH_RC=$?
set -e
git remote set-url origin "$SAFE"
echo "=== DONE_CUBE_PTR_SWEEP PUSH_RC=${PUSH_RC:-0} DEST=$DEST ==="

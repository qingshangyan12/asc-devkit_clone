#!/usr/bin/env bash
# CUBE Basic API pointerization — 910B3 / dav-2201 matrix
# Pattern from F:\all_ok\dma_task\scripts\950_run_dma_matrix.sh
# - plaintext oauth2 PTA in AUTH URL
# - GIT_TERMINAL_PROMPT=0 (no interactive git)
# - tee log + git push artifacts back to private repo
#
# Usage on 910B host:
#   bash /home/Temp_room/wj_cub_cube_ptr/scripts/910b_run_cube_matrix.sh
#
set -euo pipefail

######################## 按机器改这里 ########################
WS="${CUBE_WS:-/home/Temp_room/wj_cub_cube_ptr}"
CANN_REAL="${CANN_REAL:-/home/Temp_room/cann-9.1.0}"
TE="${TE_NPU:-/home/Temp_room/te_npu_30}"
# 明文 PTA（gitcode private）
PTA="${GITCODE_PTA:-gQR4aEUMUTjUMhyvDyx65xD1}"
REPO_PATH="qq_53202098/asc-devkit"
BRANCH="${CUBE_BRANCH:-feature/cube-basic-api-pointerization}"
ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-2201}"
DEVICE_ID="${ASCEND_RT_VISIBLE_DEVICES:-7}"
############################################################

HELPER="$WS/helper_repo"
SRC="$WS/src/asc-devkit"
OVERLAY="$WS/cann_local"
STAMP=$(date +%Y%m%d_%H%M%S)
LOGDIR="$WS/logs"
LOG="$LOGDIR/cube_matrix_910b_${STAMP}.log"
DEST="$HELPER/artifacts/910b_cube_matrix_${STAMP}"
AUTH="https://oauth2:${PTA}@gitcode.com/${REPO_PATH}.git"
SAFE="https://gitcode.com/${REPO_PATH}.git"

export GIT_TERMINAL_PROMPT=0
export GIT_ASKPASS=true
export SSH_ASKPASS=true
export GCM_INTERACTIVE=never

mkdir -p "$LOGDIR" "$WS/runs_910b" "$WS/tools" "$WS/tmp" "$WS/scripts"
exec > >(tee -a "$LOG") 2>&1

echo "=== CUBE 910B matrix START $(date -Iseconds) ==="
echo "WS=$WS BRANCH=$BRANCH STAMP=$STAMP ARCH=$ARCH DEVICE=$DEVICE_ID"

set +u
if [ -f "$CANN_REAL/set_env.sh" ]; then
  # shellcheck disable=SC1090
  source "$CANN_REAL/set_env.sh"
else
  echo "FATAL: missing $CANN_REAL/set_env.sh"
  exit 2
fi
set +u

mkdir -p "$OVERLAY"
export ASCEND_HOME_PATH="$OVERLAY"
export ASCEND_TOOLKIT_HOME="$OVERLAY"
export ASCEND_OPP_PATH="${ASCEND_OPP_PATH:-$CANN_REAL/opp}"
export ASCEND_AICPU_PATH="${ASCEND_AICPU_PATH:-$CANN_REAL}"
export PATH="$TE/tools/bin:$CANN_REAL/bin:$CANN_REAL/tools/ccec_compiler/bin:${PATH:-}"
export CC="$TE/tools/bin/gcc"
export CXX="$TE/tools/bin/g++"
GCC_ROOT="$TE/tools/gcc_root"
CXXINC="$GCC_ROOT/aarch64-conda-linux-gnu/include/c++/9.5.0"
TRIPLEINC="$CXXINC/aarch64-conda-linux-gnu"
SYSINC="$GCC_ROOT/aarch64-conda-linux-gnu/sysroot/usr/include"
export CPLUS_INCLUDE_PATH="$CXXINC:$TRIPLEINC:$SYSINC${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
export CPATH="$CXXINC:$TRIPLEINC:$SYSINC${CPATH:+:$CPATH}"
export LD_LIBRARY_PATH="$CANN_REAL/lib64:$CANN_REAL/lib64/plugin/opskernel:${LD_LIBRARY_PATH:-}"
export CMAKE_PREFIX_PATH="$CANN_REAL/aarch64-linux/tikcpp/ascendc_kernel_cmake:$CANN_REAL/lib64/cmake"
export ASCEND_RT_VISIBLE_DEVICES="$DEVICE_ID"
export HOME="$WS"
export TMPDIR="$WS/tmp"; export TEMP="$TMPDIR"; export TMP="$TMPDIR"
ASC_FLAGS="-isystem$CXXINC -isystem$TRIPLEINC -isystem$SYSINC"
PY=/usr/bin/python3
echo "ASCEND_HOME_PATH=$ASCEND_HOME_PATH CXX=$CXX"

sync_repo() {
  local dir="$1"
  mkdir -p "$(dirname "$dir")"
  if [ ! -d "$dir/.git" ]; then
    git -c credential.helper= clone --branch "$BRANCH" --single-branch "$AUTH" "$dir" || \
      git -c credential.helper= clone "$AUTH" "$dir"
  fi
  cd "$dir"
  git remote set-url origin "$AUTH"
  set +e
  git -c credential.helper= fetch origin "$BRANCH"
  FR=$?
  set -e
  if [ "$FR" -eq 0 ]; then
    git checkout "$BRANCH" 2>/dev/null || git checkout -B "$BRANCH" "origin/$BRANCH"
    git -c credential.helper= reset --hard "origin/$BRANCH"
  else
    echo "WARN: fetch failed; using local $(git rev-parse --short HEAD 2>/dev/null || echo none)"
  fi
  git remote set-url origin "$SAFE"
  echo "REPO_HEAD=$(git rev-parse --short HEAD) @ $dir"
}

sync_repo "$HELPER"
# SRC: prefer symlink to HELPER; recreate if broken / non-git
if [ -L "$SRC" ]; then
  :
elif [ -d "$SRC/.git" ]; then
  sync_repo "$SRC"
else
  rm -rf "$SRC"
  mkdir -p "$(dirname "$SRC")"
  ln -sfn "$HELPER" "$SRC" 2>/dev/null || sync_repo "$SRC"
fi
cd "$SRC"
HEAD=$(git rev-parse --short HEAD)
echo "TEST_HEAD=$HEAD"

ACT_INC="$OVERLAY/asc/include/basic_api"
ACT_IMPL="$OVERLAY/asc/impl/basic_api"
if [ ! -d "$ACT_INC" ] && [ -d "$CANN_REAL/asc/include/basic_api" ]; then
  mkdir -p "$OVERLAY/asc/include" "$OVERLAY/asc/impl"
  cp -a "$CANN_REAL/asc/include/basic_api" "$ACT_INC"
  cp -a "$CANN_REAL/asc/impl/basic_api" "$ACT_IMPL"
fi
  if [ -d "$ACT_INC" ] && [ -d "$ACT_IMPL" ]; then
  cp -a "$SRC/include/basic_api/kernel_operator_ptr_utils.h" "$ACT_INC/" 2>/dev/null || true
  # 910B 矩阵先只 overlay mm/fixpipe（与已 PASS 的自测一致）；dump/sync 另走合入路径
  for f in kernel_operator_mm_ptr_impl.h kernel_operator_fixpipe_ptr_impl.h; do
    [ -f "$SRC/impl/basic_api/$f" ] && cp -a "$SRC/impl/basic_api/$f" "$ACT_IMPL/" || true
  done
  for pair in \
    "kernel_operator_mm_intf_impl.h:kernel_operator_mm_ptr_impl.h" \
    "kernel_operator_fixpipe_intf_impl.h:kernel_operator_fixpipe_ptr_impl.h"
  do
    host=${pair%%:*}; inc=${pair##*:}
    tgt="$ACT_IMPL/$host"
    [ -f "$tgt" ] || continue
    if ! grep -q "$inc" "$tgt" 2>/dev/null; then
      python3 - "$tgt" "$inc" <<'PY'
from pathlib import Path
import sys
p = Path(sys.argv[1])
inc = sys.argv[2]
t = p.read_text(encoding="utf-8", errors="replace")
line = '#include "%s"\n' % inc
# MUST insert AFTER closing namespace AscendC (avoid AscendC::AscendC nesting)
marker = "} // namespace AscendC"
i = t.rfind(marker)
if i != -1:
    j = i + len(marker)
    t = t[:j] + "\n" + line + t[j:]
else:
    t = t + line
p.write_text(t, encoding="utf-8")
print("patched", p)
PY
    fi
  done
  # strip accidental nested dump/sync ptr includes from prior runs
  for host in kernel_operator_dump_tensor_intf_impl.h \
              kernel_operator_block_sync_intf_impl.h \
              kernel_operator_determine_compute_sync_intf_impl.h; do
    tgt="$ACT_IMPL/$host"
    [ -f "$tgt" ] || continue
    python3 - "$tgt" <<'PY'
from pathlib import Path
import sys,re
p=Path(sys.argv[1]); t=p.read_text(encoding='utf-8',errors='replace')
nt=re.sub(r'#include "kernel_operator_(dump_tensor|block_sync|determine_compute_sync)_ptr_impl.h"\n','',t)
if nt!=t: p.write_text(nt,encoding='utf-8'); print('stripped',p)
PY
  done
  echo "OVERLAY_OK"
else
  echo "WARN: CANN asc headers not found under overlay"
fi

EX="$SRC/examples/01_simd_cpp_api/03_basic_api/03_matrix_compute"
RUNROOT="$WS/runs_910b/${STAMP}"
mkdir -p "$RUNROOT"
SUMMARY="$RUNROOT/summary.tsv"
echo -e "case\tscenario\tarch\tbuild\trun\tverify\tnote" > "$SUMMARY"

run_case() {
  local name="$1" scenario="$2"
  local srcdir="$EX/$name"
  local work="$RUNROOT/${name}_s${scenario}"
  local blog="$RUNROOT/${name}_s${scenario}_build.log"
  local rlog="$RUNROOT/${name}_s${scenario}_run.log"
  local vlog="$RUNROOT/${name}_s${scenario}_verify.log"
  echo "==== RUN $name s$scenario ===="
  if [ ! -d "$srcdir" ]; then
    echo -e "${name}\t${scenario}\t${ARCH}\tMISSING\t-\t-\t" >> "$SUMMARY"; return
  fi
  rm -rf "$work"
  mkdir -p "$work/build" "$work/input" "$work/output"
  cp -a "$srcdir"/. "$work/"
  find "$work" -name '*.asc' -print0 | xargs -0 sed -i \
    -e 's|Mmad\*/Fixpipe\*/LoadData\*|Mmad/Fixpipe/LoadData|g' \
    -e 's/ceil_divisionision/ceil_division/g' \
    -e 's/ceil_div(/ceil_division(/g' 2>/dev/null || true
  cd "$work"
  set +e
  $PY scripts/gen_data.py -scenarioNum "$scenario" >"$RUNROOT/${name}_s${scenario}_gendata.log" 2>&1 || \
    $PY scripts/gen_data.py >>"$RUNROOT/${name}_s${scenario}_gendata.log" 2>&1
  set -e
  cd "$work/build"
  set +e
  cmake -DSCENARIO_NUM="$scenario" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
    -DCMAKE_CXX_COMPILER="$CXX" -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_ASC_FLAGS="$ASC_FLAGS" .. >"$blog" 2>&1
  cmake --build . -j8 >>"$blog" 2>&1
  set -e
  if [ ! -x ./demo ]; then
    err=$(grep -m1 'error:' "$blog" | tr '\t' ' ' | cut -c1-120 || true)
    echo -e "${name}\t${scenario}\t${ARCH}\tBUILD_FAIL\t-\t-\t${err}" >> "$SUMMARY"
    return
  fi
  cd "$work"
  rm -f ./output/output.bin
  set +e
  ./build/demo >"$rlog" 2>&1
  rrc=$?
  set -e
  if [ "$rrc" -ne 0 ] || [ ! -f ./output/output.bin ]; then
    echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tRUN_FAIL\t-\texit=$rrc" >> "$SUMMARY"
    return
  fi
  vstr=SKIP
  if [ -f scripts/verify_result.py ]; then
    set +e
    if grep -q 'scenarioNum' scripts/verify_result.py; then
      $PY scripts/verify_result.py -scenarioNum "$scenario" ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    else
      $PY scripts/verify_result.py ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    fi
    vrc=$?
    set -e
    if [ "$vrc" -eq 0 ] || grep -qi 'test pass' "$vlog"; then vstr=PASS
    else vstr=FAIL; fi
  fi
  echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tPASS\t${vstr}\t" >> "$SUMMARY"
  echo "DONE $name s$scenario verify=$vstr"
}

for item in "mmad:1" "fixpipe_l0c2gm:1" "load_data_l12l0:1" "batch_matmul:1" "mmad_gemv:1"; do
  name=${item%%:*}; sc=${item##*:}
  run_case "$name" "$sc"
done

python3 - <<PY
from pathlib import Path
p=Path("$SUMMARY")
rows=[ln.strip().split("\\t") for ln in p.read_text().splitlines() if ln.strip()]
hdr,body=rows[0],rows[1:]
ok=sum(1 for r in body if len(r)>=6 and r[3]=="PASS" and r[4]=="PASS" and r[5]=="PASS")
fail=sum(1 for r in body if len(r)>=6 and (r[3]=="BUILD_FAIL" or r[4]=="RUN_FAIL" or r[5]=="FAIL"))
md=["# Ascend910B3 CUBE self-test matrix","","- HEAD: $HEAD","- ARCH: $ARCH","- STAMP: $STAMP","",
    f"- verify PASS: **{ok}**",f"- FAIL: {fail}","",
    "| "+" | ".join(hdr)+" |","|"+"|".join(["---"]*len(hdr))+"|"]
for r in body:
    while len(r)<len(hdr): r.append("")
    md.append("| "+" | ".join(r)+" |")
Path("$RUNROOT/SELFTEST_910B_MATRIX.md").write_text("\\n".join(md),encoding="utf-8")
print("\\n".join(md))
PY

GATE_OK=0
set +e
python3 - <<PY
from pathlib import Path
rows=[ln.strip().split("\\t") for ln in Path(r"""$SUMMARY""").read_text().splitlines()[1:] if ln.strip()]
bad=[r for r in rows if len(r)>=6 and r[3] not in ("SKIP","MISSING") and (r[3]!="PASS" or r[4]!="PASS" or r[5]!="PASS")]
print("bad_rows", len(bad))
raise SystemExit(0 if not bad else 1)
PY
GATE_RC=$?
set -e
[ "$GATE_RC" -eq 0 ] && GATE_OK=1 || true

# ---- upload artifacts (non-interactive) — always runs even if GATE failed ----
mkdir -p "$DEST"
cp -a "$LOG" "$DEST/" 2>/dev/null || true
cp -a "$SUMMARY" "$DEST/" 2>/dev/null || true
cp -a "$RUNROOT/SELFTEST_910B_MATRIX.md" "$DEST/" 2>/dev/null || true
shopt -s nullglob
for f in "$RUNROOT"/*.log; do
  bn=$(basename "$f" .log)
  cp -a "$f" "$DEST/${bn}.txt" 2>/dev/null || true
done
shopt -u nullglob
cp -a "$LOG" "$DEST/cube_matrix_910b_${STAMP}.txt" 2>/dev/null || true
mkdir -p "$HELPER/artifacts/910b_cube_matrix"
cp -a "$DEST/SELFTEST_910B_MATRIX.md" "$HELPER/artifacts/910b_cube_matrix/" 2>/dev/null || true
if [ "$GATE_OK" -ne 1 ]; then
  echo "GATE_FAILED bad cases present" > "$DEST/GATE_FAILED.txt"
fi

cd "$HELPER"
git remote set-url origin "$AUTH"
set +e
git -c credential.helper= pull --rebase origin "$BRANCH" || true
git add "$DEST" artifacts/910b_cube_matrix 2>/dev/null || git add "$DEST"
if [ "$GATE_OK" -eq 1 ]; then
  MSG="Add Ascend910B3 CUBE matrix ${STAMP} GREEN"
else
  MSG="Add Ascend910B3 CUBE matrix ${STAMP} INCOMPLETE"
fi
git -c user.email="qq_53202098@users.noreply.gitcode.com" -c user.name="qq_53202098" \
  commit -m "$MSG" || true
git -c credential.helper= push origin "HEAD:${BRANCH}"
PUSH_RC=$?
set -e
git remote set-url origin "$SAFE"

echo "=== DONE_CUBE_MATRIX_910B GATE_OK=$GATE_OK PUSH_RC=${PUSH_RC:-0} LOG=$LOG DEST=$DEST ==="
exit $([ "$GATE_OK" -eq 1 ] && echo 0 || echo 1)

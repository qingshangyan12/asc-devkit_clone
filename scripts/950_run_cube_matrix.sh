#!/usr/bin/env bash
# CUBE Basic API pointerization — Ascend950 / dav-3510 matrix
# Pattern from F:\all_ok\dma_task\scripts\950_run_dma_matrix.sh
set -euo pipefail

######################## 按机器改这里 ########################
WS="${CUBE_WS:-/home/Temp_room/wj_cub_cube_ptr_950}"
CANN_ENV="${CANN_ENV:-/usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh}"
PTA="${GITCODE_PTA:-gQR4aEUMUTjUMhyvDyx65xD1}"
REPO_PATH="qq_53202098/asc-devkit"
BRANCH="${CUBE_BRANCH:-feature/cube-basic-api-pointerization}"
ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"
############################################################

HELPER="$WS/helper_repo"
SRC="$WS/src/asc-devkit"
STAMP=$(date +%Y%m%d_%H%M%S)
LOGDIR="$WS/logs"
LOG="$LOGDIR/cube_matrix_950_${STAMP}.log"
DEST="$HELPER/artifacts/950_cube_matrix_${STAMP}"
AUTH="https://oauth2:${PTA}@gitcode.com/${REPO_PATH}.git"
SAFE="https://gitcode.com/${REPO_PATH}.git"

export GIT_TERMINAL_PROMPT=0
export GIT_ASKPASS=true
export SSH_ASKPASS=true
export GCM_INTERACTIVE=never

mkdir -p "$LOGDIR" "$WS/runs_950" "$WS/tools" "$WS/tmp" "$WS/scripts"
exec > >(tee -a "$LOG") 2>&1

echo "=== CUBE 950 matrix START $(date -Iseconds) ==="
echo "WS=$WS BRANCH=$BRANCH STAMP=$STAMP ARCH=$ARCH"

set +u
if [ -f "$CANN_ENV" ]; then source "$CANN_ENV"
elif [ -f /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh ]; then source /usr/local/Ascend/cann-9.1.0-beta.3/set_env.sh
elif [ -f /usr/local/Ascend/ascend-toolkit/set_env.sh ]; then source /usr/local/Ascend/ascend-toolkit/set_env.sh
elif [ -f /home/Temp_room/cann-9.1.0/set_env.sh ]; then source /home/Temp_room/cann-9.1.0/set_env.sh
else echo "FATAL: no CANN set_env"; exit 2; fi
set +u
export PATH="/usr/local/Ascend/cann-9.1.0-beta.3/tools/ccec_compiler/bin:/usr/local/Ascend/cann-9.1.0-beta.3/bin:${ASCEND_HOME_PATH:+$ASCEND_HOME_PATH/bin:}${PATH:-}"
ASC_FLAGS=""
if command -v g++ >/dev/null 2>&1; then
  export CXX="${CXX:-$(command -v g++)}"; export CC="${CC:-$(command -v gcc)}"
elif [ -x /home/Temp_room/te_npu_30/tools/bin/g++ ]; then
  GXX=/home/Temp_room/te_npu_30/tools/bin/g++; GCC_ROOT=/home/Temp_room/te_npu_30/tools/gcc_root
  CXXINC=$GCC_ROOT/aarch64-conda-linux-gnu/include/c++/9.5.0
  TRIPLEINC=$CXXINC/aarch64-conda-linux-gnu
  SYSINC=$GCC_ROOT/aarch64-conda-linux-gnu/sysroot/usr/include
  export PATH="$(dirname "$GXX"):$PATH"; export CXX="$GXX"; export CC="$(dirname "$GXX")/gcc"
  ASC_FLAGS="-isystem$CXXINC -isystem$TRIPLEINC -isystem$SYSINC"
else echo "FATAL: no g++"; exit 2; fi
echo "ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-} CXX=$CXX"

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
[ -d "$SRC/.git" ] || ln -sfn "$HELPER" "$SRC" 2>/dev/null || sync_repo "$SRC"
cd "$SRC"; HEAD=$(git rev-parse --short HEAD); echo "TEST_HEAD=$HEAD"

ACT_INC="${ASCEND_HOME_PATH}/asc/include/basic_api"
ACT_IMPL="${ASCEND_HOME_PATH}/asc/impl/basic_api"
if [ -d "$ACT_INC" ] && [ -d "$ACT_IMPL" ]; then
  cp -a "$SRC/include/basic_api/kernel_operator_ptr_utils.h" "$ACT_INC/" 2>/dev/null || true
  for f in kernel_operator_mm_ptr_impl.h kernel_operator_fixpipe_ptr_impl.h \
           kernel_operator_dump_tensor_ptr_impl.h kernel_operator_block_sync_ptr_impl.h \
           kernel_operator_determine_compute_sync_ptr_impl.h; do
    [ -f "$SRC/impl/basic_api/$f" ] && cp -a "$SRC/impl/basic_api/$f" "$ACT_IMPL/" || true
  done
  echo "OVERLAY_OK"
fi

EX="$SRC/examples/01_simd_cpp_api/03_basic_api/03_matrix_compute"
RUNROOT="$WS/runs_950/${STAMP}"; mkdir -p "$RUNROOT"
SUMMARY="$RUNROOT/summary.tsv"
echo -e "case\tscenario\tarch\tbuild\trun\tverify\tnote" > "$SUMMARY"
PY=/usr/bin/python3

run_case() {
  local name="$1" scenario="$2"
  local srcdir="$EX/$name"; local work="$RUNROOT/${name}_s${scenario}"
  local blog="$RUNROOT/${name}_s${scenario}_build.log"
  local rlog="$RUNROOT/${name}_s${scenario}_run.log"
  local vlog="$RUNROOT/${name}_s${scenario}_verify.log"
  echo "==== RUN $name s$scenario ===="
  [ -d "$srcdir" ] || { echo -e "${name}\t${scenario}\t${ARCH}\tMISSING\t-\t-\t" >> "$SUMMARY"; return; }
  rm -rf "$work"; mkdir -p "$work/build"; cp -a "$srcdir"/. "$work/"
  find "$work" -name '*.asc' -print0 | xargs -0 sed -i -e 's/ceil_divisionision/ceil_division/g' 2>/dev/null || true
  cd "$work"
  set +e; $PY scripts/gen_data.py -scenarioNum "$scenario" >"$RUNROOT/${name}_s${scenario}_gendata.log" 2>&1 || $PY scripts/gen_data.py >>"$RUNROOT/${name}_s${scenario}_gendata.log" 2>&1; set -e
  cd "$work/build"
  set +e
  cmake -DSCENARIO_NUM="$scenario" -DCMAKE_ASC_ARCHITECTURES="$ARCH" -DCMAKE_CXX_COMPILER="$CXX" -DCMAKE_ASC_FLAGS="$ASC_FLAGS" .. >"$blog" 2>&1
  make -j8 >>"$blog" 2>&1; set -e
  [ -x ./demo ] || { err=$(grep -m1 'error:' "$blog" | tr '\t' ' ' | cut -c1-120 || true); echo -e "${name}\t${scenario}\t${ARCH}\tBUILD_FAIL\t-\t-\t${err}" >> "$SUMMARY"; return; }
  cd "$work"; rm -f ./output/output.bin
  set +e; ./build/demo >"$rlog" 2>&1; rrc=$?; set -e
  [ "$rrc" -eq 0 ] && [ -f ./output/output.bin ] || { echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tRUN_FAIL\t-\texit=$rrc" >> "$SUMMARY"; return; }
  vstr=SKIP
  if [ -f scripts/verify_result.py ]; then
    set +e
    if grep -q 'scenarioNum' scripts/verify_result.py; then
      $PY scripts/verify_result.py -scenarioNum "$scenario" ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    else
      $PY scripts/verify_result.py ./output/output.bin ./output/golden.bin >"$vlog" 2>&1
    fi
    vrc=$?; set -e
    if [ "$vrc" -eq 0 ] || grep -qi 'test pass' "$vlog"; then vstr=PASS; else vstr=FAIL; fi
  fi
  echo -e "${name}\t${scenario}\t${ARCH}\tPASS\tPASS\t${vstr}\t" >> "$SUMMARY"
  echo "DONE $name s$scenario verify=$vstr"
}

for item in "mmad:1" "fixpipe_l0c2gm:1" "load_data_l12l0:1" "batch_matmul:1" "mmad_gemv:1"; do
  name=${item%%:*}; sc=${item##*:}; run_case "$name" "$sc"
done

python3 - <<PY
from pathlib import Path
p=Path("$SUMMARY")
rows=[ln.strip().split("\\t") for ln in p.read_text().splitlines() if ln.strip()]
hdr,body=rows[0],rows[1:]
ok=sum(1 for r in body if len(r)>=6 and r[3]=="PASS" and r[4]=="PASS" and r[5]=="PASS")
fail=sum(1 for r in body if len(r)>=6 and (r[3]=="BUILD_FAIL" or r[4]=="RUN_FAIL" or r[5]=="FAIL"))
md=["# Ascend950 CUBE self-test matrix","","- HEAD: $HEAD","- ARCH: $ARCH","- STAMP: $STAMP","",
    f"- verify PASS: **{ok}**",f"- FAIL: {fail}","",
    "| "+" | ".join(hdr)+" |","|"+"|".join(["---"]*len(hdr))+"|"]
for r in body:
    while len(r)<len(hdr): r.append("")
    md.append("| "+" | ".join(r)+" |")
Path("$RUNROOT/SELFTEST_950_MATRIX.md").write_text("\\n".join(md),encoding="utf-8")
print("\\n".join(md))
PY

GATE_OK=0
set +e
python3 - <<PY
from pathlib import Path
rows=[ln.strip().split("\\t") for ln in Path(r"""$SUMMARY""").read_text().splitlines()[1:] if ln.strip()]
bad=[r for r in rows if len(r)>=6 and r[3] not in ("SKIP","MISSING") and (r[3]!="PASS" or r[4]!="PASS" or r[5]!="PASS")]
print("bad_rows", len(bad)); raise SystemExit(0 if not bad else 1)
PY
GATE_RC=$?
set -e
[ "$GATE_RC" -eq 0 ] && GATE_OK=1 || true

mkdir -p "$DEST"
cp -a "$LOG" "$SUMMARY" "$RUNROOT/SELFTEST_950_MATRIX.md" "$DEST/" 2>/dev/null || true
shopt -s nullglob
for f in "$RUNROOT"/*.log; do bn=$(basename "$f" .log); cp -a "$f" "$DEST/${bn}.txt" || true; done
shopt -u nullglob
cp -a "$LOG" "$DEST/cube_matrix_950_${STAMP}.txt" 2>/dev/null || true
mkdir -p "$HELPER/artifacts/950_cube_matrix"
cp -a "$DEST/SELFTEST_950_MATRIX.md" "$HELPER/artifacts/950_cube_matrix/" 2>/dev/null || true
[ "$GATE_OK" -eq 1 ] || echo "GATE_FAILED" > "$DEST/GATE_FAILED.txt"

cd "$HELPER"
git remote set-url origin "$AUTH"
set +e
git -c credential.helper= pull --rebase origin "$BRANCH" || true
git add "$DEST" artifacts/950_cube_matrix 2>/dev/null || git add "$DEST"
MSG="Add Ascend950 CUBE matrix ${STAMP} $([ "$GATE_OK" -eq 1 ] && echo GREEN || echo INCOMPLETE)"
git -c user.email="qq_53202098@users.noreply.gitcode.com" -c user.name="qq_53202098" commit -m "$MSG" || true
git -c credential.helper= push origin "HEAD:${BRANCH}"; PUSH_RC=$?
set -e
git remote set-url origin "$SAFE"
echo "=== DONE_CUBE_MATRIX_950 GATE_OK=$GATE_OK PUSH_RC=${PUSH_RC:-0} LOG=$LOG DEST=$DEST ==="
exit $([ "$GATE_OK" -eq 1 ] && echo 0 || echo 1)

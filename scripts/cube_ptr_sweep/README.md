# CUBE pointer dtype×shape sweep

Generates **200 cases per API** (`mmad` / `load_data` / `fixpipe`) in `cases.jsonl`.
Shapes are cube-legal tiles `M,N,K ∈ {16,32,48,64,80}` (not 1024/2048 — those do not fit L0 as a single tile).

```bash
# 950
export CUBE_WS=/home/Temp_room/wj_cub_cube_ptr_950
export CMAKE_ASC_ARCHITECTURES=dav-3510
SMOKE=1 bash scripts/cube_ptr_sweep/run_sweep.sh   # 8 per API
bash scripts/cube_ptr_sweep/run_sweep.sh            # full 200×3

# 910B
export CUBE_WS=/home/Temp_room/wj_cub_cube_ptr
export CMAKE_ASC_ARCHITECTURES=dav-2201
SMOKE=1 bash scripts/cube_ptr_sweep/run_sweep.sh
```

Regenerate tables:

```bash
python3 scripts/cube_ptr_sweep/gen_cases.py
```

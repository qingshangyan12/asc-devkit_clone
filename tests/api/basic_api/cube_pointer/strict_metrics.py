#!/usr/bin/env python3
"""Compute independent mixed-tolerance metrics from final acceptance outputs."""
import argparse, json, math, struct
from pathlib import Path
import numpy as np

def metric(a, g, dtype):
    if np.issubdtype(a.dtype, np.integer):
        ok = a == g; atol = rtol = 0.0; limit = 0.0
    else:
        vals = {'float16':(2**-9,2**-9,0.1), 'float32':(2**-16,2**-10,0.01),
                'bfloat16':(2**-6,2**-6,1.0), 'float8':(2**-4,2**-2,1.0)}
        atol,rtol,limit=vals.get(dtype,(2**-16,2**-10,0.01))
        ok=np.abs(a.astype(np.float64)-g.astype(np.float64)) <= atol+rtol*np.abs(g.astype(np.float64))
        ok &= np.isfinite(a) & np.isfinite(g)
    err=np.abs(a.astype(np.float64)-g.astype(np.float64))
    ratio=float(np.mean(ok)); maximum=float(np.nanmax(err))
    return {'matched_ratio':ratio, 'max_abs_error':maximum, 'atol':atol,'rtol':rtol,'max_abs_error_limit':limit,'required_matched_ratio':0.99,'strict_pass':bool(ratio>=0.99 and maximum<=limit)}

def load_pair(path):
    meta=json.loads((path/'data-metadata.json').read_text()); dtype=meta['golden']['dtype']; shape=tuple(meta['golden']['shape'])
    dts={'float16':np.float16,'float32':np.float32,'int8':np.int8,'int32':np.int32,'uint8':np.uint8}
    if dtype not in dts: raise ValueError(f'unsupported/unknown golden dtype {dtype}')
    dt=dts[dtype]; g=(path/'output/golden.bin').read_bytes(); o=(path/'pointer.bin').read_bytes(); expected=int(np.prod(shape))*np.dtype(dt).itemsize
    if len(g)!=expected or len(o)!=expected: raise ValueError(f'length mismatch dtype={dtype} shape={shape} golden={len(g)} output={len(o)} expected={expected}')
    return np.frombuffer(o,dt),np.frombuffer(g,dt),dtype

def main():
    p=argparse.ArgumentParser();p.add_argument('root',type=Path);a=p.parse_args();r=json.loads((a.root/'results.json').read_text())
    for x in r:
        d=a.root/f"{x['sample']}_{x['scenario']}"
        if not (d/'pointer.bin').exists(): continue
        try:
            av,gv,dt=load_pair(d); x['strict_metrics']=metric(av,gv,dt); x['actual_dtype']=dt
        except Exception as e: x['strict_metrics_error']=str(e)
    (a.root/'strict-results.json').write_text(json.dumps(r,indent=2)); print(json.dumps(r,indent=2))
if __name__=='__main__': main()

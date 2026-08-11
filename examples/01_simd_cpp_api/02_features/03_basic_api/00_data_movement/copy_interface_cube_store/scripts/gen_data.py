#!/usr/bin/env python3
import argparse,os,numpy as np
D=32;p=argparse.ArgumentParser();p.add_argument("--quant-mode",type=int,default=0);p.add_argument("--route",type=int,default=0);a=p.parse_args();os.makedirs("input",exist_ok=True);os.makedirs("output",exist_ok=True)
dt=np.float16 if a.quant_mode==0 else np.int8
if a.quant_mode==0:
    x=np.eye(D,dtype=dt);y=(np.arange(D*D).reshape(D,D)-D*D//2).astype(dt)
else:
    x=((np.arange(D*D).reshape(D,D)%5)-2).astype(dt);y=(((np.arange(D*D).reshape(D,D)*3)%7)-3).astype(dt)
qscale=np.concatenate((np.ones(D//2,dtype=np.float32),np.full(D//2,0.5,dtype=np.float32)))
qbits=(qscale.view(np.uint32)&np.uint32(0xffffe000)).astype(np.uint64)|(np.uint64(1)<<np.uint64(46))
initial=np.full((D,D),-17, dtype=np.float16 if a.route==2 or a.quant_mode!=0 else np.float32)
x.tofile("input/a.bin");y.T.tofile("input/b.bin");qbits.tofile("input/quant.bin");initial.tofile("input/initial.bin")

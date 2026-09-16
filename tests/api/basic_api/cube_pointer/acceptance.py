"""Replay attachment generators once, then verify both modes on identical bytes."""
import argparse
import csv
import difflib
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def command(args, cwd, log):
    with (cwd / 'commands.jsonl').open('a') as out:
        out.write(json.dumps([str(x) for x in args]) + '\n')
    with (cwd / log).open('w') as out:
        result = subprocess.run(args, cwd=cwd, stdout=out, stderr=subprocess.STDOUT, timeout=180)
    if result.returncode:
        raise RuntimeError(f'{log}: exit {result.returncode}')


def prepare(text, sample, tensor):
    text = text.replace('Mmad*/Fixpipe*/LoadData*', 'Mmad / Fixpipe / LoadData')
    text = text.replace('(__cc__ T*)bias2Local.GetPhyAddr()', '(__biasbuf__ T*)bias2Local.GetPhyAddr()')
    if sample == 'load_data_2dmx_l12l0':
        text = text.replace('(__ca__ T*)a2Local.GetPhyAddr(), (__cb__ T*)b2Local.GetPhyAddr()',
                            '(__ca__ TAL0Type*)a2Local.GetPhyAddr(), (__cb__ TBL0Type*)b2Local.GetPhyAddr()')
    if sample == 'mmad_load3dv2':
        for space in ('ca', 'cb', 'cbuf'):
            text = text.replace(f'(__{space}__ U*)', f'(__{space}__ T*)')
    if tensor:
        text = re.sub(r'\(__[a-z]+__ [\w:]+\*\)(\w+(?:\[[^\]]+\])?)\.GetPhyAddr\(\)', r'\1', text)
    # Time repeated original launches, without moving host copies into the interval.
    pattern = r'([\w]+(?:<[^;\n]+>)?\s*<<<[^;]+?>>>\([^;]+?\);)'
    matches = list(re.finditer(pattern, text))
    if len(matches) != 1:
        raise ValueError(f'expected one host launch, found {len(matches)}')
    launch = matches[0].group(1)
    timed = '''
    for (int warm = 0; warm < 5; ++warm) { LAUNCH }
    if (aclrtSynchronizeStream(stream)) return 91;
    aclrtEvent begin, end;
    if (aclrtCreateEvent(&begin) || aclrtCreateEvent(&end)) return 92;
    for (int repeat = 0; repeat < 20; ++repeat) {
        if (aclrtRecordEvent(begin, stream)) return 93;
        LAUNCH
        if (aclrtRecordEvent(end, stream) || aclrtSynchronizeEvent(end)) return 94;
        float duration;
        if (aclrtEventElapsedTime(&duration, begin, end)) return 95;
        printf("ACCEPT_US %.9g\\n", duration * 1000);
    }
    aclrtDestroyEvent(begin); aclrtDestroyEvent(end);
    '''.replace('LAUNCH', launch)
    return text[:matches[0].start()] + timed + text[matches[0].end():]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('archive', type=Path)
    parser.add_argument('output', type=Path)
    parser.add_argument('--only')
    args = parser.parse_args()
    args.archive = args.archive.resolve(); args.output = args.output.resolve()
    args.output.mkdir(parents=True, exist_ok=True)
    counts = {'batch_matmul': 1, 'fixpipe_l0c2gm': 7, 'fixpipe_l0c2l1': 4,
              'fixpipe_l0c2ub': 4, 'load_data_2dv2_l12l0': 7, 'load_data_2dmx_l12l0': 6,
              'load_data_with_stride': 6, 'mmad': 4, 'mmad_mx': 4, 'mmad_gemv': 1,
              'mmad_unitflag': 1, 'mmad_load3dv2': 5, 'load_data_l12l0': 14, 'mmad_with_sparse': 1}
    plan = [{'sample': n, 'scenario': s} for n, count in counts.items() for s in range(1, count + 1)]
    (args.output / 'plan.json').write_text(json.dumps(plan, indent=2))
    originals = {str(p.relative_to(args.archive)): sha(p) for p in args.archive.rglob('*') if p.is_file()}
    (args.output / 'original-sha256.json').write_text(json.dumps(originals, indent=2))
    (args.output / 'git-head.txt').write_text(subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=REPO, text=True))
    results = []
    for case in plan:
        n, s = case['sample'], case['scenario']
        if args.only and n != args.only:
            continue
        case = dict(case); results.append(case)
        if n in ('load_data_l12l0', 'mmad_with_sparse'):
            case.update(status='UNSUPPORTED', reason='Original CMake restricts dav-2201'); continue
        if n == 'mmad' and s == 4:
            case.update(status='UNSUPPORTED', reason='950 LoadData2DV2 rejects int4; Tensor baseline also rejects'); continue
        work = args.output / f'{n}_{s}'; work.mkdir(exist_ok=True)
        (work / 'input').mkdir(exist_ok=True); (work / 'output').mkdir(exist_ok=True)
        sample = args.archive / n
        gen = sample / 'scripts/gen_data.py'
        argv = ['-scenarioNum', str(s)] if 'add_argument' in gen.read_text() else [f'scenarioNum={s}']
        # Trace generator return locals to preserve real dtype/shape metadata.
        generator = '''import runpy,sys,json,numpy as np
np.random.seed(9)
metadata={}
def trace(frame,event,arg):
 if event=='return' and frame.f_code.co_filename==path:
  for k,v in frame.f_locals.items():
   if isinstance(v,np.ndarray): metadata[k]={'shape':list(v.shape),'dtype':str(v.dtype)}
 return trace
path=sys.argv[1]; sys.argv=sys.argv[1:]; sys.settrace(trace)
runpy.run_path(path,run_name='__main__');sys.settrace(None)
open('data-metadata.json','w').write(json.dumps(metadata,indent=2))
'''
        try:
            command([sys.executable, '-c', generator, str(gen), *argv], work, 'generate.log')
            case['generated'] = True
            hashes = {str(p.relative_to(work)): sha(p) for p in [*(work/'input').glob('*'), work/'output/golden.bin']}
            (work/'data-sha256.json').write_text(json.dumps(hashes,indent=2))
            source = (sample/f'{n}.asc').read_text()
            for mode in ('pointer', 'tensor'):
                prepared = prepare(source, n, mode == 'tensor')
                path = work/f'{mode}.asc'; path.write_text(prepared)
                (work/f'{mode}.patch').write_text(''.join(difflib.unified_diff(source.splitlines(True),prepared.splitlines(True),fromfile='archive',tofile=mode)))
                build = work/f'build-{mode}'
                command(['cmake','-S',str(HERE),'-B',str(build),f'-DSAMPLE_DIR={sample}',f'-DSCENARIO_NUM={s}',f'-DCUBE_PREPARED_SOURCE={path}'],work,f'{mode}-configure.log')
                command(['cmake','--build',str(build),'-j2'],work,f'{mode}-build.log')
                command(['timeout','--kill-after=5s','60s',str(build/'demo')],work,f'{mode}-run.log')
                verify=sample/'scripts/verify_result.py'; vtext=verify.read_text()
                vargs=['output/output.bin','output/golden.bin']
                if 'add_argument("-scenarioNum"' in vtext: vargs=['-scenarioNum',str(s),*vargs]
                elif 'sys.argv[3]' in vtext: vargs.append(str(s))
                command([sys.executable,str(verify),*vargs],work,f'{mode}-verify.log')
                (work/f'{mode}.bin').write_bytes((work/'output/output.bin').read_bytes())
                case[mode+'_passed']=True
                case[mode+'_us']=[float(t) for t in re.findall(r'ACCEPT_US ([\d.e+-]+)',(work/f'{mode}-run.log').read_text())]
                objcopy=Path(os.environ['ASCEND_HOME_PATH'])/'tools/bisheng_compiler/bin/llvm-objcopy'
                command([str(objcopy),'--dump-section',f'.aicore_binary={work/mode}.elf',str(build/'demo')],work,f'{mode}-extract.log')
                command([str(objcopy),'--dump-section',f'.text={work/mode}.text',str(work/f'{mode}.elf')],work,f'{mode}-text.log')
            case['same_output']=(work/'pointer.bin').read_bytes()==(work/'tensor.bin').read_bytes()
            case['same_machine_code']=(work/'pointer.text').read_bytes()==(work/'tensor.text').read_bytes()
            case['data_unchanged']=all(sha(work/p)==v for p,v in hashes.items())
            case['status']='PASS' if case['same_output'] and case['data_unchanged'] else 'FAIL'
        except Exception as error:
            case.update(status='FAIL', reason=str(error))
        print(n,s,case['status'],case.get('reason',''),flush=True)
        (args.output/'results.json').write_text(json.dumps(results,indent=2))
    (args.output/'results.json').write_text(json.dumps(results,indent=2))
    with (args.output/'results.csv').open('w') as out:
        writer=csv.DictWriter(out,fieldnames=['sample','scenario','status','reason','same_output','same_machine_code','data_unchanged'],extrasaction='ignore');writer.writeheader();writer.writerows(results)


if __name__ == '__main__':
    main()

"""Recheck official runs deterministically, retaining initial logs as history."""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--output',type=Path,required=True);ap.add_argument('--verify-only',action='store_true');a=ap.parse_args()
    out=a.output.resolve(); results=[]
    for result in sorted(out.glob('*/*/result.json')):
        dest=result.parent; record=json.loads(result.read_text());project=dest/'project';build=dest/'build'
        history=dest/'initial_run';history.mkdir(exist_ok=True)
        for f in [result,*dest.glob('*.log')]:
            if not (history/f.name).exists():shutil.copy2(f,history/f.name)
        source=next(project.glob('*.asc'));text=source.read_text();clean=re.sub(r'AscendC::printf\([^;]*\);','(void)0;',text)
        commands=[]
        if clean!=text:
            source.write_text(clean)
            record['sample_adaptations'].append('omit optional device diagnostic printf absent from scoped includes')
            commands.append(('build',['cmake','--build',str(build),'-j2']))
        elif not (build/'demo').exists():
            commands.append(('build',['cmake','--build',str(build),'-j2']))
        record['source_sha256']=digest(source)
        from run_official_samples import flag_for
        gen=project/'scripts/gen_data.py';old_generate=['python3',str(gen)]
        if flag_for(gen):old_generate += [flag_for(gen),str(record['scenario'])]
        generate=['python3','-c','import numpy as np,runpy,sys; np.random.seed(20260916); p=sys.argv.pop(1); sys.argv[0]=p; runpy.run_path(p,run_name="__main__")',*old_generate[1:]]
        commands += [('generate',generate),('run',[str(build/'demo')])]
        verify=['python3',str(project/'scripts/verify_result.py'),'./output/output.bin','./output/golden.bin']
        if record['sample']=='interleave_pair':verify=verify[:2]+['./output/output_dst0.bin','./output/output_dst1.bin','./output/golden_dst0.bin','./output/golden_dst1.bin']
        if record['sample']=='gather':verify.append(str(record['scenario']))
        elif (project/'scripts/verify_result.py').exists() and flag_for(project/'scripts/verify_result.py'):
            verify += [flag_for(project/'scripts/verify_result.py'),str(record['scenario'])]
        if (project/'scripts/verify_result.py').exists():commands.append(('verify',verify))
        elif record['sample']=='reduce':
            commands.append(('verify',['python3','-c','from pathlib import Path; t=Path("../run.log").read_text(); assert "CompareResult golden.bin success!" in t and "test pass!" in t and "test failed!" not in t; print("Official embedded CompareResult: PASS")']))
            record['verification_note']='official reduce verifies golden.bin in demo; explicitly check success text because demo returns 0 on mismatch'
        else:raise RuntimeError('No verified oracle for '+str(project))
        prior_steps=[s for s in record['steps'] if s['step']!='verify']
        if a.verify_only:
            assert any(s['step']=='run' and s['returncode']==0 for s in prior_steps), str(dest)
            assert all(s['returncode']==0 for s in prior_steps), str(dest)
            commands=[s for s in commands if s[0]=='verify']
        steps=[]
        for step,cmd in commands:
            with (dest/(step+'.log')).open('w') as f:
                try:rc=subprocess.run(cmd,cwd=build,stdout=f,stderr=subprocess.STDOUT,timeout=180).returncode
                except subprocess.TimeoutExpired:rc=124
            steps.append({'step':step,'command':cmd,'returncode':rc})
            if rc:break
        record['initial_steps']=record['steps'];record['steps']=steps;record['seed']=20260916
        record['pass']=len(steps)==len(commands) and all(s['returncode']==0 for s in steps)
        if a.verify_only:record['steps']=prior_steps+steps
        record['input_sha256']={str(p.relative_to(build)):digest(p) for p in sorted((build/'input').glob('*.bin'))}
        record['output_sha256']={p.name:digest(p) for p in sorted((build/'output').glob('output*.bin'))}
        result.write_text(json.dumps(record,indent=2));results.append(record)
        print(record['sample'],record['scenario'],record['mode'],record['pass'],flush=True)
        (out/'final_summary.json').write_text(json.dumps(results,indent=2))
    groups={}
    for r in results:groups.setdefault((r['sample'],r['scenario']),{})[r['mode']]=r
    checks=[]
    for (sample,scenario),m in groups.items():
        complete=set(m)=={'baseline','tensor','pointer'}
        same_input=complete and m['baseline']['input_sha256']==m['tensor']['input_sha256']==m['pointer']['input_sha256']
        same_output=complete and bool(m['baseline']['output_sha256']) and m['baseline']['output_sha256']==m['tensor']['output_sha256']==m['pointer']['output_sha256']
        checks.append({'sample':sample,'scenario':scenario,'same_inputs':same_input,'bitwise_same_outputs':same_output,'all_verify_pass':complete and all(r['pass'] for r in m.values())})
    (out/'cross_mode_comparison.json').write_text(json.dumps(checks,indent=2))

if __name__=='__main__':main()

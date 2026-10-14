import ctypes
import importlib
import json
import os
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parent

class LowlevelTests(unittest.TestCase):
    def module(self, name):
        self.assertIsNotNone(importlib.util.find_spec(name), name + ' must be implemented')
        return importlib.import_module(name)

    def request(self, **kw):
        m = self.module('static_compile.specialization.specialization_request')
        vals = dict(resource_id='id', kernel_entry='kernel', arguments=[m.KernelArgument(4, b'abcd'), m.KernelArgument(8, None)], basic_options=['-O2'], sk_options=[], output_path='/data/hxj/out.o')
        vals.update(kw)
        return m.OwnedSpecializationRequest(m.SpecializationInput(**vals))

    def test_abi_buffers_and_full_count(self):
        owner = self.request()
        r = owner.request
        self.assertEqual(ctypes.sizeof(r), 328)
        self.assertEqual(r.argsCount, 2)
        self.assertEqual(ctypes.string_at(r.argsAddr[0], 4), b'abcd')
        self.assertIsNone(r.argsAddr[1])
        self.assertEqual(list(r.argsBytes[:2]), [4, 8])
        self.assertEqual(bytes(r.reserved), bytes(256))
        self.assertEqual(r.options[0], b'-O2')

    def test_empty_arrays_are_null_and_offsets_match_header(self):
        m = self.module('static_compile.specialization.specialization_request')
        request = self.request(arguments=[], basic_options=[], sk_options=[]).request
        for name in ('argsAddr', 'argsBytes', 'options', 'skOptions'):
            self.assertFalse(bool(getattr(request, name)), name)
        for name, offset in {'resourceId':0, 'kernelEntry':8, 'argsCount':16,
                             'argsAddr':24, 'argsBytes':32, 'options':40,
                             'optionCount':48, 'skOptions':56,
                             'skOptionCount':64, 'reserved':72}.items():
            self.assertEqual(getattr(m.AclrtcKernelSpecRequest, name).offset, offset)

    def test_public_header_real_library_call(self):
        source = r'''#include <stddef.h>
#include <string.h>
#include "/usr/local/Ascend/cann-9.3.0/include/acl/acl_rt_compile.h"
_Static_assert(sizeof(aclrtcKernelSpecRequest) == 328, "ABI size");
_Static_assert(offsetof(aclrtcKernelSpecRequest, reserved) == 72, "ABI offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, resourceId) == 0, "resourceId offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, kernelEntry) == 8, "kernelEntry offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, argsCount) == 16, "argsCount offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, argsAddr) == 24, "argsAddr offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, argsBytes) == 32, "argsBytes offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, options) == 40, "options offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, optionCount) == 48, "optionCount offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, skOptions) == 56, "skOptions offset");
_Static_assert(offsetof(aclrtcKernelSpecRequest, skOptionCount) == 64, "skOptionCount offset");
int aclrtcKernelSpecialization(const aclrtcKernelSpecRequest *r, const char *out) {
  if (r->argsCount != 2 || r->argsBytes[0] != 4 || r->argsBytes[1] != 8) return 1;
  if (memcmp(r->argsAddr[0], "abcd", 4) || r->argsAddr[1]) return 2;
  if (strcmp(r->kernelEntry, "kernel") || strcmp(r->resourceId, "id")) return 3;
  if (r->optionCount != 1 || strcmp(r->options[0], "-O2") || r->skOptionCount || r->skOptions) return 4;
  for (size_t i=0; i<256; i++) if (r->reserved[i]) return 5;
  return strcmp(out, "/data/hxj/out.o") ? 6 : 0;
}
'''
        with tempfile.TemporaryDirectory() as d:
            p = Path(d); (p/'probe.c').write_text(source)
            env = dict(os.environ, TMPDIR=d)
            subprocess.run(['cc', '-shared', '-fPIC', str(p/'probe.c'), '-o', str(p/'probe.so')], check=True, capture_output=True, env=env)
            self.assertEqual(self.request().invoke(str(p/'probe.so')), 0)

    def test_invalid_request(self):
        m = self.module('static_compile.specialization.specialization_request')
        for kw in [dict(resource_id=3), dict(kernel_entry='a\0b'), dict(basic_options='-O2'), dict(basic_options=['@a']), dict(sk_options=['-ofoo']), dict(basic_options=['--output=x']), dict(basic_options=['a\nb']), dict(basic_options=['x' * (1024*1024+1)]), dict(basic_options=['-O2']*4097), dict(basic_options=['x'*1048576]*4), dict(arguments=[m.KernelArgument(3,b'abcd')]), dict(arguments=[m.KernelArgument(True,None)])]:
            with self.subTest(kw=str(kw)[:100]), self.assertRaises(m.SpecializationError):
                self.request(**kw)

    def test_invoke_explicit_library_errors_and_binding(self):
        m = self.module('static_compile.specialization.specialization_request')
        o = self.request()
        with self.assertRaises(m.SpecializationError):
            o.invoke('libacl_rtc.so')
        with self.assertRaises(m.SpecializationError):
            o.invoke(str(ROOT / 'missing.so'))
        class Fn:
            def __call__(self, request, output):
                self.seen = ctypes.cast(request, ctypes.POINTER(m.AclrtcKernelSpecRequest)).contents.argsCount
                return 5
        fn = Fn()
        class Lib: pass
        lib = Lib(); lib.aclrtcKernelSpecialization = fn
        with patch.object(m.ctypes, 'CDLL', return_value=lib), self.assertRaises(m.SpecializationError) as failure:
            o.invoke(str(ROOT / 'fake.so'))
        message = str(failure.exception)
        for expected in ('returned 5', 'resource=id', 'entry=kernel', 'output=/data/hxj/out.o'):
            self.assertIn(expected, message)
        self.assertNotIn('abcd', message)
        self.assertEqual(fn.seen, 2)
        self.assertEqual(fn.restype, ctypes.c_int)
        self.assertEqual(fn.argtypes, [ctypes.POINTER(m.AclrtcKernelSpecRequest), ctypes.c_char_p])

    def tiling_files(self, p, **kw):
        raw = dict(tiling_args_index=4, tiling_data='abcd', tiling_key=2**64-1, block_dim=1, workspace_bytes='0 18446744073709551615 ', clear_atomic=False, schedule_mode=0, local_memory_size=0, aicpu_block_dim=0, tiling_cond=-1, kernel_name='kernel', extension={'a':[1]})
        raw.update(kw)
        (p/'run.json').write_text(json.dumps(raw))
        return raw

    def test_tiling_valid_and_strict(self):
        m = self.module('static_compile.specialization.static_op_tiling')
        with tempfile.TemporaryDirectory() as d:
            p=Path(d); raw=self.tiling_files(p)
            r=m.get_static_op_tiling(json.loads((p/'run.json').read_text()))
            self.assertEqual(r.tiling_data,b'\xab\xcd'); self.assertEqual(r.raw_run_info,raw)
            self.assertEqual(r.tiling_cond,-1); self.assertEqual(r.kernel_name,'kernel')
            for kw in [dict(block_dim=0),dict(block_dim=True),dict(tiling_key=2**64),dict(clear_atomic=0),dict(tiling_cond=-2**31-1),dict(workspace_bytes=[False]),dict(tiling_data='abc'),dict(tiling_data='gg'),dict(tiling_data=None),dict(tiling_data=123),dict(tiling_data='0xab'),dict(tiling_data='ab cd'),dict(kernel_name='')]:
                self.tiling_files(p,**kw)
                with self.subTest(kw=kw),self.assertRaises(m.SpecializationError):
                    m.get_static_op_tiling(json.loads((p/'run.json').read_text()))
            with self.assertRaises(m.SpecializationError):
                m.get_static_op_tiling(p/'missing')

    def test_tiling_kernel_name_required_and_bound_to_request(self):
        tiling_module = self.module('static_compile.specialization.static_op_tiling')
        request_module = self.module('static_compile.specialization.specialization_request')
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            raw = self.tiling_files(path, kernel_name='actual_kernel')
            result = tiling_module.get_static_op_tiling(json.loads((path/'run.json').read_text()))
            owner = request_module.OwnedSpecializationRequest(request_module.SpecializationInput(
                resource_id='id', kernel_entry=result.kernel_name, arguments=[],
                basic_options=[], sk_options=[], output_path='/data/hxj/test.o'))
            self.assertEqual(owner.request.kernelEntry, b'actual_kernel')
            raw['kernel_entry'] = raw.pop('kernel_name')
            (path/'run.json').write_text(json.dumps(raw))
            with self.assertRaisesRegex(tiling_module.SpecializationError, 'kernel_name'):
                tiling_module.get_static_op_tiling(json.loads((path/'run.json').read_text()))

    def test_tiling_json_is_the_only_byte_source(self):
        m = self.module('static_compile.specialization.static_op_tiling')
        with tempfile.TemporaryDirectory() as d:
            p = Path(d)
            for hex_data, expected in [('01ab00ff', b'\x01\xab\x00\xff'), ('ABCD', b'\xab\xcd'), ('', b'')]:
                self.tiling_files(p, tiling_data=hex_data)
                result = m.get_static_op_tiling(json.loads((p/'run.json').read_text()))
                self.assertEqual(result.tiling_data, expected)
                self.assertIsInstance(result.tiling_data, bytes)
                self.assertFalse((p/'data.bin').exists())
            raw = self.tiling_files(p)
            raw.pop('tiling_data')
            (p/'run.json').write_text(json.dumps(raw))
            with self.assertRaises(m.SpecializationError):
                m.get_static_op_tiling(json.loads((p/'run.json').read_text()))

    def test_publish_no_overwrite_symlink_and_fsync_failure(self):
        m=self.module('static_compile.specialization.artifact_publisher')
        with tempfile.TemporaryDirectory() as d:
            p=Path(d); stage=p/'stage'; stage.mkdir(); (stage/'x').write_bytes(b'data')
            dest=p/'dest'; dest.mkdir(); (dest/'old').write_bytes(b'old')
            with self.assertRaises(m.SpecializationError): m.publish_artifacts(stage,dest)
            self.assertEqual((dest/'old').read_bytes(),b'old'); self.assertTrue(stage.exists())
            (stage/'link').symlink_to(stage/'x')
            with self.assertRaises(m.SpecializationError): m.publish_artifacts(stage,p/'new')
            (stage/'link').unlink()
            with patch.object(m.os,'fsync',side_effect=OSError('write error')),self.assertRaises(m.SpecializationError): m.publish_artifacts(stage,p/'new')
            self.assertTrue(stage.exists()); self.assertFalse((p/'new').exists())
            m.publish_artifacts(stage,p/'new')
            self.assertFalse(stage.exists()); self.assertEqual((p/'new'/'x').read_bytes(),b'data')

    def test_publish_reports_rename_commit_on_parent_fsync_error(self):
        m = self.module('static_compile.specialization.artifact_publisher')
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            stage = root/'stage'
            stage.mkdir()
            (stage/'file').write_bytes(b'data')
            real_fsync = m.os.fsync
            calls = []
            def fsync(fd):
                calls.append(fd)
                if len(calls) == 3:
                    raise OSError('parent sync failure')
                return real_fsync(fd)
            with patch.object(m.os, 'fsync', side_effect=fsync), self.assertRaises(m.SpecializationError) as failure:
                m.publish_artifacts(stage, root/'dest')
            self.assertTrue(failure.exception.published)
            self.assertFalse(stage.exists())
            self.assertEqual((root/'dest/file').read_bytes(), b'data')
            with self.assertRaises(m.SpecializationError) as failure:
                m.publish_artifacts(root/'another', root/'dest')
            self.assertFalse(failure.exception.published)

if __name__=='__main__': unittest.main()

if __name__ == '__main__':
    unittest.main()

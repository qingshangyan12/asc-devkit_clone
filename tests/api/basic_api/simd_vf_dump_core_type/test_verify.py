# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

import unittest
from verify import verify

DATA = '[0.000000, 1.100000, 2.200000, 3.300000, 4.400000, 5.500000, 6.600000, 7.700000]\n'
GOOD = ''.join(f'[AIV Block 0] DumpTensor: desc={desc}, addr=0x0, data_type=float32, position={pos}, dump_size=8\n'+DATA
               for desc, pos in [(100, 'REG'), (101, 'UB')]) + 'CORE_FLAGS block=0 initial=0 before=0 after=1 actual_aiv=1\n'

class Tests(unittest.TestCase):
    def test_good(self):
        verify(GOOD)
    def test_wrong_core_even_with_correct_values(self):
        with self.assertRaisesRegex(ValueError, 'expected AIV'):
            verify(GOOD.replace('AIV', 'AIC'))
    def test_bad_evidence(self):
        for text in ['', GOOD+GOOD, GOOD.replace('after=1', 'after=0'), GOOD.replace('7.700000', '99.0'),
                     GOOD.replace('Block 0', 'Block 2'), GOOD.replace('position=REG', 'position=UB')]:
            with self.subTest(text=text), self.assertRaises(ValueError):
                verify(text)
    def test_multicore_repeat(self):
        records = GOOD.split('CORE_FLAGS')[0]
        text = records*2 + (records*2).replace('Block 0', 'Block 1')
        text += 'CORE_FLAGS block=0 initial=0 before=0 after=1 actual_aiv=1\nCORE_FLAGS block=1 initial=0 before=0 after=1 actual_aiv=1\n'
        verify(text, 2, 2)

if __name__ == '__main__':
    unittest.main()

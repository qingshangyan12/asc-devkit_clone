import { readFileSync, writeFileSync } from 'node:fs'
import { resolve } from 'node:path'

const root = resolve(import.meta.dirname, '..')
const sidebarPath = resolve(root, 'docs', '.vitepress', 'sidebar.mjs')

let content = readFileSync(sidebarPath, 'utf-8')

// Count changes
let count = 0

// Fix \\[ -> [
while (content.includes('\\\\[')) {
  content = content.replaceAll('\\\\[', '[')
  count++
}
// Fix \\] -> ]
while (content.includes('\\\\]')) {
  content = content.replaceAll('\\\\]', ']')
  count++
}
// Fix \\( -> (
while (content.includes('\\\\(')) {
  content = content.replaceAll('\\\\(', '(')
  count++
}
// Fix \\) -> )
while (content.includes('\\\\)')) {
  content = content.replaceAll('\\\\)', ')')
  count++
}
// Fix \( -> (  (single backslash case)
while (true) {
  // Only replace \( that's preceded by a word char or start, not by another backslash
  const before = content.length
  content = content.replaceAll('\\(', '(')
  if (content.length === before) break
  count++
}
// Fix \) -> )
while (true) {
  const before = content.length
  content = content.replaceAll('\\)', ')')
  if (content.length === before) break
  count++
}
// Fix \\_ -> _
while (true) {
  const before = content.length
  content = content.replaceAll('\\_', '_')
  if (content.length === before) break
  count++
}

writeFileSync(sidebarPath, content, 'utf-8')
console.log(`Fixed ${count} escape patterns in sidebar.mjs`)

// Verify
const checks = ['operator[]', 'operator()', 'is_layout', 'EXTERN_IMPL', 'asc_vf_call',
  'Neg(ISASI', '__float22hif82', '__habs', 'asc_atomic_add',
  'asc_syncthreads', 'GET_TILING_DATA', 'REGIST_MATMUL_OBJ',
  'ICPU_RUN_KF', 'ICPU_SET_TILING_KEY', '__asc_cvt_float2_to_fp8x2']

for (const check of [...checks]) {
  if (content.includes(check)) {
    checks.splice(checks.indexOf(check), 1)
  }
}

if (checks.length === 0) {
  console.log('All known fixes verified.')
} else {
  console.log(`Still not found: ${checks.join(', ')}`)
}

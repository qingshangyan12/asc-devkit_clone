import { readFileSync, writeFileSync, readdirSync, statSync } from 'fs';
import { join, extname } from 'path';

// Walk the guide directory and check which files would fail
const results = [];

function walkDir(dir) {
  const items = readdirSync(dir);
  for (const item of items) {
    const fullPath = join(dir, item);
    const stat = statSync(fullPath);
    if (stat.isDirectory()) {
      walkDir(fullPath);
    } else if (stat.isFile() && extname(fullPath) === '.md') {
      const content = readFileSync(fullPath, 'utf-8');
      // Check for potential Vue template issues in table cells
      // Look for < or > in table cells (markdown tables)
      const lines = content.split('\n');
      let inTable = false;
      for (const line of lines) {
        if (line.startsWith('|')) {
          inTable = true;
          // Check for unescaped angle brackets in table cells
          // that aren't part of markdown links/images or code spans
          const cells = line.split('|');
          for (let i = 1; i < cells.length - 1; i++) {
            const cell = cells[i];
            // Remove backtick code spans
            const noCode = cell.replace(/`[^`]*`/g, '');
            // Check for < or > not in HTML tags
            if (/<[a-zA-Z]/.test(noCode) || /<[a-zA-Z]/.test(noCode)) {
              results.push({
                file: fullPath,
                line: line.substring(0, 100) + '...',
                cell: cell.substring(0, 80)
              });
              break;
            }
          }
        } else if (line.trim() && !line.startsWith('|')) {
          inTable = false;
        }
      }
    }
  }
}

walkDir('docs/guide');

console.log(`Found ${results.length} files with potential issues:`);
results.slice(0, 30).forEach(r => {
  const relative = r.file.replace(/\\/g, '/').replace('docs/', '');
  console.log(`${relative}`);
  console.log(`  Cell: ${r.cell.trim()}`);
});
console.log(`... and ${Math.max(0, results.length - 30)} more`);

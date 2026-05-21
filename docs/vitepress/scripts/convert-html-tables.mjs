import { readFileSync, writeFileSync, readdirSync, statSync } from 'fs';
import { join, extname } from 'path';
import * as cheerio from 'cheerio';

function htmlTableToMarkdown(html) {
  const $ = cheerio.load(html, { xmlMode: false });
  const table = $('table');
  if (!table.length) return null;

  const lines = [];
  const colCounts = [];

  table.find('thead tr').first().find('th').each((i, el) => {
    let text = $(el).text().trim().replace(/\|/g, '\\|');
    colCounts.push(Math.max(colCounts[i] || 0, text.length));
    lines.push({ type: 'header', cells: [text], row: 0, cell: i });
  });
  if (lines.length === 0) {
    table.find('tr').first().find('th').each((i, el) => {
      let text = $(el).text().trim().replace(/\|/g, '\\|');
      colCounts.push(Math.max(colCounts[i] || 0, text.length));
      lines.push({ type: 'header', cells: [text], row: 0, cell: i });
    });
  }

  const bodyRows = [];
  const tbody = table.find('tbody');
  const rowContainer = tbody.length ? tbody : table;
  rowContainer.find('tr').each((ri, row) => {
    const tds = $(row).find('td');
    if (tds.length === 0) return;
    const cells = [];
    tds.each((ci, td) => {
      let text = $(td).text().trim().replace(/\n/g, ' ').replace(/\s+/g, ' ').replace(/\|/g, '\\|');
      // Escape < and > with backslash to prevent markdown html interpretation
      text = text.replace(/</g, '\\<').replace(/>/g, '\\>');
      colCounts[ci] = Math.max(colCounts[ci] || 0, text.length);
      cells.push(text);
    });
    if (cells.length > 0) bodyRows.push(cells);
  });

  const colCount = colCounts.length;

  if (lines.length === 0 && bodyRows.length > 0) {
    const firstRow = bodyRows.shift();
    firstRow.forEach((text, i) => {
      colCounts[i] = Math.max(colCounts[i] || 0, text.length);
      lines.push({ type: 'header', cells: [text], row: 0, cell: i });
    });
  }

  const result = [];
  const headerCells = [];
  for (let i = 0; i < colCount; i++) {
    const h = lines.find(l => l.cell === i);
    headerCells.push(h ? h.cells[0] : '');
  }

  if (headerCells.some(c => c !== '')) {
    result.push('| ' + headerCells.map(c => c.padEnd(colCounts[headerCells.indexOf(c)] || 0)).join(' | ') + ' |');
    result.push('|' + headerCells.map((_, i) => '-'.repeat((colCounts[i] || 3) + 2)).join('|') + '|');
  }

  bodyRows.forEach(cells => {
    const padded = cells.map((c, i) => c.padEnd(colCounts[i] || 0));
    while (padded.length < colCount) padded.push('');
    result.push('| ' + padded.join(' | ') + ' |');
  });

  return result.join('\n');
}

function convertFile(filePath) {
  let content = readFileSync(filePath, 'utf-8');
  const tableRegex = /<table\b[\s\S]*?<\/table>/gi;
  let changed = false;

  content = content.replace(tableRegex, (match) => {
    const md = htmlTableToMarkdown(match);
    if (md) {
      changed = true;
      return '\n' + md + '\n';
    }
    return match;
  });

  if (changed) {
    writeFileSync(filePath, content, 'utf-8');
    console.log(`Converted: ${filePath}`);
    return true;
  }
  return false;
}

function walkDir(dir) {
  const results = [];
  const items = readdirSync(dir);
  for (const item of items) {
    const fullPath = join(dir, item);
    const stat = statSync(fullPath);
    if (stat.isDirectory()) {
      results.push(...walkDir(fullPath));
    } else if (stat.isFile() && extname(fullPath) === '.md') {
      results.push(fullPath);
    }
  }
  return results;
}

const files = walkDir('docs/guide');
let count = 0;

files.forEach(f => {
  if (convertFile(f)) count++;
});

console.log(`\nTotal files converted: ${count}`);

<!--
  Copyright (c) 2026 Huawei Technologies Co., Ltd.
  This program is free software, you can redistribute it and/or modify it under the terms and conditions of
  CANN Open Software License Agreement Version 2.0 (the "License").
  Please refer to the License for details. You may not use this file except in compliance with the License.
  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE in the root of the software repository for the full text of the License.
-->

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useRoute } from 'vitepress'

const route = useRoute()
const isApiPage = computed(() => route.path.startsWith('/api/'))

const manifest = ref(null)
const manifestLoaded = ref(false)
const toast = ref('')

async function loadManifest() {
  if (manifestLoaded.value) return
  try {
    const resp = await fetch('/api-pdf/manifest.json')
    if (resp.ok) {
      manifest.value = await resp.json()
    }
  } catch {
    // manifest not available
  }
  manifestLoaded.value = true
}

onMounted(() => {
  if (isApiPage.value) {
    loadManifest()
  }
})

function handlePreview() {
  if (!manifestLoaded.value) {
    toast.value = '正在加载 PDF 索引...'
    loadManifest().then(doOpen)
    return
  }
  doOpen()
}

function doOpen() {
  if (!manifest.value) return

  let chipFilter = 'all'
  try {
    chipFilter = localStorage.getItem('cann-filter-selection') || 'all'
  } catch {}

  const key = chipFilter === 'all' ? '__ALL__' : '__' + chipFilter + '__'
  const pdfFile = manifest.value[key]

  if (pdfFile) {
    window.open('/api-pdf/' + encodeURIComponent(pdfFile), '_blank')
    toast.value = ''
    return
  }

  const allPdf = manifest.value['__ALL__']
  if (allPdf) {
    window.open('/api-pdf/' + encodeURIComponent(allPdf), '_blank')
    toast.value = ''
    return
  }

  toast.value = '当前无对应 PDF 文件'
  setTimeout(() => {
    if (toast.value.startsWith('当前无')) toast.value = ''
  }, 3000)
}
</script>

<template>
  <div v-if="isApiPage" class="api-download-wrapper">
    <button
      class="api-download-btn"
      :title="'预览完整 API PDF'"
      @click="handlePreview"
    >
      <svg
        width="20"
        height="20"
        viewBox="0 0 24 24"
        fill="none"
        stroke="currentColor"
        stroke-width="2"
        stroke-linecap="round"
        stroke-linejoin="round"
      >
        <path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z" />
        <polyline points="14 2 14 8 20 8" />
        <line x1="9" y1="13" x2="15" y2="13" />
        <line x1="9" y1="17" x2="15" y2="17" />
      </svg>
    </button>
    <div v-if="toast" class="download-progress">{{ toast }}</div>
  </div>
</template>

<style scoped>
.api-download-wrapper {
  position: fixed;
  bottom: 40px;
  right: 96px;
  z-index: 99;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 6px;
}

.api-download-btn {
  width: 44px;
  height: 44px;
  border: 1px solid var(--vp-c-divider);
  border-radius: 50%;
  background: var(--vp-c-bg-soft);
  color: var(--vp-c-text-2);
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.3s;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.api-download-btn:hover {
  border-color: var(--vp-c-brand-1);
  color: var(--vp-c-brand-1);
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
}

.download-progress {
  font-size: 11px;
  color: var(--vp-c-text-2);
  white-space: nowrap;
  background: var(--vp-c-bg-soft);
  padding: 2px 8px;
  border-radius: 4px;
  border: 1px solid var(--vp-c-divider);
}
</style>

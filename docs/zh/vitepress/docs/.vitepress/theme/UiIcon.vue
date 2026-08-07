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
defineProps({
  name: { type: String, required: true },
  size: { type: Number, default: 20 },
})
</script>

<template>
  <svg
    :width="size"
    :height="size"
    viewBox="0 0 24 24"
    fill="none"
    stroke="currentColor"
    stroke-width="2"
    stroke-linecap="round"
    stroke-linejoin="round"
    aria-hidden="true"
  >
    <template v-if="name === 'search'">
      <circle cx="11" cy="11" r="8" />
      <path d="m21 21-4.3-4.3" />
    </template>
    <template v-else-if="name === 'play'">
      <path d="M5 5a2 2 0 0 1 3-1.7l11 6.7a2 2 0 0 1 0 3.4L8 20a2 2 0 0 1-3-1.7Z" />
    </template>
    <template v-else-if="name === 'book'">
      <path d="M12 7v14" />
      <path d="M3 18a1 1 0 0 1-1-1V5a2 2 0 0 1 2-2h5a3 3 0 0 1 3 3v15a3 3 0 0 0-3-3Z" />
      <path d="M21 18a1 1 0 0 0 1-1V5a2 2 0 0 0-2-2h-5a3 3 0 0 0-3 3v15a3 3 0 0 1 3-3Z" />
    </template>
    <template v-else-if="name === 'gauge'">
      <path d="M20.4 15a9 9 0 1 0-16.8 0" />
      <path d="M12 12 8 8" />
      <path d="M3.6 15h16.8" />
    </template>
    <template v-else-if="name === 'refresh'">
      <path d="M20 11a8.1 8.1 0 0 0-15.5-2M4 4v5h5" />
      <path d="M4 13a8.1 8.1 0 0 0 15.5 2M20 20v-5h-5" />
    </template>
    <template v-else-if="name === 'library'">
      <path d="m16 6 4 14" />
      <path d="M12 6v14" />
      <path d="M8 8v12" />
      <path d="M4 4v16" />
    </template>
    <template v-else-if="name === 'braces'">
      <path d="M8 3H7a2 2 0 0 0-2 2v5a2 2 0 0 1-2 2 2 2 0 0 1 2 2v5a2 2 0 0 0 2 2h1" />
      <path d="M16 21h1a2 2 0 0 0 2-2v-5a2 2 0 0 1 2-2 2 2 0 0 1-2-2V5a2 2 0 0 0-2-2h-1" />
    </template>
    <template v-else-if="name === 'terminal'">
      <path d="m4 17 6-6-6-6" />
      <path d="M12 19h8" />
    </template>
    <template v-else-if="name === 'wrench'">
      <path d="M14.7 6.3a4 4 0 0 0-5-5l2.2 2.2-2.4 2.4-2.2-2.2a4 4 0 0 0 5 5l6.8 6.8a2.1 2.1 0 0 1-3 3l-6.8-6.8" />
    </template>
    <template v-else>
      <path d="M5 12h14" />
      <path d="m13 6 6 6-6 6" />
    </template>
  </svg>
</template>

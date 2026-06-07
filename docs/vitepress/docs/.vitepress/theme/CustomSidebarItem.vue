<script setup>
import { ref, computed, watch } from 'vue'
import { useRoute } from 'vitepress'

const props = defineProps({
  item: { type: Object, required: true },
  depth: { type: Number, required: true },
})

const route = useRoute()

const currentPath = computed(() => route.path.replace(/\.html$/, ''))

const hasChildren = computed(() => props.item.items && props.item.items.length > 0)
const isLink = computed(() => !!props.item.link)
const isActiveLink = computed(() => isLink.value && currentPath.value === props.item.link)

function hasActiveChild(item) {
  if (!item.items) return currentPath.value === (item.link || false)
  return item.items.some(c => hasActiveChild(c))
}
const hasActive = computed(() => hasActiveChild(props.item))

const collapsed = ref(
  hasActiveChild(props.item) ? false
    : (props.item.collapsed !== undefined ? props.item.collapsed : true)
)

watch(hasActive, (active) => {
  if (active && hasChildren.value) {
    collapsed.value = false
  }
})

const sectionTag = computed(() => hasChildren.value ? 'section' : 'div')
const linkTag = computed(() => isLink.value ? 'a' : 'div')
const textTag = computed(() => {
  if (!hasChildren.value) return 'p'
  return props.depth + 2 === 7 ? 'p' : `h${props.depth + 2}`
})

const itemRole = computed(() => isLink.value ? undefined : 'button')

const classes = computed(() => [
  `level-${props.depth}`,
  { collapsible: hasChildren.value && props.item.items.length > 0 },
  { collapsed: collapsed.value },
  { 'is-link': isLink.value },
  { 'is-active': isActiveLink.value },
  { 'has-active': hasActive.value },
])

function onItemInteraction(e) {
  if ('key' in e && e.key !== 'Enter') return
  toggle()
}

function toggle() {
  collapsed.value = !collapsed.value
}
</script>

<template>
  <component :is="sectionTag" class="VPSidebarItem" :class="classes">
    <div v-if="item.text" class="item" :role="itemRole" :tabindex="hasChildren ? 0 : undefined"
      v-on="hasChildren ? { click: onItemInteraction, keydown: onItemInteraction } : {}">
      <div class="indicator" />
      <component :is="linkTag" v-if="isLink" class="link" :href="item.link">
        <component :is="textTag" class="text">{{ item.text }}</component>
      </component>
      <component v-else :is="textTag" class="text">{{ item.text }}</component>
      <div
        v-if="hasChildren"
        class="caret"
        role="button"
        aria-label="toggle section"
        @click.stop="toggle"
        @keydown.enter="toggle"
        tabindex="0"
      >
        <span class="vpi-chevron-right caret-icon" />
      </div>
    </div>
    <div v-if="item.items && item.items.length" class="items">
      <template v-if="depth < 5">
        <CustomSidebarItem
          v-for="child in item.items"
          :key="child.text"
          :item="child"
          :depth="depth + 1"
        />
      </template>
    </div>
  </component>
</template>

<style scoped>
.VPSidebarItem.level-0 {
  padding-bottom: 24px;
}

.VPSidebarItem.collapsed.level-0 {
  padding-bottom: 10px;
}

.item {
  position: relative;
  display: flex;
  width: 100%;
}

.VPSidebarItem.collapsible > .item {
  cursor: pointer;
}

.caret {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-right: -7px;
  width: 32px;
  height: 32px;
  color: var(--vp-c-text-3);
  cursor: pointer;
  transition: color 0.25s;
  flex-shrink: 0;
}

.item:hover .caret {
  color: var(--vp-c-text-2);
}

.item:hover .caret:hover {
  color: var(--vp-c-text-1);
}

.caret-icon {
  font-size: 18px;
  transform: rotate(90deg);
  transition: transform 0.25s;
}

.VPSidebarItem.collapsed .caret-icon {
  transform: rotate(0);
}

.indicator {
  position: absolute;
  top: 6px;
  bottom: 6px;
  left: -17px;
  width: 2px;
  border-radius: 2px;
  transition: background-color 0.25s;
}

.VPSidebarItem.level-2.is-active > .item > .indicator,
.VPSidebarItem.level-3.is-active > .item > .indicator,
.VPSidebarItem.level-4.is-active > .item > .indicator,
.VPSidebarItem.level-5.is-active > .item > .indicator {
  background-color: var(--vp-c-brand-1);
}

.link {
  display: flex;
  align-items: center;
  flex-grow: 1;
}

.text {
  flex-grow: 1;
  padding: 4px 0;
  line-height: 24px;
  font-size: 14px !important;
  transition: color 0.25s;
  margin: 0;
}

.VPSidebarItem.level-0 .text {
  font-weight: 700;
  color: var(--vp-c-text-1);
}

.VPSidebarItem.level-1 .text,
.VPSidebarItem.level-2 .text,
.VPSidebarItem.level-3 .text,
.VPSidebarItem.level-4 .text,
.VPSidebarItem.level-5 .text {
  font-weight: 500;
  color: var(--vp-c-text-2);
}

.VPSidebarItem.level-0.is-link > .item > .link:hover .text,
.VPSidebarItem.level-1.is-link > .item > .link:hover .text,
.VPSidebarItem.level-2.is-link > .item > .link:hover .text,
.VPSidebarItem.level-3.is-link > .item > .link:hover .text,
.VPSidebarItem.level-4.is-link > .item > .link:hover .text,
.VPSidebarItem.level-5.is-link > .item > .link:hover .text {
  color: var(--vp-c-brand-1);
}

.VPSidebarItem.level-0.has-active > .item > .text,
.VPSidebarItem.level-1.has-active > .item > .text,
.VPSidebarItem.level-2.has-active > .item > .text,
.VPSidebarItem.level-3.has-active > .item > .text,
.VPSidebarItem.level-4.has-active > .item > .text,
.VPSidebarItem.level-5.has-active > .item > .text,
.VPSidebarItem.level-0.has-active > .item > .link > .text,
.VPSidebarItem.level-1.has-active > .item > .link > .text,
.VPSidebarItem.level-2.has-active > .item > .link > .text,
.VPSidebarItem.level-3.has-active > .item > .link > .text,
.VPSidebarItem.level-4.has-active > .item > .link > .text,
.VPSidebarItem.level-5.has-active > .item > .link > .text {
  color: var(--vp-c-text-1);
}

.VPSidebarItem.level-0.is-active > .item .link > .text,
.VPSidebarItem.level-1.is-active > .item .link > .text,
.VPSidebarItem.level-2.is-active > .item .link > .text,
.VPSidebarItem.level-3.is-active > .item .link > .text,
.VPSidebarItem.level-4.is-active > .item .link > .text,
.VPSidebarItem.level-5.is-active > .item .link > .text {
  color: var(--vp-c-brand-1);
}

.VPSidebarItem.level-1 .items,
.VPSidebarItem.level-2 .items,
.VPSidebarItem.level-3 .items,
.VPSidebarItem.level-4 .items,
.VPSidebarItem.level-5 .items {
  border-left: 1px solid var(--vp-c-divider);
  padding-left: 16px;
}

.VPSidebarItem.collapsed .items {
  display: none;
}
</style>

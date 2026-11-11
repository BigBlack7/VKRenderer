# 04 — Vertex Buffers

> Phase 2 · GPU Resources · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[03 Drawing a Triangle](03-Drawing-a-Triangle.md) ｜ 下一章：[05 Uniform Buffers](05-Uniform-Buffers.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

<a id="lesson-04-0"></a>

## 04.0 — Vertex Input Description

- [ ] Vertex Struct
- [ ] Binding Description
- [ ] Attribute Description
- [ ] Location
- [ ] Format
- [ ] Stride / Offset
- [ ] C++ memory layout ↔ Shader input

---

<a id="lesson-04-1"></a>

## 04.1 — Vertex Buffer Creation

- [ ] Buffer
- [ ] Memory Requirements
- [ ] Physical Device Memory Properties
- [ ] Memory Type Selection
- [ ] Device Memory Allocation
- [ ] Bind Buffer Memory
- [ ] Map / Unmap
- [ ] Host Visible / Host Coherent / Device Local

---

<a id="lesson-04-2"></a>

## 04.2 — Staging Buffer

- [ ] GPU memory hierarchy
- [ ] Staging Upload
- [ ] Transfer Source / Destination
- [ ] Copy Buffer Command
- [ ] Device Local Memory
- [ ] One-shot command 的实现与代价

**Core 候选：**
开始观察 `Buffer`、memory type selection、upload helper 是否已经形成稳定重复。

---

<a id="lesson-04-3"></a>

## 04.3 — Index Buffer

- [ ] Vertex Reuse
- [ ] Index Buffer
- [ ] Index Type
- [ ] Bind Index Buffer
- [ ] `drawIndexed`

**Milestone：GPU Geometry Buffer Rendering**

---

[返回学习地图](../VulkanLearningRoadmap.md)

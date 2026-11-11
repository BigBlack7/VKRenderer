# 16 — Rendering Multiple Objects

> Phase 5 · Vulkan 工程化与兼容性 · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[15 Migrating to glTF and KTX2](15-Migrating-to-glTF-and-KTX2.md) ｜ 下一章：[17 Multithreading](17-Multithreading.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

- [ ] 16.1 Introduction
- [ ] 16.2 Shared vs Per-object Resources
- [ ] 16.3 Game / Render Object Data Model
- [ ] 16.4 Transform — Position / Rotation / Scale
- [ ] 16.5 Model Matrix
- [ ] 16.6 Uniform Buffers per Object
- [ ] 16.7 Descriptor Sets per Object
- [ ] 16.8 Update All Object Uniforms
- [ ] 16.9 Recording Multiple Draw Calls
- [ ] 16.10 Resource Reuse
- [ ] 16.11 Draw-loop organization

**Core 重构重点：**
此处是第一次较大的架构 checkpoint，重点讨论：

- Mesh
- Material
- Transform
- RenderObject
- Resource ownership
- Renderer 与 Scene 数据边界

---

[返回学习地图](../VulkanLearningRoadmap.md)

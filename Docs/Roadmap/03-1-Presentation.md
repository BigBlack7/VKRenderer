# 03.1 — Presentation

> Phase 1 · [第 03 章目录](03-Drawing-a-Triangle.md) · [学习地图](../VulkanLearningRoadmap.md)

> 上一单元：[03.0 Setup](03-0-Setup.md) ｜ 下一单元：[03.2 Graphics Pipeline Basics](03-2-Graphics-Pipeline-Basics.md)

<a id="lesson-03-1-0"></a>

## 03.1.0 — Window Surface

- [ ] WSI
- [ ] Vulkan 的平台无关性
- [ ] SurfaceKHR
- [ ] Win32 Surface 与 GLFW abstraction
- [ ] Presentation Support
- [ ] Graphics Queue 与 Present Queue 的关系

**Core 候选：**
重新评估 `Window` 是否已经形成稳定职责。

---

<a id="lesson-03-1-1"></a>

## 03.1.1 — Swap Chain

- [ ] Surface Capabilities
- [ ] Surface Format / Color Space
- [ ] Present Mode
- [ ] Swap Extent
- [ ] Image Count
- [ ] Image Sharing Mode
- [ ] Swapchain Images
- [ ] Acquire → Render → Present 模型
- [ ] Double / Triple Buffering 与 VSync

**实践：**
创建 Swapchain 并获取 Swapchain Images。

---

<a id="lesson-03-1-2"></a>

## 03.1.2 — Image Views

- [ ] Image 与 ImageView 的区别
- [ ] View Type
- [ ] Format
- [ ] Component Mapping
- [ ] Subresource Range
- [ ] Aspect / Mip Level / Array Layer

**实践：**
为所有 Swapchain Image 建立 ImageView。

---

[返回第 03 章](03-Drawing-a-Triangle.md) · [返回学习地图](../VulkanLearningRoadmap.md)

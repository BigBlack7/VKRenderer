# 03.2 — Graphics Pipeline Basics

> Phase 1 · [第 03 章目录](03-Drawing-a-Triangle.md) · [学习地图](../VulkanLearningRoadmap.md)

> 上一单元：[03.1 Presentation](03-1-Presentation.md) ｜ 下一单元：[03.3 Drawing](03-3-Drawing.md)

<a id="lesson-03-2-0"></a>

## 03.2.0 — Introduction

- [ ] Graphics Pipeline 全流程
- [ ] Programmable Stage
- [ ] Fixed Function Stage
- [ ] Pipeline State Object
- [ ] Vulkan 为什么倾向预先创建 Pipeline

---

<a id="lesson-03-2-1"></a>

## 03.2.1 — Shader Modules

- [ ] Vertex Shader
- [ ] Fragment Shader
- [ ] Slang 基础
- [ ] Entry Point
- [ ] Shader Stage
- [ ] SPIR-V
- [ ] Shader Module
- [ ] CPU / Shader interface
- [ ] CMake 自动执行 `slangc`
- [ ] SPIR-V 输出到 Build 目录

**实践：**
在 `Slang/` 建立第一个正式 shader，并由 CMake 自动编译。

---

<a id="lesson-03-2-2"></a>

## 03.2.2 — Fixed Functions

- [ ] Vertex Input State
- [ ] Input Assembly
- [ ] Viewport
- [ ] Scissor
- [ ] Rasterizer
- [ ] Multisampling State
- [ ] Depth / Stencil State
- [ ] Color Blend State
- [ ] Dynamic State
- [ ] Pipeline Layout

---

<a id="lesson-03-2-3"></a>

## 03.2.3 — Dynamic Rendering

- [ ] 传统 RenderPass / Framebuffer 模型
- [ ] Dynamic Rendering 的意义
- [ ] Pipeline Rendering Info
- [ ] Attachment Format
- [ ] `beginRendering` / `endRendering`

---

<a id="lesson-03-2-4"></a>

## 03.2.4 — Conclusion / Graphics Pipeline Creation

- [ ] Shader Stage + Fixed Function + Pipeline Layout
- [ ] Graphics Pipeline CreateInfo
- [ ] 创建 Graphics Pipeline
- [ ] Pipeline 生命周期与依赖关系

**Core 候选：**
只有在后续多个 Sandbox 出现真实重复后，再讨论 `Pipeline` 抽象。

---

[返回第 03 章](03-Drawing-a-Triangle.md) · [返回学习地图](../VulkanLearningRoadmap.md)

# 03.2 — Graphics Pipeline Basics

> Phase 1 · [第 03 章目录](03-Drawing-a-Triangle.md) · [学习地图](../VulkanLearningRoadmap.md)

> 上一单元：[03.1 Presentation](03-1-Presentation.md) ｜ 下一单元：[03.3 Drawing](03-3-Drawing.md)

<a id="lesson-03-2-0"></a>

## 03.2.0 — Introduction

- [x] Graphics Pipeline 全流程
  - 理解 Input Assembly
  - 理解 Vertex Shader
  - 理解 Tessellation / Geometry 等可选阶段
  - 理解 Rasterization
  - 理解 Fragment Shader
  - 理解 Fragment Operations 与 Color Blending
  - 理解渲染结果写入 Render Target 的过程
- [x] Programmable Stage
  - 理解 Shader 的职责
  - 区分 Vertex Shader 与 Fragment Shader
  - 理解可编程阶段的并行处理模型
- [x] Fixed Function Stage
  - 理解固定功能阶段的处理逻辑
  - 理解固定功能不代表状态不可配置
  - 区分固定功能状态与 Shader 程序
- [x] Pipeline State Object
  - 理解 Graphics Pipeline 的状态组织方式
  - 区分 Pipeline、Pipeline Layout 与 Command Buffer
  - 理解 Pipeline 创建不等于 GPU 执行绘制
  - 理解 Static State 与 Dynamic State
- [x] Vulkan 为什么倾向预先创建 Pipeline
  - 理解状态配置前移的设计动机
  - 理解驱动提前进行 Shader 编译及相关优化
  - 理解 Dynamic State 对管线灵活性的补充

**工程准备：**

- 在 `HelloTriangleApplication` 中新增 `createGraphicsPipeline()`。
- 当前仅定义空函数，尚未实际创建 Graphics Pipeline。
- 下一小节将该函数接入 `initVulkan()`。
- 不提前创建 Pipeline Layout、Shader Module 或其他 Pipeline 资源。
- 不新增 Pipeline Manager 或其他 Core 抽象。

**原理理解验收（2026-11-24）：**

四道原理题全部通过。

已理解：

1. Graphics Pipeline 与 Swapchain ImageView 的职责区别。
2. Graphics Pipeline 各主要阶段的输入、处理与输出。
3. Programmable Stage 与 Fixed-Function Stage 的区别。
4. Fragment Shader 输出不一定直接成为最终图像颜色。
5. Pipeline State Object 的设计动机。
6. Static State 与 Dynamic State 的区别。
7. 创建 Pipeline、录制命令、Queue Submission 与 GPU 执行是不同阶段。
8. GPU 渲染结果通常直接写入图像资源，不需要将像素读回 CPU。
9. Presentation Engine 负责后续图像呈现。
10. Dynamic Rendering 与 Graphics Pipeline 的基本关系。

**GitHub Code Review：**

工程准备 Commit：

`bddfce32789255b37bedad6d5f7ce3666df41e31`

审查结果：

- 已定义 `createGraphicsPipeline()`。
- 尚未在 `initVulkan()` 中调用该函数。
- 下一小节接入初始化流程即可。
- 无未解决的 P0 / P1 问题。
- 当前为理论章节，不强制进行独立编译运行验证。

**最终验收结论（2026-11-24）：**

- Graphics Pipeline 原理理解通过。
- Programmable / Fixed-Function Stage 理解通过。
- Pipeline State 与 Dynamic State 理解通过。
- CPU / GPU 协作流程理解通过。
- 工程准备代码静态审查通过。

**小节状态：已完成。**

**下一小节：**

03.2.1 — Shader Modules

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

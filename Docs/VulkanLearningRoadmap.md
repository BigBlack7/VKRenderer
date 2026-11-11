# VKRenderer Vulkan 学习总纲

> 基于 Khronos Vulkan Tutorial（latest）整理  
> 项目环境：Windows + VSCode + MSVC + CMake + C++20  
> 主线技术：Vulkan 1.4 / Vulkan-Hpp RAII / Dynamic Rendering / Timeline Semaphore / Slang  
> 仓库：`BigBlack7/VKRenderer`

---

## 0. 使用说明

这份文档既是课程目录，也是学习进度表。

- `[ ]`：尚未完成
- `[x]`：已达到对应小节的验收要求
- `Optional`：可选章节，不阻塞主线
- `Advanced`：高级专题，完成必要前置学习后进入


每完成一个教学小节：

1. 完成本节理论学习，以及该节要求的代码实践。
2. 理论小节通过理解检查后，可直接标记完成并继续下一节。
3. 简单理论章节允许在同一会话中连续推进，无需逐节 Git 提交。
4. 理论大章节结束后，统一更新学习进度并提交 Git。
5. 实践小节需完成相应的编译、运行和必要的 Validation。
6. 实践代码按阶段或里程碑提交 GitHub，进行 Code Review。
7. 修正问题并通过验收后，将对应 `[ ]` 修改为 `[x]`。
8. 如果出现稳定且重复的抽象，再讨论是否提升到 Core。

> **原则：先验证，后抽象。Sandbox 是实验场，Core 是稳定复用层。**

---

# 1. 项目架构约定

基础目录：

```text
VKRenderer/
├─ Assets/
│  ├─ Model/
│  └─ Texture/
├─ Core/
├─ Docs/
├─ Sandbox/
├─ Slang/
├─ ThirdParty/
├─ CMakeLists.txt
├─ .gitignore
└─ README.md
```

职责约定：

- `Sandbox/`
  - Vulkan 学习、实验和章节 checkpoint。
  - 每个有独立实验意义的用例都应能够单独编译和运行。
  - Sandbox 之间禁止互相依赖。
  - Sandbox 可以依赖 Core 和 ThirdParty。

- `Core/`
  - 只放经过多个 Sandbox 验证、责任边界已经明确的可复用代码。
  - 不因为“以后可能会用到”而提前设计抽象。
  - Core API 修改后，需要重新验证所有依赖它的 Sandbox。

- `ThirdParty/`
  - 工程内显式管理第三方依赖。
  - 当前基础依赖：GLFW、GLM。
  - 后续 tinyobjloader、stb、tinygltf、KTX 等按课程进度加入。
  - Vulkan SDK 属于开发环境依赖，不复制进 ThirdParty。

- `Slang/`
  - GPU Shader 源码。
  - 编译后的 SPIR-V 属于构建产物，应输出到 Build 目录，而不是源码树。

- `Assets/`
  - Runtime 资源。
  - `Model/`：模型资源。
  - `Texture/`：纹理资源。

- `Docs/`
  - 教学总纲、阶段总结、架构设计记录等项目文档。

---

# 2. 固定教学流程

每个正式小节按照以下顺序推进：

1. **官方文档拆解**
   - 逐段解释官方章节。
   - 明确这一节在完整 Vulkan 系统中的位置。

2. **原理**
   - 为什么 Vulkan 需要这个对象 / 状态 / 流程。
   - CPU、Driver、GPU、Presentation Engine 之间发生什么。

3. **API**
   - Vulkan-Hpp / RAII 对象。
   - CreateInfo、Flags、Feature、Extension、生命周期和依赖关系。

4. **代码**
   - 在当前 Sandbox 中完成最直接、最容易理解的实现。

5. **验证**
   - Validation Layer。
   - RenderDoc / vkconfig / vulkaninfo 等工具按需要加入。

6. **架构判断**
   - 是否出现真正稳定的重复逻辑。
   - 是否值得提升到 Core。
   - 不强行为每个 Vulkan handle 创建 wrapper class。

7. **作业与 Review**
   - Push GitHub。
   - 检查正确性、RAII 生命周期、CMake、现代 C++、性能隐患和 Vulkan Validation。

---

# Phase 0 — 世界观与开发环境

## 00 — Introduction

官方：
`00_Introduction`

目标：明确当前 Khronos 教程采用的现代 Vulkan 技术栈和学习边界。

- [x] 00.1 Attribution / 教程来源与 Khronos 版本
- [x] 00.2 Differences / 新版教程与旧 Vulkan Tutorial 的差异
  - Vulkan 1.4 baseline
  - Dynamic Rendering
  - Timeline Semaphore
  - Slang
  - C++20
  - Vulkan-Hpp + RAII
- [x] 00.3 About / Vulkan 的定位、优势与代价
- [x] 00.4 前置知识与工具链要求
- [x] 00.5 Vulkan-Hpp / RAII 的教学路线
- [x] 00.6 Tutorial Structure / 官方教程整体结构
- [x] 00.7 Advanced Topics / 高级专题入口

**课程产物：**
- Vulkan 学习目标与技术基线确认。
- 不要求产生正式 Vulkan 代码。

**验收记录（2026-10-09）：**
- Introduction 七个小节的理论学习已完成。
- 明确 VKRenderer 的现代 Vulkan 技术基线。
- 明确 Sandbox 与 Core 的架构职责。
- 本章无正式 Vulkan 代码产物，无需代码 Review。

**章节状态：已完成。**

---


## 01 — Overview

官方：
`01_Overview`

目标：在写代码前建立完整 Vulkan mental model。

- [x] 01.1 Origin of Vulkan
  - 传统图形 API 的 Driver 隐式工作
  - Vulkan 为什么选择显式模型
  - CPU overhead 与多线程
- [x] 01.2 Coding Conventions
  - Vulkan C API 与 Vulkan-Hpp
  - handle / enum / flags / struct 基本阅读方式
- [x] 01.3 What It Takes to Draw a Triangle
  - [x] Step 1 — Instance & Physical Device
  - [x] Step 2 — Logical Device & Queue Families
  - [x] Step 3 — Window Surface & Swapchain
  - [x] Step 4 — Swapchain Image Views
  - [x] Step 5 — Dynamic Rendering
  - [x] Step 6 — Graphics Pipeline
  - [x] Step 7 — Command Pool & Command Buffer
  - [x] Step 8 — Main Loop / Acquire / Submit / Present
- [x] 01.4 Summary / 第一帧的完整数据流
- [x] 01.5 API Concepts
  - CreateInfo
  - `sType` / `pNext`
  - `vk::Result`
  - Exception
  - Allocation callbacks
  - Vulkan-Hpp / RAII 所有权
- [x] 01.6 Validation Layers
  - `VK_LAYER_KHRONOS_validation`
  - `VK_EXT_debug_utils`
  - Debug Messenger
  - Severity / Message Type
  - VUID 与规范查询
  - Validation 能力边界

**课程产物：**
- 建立从 Instance 到第一帧呈现的完整对象关系模型。
- 理解 CPU 命令录制、GPU 执行和 Presentation 的区别。
- 理解 Vulkan 对象所有权、生命周期及同步的基本约束。
- 理解 Vulkan-Hpp、RAII、CreateInfo、pNext 和 Result。
- 建立使用 Validation Layer、Debug Messenger 和 VUID 排查问题的基本方法。

**验收记录（2026-11-10）：**
- 01.1～01.6 全部完成理论学习并通过理解检查。
- 01.3 的八个 Step 全部通过单独验收。
- 能够解释第一帧 Vulkan 渲染的主要对象、执行流程和同步关系。
- 已区分 Surface、Swapchain、ImageView 的职责及所有权。
- 已区分 Graphics Queue、Present Queue 和 Queue Family。
- 已区分 Command Buffer 录制、Queue Submission 和 GPU 执行。
- 已区分 Graphics Pipeline、Dynamic Rendering 和 Image Layout。
- 已理解 Validation Layer 的作用及其不能保证程序完全正确的局限。
- 本章属于理论章节，无正式 Vulkan 代码产物，不要求编译及代码 Review。

**后续实践仍需验证：**
- 真实 Vulkan 对象的创建与 RAII 生命周期。
- GPU 同步、图像布局转换及 Swapchain 重建。
- Vulkan-Hpp / C++20 / MSVC 的实际编译兼容性。
- Validation Layers 的实际运行及错误诊断。

**章节状态：已完成（理论验收通过）。**

---

## 02 — Development Environment

官方：
`02_Development_environment`

目标：建立 VKRenderer 零号工程基线，验证 Windows、MSVC、CMake、Vulkan SDK、Slang 和 ThirdParty 依赖，确保后续 Vulkan 开发环境稳定可用。

- [x] 02.1 Repository / Git 基线
  - 确认 GitHub 仓库与 main 分支
  - 保留现有 VKRenderer 工程作为开发基线
- [x] 02.2 Windows + VSCode + MSVC
  - Visual Studio 2022
  - MSVC 19.44.35228.0
  - Windows SDK 10.0.26100.0
  - C++20 编译通过
- [x] 02.3 CMake
  - configure / generate / build 成功
  - Visual Studio 17 2022 / x64
  - Debug / Release 多配置构建成功
- [x] 02.4 Vulkan SDK
  - Vulkan SDK 1.4.357
  - Vulkan Headers / Vulkan-Hpp RAII
  - Vulkan Loader 1.4.357
  - `find_package(Vulkan REQUIRED)` 成功
  - `Vulkan::Vulkan` 链接成功
  - `vulkaninfo` 正常运行
  - `vkcube` 正常运行
  - `VK_LAYER_KHRONOS_validation` 已检测到
  - `VK_EXT_debug_utils` 已检测到
  - `vkconfig` 未单独验证，不阻塞 M0
- [x] 02.5 Slang Toolchain
  - `slangc` 2026.13.1 可用
  - Slang → SPIR-V 的自动编译将在 Shader Modules 小节实现
- [x] 02.6 Window Management — GLFW
  - GLFW 3.4.0
  - 头文件包含成功
  - 静态库链接成功
  - `glfwGetVersion()` 调用成功
- [x] 02.7 Math Library — GLM
  - GLM 头文件包含成功
  - `glm::vec3` 测试成功
- [x] 02.8 Texture / Model 第三方依赖策略
  - 当前阶段不提前引入纹理与模型加载库
  - stb_image、tinyobjloader 等按后续教程进度加入
- [x] 02.9 VKRenderer 目录职责确认
  - Core
  - Sandbox
  - ThirdParty
  - Slang
  - Assets
  - Docs
- [x] 02.10 Root CMake 基线
  - C++20
  - MSVC 编译选项
  - 构建输出目录
  - 基础子目录配置
- [x] 02.11 ThirdParty CMake 基线
  - GLFW Imported Static Library
  - GLM Interface Library
  - CMake Target 依赖传递正常
- [x] 02.12 Core / Sandbox CMake 基线
  - 保留现有 Core / Sandbox 结构
  - Sandbox 建立 EnvironmentCheck 构建目标
  - Core 暂无复用代码，不提前抽象
- [x] 02.13 首个 Environment Check 可执行目标
  - `Sandbox/EnvironmentCheck/Main.cpp`
  - GLFW / GLM 基础功能验证
  - Vulkan Loader 版本查询
  - Vulkan-Hpp RAII 头文件编译验证
  - 不创建正式 Vulkan 渲染对象
- [x] 02.14 Debug / Release 均能 configure、build、run
  - Debug 构建及运行成功
  - Release 构建及运行成功
  - Debug LNK4098 链接警告已修正
  - Debug Clean Build 重新验证通过
- [x] 02.15 零号基线 Review
  - EnvironmentCheck 已提交 GitHub。
  - 最新实践 Commit：`7fc7bbcae08539fd49bcb44d841d3a4598d93315`
  - Root CMake、ThirdParty CMake、Sandbox CMake 审查通过。
  - EnvironmentCheck C++20 源码审查通过。
  - Debug / Release 本地编译与运行验证通过。
  - 无 P0 / P1 问题。
  - M0 最终验收通过。

**课程产物：**

- 建立能够正常配置、编译和运行的 C++20 工程基线。
- 验证 Vulkan SDK、Loader、Vulkan-Hpp 和 Validation Layer 的基础可用性。
- 验证 GLFW、GLM、Slang 工具链。
- 完成 Debug / Release 双配置构建验证。
- 建立最小 EnvironmentCheck Sandbox。
- 保留渐进式架构，不提前实现正式 Vulkan 渲染功能。

**本机环境记录（2026-11-10）：**

- 操作系统：Windows x64
- 编译器：MSVC 19.44.35228.0
- Windows SDK：10.0.26100.0
- CMake 生成器：Visual Studio 17 2022
- C++ 标准：C++20
- Vulkan SDK / Loader：1.4.357
- GLFW：3.4.0
- Slang：2026.13.1
- 主要 GPU：NVIDIA GeForce RTX 4080 Laptop GPU
- NVIDIA 驱动：621.01
- GPU Vulkan API：1.4.357
- Validation Layer：VK_LAYER_KHRONOS_validation 已发现

**EnvironmentCheck 验证结果：**

Debug / Release 均成功输出：

    GLFW: 3.4.0
    GLM test: 6
    Vulkan Loader API: 1.4.357

**问题修正记录：**

1. Debug 构建初次出现 MSVC `LNK4098` 运行时库冲突警告。
   - EnvironmentCheck Target 使用 `MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"`。
   - 当前测试目标统一使用 `/MD`。
   - Debug Clean Build 成功，警告已消失。
   - 后续正式项目需注意 Debug / Release CRT 与第三方库的兼容性。

2. `vulkaninfo` 出现 OBS Hook 与部分 ICD Display Extension 警告。
   - Vulkan GPU 枚举正常。
   - NVIDIA GPU Vulkan 1.4 支持已确认。
   - 当前未发现阻断基础环境验证的问题。
   - 后续如发生 Layer 冲突，再进行针对性排查。

**GPU 兼容性说明：**

- NVIDIA RTX 4080 Laptop GPU 支持 Vulkan 1.4.357。
- Intel RaptorLake-S Mobile Graphics 支持 Vulkan 1.3.295。
- 系统还枚举出 Dozen / Direct3D12 兼容设备。
- 后续实践优先使用 NVIDIA 独立 GPU。
- 不依赖固定 Physical Device 枚举索引。
- Vulkan API 版本满足要求不等于所有 Feature 均已验证，后续创建 Physical Device / Logical Device 时进行正式能力检查。

**架构决策：**

- 继续沿用现有 VKRenderer 工程。
- Sandbox 用于独立实验和章节验证。
- Core 暂不添加无实际需求的抽象。
- ThirdParty 保留当前 GLFW / GLM 配置。
- 不提前创建 Window、Context、Renderer 等封装。
- 不提前实现 Drawing a Triangle。

**验收记录（2026-11-10）：**

- M0 基础工具链验证完成。
- CMake / MSVC / C++20 编译验证完成。
- Vulkan SDK、Loader、Vulkan-Hpp 基础兼容性验证完成。
- GLFW、GLM、Slang 基础验证完成。
- Debug / Release 编译运行验证完成。
- Debug CRT 链接警告已修正。
- GitHub 最终 Code Review 已完成。
- 审查 Commit：`7fc7bbcae08539fd49bcb44d841d3a4598d93315`
- 未发现 P0 / P1 问题。
- EnvironmentCheck 保留为独立 Sandbox。
- Core 暂不引入不必要的封装。
- 后续正式 Vulkan Sandbox 需要重新评估 Debug CRT 与第三方库的兼容性。
- Vulkan Feature、设备选择和真实 Validation 运行将在后续相关章节验证。
- M0 开发环境基线通过最终验收。

**章节状态：已完成（本地验证与 GitHub Code Review 通过）。**

---

# Phase 1 — Drawing a Triangle

# 03 — Drawing a Triangle

---

## 03.0 — Setup

### 03.0.0 — Base Code

- [x] GLFW 初始化与窗口循环
  - `glfwInit()` 初始化与失败检查
  - `glfwCreateWindow()` 创建窗口与失败检查
  - `glfwWindowShouldClose()` 窗口关闭状态
  - `glfwPollEvents()` 事件轮询
  - `glfwDestroyWindow()` 窗口销毁
  - `glfwTerminate()` GLFW 终止
- [x] `GLFW_NO_API`
  - 不创建 OpenGL / OpenGL ES Context
  - 理解 GLFW Window 与 Vulkan Context 的区别
  - 当前阶段使用不可调整大小的窗口
- [x] Vulkan-Hpp / `vulkan_raii.hpp`
  - 正确包含 Vulkan-Hpp RAII 头文件
  - 配置 `VULKAN_HPP_NO_STRUCT_CONSTRUCTORS`
  - 保持 C++20 技术路线
- [x] RAII Context
  - 理解 `vk::raii::Context` 的基本定位
  - 理解 RAII 对象所有权与生命周期
  - 实际创建和使用将在 03.0.1 Instance 中进行
- [x] 程序异常边界
  - `std::runtime_error` 错误报告
  - `try/catch` 异常捕获
  - 异常清理后重新抛出
  - `EXIT_SUCCESS` / `EXIT_FAILURE`
  - GLFW 初始化及窗口创建失败处理
- [x] 初始化 / 主循环 / 清理的职责关系
  - `initWindow()`
  - `initVulkan()`（当前阶段为空）
  - `mainLoop()`
  - `cleanup()`
  - `run()`
  - `main()`

**实践：**

创建第一个正式 Vulkan Sandbox。

- 新增 `Sandbox/HelloTriangle/Main.cpp`
- 新增 `HelloTriangle` CMake Target
- 保留独立的 `EnvironmentCheck` Sandbox
- 创建 800×600 GLFW 窗口
- 正常处理窗口事件与退出
- 完成 GLFW 资源清理及异常边界
- 不提前创建 Vulkan Instance、Surface 或 Swapchain

**工程设计：**

- `HelloTriangleApplication` 暂时保留在 `Main.cpp` 中。
- 使用最小必要的 Application 类组织运行流程。
- 当前窗口由 Application 显式管理。
- 不提前引入独立 Window 类。
- 不提前建立 Vulkan Context / Renderer 抽象。
- 不向 Core 提升未经验证的代码。
- 保留现有 GLFW 静态库与 MSVC `/MD` 兼容配置。
- Vulkan-Hpp RAII 对象的实际创建留给后续小节。

**编译与运行验证（2026-11-11）：**

- CMake Configure 成功。
- Visual Studio 17 2022 / x64。
- MSVC 19.44.35228.0。
- C++20。
- Vulkan SDK 1.4.357。
- Debug 编译成功。
- Debug 运行正常。
- Release 编译成功。
- Release 运行正常。
- GLFW 窗口正常创建、响应事件并关闭。
- GLFW 窗口创建失败路径已增加错误检查。
- 异常路径测试由开发者确认符合预期。
- 当前尚未创建 Vulkan Instance，不涉及实际 GPU 绘制。
- 当前不要求 Vulkan Validation Layer 验证。

**GitHub Code Review：**

- 首次实现 Commit：
  `3cce73676a2a73406265c5f85ae097ad19dfb2cd`
- 最终修复 Commit：
  `b99f2e8df049efb61ddbe5f00a20f10516f6aac5`

首次审查发现：

1. P1：未检查 `glfwCreateWindow()` 返回值。
   - 窗口创建失败可能导致空指针继续传入 GLFW API。
   - 修复方式：检查返回值并抛出 `std::runtime_error`。
   - 通过现有异常处理流程完成 GLFW 资源清理。

第二轮审查：

- P1 问题已修复。
- GLFW 初始化及清理逻辑正确。
- Application 职责划分合理。
- CMake Target 配置正确。
- 未引入不必要的架构抽象。
- 未发现新的 P0 / P1 问题。
- GitHub 静态代码审查通过。

**验收记录（2026-11-11）：**

- 四道原理理解题全部通过。
- 已正确理解 GLFW 与 Vulkan 的职责区别。
- 已理解窗口事件循环的作用。
- 已理解异常清理及重新抛出的执行流程。
- 已理解 GLFW 裸指针所有权与 Vulkan-Hpp RAII 的区别。
- Debug / Release 编译运行验证通过。
- 窗口创建失败处理已修复。
- GitHub 最终 Code Review 通过。
- 无遗留 P0 / P1 问题。
- 本节无需进一步架构重构。
- 03.0.0 Base Code 通过最终验收。

**Core 判断：**

当前仍未出现稳定的跨 Sandbox 复用需求。

继续保留最小 Application 结构，暂不抽象 Window、VulkanContext 或 Renderer。

后续进入 Surface、Swapchain 和 Resize 相关章节时，再根据实际资源依赖和窗口职责重新评估。

**小节状态：已完成（理论、实现、编译运行与 GitHub Code Review 通过）。**

---

### 03.0.1 — Instance

- [ ] Vulkan Loader → Context → Instance
- [ ] `vk::ApplicationInfo`
- [ ] `vk::InstanceCreateInfo`
- [ ] Vulkan API Version
- [ ] Instance Extensions
- [ ] GLFW required extensions
- [ ] Instance 生命周期

**实践：**
成功创建 `vk::raii::Instance`。

---

### 03.0.2 — Validation Layers

- [ ] Vulkan Layer 模型
- [ ] `VK_LAYER_KHRONOS_validation`
- [ ] Debug Utils Extension
- [ ] Debug Messenger
- [ ] Severity / Message Type
- [ ] Debug Callback
- [ ] Debug / Release validation 策略
- [ ] 使用 vkconfig 辅助调试

**实践：**
故意制造一个 Validation 错误并读懂其消息。

---

### 03.0.3 — Physical Devices and Queue Families

- [ ] 枚举 Physical Device
- [ ] Device Properties
- [ ] Device Features
- [ ] Device Extensions
- [ ] Queue Family Properties
- [ ] Graphics / Compute / Transfer Queue
- [ ] GPU suitability / selection strategy

**实践：**
打印本机 GPU 与 Queue Family 能力。

---

### 03.0.4 — Logical Device and Queues

- [ ] Logical Device
- [ ] Queue Create Info
- [ ] Queue Priority
- [ ] Feature Enable
- [ ] Extension Enable
- [ ] 获取 Graphics Queue
- [ ] Device 与 PhysicalDevice 的职责区别

**实践：**
成功建立能够提交 GPU 工作的 Logical Device + Queue。

---

## 03.1 — Presentation

### 03.1.0 — Window Surface

- [ ] WSI
- [ ] Vulkan 的平台无关性
- [ ] SurfaceKHR
- [ ] Win32 Surface 与 GLFW abstraction
- [ ] Presentation Support
- [ ] Graphics Queue 与 Present Queue 的关系

**Core 候选：**
重新评估 `Window` 是否已经形成稳定职责。

---

### 03.1.1 — Swap Chain

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

### 03.1.2 — Image Views

- [ ] Image 与 ImageView 的区别
- [ ] View Type
- [ ] Format
- [ ] Component Mapping
- [ ] Subresource Range
- [ ] Aspect / Mip Level / Array Layer

**实践：**
为所有 Swapchain Image 建立 ImageView。

---

## 03.2 — Graphics Pipeline Basics

### 03.2.0 — Introduction

- [ ] Graphics Pipeline 全流程
- [ ] Programmable Stage
- [ ] Fixed Function Stage
- [ ] Pipeline State Object
- [ ] Vulkan 为什么倾向预先创建 Pipeline

---

### 03.2.1 — Shader Modules

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

### 03.2.2 — Fixed Functions

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

### 03.2.3 — Dynamic Rendering

- [ ] 传统 RenderPass / Framebuffer 模型
- [ ] Dynamic Rendering 的意义
- [ ] Pipeline Rendering Info
- [ ] Attachment Format
- [ ] `beginRendering` / `endRendering`

---

### 03.2.4 — Conclusion / Graphics Pipeline Creation

- [ ] Shader Stage + Fixed Function + Pipeline Layout
- [ ] Graphics Pipeline CreateInfo
- [ ] 创建 Graphics Pipeline
- [ ] Pipeline 生命周期与依赖关系

**Core 候选：**
只有在后续多个 Sandbox 出现真实重复后，再讨论 `Pipeline` 抽象。

---

## 03.3 — Drawing

### 03.3.0 — Dynamic Rendering

- [ ] Rendering Attachment
- [ ] Load / Store Op
- [ ] Clear Value
- [ ] Image Layout
- [ ] Rendering Area

---

### 03.3.1 — Command Buffers

- [ ] Command Pool
- [ ] Queue Family 与 Command Pool
- [ ] Primary / Secondary Command Buffer
- [ ] Begin / Record / End
- [ ] Bind Pipeline
- [ ] Draw
- [ ] Reset / Reuse

---

### 03.3.2 — Rendering and Presentation

- [ ] Acquire Swapchain Image
- [ ] Record Command Buffer
- [ ] Queue Submit
- [ ] Present
- [ ] Binary Semaphore
- [ ] Fence
- [ ] CPU / GPU / Presentation Engine 同步关系
- [ ] Image Layout Transition
- [ ] 第一张 Triangle

**Milestone：First Triangle**

---

### 03.3.3 — Frames in Flight

- [ ] 为什么需要 Frames in Flight
- [ ] Frame Index
- [ ] Swapchain Image Index
- [ ] Per-frame Command Buffer
- [ ] Per-frame Synchronization
- [ ] CPU / GPU overlap
- [ ] Resource lifetime across frames

---

## 03.4 — Swap Chain Recreation

- [ ] `VK_ERROR_OUT_OF_DATE_KHR`
- [ ] `VK_SUBOPTIMAL_KHR`
- [ ] Window Resize
- [ ] Minimized Window
- [ ] Framebuffer Size Callback
- [ ] Recreate Swapchain
- [ ] Swapchain-dependent object graph
- [ ] Device Idle 与更精细同步的差异

**架构任务：**
第一次系统梳理 Vulkan 对象销毁 / 重建依赖图。

---

# Phase 2 — GPU Resources

# 04 — Vertex Buffers

## 04.0 — Vertex Input Description

- [ ] Vertex Struct
- [ ] Binding Description
- [ ] Attribute Description
- [ ] Location
- [ ] Format
- [ ] Stride / Offset
- [ ] C++ memory layout ↔ Shader input

---

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

## 04.3 — Index Buffer

- [ ] Vertex Reuse
- [ ] Index Buffer
- [ ] Index Type
- [ ] Bind Index Buffer
- [ ] `drawIndexed`

**Milestone：GPU Geometry Buffer Rendering**

---

# 05 — Uniform Buffers

## 05.0 — Descriptor Set Layout and Buffer

- [ ] Uniform Buffer
- [ ] Model / View / Projection
- [ ] Descriptor 概念
- [ ] Descriptor Binding
- [ ] Descriptor Type
- [ ] Descriptor Set Layout
- [ ] Shader Resource Binding
- [ ] Per-frame Uniform Buffer

---

## 05.1 — Descriptor Pool and Sets

- [ ] Descriptor Pool
- [ ] Descriptor Set Allocation
- [ ] Descriptor Buffer Info
- [ ] Write Descriptor Set
- [ ] Update Descriptor Sets
- [ ] Bind Descriptor Set
- [ ] Descriptor 生命周期

**Milestone：可动态变换的 Geometry**

---

# 06 — Texture Mapping

## 06.0 — Images

- [ ] stb_image（当前教程阶段）
- [ ] Image Creation
- [ ] Image Memory
- [ ] Image Tiling
- [ ] Image Usage
- [ ] Image Layout
- [ ] Staging Buffer → Image
- [ ] Buffer-to-Image Copy
- [ ] Pipeline Barrier / Synchronization
- [ ] Layout Transition

---

## 06.1 — Image View and Sampler

- [ ] Texture Image View
- [ ] Sampler
- [ ] Filtering
- [ ] Address Mode
- [ ] Anisotropy
- [ ] Normalized Coordinates
- [ ] Sampler Limits

---

## 06.2 — Combined Image Sampler

- [ ] Combined Image Sampler Descriptor
- [ ] Descriptor Set Layout 扩展
- [ ] Image Info
- [ ] Shader Texture Sampling
- [ ] UV Coordinates

**Milestone：Textured Geometry**

---

# Phase 3 — 3D Rendering Foundations

# 07 — Depth Buffering

- [ ] 07.1 Introduction
- [ ] 07.2 3D Geometry
- [ ] 07.3 Vulkan Depth Range 与 `GLM_FORCE_DEPTH_ZERO_TO_ONE`
- [ ] 07.4 Depth Image and View
- [ ] 07.5 Depth Format Selection
- [ ] 07.6 Command Buffer — Clear Values
- [ ] 07.7 Command Buffer — Dynamic Rendering Depth Attachment
- [ ] 07.8 Explicit Depth Image Layout Transition
- [ ] 07.9 Depth and Stencil Pipeline State
- [ ] 07.10 Handling Window Resize / Recreate Depth Resource

**Milestone：正确的 3D Occlusion**

---

# 08 — Loading Models

- [ ] 08.1 Introduction
- [ ] 08.2 tinyobjloader
- [ ] 08.3 Sample Mesh / Asset Path
- [ ] 08.4 Loading Vertices and Indices
- [ ] 08.5 OBJ Attribute / Face / Index Model
- [ ] 08.6 Texture Coordinate Orientation
- [ ] 08.7 Vertex Deduplication
- [ ] 08.8 Hashable Vertex

**Core 候选：**
开始评估 `Mesh` / `Model` 的数据职责，但不急于引擎化。

---

# 09 — Generating Mipmaps

- [ ] 09.1 Introduction / Mipmap 与 LOD
- [ ] 09.2 Image Creation with Mip Levels
- [ ] 09.3 Image View covering Mip Chain
- [ ] 09.4 Generating Mipmaps with Blit
- [ ] 09.5 Per-Mip Image Layout Transition
- [ ] 09.6 Linear Filtering Support
- [ ] 09.7 Format Capability Check
- [ ] 09.8 Sampler LOD / mipmapMode / minLod / maxLod

---

# 10 — Multisampling

- [ ] 10.1 Introduction / Aliasing
- [ ] 10.2 Getting Available Sample Count
- [ ] 10.3 Multisampled Color Resource
- [ ] 10.4 Multisampled Depth Resource
- [ ] 10.5 Pipeline Multisample State
- [ ] 10.6 Dynamic Rendering Resolve Attachment
- [ ] 10.7 MSAA Resolve
- [ ] 10.8 Quality Improvements / Sample Shading

**Milestone：基础 Raster 3D Renderer**

---

# Phase 4 — GPU Programming

# 11 — Compute Shader

- [ ] 11.1 Introduction
- [ ] 11.2 Advantages of GPU Compute
- [ ] 11.3 Vulkan Graphics / Compute Pipeline Relationship
- [ ] 11.4 GPU Particle System Example
- [ ] 11.5 Data Manipulation
  - [ ] SSBO
  - [ ] Storage Image
- [ ] 11.6 Compute Queue Families
- [ ] 11.7 Compute Shader Stage
- [ ] 11.8 Loading Compute Shader
- [ ] 11.9 Preparing Shader Storage Buffers
- [ ] 11.10 Compute Descriptors
- [ ] 11.11 Compute Pipeline
- [ ] 11.12 Compute Space
  - Work Group
  - Invocation
  - Local Size
  - Hardware Limits
- [ ] 11.13 Compute Shader
- [ ] 11.14 Running Compute Commands
  - [ ] Dispatch
  - [ ] Submitting Work
  - [ ] Graphics / Compute Synchronization
  - [ ] Timeline Semaphores
- [ ] 11.15 Drawing the Particle System
- [ ] 11.16 Conclusion

**Milestone：Graphics + Compute 协同工作**

---

# Phase 5 — Vulkan 工程化与兼容性

# 12 — Ecosystem Utilities and GPU Compatibility

- [ ] 12.1 Introduction
- [ ] 12.2 Vulkan Hardware Database / GPUInfo
- [ ] 12.3 `vulkaninfo`
- [ ] 12.4 Vulkan Configurator / `vkconfig`
- [ ] 12.5 Validation Configuration
- [ ] 12.6 GPU Feature / Extension Capability Detection
- [ ] 12.7 Vulkan Version Compatibility
- [ ] 12.8 Dynamic Rendering Fallback 思路
- [ ] 12.9 Traditional RenderPass / Framebuffer Compatibility
- [ ] 12.10 Synchronization2 Compatibility
- [ ] 12.11 Feature-based Code Paths
- [ ] 12.12 Production Compatibility Strategy

---

# 13 — Vulkan Profiles

- [ ] 13.1 Introduction
- [ ] 13.2 Understanding Vulkan Profiles
- [ ] 13.3 What Are Vulkan Profiles
- [ ] 13.4 Profile Requirements
- [ ] 13.5 Hardware Profile Support Check
- [ ] 13.6 Creating Device with Profile
- [ ] 13.7 Replacing Manual Feature Detection
- [ ] 13.8 Profile + Fallback Strategy
- [ ] 13.9 Renderer Capability Contract

---

# 14 — Android — Optional

- [ ] 14.1 Android Vulkan Environment
- [ ] 14.2 Android Native Window / WSI
- [ ] 14.3 Lifecycle Differences
- [ ] 14.4 Mobile GPU Considerations
- [ ] 14.5 Desktop GLFW 与 Android 平台层差异

> Windows 主线不要求完成 Android 实践；需要跨平台时再开启独立 Sandbox / branch。

---

# 15 — Migrating to glTF and KTX2

- [ ] 15.1 Introduction
- [ ] 15.2 OBJ → glTF 的动机
- [ ] 15.3 tinygltf
- [ ] 15.4 glTF Buffer / BufferView / Accessor
- [ ] 15.5 Mesh / Primitive / Attribute
- [ ] 15.6 glTF Indices
- [ ] 15.7 glTF Materials / Textures 基础
- [ ] 15.8 PNG / JPEG → KTX2 的动机
- [ ] 15.9 KTX2 Loading
- [ ] 15.10 KTX2 GPU Format / Mipmap / Layer
- [ ] 15.11 Cubemap / Texture Array 基础
- [ ] 15.12 Converting OBJ to glTF
- [ ] 15.13 Creating KTX2
- [ ] 15.14 Compression Options
- [ ] 15.15 Mipmap Generation
- [ ] 15.16 KTX2 Metadata
- [ ] 15.17 KTX2 Toolchain

**架构任务：**
重新设计 Assets 读取路径，为现代 Asset Pipeline 做准备。

---

# 16 — Rendering Multiple Objects

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

# 17 — Multithreading

- [ ] 17.1 Introduction
- [ ] 17.2 Vulkan Multithreading Model
- [ ] 17.3 Externally Synchronized Objects
- [ ] 17.4 Thread-safe / Non-thread-safe Vulkan Operations
- [ ] 17.5 Worker Threads
- [ ] 17.6 Per-thread Command Pools
- [ ] 17.7 Parallel Command Buffer Recording
- [ ] 17.8 Thread Resource Management
- [ ] 17.9 Queue Submission Synchronization
- [ ] 17.10 CPU Synchronization
  - Mutex
  - Condition Variable
  - Atomics
- [ ] 17.11 Asynchronous Resource Loading
- [ ] 17.12 Performance Considerations
  - Thread creation overhead
  - Work granularity
  - False sharing
  - Contention
- [ ] 17.13 Profiling & 判断并行是否真的有收益

**架构任务：**
到此阶段才讨论 Render Thread / Worker / Job System 等现代 Renderer 并行架构。

---

# Phase 6 — Advanced

# 18 — Ray Tracing — Advanced

当前官方入口指向 SIGGRAPH Hands-on Vulkan Ray Tracing Course。

- [ ] 18.1 Ray Tracing Overview
- [ ] 18.2 Dynamic Rendering Preparation
- [ ] 18.3 Acceleration Structure 概念
- [ ] 18.4 BLAS
- [ ] 18.5 TLAS
- [ ] 18.6 Ray Query
- [ ] 18.7 Ray-traced Shadows
- [ ] 18.8 TLAS Animation / Update
- [ ] 18.9 Shadow Transparency
- [ ] 18.10 Reflections
- [ ] 18.11 Raster + Ray Tracing Hybrid Renderer

---

# Phase 7 — Building VKRenderer

官方高级路线：
`Building a Simple Engine`

> 这一阶段不再以“复制教程代码”为目标，而是把此前所有 Sandbox 中验证过的知识收束成真正的 VKRenderer Core。

- [ ] E00 — Engine Architecture
  - Renderer responsibilities
  - Platform / Window layer
  - GPU context
  - resource ownership
  - frame lifecycle
- [ ] E01 — Camera & Transformations
- [ ] E02 — Loading Models
- [ ] E03 — Lighting & Materials
- [ ] E04 — GUI
- [ ] E05 — Subsystems
- [ ] E06 — Tooling
- [ ] E07 — Advanced Topics
- [ ] E08 — Performance / Profiling
- [ ] E09 — Resource Lifetime & Deferred Destruction
- [ ] E10 — Modern Render Architecture Review

最终 Core 的目录由实际实现自然演化，不提前锁死。

可能出现但不预设的组件包括：

```text
Core/
├─ Platform/
├─ Renderer/
├─ Resource/
├─ Scene/
└─ Asset/
```

---

# 3. 关键里程碑

| Milestone | 目标 | 状态 |
|---|---|---|
| M0 | 开发环境 / CMake / ThirdParty / SDK 基线 | [x] |
| M1 | First Triangle | [ ] |
| M2 | Vertex + Index Buffer | [ ] |
| M3 | Uniform / Descriptor / Transform | [ ] |
| M4 | Texture Mapping | [ ] |
| M5 | Depth + Model + Mipmap + MSAA | [ ] |
| M6 | Compute Shader | [ ] |
| M7 | GPU Compatibility / Profiles | [ ] |
| M8 | glTF + KTX2 | [ ] |
| M9 | Multiple Objects | [ ] |
| M10 | Multithreaded Vulkan | [ ] |
| M11 | Ray Tracing | [ ] |
| M12 | VKRenderer Core Architecture | [ ] |

---

# 4. 每次仓库 Review 的固定检查项

每完成一小节，Review 至少检查：

- [ ] 功能结果与官方章节目标一致
- [ ] Vulkan Validation 无未解释错误
- [ ] Vulkan 对象创建和销毁依赖正确
- [ ] RAII 生命周期正确
- [ ] 没有悬空引用 / handle
- [ ] Move / copy 语义合理
- [ ] Exception 不被错误吞掉
- [ ] CMake target dependency 正确
- [ ] Debug / Release 行为合理
- [ ] 不硬编码本机绝对路径
- [ ] Shader 构建依赖正确
- [ ] Assets path 不依赖脆弱的 `../../../`
- [ ] Sandbox 之间无依赖
- [ ] Core 没有过度抽象
- [ ] 没有不必要的 `device.waitIdle()`
- [ ] 同步逻辑能够明确解释 stage / access / semaphore / fence
- [ ] 代码命名遵守 VKRenderer 约定
- [ ] README / 本学习总纲在需要时同步更新

---

# 5. Core 提升准则

一段代码进入 Core 前至少满足以下条件：

1. 已在一个或多个 Sandbox 中实际使用。
2. Responsibility 可以用一句话解释。
3. 输入、输出和所有权关系稳定。
4. Vulkan 对象依赖关系明确。
5. 抽象不会隐藏当前正在学习的关键 Vulkan 概念。
6. 抽出来能减少真实重复，而不是为了“看起来像引擎”。
7. 旧 Sandbox 在重构后仍能正常编译运行。

如果不能满足，则继续保留在 Sandbox。

---

# 6. 当前进度

## 学习阶段

Phase 1 — Drawing a Triangle

```text
00 Introduction             [COMPLETED]
01 Overview                 [COMPLETED]
02 Development Environment  [COMPLETED]
03 Drawing a Triangle       [IN PROGRESS]
  03.0.0 Base Code          [COMPLETED]
  03.0.1 Instance           ← NEXT
```

## 已完成的理论学习

- 00 Introduction：已完成（2026-10-09）。
- 01 Overview：已完成（2026-11-10）。
  - 01.1 Origin of Vulkan
  - 01.2 Coding Conventions
  - 01.3 What It Takes to Draw a Triangle（8/8）
  - 01.4 Summary
  - 01.5 API Concepts
  - 01.6 Validation Layers

上述内容已通过理论验收。

## 当前工程状态

截至 2026-11-10：

- 默认分支：`main`
- 最近通过 Code Review 的 Commit：`7fc7bbcae08539fd49bcb44d841d3a4598d93315`
- Root CMake 已配置 C++20、MSVC 选项及基础子目录。
- ThirdParty 已包含 GLFW 3.4 和 GLM。
- Core 暂无正式复用代码。
- Sandbox 已建立 EnvironmentCheck 构建目标。
- Vulkan SDK / Loader 1.4.357 已验证。
- Slang 2026.13.1 已验证。
- NVIDIA RTX 4080 Laptop GPU 已确认支持 Vulkan 1.4。
- Debug / Release 编译运行成功。
- 初始 Debug 链接警告 LNK4098 已消除。
- M0 本地环境验证与 GitHub Code Review 均已通过。
- 02 Development Environment 已完成最终验收。
- EnvironmentCheck 作为独立环境测试 Sandbox 保留。
- 当前未发现阻断下一章节的 P0 / P1 问题。
- 尚未开始正式 Vulkan 渲染 Sandbox。

## 下一阶段任务

**03 Drawing a Triangle — M1**

当前学习目标：

`03_Drawing_a_triangle/00_Setup/00_Base_code`

官方教学顺序：

1. Base Code
2. Instance
3. Validation Layers
4. Physical Devices and Queue Families
5. Logical Device and Queues

**03.0.0 Base Code 学习任务：**

- 理解 Vulkan Application 的基础组织结构。
- 理解 GLFW 初始化、窗口创建和事件循环。
- 理解 `GLFW_NO_API` 的作用。
- 理解 Vulkan-Hpp / RAII 的基础使用方式。
- 明确初始化、主循环和清理阶段的职责。
- 讨论当前 Sandbox 的最小架构设计。
- 在现有 VKRenderer 工程基础上逐步实现。
- 完成编译、运行与必要的代码审查。

**工程约束：**

- 继续使用现有 Windows / MSVC / CMake / C++20 工具链。
- 保留 M0 EnvironmentCheck，作为独立环境基线。
- 新 Vulkan 实验放入 Sandbox。
- 不提前将代码提升到 Core。
- 不引入复杂继承体系或不必要的 Manager。
- 不提前实现后续小节的 Instance、Swapchain 或 Graphics Pipeline。
- 每个实践小节完成后按约定进行验收。

**下一里程碑：M1 — First Triangle**

当前状态：尚未开始。

在完成 03 Drawing a Triangle 的相关小节并通过实践验收后，标记 M1 完成。

## 学习与 Git 约定

- 简单理论章节可在同一会话中连续推进。
- 理论小节通过验收后即可继续下一小节，不要求立即提交 Git。
- 理论大章节结束时，统一整理学习记录并提交 Git。
- 复杂实践章节根据自然的知识单元与工程里程碑拆分会话。
- 实践代码需要编译、运行、必要的 Validation 和 GitHub Review。
- 新会话优先核查当前 Roadmap、最新 Commit 和工程状态。
- 不将理论验收等同于编译或运行验收。
- 不因环境配置细节而过度设计架构。

---

## 官方参考

- Khronos Vulkan Tutorial: https://docs.vulkan.org/tutorial/latest/
- Vulkan Specification: https://docs.vulkan.org/spec/latest/
- Vulkan-Hpp: https://github.com/KhronosGroup/Vulkan-Hpp
- Slang: https://shader-slang.org/
- Vulkan SDK: https://vulkan.lunarg.com/

---

> 课程总纲会随着 Khronos 官方教程和 VKRenderer 架构演进更新。  
> 进度以此文档中的 checkbox 和仓库中可运行的 Sandbox 为准。

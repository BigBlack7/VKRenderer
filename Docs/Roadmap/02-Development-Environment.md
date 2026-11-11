# 02 — Development Environment

> Phase 0 · 世界观与开发环境 · [总目录](../VulkanLearningRoadmap.md)
> 上一章：[01 Overview](01-Overview.md) ｜ 下一章：[03 Drawing a Triangle](03-Drawing-a-Triangle.md)
> 维护约定：本文件保存章节任务、实践与验收记录；总体进度只在[学习地图](../VulkanLearningRoadmap.md#current-progress)维护。

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

[返回学习地图](../VulkanLearningRoadmap.md)

# 03.0 — Setup

> Phase 1 · [第 03 章目录](03-Drawing-a-Triangle.md) · [学习地图](../VulkanLearningRoadmap.md)

> 下一单元：[03.1 Presentation](03-1-Presentation.md)

<a id="lesson-03-0-0"></a>

## 03.0.0 — Base Code

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

<a id="lesson-03-0-1"></a>

## 03.0.1 — Instance

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

<a id="lesson-03-0-2"></a>

## 03.0.2 — Validation Layers

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

<a id="lesson-03-0-3"></a>

## 03.0.3 — Physical Devices and Queue Families

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

<a id="lesson-03-0-4"></a>

## 03.0.4 — Logical Device and Queues

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

[返回第 03 章](03-Drawing-a-Triangle.md) · [返回学习地图](../VulkanLearningRoadmap.md)

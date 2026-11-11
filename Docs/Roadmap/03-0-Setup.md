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

- [x] Vulkan Loader → Context → Instance
  - 理解 Vulkan Loader 的加载与命令分发职责
  - 理解 Vulkan Instance 的顶层对象定位
  - 区分 Vulkan C API Handle 与 Vulkan-Hpp RAII 对象
  - 理解 `vk::raii::Context` 与 `vk::raii::Instance` 的区别
- [x] `vk::ApplicationInfo`
  - 应用程序名称与版本
  - Engine 名称与版本
  - Vulkan API Version
  - C++20 指定初始化器
- [x] `vk::InstanceCreateInfo`
  - `pApplicationInfo`
  - Instance Layer 数量与名称
  - Instance Extension 数量与名称
  - CreateInfo 输入数据的生命周期
- [x] Vulkan API Version
  - 使用 Vulkan 1.4 作为当前课程 API 基线
  - 区分 Application Version 与 Vulkan API Version
  - 理解 API Version 不等于所有 Device Features 自动可用
- [x] Instance Extensions
  - 理解 Instance Extension 的作用
  - 使用 `vk::raii::Context` 枚举可用扩展
  - 理解支持扩展与启用扩展的区别
  - 使用 `std::ranges::any_of` 检查扩展支持情况
- [x] GLFW required extensions
  - 使用 `glfwGetRequiredInstanceExtensions()`
  - 检查返回指针和扩展数量
  - 正确遍历 GLFW 要求的全部 Instance Extensions
  - 缺失扩展时抛出明确异常
  - 理解 GLFW 返回的扩展名称数组所有权与生命周期
- [x] Instance 生命周期
  - 创建 `vk::raii::Context`
  - 创建 `vk::raii::Instance`
  - 使用 RAII 管理 Vulkan Instance Handle
  - 保证 Context 先构造、后析构
  - 理解临时 CreateInfo 数据与 Instance 对象的生命周期区别
  - 理解异常传播与 Application 清理边界

**实践：**

在现有 `HelloTriangle` Sandbox 中创建第一个 Vulkan Instance。

已完成：

- 在 `HelloTriangleApplication` 中增加 `vk::raii::Context`。
- 增加初始为空的 `vk::raii::Instance` 成员。
- 保留 `initWindow()` → `initVulkan()` → `mainLoop()` 的执行流程。
- 在 `initVulkan()` 中调用 `createInstance()`。
- 配置 `vk::ApplicationInfo`，使用 Vulkan 1.4 API 基线。
- 从 GLFW 获取当前平台需要的 Instance Extensions。
- 枚举并逐一检查必需扩展是否受支持。
- 使用 `vk::InstanceCreateInfo` 配置创建参数。
- 成功创建 `vk::raii::Instance`。
- 使用现有异常处理机制报告创建失败。
- 保留原有 GLFW 窗口与事件循环。
- 未提前实现 Validation Layers、Physical Device、Logical Device 或 Surface。

**工程设计：**

- 继续使用 `Sandbox/HelloTriangle/Main.cpp` 作为当前实现入口。
- 当前无需拆分 Application 类。
- `vk::raii::Context` 和 `vk::raii::Instance` 直接由 Application 持有。
- Context 声明在 Instance 之前，确保析构顺序正确。
- Instance 使用 RAII 自动释放，不手动调用 `vkDestroyInstance`。
- `createInstance()` 中使用局部 `ApplicationInfo` 和 `InstanceCreateInfo`。
- GLFW 必需扩展名称数组由 GLFW 管理，不手动释放。
- 保留现有 GLFW 生命周期管理方式。
- 暂不将 Context、Instance 或 Window 抽象提升至 Core。
- 不新增独立 VulkanContext、Renderer 或 Manager 类。
- 暂不创建 Vulkan Surface；后续创建 Surface 时重新审查窗口与 Vulkan 对象的销毁顺序。

**GitHub Code Review：**

首次实现 Commit：

`832810c00ba809a9c4ebf39423c44f3a9cfdd5da`

修复 Commit：

`80e4a23f09b36535ae51bb32b1cd93dc84d4f7b7`

第一轮审查发现：

1. P0：GLFW 扩展数组越界。
   - 错误使用 `glfwExtensions[glfwExtensionCount]`。
   - 合法索引范围为 `[0, glfwExtensionCount)`。
   - 越界访问存在 C++ Undefined Behavior 风险。
   - 修复：使用 `i < glfwExtensionCount` 遍历每个必需扩展。

2. P1：扩展支持性检查结果被忽略。
   - `std::ranges::any_of()` 的返回值未被使用。
   - MSVC 报告 `warning C4834`。
   - 原实现无法正确判断是否缺少必需扩展。
   - 修复：逐个检查扩展，保存 `supported` 结果。
   - 不支持扩展时抛出包含扩展名称的异常。

3. P2：Application 构造发生在 `main()` 的异常捕获范围之外。
   - 修复：将 Application 对象创建移动到 `try` 内。
   - 确保 RAII Context 构造异常能够进入现有错误处理路径。

4. P2：`createInstance()` 包含重复的异常捕获。
   - 原捕获仅打印通用失败信息并重新抛出。
   - 修复：删除重复捕获，统一由 `main()` 报告异常。

第二轮审查结果：

- 数组越界已修复。
- 扩展检查逻辑已修复。
- `C4834` 对应的返回值忽略问题已解决。
- Context 与 Instance 的所有权及声明顺序正确。
- `ApplicationInfo` 与 `InstanceCreateInfo` 配置正确。
- 异常边界更加合理。
- 未发现新的 P0 / P1 问题。
- GitHub 静态代码审查通过。

**编译与运行验证（2026-11-11）：**

开发环境：

- Windows x64
- Visual Studio 2022
- MSVC 19.44.35228.0
- C++20
- CMake
- Vulkan SDK / Loader 1.4.357
- GLFW 3.4

验证结果：

- Debug Clean Build 成功。
- Release Clean Build 成功。
- Debug 程序正常运行和退出。
- Release 程序正常运行和退出。
- Debug `$LASTEXITCODE = 0`。
- Release `$LASTEXITCODE = 0`。
- `warning C4834` 已解决。
- 修复过程中出现的 `warning C4819` 已通过本地 CMake 编译配置调整解决。
- 最终 Debug / Release 构建日志无上述编译警告。
- Vulkan Instance 创建流程已由代码审查及正常运行路径确认。
- 本节不要求提前启用 Vulkan Validation Layers。
- 本节尚未提交 GPU 绘制命令。

说明：

- C4819 修复已完成本地构建验证。
- 最终 CMake 配置改动将在本节文档更新后统一提交 GitHub。
- 完成统一提交后，对最终 Commit 进行一次仓库复核。

**原理理解验收（2026-11-11）：**

四道理解题全部通过。

已理解：

1. Vulkan Loader、RAII Context 与 Instance 的职责区别。
2. Application Version 与 Vulkan API Version 的区别。
3. GLFW 必需 Instance Extensions 的来源与检查目的。
4. Context、Instance 和局部创建信息的生命周期关系。

**最终验收结论（2026-11-11）：**

- 原理理解通过。
- Instance 创建实现通过。
- GLFW 必需扩展检查通过。
- Vulkan-Hpp RAII 生命周期检查通过。
- Debug / Release 本地运行验证通过。
- 已修复全部 P0 / P1 问题。
- 无遗留阻断问题。
- C4819 已在本地修复，等待最终 GitHub Commit 复核。
- 无需进行额外架构重构。
- 03.0.1 Instance 通过最终验收。

**Core 判断：**

当前只存在一个正式 Vulkan 渲染 Sandbox，尚未出现经过验证的跨 Sandbox 复用需求。

继续保留最小 Application 结构。

暂不抽象 VulkanContext、Window、Renderer 或其他 Vulkan 对象管理模块。

后续随着 Device、Surface、Swapchain 等对象加入，再根据真实依赖和生命周期重新评估架构。

**下一小节：**

03.0.2 — Validation Layers

下一节重点学习 Instance Layer、`VK_LAYER_KHRONOS_validation`、Debug Utils Extension 和 Debug Messenger，并建立正式的 Vulkan 诊断机制。

在下一节验收之前，不将 M1 First Triangle 标记为完成。

**小节状态：已完成（理论、实现、编译运行与 GitHub Code Review 通过）。**

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

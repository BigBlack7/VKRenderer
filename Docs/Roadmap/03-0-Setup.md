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

- [x] Vulkan Layer 模型
  - 理解 Vulkan Loader、Layer 与 ICD 的调用关系
  - 理解 Vulkan Valid Usage Conditions
  - 区分 Vulkan 规范检查与 C++ 内存安全检查
  - 理解 Validation Layer 的能力边界
- [x] `VK_LAYER_KHRONOS_validation`
  - 使用 `vk::raii::Context` 枚举 Instance Layers
  - 检查所需 Validation Layer 是否可用
  - 根据 Debug / Release 配置决定是否启用
  - 使用 `vk::InstanceCreateInfo` 配置 Instance Layers
- [x] Debug Utils Extension
  - 理解 `VK_EXT_debug_utils` 的作用
  - 区分 Instance Layer 与 Instance Extension
  - 在 Debug 构建中添加 Debug Utils Extension
  - 合并 GLFW 与 Debug Utils 所需扩展
  - 验证全部所需 Instance Extensions 是否受支持
- [x] Debug Messenger
  - 使用 `vk::DebugUtilsMessengerCreateInfoEXT`
  - 配置 Severity 与 Message Type 过滤条件
  - 创建 `vk::raii::DebugUtilsMessengerEXT`
  - 理解 Messenger 与 Instance 的依赖关系
  - 保证 Messenger 先于 Instance 析构
- [x] Severity / Message Type
  - 区分消息严重程度与消息类别
  - 理解 Verbose、Info、Warning、Error
  - 理解 General、Validation、Performance
  - 理解 Severity / Message Type 组合过滤机制
- [x] Debug Callback
  - 实现符合 Vulkan 调用约定的静态回调函数
  - 读取并输出诊断消息
  - 正常返回 `VK_FALSE`
  - 理解回调不能调用 Vulkan 命令的约束
  - 理解回调可能在 Vulkan API 调用路径中被调用
  - 区分应用主动提交的诊断消息与 Validation Layer 生成的消息
- [x] Debug / Release Validation 策略
  - 使用 `NDEBUG` 控制显式 Validation 配置
  - Debug：启用 Validation Layer 与 Debug Messenger
  - Release：不显式启用 Validation Layer，不创建 Debug Messenger
  - 验证 Debug / Release 构建和运行行为
- [x] Vulkan Validation 诊断实验
  - 验证 Debug Messenger 回调通道
  - 通过有效 API 调用提交自定义测试消息
  - 通过受控无效参数触发真实 Validation Error
  - 阅读 Severity、Message Type 与 VUID
  - 根据 VUID 查询 Vulkan Specification
  - 删除临时实验代码并恢复正常程序
- [x] vkconfig 辅助调试
  - 理解 vkconfig 的定位与用途
  - 当前阶段不强制使用 vkconfig
  - GPU-Assisted Validation 等高级功能留待实际需要时学习

**实践：**

在现有 `HelloTriangle` 中建立 Vulkan Validation 诊断机制。

已完成：

- 创建并管理 `vk::raii::DebugUtilsMessengerEXT`。
- 添加 Debug / Release Validation 开关。
- 在 Debug 构建中启用 `VK_LAYER_KHRONOS_validation`。
- 在 Debug 构建中启用 `VK_EXT_debug_utils`。
- 验证所需 Instance Layers 与 Extensions 是否受支持。
- 将扩展收集与扩展检查职责分离。
- 实现静态 Debug Callback。
- 输出消息类型、严重程度与消息内容。
- 使用 RAII 保证 Messenger 正确销毁。
- 通过自定义消息验证 Callback 通道。
- 通过真实 VUID 实验验证 Validation Layer 的诊断能力。
- 完成实验后移除临时测试代码。

**工程设计：**

- 继续保留 `HelloTriangleApplication` 结构。
- 保留 `createInstance()` 作为 Instance 创建入口。
- 使用 `getRequiredInstanceExtensions()` 收集所需扩展。
- 在 `createInstance()` 中统一检查最终扩展列表。
- 使用 `setupDebugMessenger()` 配置诊断回调。
- `initVulkan()` 按顺序调用 `createInstance()` 与 `setupDebugMessenger()`。
- Context、Instance 与 Debug Messenger 直接由 Application 持有。
- 成员声明顺序为 Context → Instance → Debug Messenger。
- 析构顺序为 Debug Messenger → Instance → Context。
- 不手动销毁 RAII Vulkan 对象。
- 不提前引入 VulkanContext、Renderer 或 DebugManager。
- 不提前实现 Physical Device、Logical Device、Surface 或 Swapchain。

**GitHub Code Review：**

首次实现 Commit：

`eeb5141c09e51956b1d0ab275f1bf7d476c34905`

修复与第二轮审查 Commit：

`47c86e00abdbff4b651355c4174ca344683ef27c`

第一轮审查发现：

1. P1：Release 构建仍然显式启用了 Validation Layer。
   - 原实现无条件传入 Validation Layer 列表。
   - Debug / Release 开关未正确作用于 `enabledLayerCount`。
   - 修复：根据 `kEnableValidationLayers` 构造实际需要启用的 Layer 列表。
   - Debug 显式启用 Validation，Release 不显式启用 Validation。

2. P2：新增 Debug Utils Extension 后未检查其支持情况。
   - 原实现只检查 GLFW 返回的扩展。
   - 修复：统一检查完整的 Instance Extension 列表。

3. P2：对 `extensions.data()` 使用了不必要的 `std::move()`。
   - `data()` 返回裸指针，不涉及容器所有权转移。
   - 修复：直接使用 `extensions.data()`。

第二轮审查结果：

- P1 已修复。
- 两项 P2 已修复。
- Debug / Release 配置逻辑正确。
- 所需扩展检查覆盖 GLFW 和 Debug Utils。
- Debug Callback 的签名、过滤条件与返回值正确。
- Debug Messenger 的 RAII 生命周期正确。
- 未发现新的 P0 / P1 问题。
- GitHub 静态代码审查通过。

**编译与运行验证（2026-11-12）：**

- Debug Clean Build 成功。
- Release Clean Build 成功。
- Debug 程序正常运行。
- Release 程序正常运行。
- Debug / Release 退出码均为 0。
- 编译日志未出现新的 MSVC 警告。
- Debug 能够接收自定义测试消息。
- Release 按应用配置不创建 Debug Messenger。

**Debug Messenger 自测：**

使用 `submitDebugUtilsMessageEXT()` 主动提交合法测试消息。

实际输出：

```text
Type: { General }, Severity: Warning, Message: Debug Messenger callback test
```

验证结果：

- Debug Utils 消息通道工作正常。
- Debug Messenger 的消息过滤条件正确。
- Debug Callback 能够读取并输出消息。
- 该测试消息由应用程序主动提交。
- 此实验不能单独证明 Validation Layer 检测到了规范违规。

**真实 Validation VUID 实验（2026-11-13）：**

实验 API：

`vkSubmitDebugUtilsMessageEXT`

故意传入的无效参数：

`messageSeverity = 0`

对应 Vulkan Valid Usage ID：

`VUID-vkSubmitDebugUtilsMessageEXT-messageSeverity-parameter`

实际输出：

```text
Type: { Validation }, Severity: Error, Message: vkSubmitDebugUtilsMessageEXT(): messageSeverity is zero.
The Vulkan spec states: messageSeverity must be a valid VkDebugUtilsMessageSeverityFlagBitsEXT value
```

规范位置：

https://docs.vulkan.org/spec/latest/chapters/debugging.html#VUID-vkSubmitDebugUtilsMessageEXT-messageSeverity-parameter

实验结果：

- Debug 构建成功。
- Validation Layer 检测到无效 Severity 参数。
- Debug Messenger 正确转发诊断消息。
- Debug Callback 输出 Severity、Type 与具体错误。
- 诊断内容与 Vulkan Specification 中对应 VUID 一致。
- 实验进程退出码为 0。
- 理解 Validation Error 不必然使应用程序立即退出。
- 理解自定义消息与真实 Validation 诊断的本质区别。
- 实验完成后，开发者已确认删除临时无效调用代码。
- 开发者已确认恢复后的正常程序运行无异常。

说明：

- 真实 VUID 实验在本地完成，不保留故意违反 Vulkan 规范的调用。
- GitHub Commit `47c86e00` 包含阶段性的合法消息自测代码。
- 清理后的正式代码将与本次学习文档统一提交，随后复核。
- 本节的运行证据由开发者提供，GitHub 审查为静态代码审查。

**原理理解验收（2026-11-13）：**

四道原理题及补充理解检查全部通过。

已理解：

1. Validation Layer、Debug Utils Extension 与 Debug Messenger 的职责区别。
2. Severity 与 Message Type 的不同含义。
3. Debug Callback 的调用约定和 `VK_FALSE` 返回值要求。
4. Debug Messenger 与 Instance 的生命周期依赖。
5. 普通 Messenger 无法保证捕获其注册前的 Instance 创建诊断。
6. 可通过 `VkInstanceCreateInfo::pNext` 连接 Debug Messenger 创建信息。
7. 合法自定义消息不能证明 Validation Layer 检测到了规范错误。
8. VUID 是 Vulkan 有效使用条件的标识符。
9. Validation Layer 报告的是应用程序违反规范的行为，而不是验证层自身出错。
10. 没有 Validation Error 不等于 Vulkan 程序完全正确。

**最终验收结论（2026-11-13）：**

- 原理理解通过。
- Validation Layer 配置通过。
- Debug / Release 策略通过。
- Debug Utils Extension 检查通过。
- Debug Messenger 创建与 RAII 生命周期审查通过。
- Debug Callback 功能验证通过。
- 真实 Validation VUID 实验通过。
- P0 / P1 问题已全部解决。
- 不需要新增 Core 抽象。
- 本地测试代码已由开发者确认清理。
- 正式清理版本等待统一 GitHub Commit 复核。

**小节状态：已完成（理论、实现、Debug Messenger 自测、真实 VUID 实验及阶段性 GitHub Code Review 通过）。**

**下一小节：**

03.0.3 — Physical Devices and Queue Families

在正式进入下一小节前，先完成本节最终代码与文档的统一提交和复核。

---

<a id="lesson-03-0-3"></a>

## 03.0.3 — Physical Devices and Queue Families

- [x] 枚举 Physical Device
  - 使用 `vk::raii::Instance::enumeratePhysicalDevices()`
  - 检查设备集合是否为空
  - 理解 Physical Device 与 Instance 的生命周期关系
- [x] Device Properties
  - 查询设备名称、类型与 Vulkan API Version
  - 使用 Vulkan 1.4 作为当前项目的最低 API 基线
  - 区分 SDK 版本与 Physical Device 支持的版本
- [x] Device Features
  - 理解支持 Feature 与启用 Feature 的区别
  - 使用 `getFeatures2()` 和 Vulkan-Hpp StructureChain 查询特性
  - 检查 `shaderDrawParameters`
  - 检查 `dynamicRendering`
  - 检查 `extendedDynamicState`
- [x] Device Extensions
  - 枚举 Device Extensions
  - 检查 `VK_KHR_swapchain`
  - 区分 Instance Extensions 与 Device Extensions
- [x] Queue Family Properties
  - 查询 Queue Family 数量
  - 输出 Queue Family Index、Queue Count 和 Queue Flags
  - 理解 Queue Family 与 Queue 的关系
- [x] Graphics / Compute / Transfer Queue
  - 理解 Graphics、Compute、Transfer 队列能力
  - 使用位标志判断队列族能力
  - 确认当前最低需求是 Graphics Queue Family
  - 理解 Graphics Queue 不一定支持指定 Surface 的 Present
  - 理解 Graphics / Compute 队列对传输操作的隐式支持规则
- [x] GPU Suitability / Selection Strategy
  - 检查 API Version
  - 检查 Graphics Queue Family
  - 检查 Device Extensions
  - 检查必要的 Device Features
  - 选中第一个满足全部必要条件的设备
  - 使用 `std::ranges::find_if`
  - 使用移动语义保存 `vk::raii::PhysicalDevice`

**实践：**

在 `HelloTriangle` 中完成 Physical Device 枚举、能力查询与选择。

实现内容：

- 新增 `vk::raii::PhysicalDevice` 成员。
- 新增 `pickPhysicalDevice()`。
- 新增 `isDeviceSuitable()`。
- 枚举全部可用 Physical Devices。
- 输出 GPU 名称、类型和 API Version。
- 输出 Queue Family 信息。
- 检查设备扩展与特性。
- 选择满足需求的 Physical Device。
- 使用 Vulkan-Hpp RAII 管理对象生命周期。
- 未提前创建 Logical Device 或实际 Queue。

**本机设备查询结果：**

| Physical Device | Vulkan API Version | 本次选择结果 |
|---|---|---|
| Intel RaptorLake-S Mobile Graphics Controller | 1.3.295 | 不满足 Vulkan 1.4 基线 |
| Microsoft Direct3D12 (Intel UHD Graphics) | 1.2.359 | 不满足 Vulkan 1.4 基线 |
| NVIDIA GeForce RTX 4080 Laptop GPU | 1.4.357 | 满足全部必要条件 |

最终选中：

`NVIDIA GeForce RTX 4080 Laptop GPU`

RTX 4080 共枚举出 6 个 Queue Families。

其中 Queue Family 0：

- Queue Count：16
- Graphics：支持
- Compute：支持
- Transfer：支持
- Sparse Binding：支持

注意：

- 当前尚未查询 Surface Present Support。
- 当前只保存 Physical Device，不创建 Logical Device。
- 当前设备选择采用第一个满足全部必要条件的候选设备。
- Vulkan 1.4 是本项目当前约定的技术基线；官方教程对应代码的最低版本条件为 Vulkan 1.3。

**GitHub Code Review：**

首次实现 Commit：

`8ed6974d8b2aa2075c3aa0d67aba192e8a99889d`

修复 Commit：

`0e6720684ad983e9532bc3638a9273d6be322b55`

第一轮审查发现：

1. P1：设备选择条件对 Queue Family 要求过严。
   - 错误地将 Graphics、Compute 和显式 Transfer 标志同时作为必要条件。
   - 当前渲染需求只要求支持 Graphics 的队列族。
   - Graphics 或 Compute 队列族可以隐式支持传输操作。
   - 修复：仅检查必要的 Graphics Queue Family。

2. P2：`const` 容器妨碍 RAII Physical Device 的正常移动语义。
   - 原代码将枚举结果声明为 `const std::vector`。
   - `std::move(*it)` 因而无法得到可正常移动的非 const 对象。
   - 修复：移除容器的 `const`，使用移动赋值保存选中对象。

第二轮审查结果：

- P1 已修复。
- P2 已修复。
- Device Suitability 条件正确。
- Physical Device 枚举与能力查询正确。
- Vulkan-Hpp RAII 生命周期正确。
- 未发现新的 P0 / P1 问题。
- GitHub 静态代码审查通过。

**编译与运行验证（2026-11-13）：**

- Debug Clean Build 成功。
- Release Clean Build 成功。
- 无新增编译警告。
- Debug 程序正常运行，退出码为 0。
- Release 程序正常运行，退出码为 0。
- 正确枚举多个 Vulkan Physical Devices。
- 正确排除低于项目 Vulkan 1.4 基线的设备。
- 正确选中 NVIDIA GeForce RTX 4080 Laptop GPU。
- 验证日志由开发者提供。
- GitHub 审查为静态代码审查，不等同于重新在本机运行。

**原理理解验收：**

四道原理题全部通过。

已理解：

1. Physical Device 与 Logical Device 的区别。
2. Physical Device 不能独立销毁，依赖所属 Instance。
3. Properties、Features、Extensions 的不同职责。
4. Feature 支持与启用的区别。
5. Queue Family 与 Queue 的关系。
6. Graphics、Compute、Transfer 能力的区别。
7. Graphics Queue 不保证支持指定 Surface 的 Present。
8. GPU 选择必须先满足必要条件，不能只按设备类型或性能判断。

**工程设计：**

- 继续使用现有 `HelloTriangleApplication`。
- Physical Device 直接由 Application 保存。
- 不新增 DeviceManager 或通用 GPU Selection Framework。
- 不提前缓存不必要的设备状态。
- 下一节创建 Logical Device 时再处理 Queue Family Index 和 Queue 创建参数。
- 不提前实现 Surface 或 Swapchain。

**最终验收结论（2026-11-13）：**

- 原理理解通过。
- Physical Device 枚举通过。
- Device Properties 查询通过。
- Device Features 查询通过。
- Device Extensions 查询通过。
- Queue Family 查询通过。
- GPU Suitability 与 Selection 通过。
- Debug / Release 本地验证通过。
- GitHub Code Review 通过。
- P0 / P1 已全部解决。
- 本节无需额外架构重构。

**小节状态：已完成（理论、实现、编译运行及 GitHub Code Review 通过）。**

**下一小节：**

03.0.4 — Logical Device and Queues

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

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

<<a id="lesson-03-2-1"></a>

## 03.2.1 — Shader Modules

- [x] Vertex Shader
  - 理解顶点着色器的职责与执行模型
  - 理解 Model Space、World Space、View Space、Clip Space、NDC 和 Framebuffer Space
  - 理解齐次坐标与 Perspective Division
  - 理解图元裁剪发生在透视除法之前
  - 理解 Vulkan 默认裁剪范围与 Viewport 坐标变换
  - 理解正高度与负高度 Viewport 的区别
  - 使用 SV_VertexID 获取顶点索引
  - 使用 SV_Position 输出裁剪空间位置
  - 当前直接在 Shader 中定义三角形顶点
- [x] Fragment Shader
  - 理解 Fragment 与 Pixel 的区别
  - 使用 SV_Target 输出颜色
  - 理解顶点颜色向片元着色器传递的过程
  - 理解 Linear Interpolation
  - 理解 Barycentric Coordinates
  - 理解 Perspective-Correct Interpolation
  - 区分 Interpolation 与 Color Blending
- [x] Slang 基础
  - 理解 float2、float3、float4 等向量类型
  - 理解 Swizzle
  - 理解 Shader 内部数据与外部资源的区别
  - 理解 Shader 程序与 Mesh 数据解耦的意义
  - 使用共享结构体描述 Shader Interface
- [x] Entry Point
  - 使用 vertMain 作为 Vertex Shader 入口
  - 使用 fragMain 作为 Fragment Shader 入口
  - 理解 -fvk-use-entrypoint-name 的作用
  - 理解一个 SPIR-V Module 可以包含多个 Entry Points
- [x] Shader Stage
  - 理解 Programmable Stage
  - 使用 vk::PipelineShaderStageCreateInfo
  - 理解 stage、module、pName 的关系
  - 使用同一个 Shader Module 配置 Vertex 和 Fragment Stage
  - 理解 Entry Point 执行模型必须与 Shader Stage 匹配
- [x] SPIR-V
  - 理解 SPIR-V 是标准化的二进制中间表示
  - 区分 SPIR-V 版本与 Vulkan API 版本
  - 理解 32-bit Word 与 SPIR-V Header
  - 理解 Magic Number、Version、Generator、Bound 和 Schema
  - 理解 SPIR-V 与 GPU 原生机器指令的区别
  - 使用 spirv-dis 反汇编
  - 使用 spirv-val 验证 Vulkan 1.4 目标环境
  - 检查 OpEntryPoint、OpCapability、OpDecorate 和 Location
  - 理解 DrawParameters 与 shaderDrawParameters Feature 的关系
- [x] Shader Module
  - 使用 GetModuleFileNameW() 获取当前 EXE 路径
  - 区分 EXE 所在目录与进程当前工作目录
  - 使用 std::filesystem::path 拼接资源路径
  - 使用二进制模式读取 SPIR-V 文件
  - 使用 std::vector<uint32_t> 保存 SPIR-V Words
  - 检查文件大小、整数转换范围、读取状态与 Magic Number
  - 使用 vk::ShaderModuleCreateInfo
  - 使用 vk::raii::ShaderModule
  - 理解 Shader Module 与 Graphics Pipeline 的生命周期区别
- [x] CPU / Shader Interface
  - 理解 CPU 负责准备资源与录制命令
  - 理解 GPU 根据绘制命令执行 Shader
  - 理解系统值语义与普通 Shader 属性
  - 理解 Shader Stage 之间的 Location 匹配
  - 理解 Shader Module 创建不等于实际 GPU 执行
- [x] CMake 自动执行 slangc
  - 使用 find_program() 查找 slangc.exe
  - 使用 add_custom_command() 定义 SPIR-V 生成规则
  - 使用 OUTPUT 与 DEPENDS 建立文件级依赖
  - 使用 add_custom_target() 定义 Shader 构建目标
  - 使用 add_dependencies() 建立 Target 级依赖
  - 实现 Shader 增量编译
  - 实现生成产物缺失时自动重新编译
- [x] SPIR-V 输出到 Build 目录
  - 理解 CMake 单配置与多配置生成器的区别
  - 使用 $<CONFIG> 区分 Debug / Release
  - 将 SPIR-V 输出到对应 EXE 旁边的 shaders 目录
  - 不将生成的 SPIR-V 文件提交 Git

**实践：**

完成第一个正式 Slang Shader，建立 CMake 自动编译、
SPIR-V 验证、Shader 文件读取与 Vulkan Shader Module 创建流程。

本小节划分为四个知识单元：

- 03.2.1-A — Vertex Shader
- 03.2.1-B — Fragment Shader
- 03.2.1-C — SPIR-V & CMake
- 03.2.1-D — Shader Module & RAII

四个知识单元均已通过验收。

**已完成实现：**

- 新增 `Slang/HelloTriangle.slang`。
- 定义三个顶点的位置及 RGB 颜色。
- 实现 `vertMain()` 和 `fragMain()`。
- 使用 Slang 编译包含两个 Entry Points 的 SPIR-V 模块。
- 在 `Sandbox/CMakeLists.txt` 中建立 Shader 自动编译流程。
- 新增 `HelloTriangleShaders` 构建目标。
- 将 Shader 构建目标纳入 HelloTriangle 的构建依赖。
- 分别生成 Debug 和 Release 的 SPIR-V 文件。
- 新增 `getExecutableDirectory()`。
- 新增 `readSpirv()`。
- 新增 `createShaderModule()`。
- 将 `createGraphicsPipeline()` 接入 `initVulkan()`。
- 创建局部 `vk::raii::ShaderModule`。
- 配置 Vertex 和 Fragment Shader Stage CreateInfo。
- 使用 `std::array` 保存两个 Shader Stage 配置。
- 暂未实际创建 Graphics Pipeline 或 Pipeline Layout。

**Shader 编译环境：**

- Slang Compiler：2026.13.1-1-g84792eb15
- SPIRV-Tools：v2026.3
- SPIR-V Profile：spirv_1_4
- Vulkan Validation Target：vulkan1.4
- CMake：项目最低要求 3.30
- 构建生成器：Visual Studio / MSBuild 多配置生成器

**SPIR-V 编译与反汇编结果：**

- Vertex Entry Point：vertMain
- Fragment Entry Point：fragMain
- 单入口测试产物：776 字节
- 双入口正式产物：1472 字节
- 正式产物 Word Count：368
- Debug / Release SPIR-V 均通过 spirv-val 验证。
- spirv-dis 反汇编成功。
- Vertex Color Output 与 Fragment Color Input 均使用 Location 0。
- Vertex Shader 输出使用 BuiltIn Position。
- SPIR-V 声明 Shader 与 DrawParameters Capability。
- Fragment Shader 使用 OriginUpperLeft Execution Mode。
- 已理解 SV_VertexID 与 VertexIndex、BaseVertex 的关系。
- 已确认当前 Logical Device 启用了 shaderDrawParameters。

**CMake 增量构建验证（2026-11-26）：**

- CMake Configure / Generate 成功。
- Debug Clean Build 成功。
- Release Clean Build 成功。
- 两套配置分别生成 HelloTriangle.spv。
- 未修改 Shader 时，重复构建不重新执行 slangc。
- Shader 源码更新后，自动重新执行 slangc。
- 删除 SPIR-V 后，构建系统能够恢复生成文件。
- 已解决 PowerShell 中未初始化 spirv-val 路径的问题。
- 已理解 `$?` 与 `$LASTEXITCODE` 的区别。

**资源所有权与生命周期：**

- SPIR-V 文件属于磁盘资源。
- `std::vector<uint32_t>` 保存 CPU 端 Shader 二进制数据。
- SPIR-V CPU 数据需要在 Shader Module 创建调用期间保持有效。
- Shader Module 由 Logical Device 创建。
- Shader Module 使用 Vulkan-Hpp RAII 管理。
- Shader Stage CreateInfo 不拥有 Shader Module。
- 当前 Shader Module 为 createGraphicsPipeline() 内部局部变量。
- 后续实际 Pipeline 创建必须在局部 Shader Module 仍然有效时完成。
- 普通同步 Graphics Pipeline 创建完成后，可以销毁 Shader Module。
- Graphics Pipeline 在销毁前必须确保引用它的已提交 GPU 命令完成。
- 当前不提前引入 ShaderManager、PipelineManager 或通用 ResourceManager。

**GitHub Code Review：**

Shader 与 CMake 初步实现 Commit：

`d779dd6e600edd9d9a8b5b96d1d09bb5556eec66`

Shader Module 实现 Commit：

`2f3b71d3f00e903b340e8dc9d599e0e5b50e36a8`

修复与最终审查 Commit：

`777fb2de56d01e55097a74605e88ed479798e4a7`

审查发现及修复：

- P0：readSpirv() 在验证文件大小前分配 Vector。
- P0：未检查文件大小是否为 4 的整数倍，存在缓冲区越界写入风险。
- P0：未检查最小 Header 长度，空文件可能导致 front() 非法访问。
- 上述问题属于同一个文件读取安全性阻断问题。
- 修复后先检查 tellg()、文件长度、Word 完整性及整数转换范围，再分配 Vector。
- 保留完整读取状态与 Magic Number 检查。
- SPIR-V Version 与 Bound 注释已修正。
- 最终复审确认 P0 已关闭。
- 未发现新增 P0 / P1 问题。

**文件读取与异常验证（2026-11-28）：**

- 从仓库根目录启动 HelloTriangle 成功。
- 从 TEMP 目录启动 HelloTriangle 成功，退出码为 0。
- Shader 文件缺失时正确报错，退出码为 1。
- 0 字节文件：SPIR-V file is too small，退出码为 1。
- 16 字节文件：SPIR-V file is too small，退出码为 1。
- 21 字节文件：SPIR-V file size is not a multiple of 4，退出码为 1。
- 20 字节全零文件：Invalid SPIR-V magic number，退出码为 1。
- 四项损坏文件边界测试全部通过。
- 恢复正式 SPIR-V 后，Debug 运行成功。
- 修复后 Debug / Release Clean Build 成功。
- Debug / Release 正常运行退出码均为 0。
- 提供的日志中未发现新增 Validation Error。
- 文件大小与 Magic Number 检查不能替代完整 SPIR-V 验证。
- 本地编译运行由开发者完成，GitHub Review 为静态代码审查。

**原理理解验收：**

A、B、C、D 四个知识单元的原理题及必要补充题完成验收。

重点理解：

1. Clip Space、NDC 与 Framebuffer 坐标的关系。
2. 齐次坐标、裁剪与透视除法。
3. Vertex Shader 与 Fragment Shader 的职责。
4. Shader Stage Interface 与 Location 匹配。
5. 重心坐标与透视正确插值。
6. SPIR-V 与 GPU 机器指令的区别。
7. CMake 文件级依赖与 Target 级依赖。
8. 增量构建与多配置构建。
9. 二进制文件读取的安全前置条件。
10. Shader Module、Shader Stage 与 Graphics Pipeline 的区别。
11. RAII 对象生命周期与 GPU 执行同步的区别。

**最终验收结论（2026-11-28）：**

- Vertex Shader 理论与实现通过。
- Fragment Shader 理论与实现通过。
- SPIR-V 编译、验证与反汇编通过。
- CMake 自动编译与增量构建通过。
- Shader 资源路径验证通过。
- SPIR-V 文件读取及边界测试通过。
- Shader Module 创建通过。
- Shader Stage 配置静态审查通过。
- RAII 生命周期与异常安全审查通过。
- Debug / Release 本地验证通过。
- GitHub Code Review 通过。
- 无未解决 P0 / P1 问题。

**小节状态：已完成。**

**下一小节：**

03.2.2 — Fixed Functions

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

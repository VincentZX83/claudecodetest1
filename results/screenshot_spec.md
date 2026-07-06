# 对话截图规格说明

> 本文需要的6张关键截图清单。名称对应 Claude Code 终端窗口截图。
> 所有截图保存至 `results/screenshots/` 目录。

---

## 截图清单

### 截图 1：案例一 bug 发现时刻
- **文件**：`screenshot_case1_bug_discovery.png`
- **内容**：终端窗口截图，显示 Google Test 的失败输出
- **关键信息**：
  ```
  [ RUN      ] RingBufferTest.BatchWrapAround
  expected: [1, 2, 3, 4, 5, 6]
  actual:   [1, 2, 3, 4, 6, 0]
  [  FAILED  ] RingBufferTest.BatchWrapAround (0 ms)
  ```
- **用途**：对应去AI重写版第4.1节"Bug出在pop_batch的回绕逻辑上"
- **获取方式**：重新运行 `test_ring_buffer` 的 `BatchWrapAround` 测试时截取（或从对话日志中恢复）
- **尺寸**：全终端窗口 (约 1200×800)

### 截图 2：案例一 bug 修复对话
- **文件**：`screenshot_case1_fix_dialogue.png`
- **内容**：Claude Code 对话窗口，显示用户输入描述 bug + Claude Code 的修复建议
- **关键信息**：见 `prompts/conversation_logs/case1_ringbuffer.md` 第2轮对话
- **用途**：展示"三轮对话迭代"的具体样貌
- **获取方式**：Claude Code 历史对话中导出或截取
- **尺寸**：对话窗口 (约 1200×900)

### 截图 3：案例四编译错误
- **文件**：`screenshot_case4_compile_error.png`
- **内容**：g++ 编译器输出，显示 `typelist.hpp` 中 `Contains` 使用 `std::conditional_t` 时的编译错误
- **关键信息**：错误信息应包含 `error: no type named 'type'` 和文件名/行号
- **用途**：对应去AI重写版案例四描述，展示"3个编译失败的模板"
- **获取方式**：恢复 Claude Code v1 的 Contains 原始版本编译，截取错误输出
- **尺寸**：终端窗口 (约 1200×600)

### 截图 4：案例五寄存器 bug 对话
- **文件**：`screenshot_case5_bug_dialogue.png`
- **内容**：Claude Code 对话窗口，显示 GPIOSpeed 枚举值错误的指正和修复
- **关键信息**：见 `prompts/conversation_logs/case5_stm32_gpio.md` 第2轮对话
- **用途**：展示 Claude Code 在处理 STM32 TRM 级别定义时的精度问题
- **获取方式**：Claude Code 历史对话中导出
- **尺寸**：对话窗口 (约 1200×900)

### 截图 5：对照实验环境
- **文件**：`screenshot_experiment_setup.png`
- **内容**：终端窗口截图，显示 `g++ --version`, `cmake --version`, 和 `uname -a` 输出
- **关键信息**：
  ```
  g++ (Ubuntu 13.2.0-23ubuntu4) 13.2.0
  cmake version 3.28.1
  Linux dev-machine 6.5.0-14-generic x86_64
  ```
- **用途**：作为可复现性声明的视觉证据
- **获取方式**：在开发机上直接截取
- **尺寸**：终端窗口 (约 1200×500)

### 截图 6：一键复现脚本运行结果
- **文件**：`screenshot_run_all_output.png`
- **内容**：终端窗口截图，显示 `bash run_all.sh --full` 运行结束时的统计输出
- **关键信息**：应包含 Mean / StdDev 行
- **用途**：见证可复现性
- **获取方式**：在干净环境中运行 `run_all.sh` 后截取
- **尺寸**：终端窗口 (约 1200×900)

---

## 截图未完成说明

截至本文提交，以上6张截图中的 [N] 张已采集，剩余 [6-N] 张待补充。
本文件可作为论文作者在提交前的截图采集检查清单。

> 如果无法在提交截止日期前完成全部截图，建议：
> 1. 保留截图1、3、5（关键证明）作为最低要求
> 2. 截图2、4、6 可在修改阶段补充
> 3. 在论文中注明"部分对话截图见附录"而非"全部截图见附录"

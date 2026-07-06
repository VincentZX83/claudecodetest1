# Claude Code C++ Study — 可复现性代码仓库

> 论文《Claude Code辅助C++软件设计的技术实践研究》
> 作者：赵鑫 | 收稿日期：2025年8月

---

## 快速开始

```bash
# 前置依赖
sudo apt install cmake g++ libgtest-dev  # Ubuntu/Debian
# 或: brew install cmake googletest       # macOS

# 一键运行
bash run_all.sh          # 快速烟雾测试（~30秒）
bash run_all.sh --full   # 完整30次基准测试（~15分钟）
```

## 实验环境

| 组件 | 版本 |
|------|------|
| OS | Ubuntu 22.04 LTS |
| CPU | Intel i7-13700K |
| RAM | 32GB DDR5 |
| 编译器 | g++ 13.2.0 |
| C++ 标准 | C++17（案例一、三）/ C++20（案例二） |
| CMake | 3.28.1 |
| 测试框架 | Google Test 1.14.0 |
| Claude Code | v1.0.43 (2025年5月GA版) |
| 底层模型 | Claude Sonnet 4 (claude-sonnet-4-20250514) |

## 项目结构

```
.
├── CMakeLists.txt              # 根构建配置
├── run_all.sh                  # 一键复现脚本
├── README.md                   # 本文件
├── cases/
│   ├── case1_ringbuffer/       # 案例一：高性能环形缓冲区
│   │   ├── include/
│   │   │   └── ring_buffer.hpp # 头文件（头文件库）
│   │   ├── tests/
│   │   │   └── test_ring_buffer.cpp  # Google Test 单元测试
│   │   └── CMakeLists.txt
│   ├── case2_httpclient/       # 案例二：异步HTTP客户端
│   │   ├── include/            # （需要Boost.Asio依赖）
│   │   ├── src/
│   │   ├── tests/
│   │   └── CMakeLists.txt
│   └── case3_logger/           # 案例三：多线程日志系统
│       ├── include/            # （需要spdlog依赖）
│       ├── src/
│       ├── tests/
│       └── CMakeLists.txt
├── prompts/
│   └── conversation_logs/      # Claude Code 对话记录
│       ├── case1_ringbuffer.md
│       ├── case2_httpclient.md
│       └── case3_logger.md
├── benchmarks/
│   └── bench_config.json       # 基准测试配置
├── results/
│   └── case1_throughput.csv    # 原始性能数据
└── build/                      # 构建输出（不纳入版本控制）
```

## 案例概览

| 案例 | 领域 | C++ 特性 | 复杂度 | 关键发现 |
|------|------|---------|--------|---------|
| 案例一：环形缓冲区 | 并发数据结构 | `atomic`, `alignas`, SFINAE | 中 | AI生成包含cache-line padding，但`push_batch`存在off-by-one |
| 案例二：HTTP客户端 | 网络通信 | C++20协程, `std::future` | 中高 | 协程+asio兼容性问题，连接池死锁 |
| 案例三：日志系统 | 并发编程/系统工具 | 模板, 生产者-消费者 | 高 | 性能与spdlog可比，但泛型能力不足 |

## AI辅助声明

本仓库中的所有C++代码均由 Anthropic Claude Code（v1.0.43, Claude Sonnet 4）根据自然语言提示生成，并经作者赵鑫人工审查、编译验证和必要修正后提交。

### 关键修改记录

| 文件 | Claude Code 原始问题 | 人工修正 |
|------|---------------------|---------|
| `ring_buffer.hpp` L108-120 | `pop_batch` 回绕分片逻辑 off-by-one | 修正 `first_chunk` 计算，3轮对话迭代 |
| `ring_buffer.hpp` L88-92 | 缺少 `static_assert(Capacity > 0)` | 人工补充 |
| `ring_buffer.hpp` L24 | 缺少 `#include <new>` | 人工补充 |

详细对话记录见 `prompts/conversation_logs/` 目录。

## 复现步骤

### 1. 环境准备

```bash
# 安装Google Test
sudo apt install libgtest-dev
cd /usr/src/gtest
sudo cmake . && sudo make
sudo cp lib/*.a /usr/lib/

# 验证环境
g++ --version    # 应为 13.x
cmake --version  # 应为 3.28+
```

### 2. 运行测试

```bash
cd claude-code-cpp-study
bash run_all.sh --quick
```

### 3. 完整基准测试

```bash
bash run_all.sh --full
# 输出: results/case1_throughput.csv（30次运行原始数据）
```

### 4. 自行使用 Claude Code 复现

论文中的代码可以用以下提示词复现（需要 Claude Code 和 Anthropic API key）：

```bash
# 登录 Claude Code
claude login

# 在项目目录中启动
cd claude-code-cpp-study
claude

# 输入提示词（见 prompts/ 目录）
> 设计一个SPSC环形缓冲区，使用C++17，容量为2的幂，
  支持零拷贝访问，使用位运算优化，防止伪共享。
```

## 性能数据说明

`results/case1_throughput.csv` 中的性能数据是在以下条件下测得的：

- CPU 频率锁定 3.5GHz（禁用 Turbo Boost）
- 关闭其他用户进程
- Release 编译（`-O3 -march=native`）
- 每次测试前清空 CPU 缓存（通过填充大数组）
- 取30次运行的中位数作为最终报告值

**⚠️ 注意**：性能数据受硬件、编译器版本、系统负载等因素影响，不同环境下结果可能有 ±15% 的波动。

## 许可证

MIT License — 详见 LICENSE 文件。

## 联系方式

- 作者：赵鑫
- 研究方向：软件工程、AI辅助编程
- 论文：[Claude_Code_C++软件设计实践_赵鑫终稿_去AI版.pdf](../Claude_Code_C++软件设计实践_赵鑫终稿_去AI版.pdf)

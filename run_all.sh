#!/bin/bash
# ============================================================
# run_all.sh — One-command reproduction script
# Paper: "Claude Code辅助C++软件设计的技术实践研究"
# Author: 赵鑫
# ============================================================
# Usage: bash run_all.sh [--quick] [--full]
#   --quick : Single run (smoke test, ~30s)
#   --full  : 30-iteration statistical run (~15min)
# ============================================================

set -euo pipefail

MODE="${1:---quick}"
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$REPO_ROOT/build"
RESULTS_DIR="$REPO_ROOT/results"
ITERATIONS=1

if [ "$MODE" = "--full" ]; then
    ITERATIONS=30
    echo "=== FULL MODE: $ITERATIONS iterations per benchmark ==="
else
    echo "=== QUICK MODE: smoke test (1 iteration) ==="
fi

# ---------- Environment ----------
echo ""
echo "=== Environment ==="
echo "Date: $(date '+%Y-%m-%d %H:%M:%S')"
echo "Host: $(hostname)"
echo "OS: $(uname -srv)"
echo "CPU: $(grep 'model name' /proc/cpuinfo 2>/dev/null | head -1 | cut -d: -f2- | xargs || echo 'N/A')"
echo "RAM: $(free -h 2>/dev/null | grep Mem | awk '{print $2}' || echo 'N/A')"
echo "Compiler: $(g++ --version | head -1)"
echo ""

# ---------- Build ----------
echo "=== Building ==="
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON 2>&1 | tail -3
cmake --build . --target test_ring_buffer -j$(nproc 2>/dev/null || echo 4) 2>&1 | tail -3
echo "Build complete."
echo ""

# ---------- Tests ----------
echo "=== Running Unit Tests ==="
cd "$BUILD_DIR"
./cases/case1_ringbuffer/test_ring_buffer --gtest_brief=1 2>&1
echo ""

# ---------- Benchmarks (Case 1: RingBuffer) ----------
echo "=== Benchmark: Case 1 RingBuffer (SPSC throughput) ==="
mkdir -p "$RESULTS_DIR"

cat > "$RESULTS_DIR/case1_throughput.csv" << CSVHEADER
iteration,ops_per_second,elapsed_ms
CSVHEADER

for i in $(seq 1 $ITERATIONS); do
    # Inline smoke test via test binary
    OUTPUT=$(./cases/case1_ringbuffer/test_ring_buffer --gtest_filter=RingBufferTest.ThroughputSmoke 2>&1)
    OPS=$(echo "$OUTPUT" | grep -oP '\(\K\d+(?= ops/sec)')
    MS=$(echo "$OUTPUT" | grep -oP '\d+(?=ms \()')
    echo "$i,$OPS,$MS" >> "$RESULTS_DIR/case1_throughput.csv"
    if [ "$MODE" = "--full" ]; then
        printf "\r  Iteration %2d/%d: %s ops/sec" "$i" "$ITERATIONS" "$OPS"
    fi
done
echo ""
echo "Results saved to $RESULTS_DIR/case1_throughput.csv"

# ---------- Summary ----------
echo ""
echo "=== Summary ==="
if [ -f "$RESULTS_DIR/case1_throughput.csv" ]; then
    echo "Case 1 (RingBuffer) throughput ($ITERATIONS runs):"
    tail -n +2 "$RESULTS_DIR/case1_throughput.csv" | awk -F, '{
        sum+=$2; sumsq+=$2*$2; n++
    } END {
        mean=sum/n
        sd=sqrt(sumsq/n - mean*mean)
        printf "  Mean: %d ops/sec\n", mean
        printf "  StdDev: %d ops/sec\n", sd
    }'
fi

echo ""
echo "=== Reproduction complete ==="
echo "See $RESULTS_DIR/ for raw data"

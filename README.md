# CPU Pipeline Execution Simulator

A simple educational simulator for a **5-stage CPU pipeline** (IF → ID → EX → MEM → WB), built for a Computer Organization & Architecture (COA) project. It is provided in two forms:

- `pipeline_simulator.cpp` – command-line C++ version
- `index.html` – browser version (a JavaScript port of the same logic, no installation needed)

## Features

- Timing table showing which stage each instruction is in during every cycle
- RAW (data) hazard detection with stalls (data forwarding is **disabled**)
- Control hazard handling with a **2-cycle branch penalty** (BEQ, BNE, JMP, BR)
- Performance report: ideal cycles, actual cycles, stall cycles, non-pipelined cycles, and speedup

## Run the web version

Open `index.html` in any browser, or host it for free with GitHub Pages:

1. Push this folder to a GitHub repository.
2. Go to **Settings → Pages**.
3. Under *Branch*, choose `main` and `/ (root)`, then **Save**.
4. Your site will be live at `https://<your-username>.github.io/<repo-name>/`.

## Run the C++ version

```bash
g++ -std=c++11 pipeline_simulator.cpp -o pipeline_simulator
./pipeline_simulator
```

## Input format

Each instruction is written as:

```
OPCODE DEST SRC1 SRC2
```

Use `-` when a register is not needed. Up to 15 instructions are supported.

```
ADD R3 R1 R2
SUB R4 R3 R5
BEQ - R4 R0
```

## How the model works

- Each instruction normally starts one cycle after the previous one.
- **RAW hazard:** an instruction that reads a register written by an earlier instruction waits until that instruction finishes WB.
- **Control hazard:** after a branch, the next instruction waits two extra cycles.
- Speedup = (instructions × 5) / actual pipeline cycles.

## Limitations

This is a simplified scheduling model. Branch outcomes, cache misses, and memory delays are not simulated.

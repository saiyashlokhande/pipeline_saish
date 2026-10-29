#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
using namespace std;

struct Instruction {
    string opcode, dest, src1, src2;
    bool branch;
    int start = 0;
};

bool isBranch(string op) {
    for (char &c : op)
        c = toupper(static_cast<unsigned char>(c));
    return op == "BEQ" || op == "BNE" || op == "JMP" || op == "BR";
}

bool readsRegister(const Instruction &in, const string &reg) {
    if (reg == "-" || reg.empty())
        return false;
    return in.src1 == reg || in.src2 == reg;
}

int main() {
    int n;
    cout << "========================================\n";
    cout << "     CPU PIPELINE EXECUTION SIMULATOR\n";
    cout << "========================================\n";
    cout << "Pipeline: IF -> ID -> EX -> MEM -> WB\n";
    cout << "Data forwarding: Disabled\n";
    cout << "Branch penalty: 2 cycles\n\n";
    cout << "Enter number of instructions (1-15): ";
    cin >> n;
    if (n < 1 || n > 15) {
        cout << "Invalid number of instructions.\n";
        return 0;
    }
    vector<Instruction> ins(n);
    cout << "\nEnter each instruction as:\n";
    cout << "OPCODE DEST SRC1 SRC2\n";
    cout << "Use - when a register is not needed.\n\n";
    cout << "Example: ADD R3 R1 R2\n";
    cout << "Example: SUB R4 R3 R5\n";
    cout << "Example: BEQ - R4 R0\n\n";
    for (int i = 0; i < n; i++) {
        cout << "Instruction I" << i + 1 << ": ";
        cin >> ins[i].opcode >> ins[i].dest >> ins[i].src1 >> ins[i].src2;
        ins[i].branch = isBranch(ins[i].opcode);
        if (ins[i].branch)
            ins[i].dest = "-";
    }

    // Each instruction normally starts one cycle after the previous one.
    ins[0].start = 1;
    for (int i = 1; i < n; i++) {
        int start = ins[i - 1].start + 1;
        // RAW hazard: wait until the producer completes WB (no forwarding).
        for (int j = 0; j < i; j++) {
            if (ins[j].dest != "-" && readsRegister(ins[i], ins[j].dest))
                start = max(start, ins[j].start + 4);
        }
        // Simplified control hazard: two empty issue cycles after a branch.
        for (int j = 0; j < i; j++) {
            if (ins[j].branch)
                start = max(start, ins[j].start + 3);
        }
        ins[i].start = start;
    }

    const string stages[] = {"IF", "ID", "EX", "MEM", "WB"};
    int totalCycles = ins[n - 1].start + 4;
    int idealCycles = n + 4;
    int stallCycles = max(0, totalCycles - idealCycles);
    int nonPipelineCycles = n * 5;

    cout << "\n\n========== PIPELINE TIMING TABLE ==========\n\n";
    cout << left << setw(13) << "Instruction";
    for (int c = 1; c <= totalCycles; c++)
        cout << setw(7) << c;
    cout << "\n";
    cout << string(13 + totalCycles * 7, '-') << "\n";
    for (int i = 0; i < n; i++) {
        cout << left << setw(13) << ("I" + to_string(i + 1));
        for (int c = 1; c <= totalCycles; c++) {
            string cell = "--";
            int offset = c - ins[i].start;
            if (offset >= 0 && offset < 5)
                cell = stages[offset];
            cout << setw(7) << cell;
        }
        cout << "\n";
    }

    cout << "\n========== HAZARD REPORT ==========\n";
    bool found = false;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (ins[j].dest != "-" && readsRegister(ins[i], ins[j].dest)) {
                cout << "Data hazard: I" << j + 1 << " -> I" << i + 1
                     << " (register " << ins[j].dest << ")\n";
                found = true;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (ins[i].branch) {
            cout << "Control hazard: I" << i + 1 << " is a branch instruction.\n";
            found = true;
        }
    }
    if (!found)
        cout << "No data or control hazards detected.\n";

    cout << "\n========== PERFORMANCE REPORT ==========\n";
    cout << "Number of instructions : " << n << "\n";
    cout << "Ideal pipeline cycles  : " << idealCycles << "\n";
    cout << "Actual pipeline cycles : " << totalCycles << "\n";
    cout << "Additional stall cycles: " << stallCycles << "\n";
    cout << "Non-pipeline cycles    : " << nonPipelineCycles << "\n";
    double speedup = static_cast<double>(nonPipelineCycles) / totalCycles;
    cout << fixed << setprecision(2);
    cout << "Pipeline speedup       : " << speedup << "x\n";
    cout << "\nNote: This is an educational scheduling model.\n";
    cout << "Branch outcomes, cache misses, and memory delays\n";
    cout << "are not simulated. Hazard handling is simplified.\n";
    cout << "\n========== PROJECT COMPLETED ==========\n";
    return 0;
}

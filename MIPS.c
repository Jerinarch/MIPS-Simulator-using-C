#define REG_COUNT 32
#define MEM_SIZE 1024

#include <stdio.h>
#include <string.h>

enum instruction_type {
    ADD,
    SUB,
    LW,
    SW,
    BEQ
};

int registers[REG_COUNT] = {0};
int memory[MEM_SIZE] = {0};

void add(int rd, int rs, int rt) {
    registers[rd] = registers[rs] + registers[rt];
}

void sub(int rd, int rs, int rt) {
    registers[rd] = registers[rs] - registers[rt];
}

void lw(int rt, int offset, int rs) {
    int address = registers[rs] + offset;
    if (address >= 0 && address < MEM_SIZE) {
        registers[rt] = memory[address];
    } else {
        printf("Memory access out of bounds\n");
    }
}

void sw(int rt, int offset, int rs) {
    int address = registers[rs] + offset;
    if (address >= 0 && address < MEM_SIZE) {
        memory[address] = registers[rt];
    } else {
        printf("Memory access out of bounds\n");
    }
}

int beq(int rs, int rt, int offset) {
    if (registers[rs] == registers[rt]) {
        return offset;
    }
    return 0;
}

void execute_instruction(char* instruction) {
    int rd, rs, rt, offset;
    enum instruction_type type;

    if (sscanf(instruction, "ADD R%d, R%d, R%d", &rd, &rs, &rt) == 3) {
        type = ADD;
    } else if (sscanf(instruction, "SUB R%d, R%d, R%d", &rd, &rs, &rt) == 3) {
        type = SUB;
    } else if (sscanf(instruction, "LW R%d, %d(R%d)", &rt, &offset, &rs) == 3) {
        type = LW;
    } else if (sscanf(instruction, "SW R%d, %d(R%d)", &rt, &offset, &rs) == 3) {
        type = SW;
    } else if (sscanf(instruction, "BEQ R%d, R%d, %d", &rs, &rt, &offset) == 3) {
        type = BEQ;
    } else {
        printf("Invalid instruction\n");
        return;
    }

    int branch_offset;
    switch (type) {
        case ADD:
            add(rd, rs, rt);
            break;
        case SUB:
            sub(rd, rs, rt);
            break;
        case LW:
            lw(rt, offset, rs);
            break;
        case SW:
            sw(rt, offset, rs);
            break;
        case BEQ:
            branch_offset = beq(rs, rt, offset);
            if (branch_offset != 0) {
                printf("Branching by offset %d\n", branch_offset);
            }
            break;
    }
}

int main() {
    int instruction_count, reg_num, reg_value, i;

    printf("\n**\n");
    printf("WELCOME TO MIPS SIMULATOR\n");
    printf("\n");

    int reg_init_count;
    printf("\n\nHow many registers do you want to initialize? : ");
    scanf("%d", &reg_init_count);
    for (i = 0; i < reg_init_count; i++) {
        printf("Enter register number to initialize (0-31): ");
        scanf("%d", &reg_num);
        if (reg_num >= 0 && reg_num < 32) {
            printf("Enter value for register R%d: ", reg_num);
            scanf("%d", &reg_value);
            registers[reg_num] = reg_value;
        } else {
            printf("Invalid register number. Please enter a number between 0 and 31.\n");
            i--; 
        }
    }

    printf("Enter the number of instructions: ");
    scanf("%d", &instruction_count);
    getchar();

    char instructions[instruction_count][100];

    for (i = 0; i < instruction_count; i++) {
        printf("Enter instruction %d: ", i + 1);
        fgets(instructions[i], sizeof(instructions[i]), stdin);
        instructions[i][strcspn(instructions[i], "\n")] = 0;
    }

    printf("\n**\n");
    printf("EXECUTING INSTRUCTIONS\n");
    printf("\n");

    for (i = 0; i < instruction_count; i++) {
        printf("Executing instruction: %s\n", instructions[i]);
        execute_instruction(instructions[i]);
    }

    printf("\n------------------------------------------------\n");
    printf("REGISTER VALUES AFTER EXECUTION\n");
    printf("------------------------------------------------\n");
    for (i = 0; i < 32; i++) {
        printf("R%-2d = %-10d\n", i, registers[i]);
    }

    return 0;
}


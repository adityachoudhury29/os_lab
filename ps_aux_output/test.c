#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_THRESHOLD 80.0  // Define threshold for CPU-intensive processes
#define MEM_THRESHOLD 50.0  // Define threshold for memory-hungry processes

void execute_ps_command() {
    // Run the `ps aux` command and redirect output to "ps_aux.txt"
    system("ps aux > ps_aux.txt");

    // Run the `ps -ef` command and redirect output to "ps_ef.txt"
    system("ps -ef > ps_ef.txt");
}

void parse_ps_aux() {
    FILE *file = fopen("ps_aux.txt", "r");
    if (!file) {
        perror("Failed to open ps_aux.txt");
        return;
    }

    char line[1024];
    int total_processes = 0;
    int running_processes = 0;
    int terminals = 0;
    int cpu_intensive_processes = 0;
    int memory_hungry_processes = 0;
    printf("CPU Intensive Processes (PID):\n");
    printf("Memory Hungry Processes (PID):\n");

    // Skip the header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        total_processes++;

        char user[32], pid[16], cpu[8], mem[8], vsz[16], rss[16], tty[8], stat[8], start[16], time[16], command[256];
        sscanf(line, "%s %s %s %s %s %s %s %s %s %s %[^\n]", user, pid, cpu, mem, vsz, rss, tty, stat, start, time, command);

        float cpu_usage = atof(cpu);
        float mem_usage = atof(mem);

        // Count running processes
        if (strchr(stat, 'R') != NULL) {
            running_processes++;
        }

        // Count terminals
        if (strcmp(tty, "?") != 0) {
            terminals++;
        }

        // List CPU-intensive processes
        if (cpu_usage > CPU_THRESHOLD) {
            cpu_intensive_processes++;
            printf("%s ", pid);
        }

        // List memory-hungry processes
        if (mem_usage > MEM_THRESHOLD) {
            memory_hungry_processes++;
            printf("%s ", pid);
        }
    }

    fclose(file);

    printf("\n\nSummary:\n");
    printf("Total Processes: %d\n", total_processes);
    printf("Running Processes: %d\n", running_processes);
    printf("Number of Terminals: %d\n", terminals);
    printf("CPU Intensive Processes: %d\n", cpu_intensive_processes);
    printf("Memory Hungry Processes: %d\n", memory_hungry_processes);
}

int main() {
    execute_ps_command();  // Step 1: Execute ps aux and ps -ef

    printf("Parsing ps_aux.txt...\n");
    parse_ps_aux();  // Step 2: Parse ps_aux.txt

    return 0;
}

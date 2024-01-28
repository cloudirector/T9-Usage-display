#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COMMAND_COUNT 3
char *commands[] = {
    "top -bn1 | grep 'Cpu(s)' | awk '{printf \"cpu:%.2f%%\", $2 + $4}'",
    "free -m | awk '/Mem/ {printf \"%dMB/%dMB\", $3, $2}'",
    "uptime | cut -d ',' -f 1 | awk '{print $1}'"
};

void execute_command(const char *command, char *output) {
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    fgets(output, 256, fp);
    pclose(fp);
}

void update_output(const char *command, char *prev_output) {
    char output[256];
    execute_command(command, output);
    if (strcmp(output, prev_output) != 0) {
        char adb_command[512];
        snprintf(adb_command, sizeof(adb_command), "adb shell /usr/bin/guimgr_cli lcd_eng_mode x%s", output);
        system(adb_command);
        strncpy(prev_output, output, sizeof(output));
    }
}

int main() {
    char prev_output[256];
    while (1) {
        for (int i = 0; i < COMMAND_COUNT; ++i) {
            for (int j = 0; j < 10; ++j) {
                update_output(commands[i], prev_output);
                sleep(1);
            }
        }
    }
    return 0;
}

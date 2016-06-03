#include "espressif/esp_common.h"
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "midea-ir.h"

MideaIR ir;

static void process_command(const char *cmd)
{
    if (!strcmp(cmd, "on")) {
        ir.enabled = true; 
    } else if (!strcmp(cmd, "off")) {
        ir.enabled = false;
    } else if (!strcmp(cmd, "auto")) {
        ir.mode = MODE_AUTO;
    } else if (!strcmp(cmd, "cool")) {
        ir.mode = MODE_COOL;
    } else if (!strcmp(cmd, "heat")) {
        ir.mode = MODE_HEAT;
    } else if (!strncmp(cmd, "temp", 4)) {
        ir.temperature = atoi(&cmd[5]);
//    } else if (!strncmp(cmd, "fan", 3)) {
//        ir.fan_level = atoi(&cmd[4]);
    } else if (!strcmp(cmd, "direct")) {
        midea_ir_move_deflector(&ir);
        return;
    } else {
        printf("Unknown  command: %s\n", cmd);
        return;
    }
    printf("Sending ir code\n");
    midea_ir_send(&ir); 
}

#define LINE_BUFF_SIZE 64

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    printf("Please enter a command\n");
    printf("Commands: on, off, auto, cool, heat, temp, fan, direct\n");
    printf("> ");

    midea_ir_init(&ir, 14);

    char line_buff[LINE_BUFF_SIZE] = {0};
    uint8_t chars = 0;

    while(1) {
        int c = getchar();
        if (c == EOF || c == '\r') {
            continue;
        }

        putchar(c);  // echo

        if (c == '\n') {
            if (chars != 0) {
                process_command(line_buff);
                chars = 0;
                line_buff[0] = 0;
            }
            printf("\n> ");
        } else {
            line_buff[chars] = c;
            chars++;
            line_buff[chars] = 0;
        }
    }
}

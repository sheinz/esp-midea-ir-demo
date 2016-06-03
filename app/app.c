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
        return;
    }
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

    midea_ir_init(&ir);

    char line_buff[LINE_BUFF_SIZE] = {0};

    while(1) {
        int c = getchar();
        if (c == EOF) {
            continue;
        }
        if (c == '\n') {
            process_command(line_buff);
            line_buff[0] = '\0';
            printf("> ");
        } else {
            line_buff[strlen(line_buff)] = c;
        }
        
        putchar(c);
    }
}

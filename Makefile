PROGRAM = esp-gizmo-ir-remote
OTA = 1
DEVICE_IP = 192.168.0.108
EXTRA_COMPONENTS = extras/rboot-ota extras/pwm ./midea-ir
PROGRAM_SRC_DIR = ./app
ESPPORT = /dev/tty.SLAB_USBtoUART

include ../esp-open-rtos/common.mk

upload:
	tftp -v -m octet $(DEVICE_IP) -c put firmware/$(PROGRAM).bin firmware.bin

console:
	picocom -b 115200 /dev/ttyUSB0

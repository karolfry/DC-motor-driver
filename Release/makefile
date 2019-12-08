################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
DC_motor_driver.lss \

FLASH_IMAGE += \
DC_motor_driver.hex \

EEPROM_IMAGE += \
DC_motor_driver.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: DC_motor_driver.elf secondary-outputs

# Tool invocations
DC_motor_driver.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,DC_motor_driver.map -mmcu=atmega8 -o "DC_motor_driver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DC_motor_driver.lss: DC_motor_driver.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S DC_motor_driver.elf  >"DC_motor_driver.lss"
	@echo 'Finished building: $@'
	@echo ' '

DC_motor_driver.hex: DC_motor_driver.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex DC_motor_driver.elf  "DC_motor_driver.hex"
	@echo 'Finished building: $@'
	@echo ' '

DC_motor_driver.eep: DC_motor_driver.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex DC_motor_driver.elf  "DC_motor_driver.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: DC_motor_driver.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 DC_motor_driver.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) DC_motor_driver.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets

################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/simplelink_cc32xx_sdk_2_40_02_00/examples/rtos/CC3220S_LAUNCHXL/drivers/Proyecto_LAB_PIM" --include_path="C:/ti/simplelink_cc32xx_sdk_2_40_02_00/source" --include_path="C:/ti/simplelink_cc32xx_sdk_2_40_02_00/kernel/nortos" --include_path="C:/ti/simplelink_cc32xx_sdk_2_40_02_00/kernel/nortos/posix" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=NORTOS_SUPPORT -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>

#define  F_CPU 16000000UL

#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      ( F_CPU )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    ( 2 )
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 85 )
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 1500 ) )
#define configMAX_TASK_NAME_LEN                 ( 8 )
#define configUSE_TRACE_FACILITY                0
#define configUSE_16_BIT_TICKS                  1
#define configIDLE_SHOULD_YIELD                 1
#define configQUEUE_REGISTRY_SIZE               0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/* Set the following definitions to 1 to include the API function, or zero to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskCleanUpResources           0
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1

#endif /* FREERTOS_CONFIG_H */

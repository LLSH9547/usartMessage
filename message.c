#include "message.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "sample.h"

/**
 * @brief prot
 *
 */

void message_send_data(uint8_t *Data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, Data, len, 1000);
}

/**
 * @brief HAL
 *
 */

cJSON *messageToEsp = NULL;
/**
 * @brief
 *
 */
void message_init()
{
    messageToEsp = cJSON_CreateObject();
    cJSON_AddNumberToObject(messageToEsp, "tvco", 11);
    cJSON_AddNumberToObject(messageToEsp, "hum", 22);
    cJSON_AddNumberToObject(messageToEsp, "temp", 33);
}

/**
 * @brief 信息发送函数
 *
 */
void message_printf(void)
{
    char *string = cJSON_Print(messageToEsp);
    message_send_data(string, strlen(string));
    HAL_UART_Transmit(&huart2, string, strlen(string), 1000);
    free(string);
}
uint8_t temp;
void message_exe(void)
{
    temp = humi_get();
    cJSON_ReplaceItemInObject(messageToEsp, "tvco", cJSON_CreateNumber(tvoc_get()));
    cJSON_ReplaceItemInObject(messageToEsp, "hum", cJSON_CreateNumber(temp));
    temp = temp_get();
    cJSON_ReplaceItemInObject(messageToEsp, "temp", cJSON_CreateNumber(temp));
    message_printf();
}

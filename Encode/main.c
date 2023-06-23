#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define STM32MCP_TORQUE_CONFIG_PAYLOAD_LENGTH        0x06
#define STM32MCP_TORQUE_RAMP_CONFIG_PAYLOAD_LENGTH   0x08
#define STM32_MOTOR_LAST_ID                          0x00
#define STM32_MOTOR_1_ID                             0x20
#define STM32_MOTOR_2_ID                             0x40
#define STM32MCP_TORQUE_CONFIG_ID                    0x1E
#define STM32MCP_TORQUE_RAMP_CONFIG_ID               0x1D

uint8_t STM32MCP_calChecksum(uint8_t *msg, uint8_t size)
{
    uint16_t total = 0;
    uint8_t n = 0;
    while(n != size)
    {
        total += msg[n];
        n++;
    }
    return (total & 0xFF) + ((total>>8) & 0xFF);
}

void print_payload(uint8_t *msg, uint8_t size)
{
    for (int i = 0; i<size; i++)
    {
        printf("%X", msg[i]);
    }

}

void STM32MCP_TorqueConfiguration(uint16_t allowableSpeed, uint16_t IQValue)
{
   //Make a payload and insert into the packet
   uint8_t *txFrame = (uint8_t *)malloc(sizeof(uint8_t)*(STM32MCP_TORQUE_CONFIG_PAYLOAD_LENGTH)+3);
   txFrame[0] = STM32_MOTOR_1_ID | STM32MCP_TORQUE_CONFIG_ID;
   txFrame[1] = STM32MCP_TORQUE_CONFIG_PAYLOAD_LENGTH;
   txFrame[2]  =  allowableSpeed         & 0xFF;
   txFrame[3]  = (allowableSpeed >>  8)  & 0xFF;
   txFrame[4]  = (allowableSpeed >> 16)  & 0xFF;
   txFrame[5]  = (allowableSpeed >> 24)  & 0xFF;
   txFrame[6]  =  IQValue        & 0xFF;
   txFrame[7]  = (IQValue >>  8) & 0xFF;
   txFrame[8] =  STM32MCP_calChecksum(txFrame,8);
   printf("%X\n", txFrame[0]);
   printf("%X\n", txFrame[1]);
   printf("%X\n", txFrame[2]);
   printf("%X\n", txFrame[3]);
   printf("%X\n", txFrame[4]);
   printf("%X\n", txFrame[5]);
   printf("%X\n", txFrame[6]);
   printf("%X\n", txFrame[7]);
   printf("%X\n", txFrame[8]);
   print_payload(txFrame, 9);
   printf("\n");
   uint16_t SPEED = txFrame[2] + (txFrame[3] << 8) + (txFrame[4] << 16) + (txFrame[5] << 24);
   printf("Speed is : %u\n", SPEED);
   uint16_t IQ_Current = txFrame[6] + (txFrame[7] << 8);
   printf("Current (s16) is : %u\n", IQ_Current);
}

void STM32MCP_TorqueRampConfiguration(uint16_t torqueIQ, uint16_t allowableSpeed, uint16_t rampRate)
{
   //Make a payload and insert into the packet
   uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_TORQUE_RAMP_CONFIG_PAYLOAD_LENGTH + 3));
   txFrame[0]  = STM32_MOTOR_1_ID | STM32MCP_TORQUE_RAMP_CONFIG_ID;
   txFrame[1]  = STM32MCP_TORQUE_RAMP_CONFIG_PAYLOAD_LENGTH;
   txFrame[2]  =  torqueIQ              & 0xFF;
   txFrame[3]  = (torqueIQ >> 8)        & 0xFF;
   txFrame[4]  =  allowableSpeed        & 0xFF;
   txFrame[5]  = (allowableSpeed >> 8)  & 0xFF;
   txFrame[6]  = (allowableSpeed >> 16) & 0xFF;
   txFrame[7]  = (allowableSpeed >> 24) & 0xFF;
   txFrame[8]  =  rampRate              & 0xFF;
   txFrame[9]  = (rampRate >> 8)        & 0xFF;
   txFrame[10] = STM32MCP_calChecksum(txFrame, 10);
   printf("%X\n", txFrame[0]);
   printf("%X\n", txFrame[1]);
   printf("%X\n", txFrame[2]);
   printf("%X\n", txFrame[3]);
   printf("%X\n", txFrame[4]);
   printf("%X\n", txFrame[5]);
   printf("%X\n", txFrame[6]);
   printf("%X\n", txFrame[7]);
   printf("%X\n", txFrame[8]);
   printf("%X\n", txFrame[9]);
   printf("%X\n", txFrame[10]);
   print_payload(txFrame, 11);
   printf("\n");
   uint16_t Current = txFrame[2] + (txFrame[3] << 8);
   printf("Maximum Current Allowed (s16): %u\n",Current);
   uint16_t RPM = txFrame[4] + (txFrame[5] << 8) + (txFrame[6] << 16) + (txFrame[7] << 24);
   printf("Maximum Speed Allowed : %u\n", RPM);
   uint16_t TIME = txFrame[8] + (txFrame[9] << 8);
   printf("Torque Ramping (ms) : %u ms\n",TIME);
}

int main()
{
    uint16_t speed;
    uint16_t IQValue;
    uint16_t Ramp;
    uint16_t max_current;
    printf("Input the allowable speed (RPM): ");
    scanf("%hu", &speed);
    printf("Input the target Torque (Iq): ");
    scanf("%hu", &IQValue);
    printf("Input the Ramp Rate (ms): ");
    scanf("%hu", &Ramp);
    printf("Input the maximum current (Iq): ");
    scanf("%hu", &max_current);
    STM32MCP_TorqueConfiguration(speed,IQValue);
    printf("\n");
    STM32MCP_TorqueRampConfiguration(max_current, speed, Ramp);
    return 0;
}

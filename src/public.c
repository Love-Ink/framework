#include "public.h"

/**
 * @brief GPIO初始化
 * 
 * @param GPIOx GPIOx(A~G)
 * @param GPIO_Pin GPIO_Pin_x (0~15)
 * @param GPIO_Mode GPIO模式 详见 GPIOMode_TypeDef
 */
void gpio_init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode) {
    GPIO_InitTypeDef GPIO_Initstructure;
    
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOx, &GPIO_Initstructure);
}



#ifndef __FRAMEWORK_PUBLIC_H
#define __FRAMEWORK_PUBLIC_H

#include "ch32v20x.h"

void gpio_init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
#endif
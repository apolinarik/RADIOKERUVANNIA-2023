#include "main.h"

TIM_HandleTypeDef htim2;

GPIO_InitTypeDef GPIO_InitStruct;

int buttonState = 0;
int previousButtonState = 0;
int servoPosition = 0; // 0 - pocjacok, 1 - 90 grad.

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void Error_Handler(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  
  while (1) {
    buttonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
    
    if (buttonState != previousButtonState) {
      if (buttonState == GPIO_PIN_SET) {
        // if cnopka najata
        if (servoPosition == 0) {
          // plavno do 90 grad
          for (int i = 60; i <= 210; i++) {
            TIM2->CCR1 = i; // Hastroyka SHIM plavnosti
            HAL_Delay(10);  // Zatrimka dlya plavnosty
          }
          servoPosition = 1; // Onovlennia pozicii
        } else {
          // plavno do 0 grad
          for (int i = 210; i >= 60; i--) {
            TIM2->CCR1 = i; // Halastuv. SHIM plavnosti
            HAL_Delay(10);  // Zatrumka dlya plavnosty
          }
          servoPosition = 0; // Onovlenya pozicii
        }
      }
    }
    
    previousButtonState = buttonState;
  }
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_TIM2_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;  // 72MHz / 72 = 1MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1439;  // 1MHz / 1440 = 50Hz
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_Base_Init(&htim2);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htim2);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_MspPostInit(&htim2);
}

static void MX_GPIO_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void) {
  __disable_irq();
  while (1) {
    // obrobka Pomulku
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
  // obrobka pomulok assert
}
#endif /* USE_FULL_ASSERT */
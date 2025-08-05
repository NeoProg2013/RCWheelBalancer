#include "core.h"

static void system_clock_init();
static void gpio_init();
static void error_handler();

volatile uint32_t pulse_count = 0;

void TIM1_CC_IRQHandler(void) {
    ++pulse_count;
    TIM1->SR = ~(TIM_SR_CC4IF | TIM_SR_CC3IF | TIM_SR_CC2IF | TIM_SR_CC1IF);
}

uint32_t get_pulse_count() {
    uint32_t t = pulse_count;
    pulse_count = 0;
    return t;
}


int32_t pwm = 0;
uint32_t count = 0;

int main() {
    HAL_Init();
    system_clock_init();
    //systimer_init();
    gpio_init();
    
    
    
    __HAL_RCC_TIM1_CLK_ENABLE();
    
    TIM_HandleTypeDef tim1 = {0};
    tim1.Instance = TIM1;
    tim1.Init.Prescaler = 72 - 1;
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.Period = 65535;
    tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&tim1) != HAL_OK) {
        error_handler();
    }
    
    TIM_IC_InitTypeDef tim_ic = {0};
    tim_ic.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    tim_ic.ICSelection = TIM_ICSELECTION_DIRECTTI;
    tim_ic.ICPrescaler = TIM_ICPSC_DIV1;
    tim_ic.ICFilter    = 0;     
    if (HAL_TIM_IC_ConfigChannel(&tim1, &tim_ic, TIM_CHANNEL_1) != HAL_OK) {
        error_handler();
    }
    
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
    HAL_TIM_IC_Start_IT(&tim1, TIM_CHANNEL_1);
    
    
    
    __HAL_RCC_TIM3_CLK_ENABLE();
    
    TIM_HandleTypeDef tim3 = {0};
    tim3.Instance = TIM3;
    tim3.Init.Prescaler = 72 - 1;
    tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim3.Init.Period = 1000;
    tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&tim3) != HAL_OK) {
        error_handler();
    }
    
    TIM_OC_InitTypeDef tim3_oc = {0};
    tim3_oc.OCMode = TIM_OCMODE_PWM1;
    tim3_oc.Pulse = 300; // duty cycle: 50% (Pulse = Period/2)
    tim3_oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    tim3_oc.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&tim3, &tim3_oc, TIM_CHANNEL_3) != HAL_OK) {
        error_handler();
    }
    HAL_TIM_PWM_Start(&tim3, TIM_CHANNEL_3);
    
    /*HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
    HAL_TIM_IC_Start_IT(&tim1, TIM_CHANNEL_1);*/

    
    
    
    
    
    uint32_t start_time = HAL_GetTick();
    while (1) {
        if (HAL_GetTick() - start_time > 100) {
            start_time = HAL_GetTick();
            
            count = get_pulse_count();
            
            const int32_t target = 80;
            if (count < target) {
                pwm++;
                if (pwm > 500) {
                  pwm = 500;
                }
            } else if (count > target) {
                pwm--;
                if (pwm < 0) {
                  pwm = 0;
                }
            }
            
            TIM3->CCR3 = pwm;
                    
            asm("nop");
        }
        
    }
}

static void system_clock_init() {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error_handler();
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        error_handler();
    }
}

static void gpio_init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure GPIO pin : PA8
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure GPIO pin : PB0
    GPIO_InitStruct.Pin  = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void error_handler(void) {
    __disable_irq();
    while (1) {}
}

/// ***************************************************************************
/// @brief  Systimer ISR
/// @return none
/// ***************************************************************************
#pragma call_graph_root="interrupt"
void SysTick_Handler(void) {
    HAL_IncTick();
}

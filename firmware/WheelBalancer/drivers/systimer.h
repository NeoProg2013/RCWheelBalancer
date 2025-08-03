/// ***************************************************************************
/// @file    systimer.h
/// @author  NeoProg
/// @brief   System timer (1 kHz)
/// ***************************************************************************
#ifndef _SYSTIMER_H_
#define _SYSTIMER_H_

#include <stdint.h>

/// ***************************************************************************
/// @brief  System timer initialize
/// @param  none
/// @return none
/// ***************************************************************************
extern void systimer_init(void);

/// ***************************************************************************
/// @brief  Get current time in milliseconds
/// @param  none
/// @return Milliseconds
/// ***************************************************************************
extern uint64_t get_time_ms(void);

/// ***************************************************************************
/// @brief  Synchronous delay
/// @param  ms: time delay [ms]
/// @return none
/// ***************************************************************************
extern void delay_ms(uint32_t ms);


#endif // _SYSTIMER_H_

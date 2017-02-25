#pragma once

#include <functional>
#include "avr_ioport.h"

void avr_irq_register_fun(struct avr_irq_t* irq, const std::function<void(avr_irq_t*, uint32_t)>& cb);

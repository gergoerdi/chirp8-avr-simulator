#include "Util.hh"

#include "avr_ioport.h"

namespace {
    void irq_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
    {
        (*static_cast<std::function<void(avr_irq_t*, uint32_t)>*>(closure))(irq, value);
    }
}

void avr_irq_register_fun(struct avr_irq_t* irq, const std::function<void(avr_irq_t*, uint32_t)>& cb)
{
    avr_irq_register_notify(irq, irq_cb, new auto(cb));
}

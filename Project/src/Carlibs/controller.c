#include "controller.h"

#define DBG_SECTION_NAME "controller"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

controller_t controller_create(rt_size_t size, rt_uint16_t sample_time)
{
    controller_t new_controller = (controller_t)rt_malloc(size);

    new_controller->sample_time = sample_time;

    return new_controller;
}

rt_err_t controller_update(controller_t controller, float current_point)
{
    RT_ASSERT(controller != RT_NULL);

    controller->update(controller, current_point);

    return RT_EOK;
}

rt_err_t controller_set_target(controller_t controller, float target)
{
    RT_ASSERT(controller != RT_NULL);

    controller->target = target;
    return RT_EOK;
}

rt_err_t controller_set_param(controller_t controller, float Kp, float Ki, float Kd)
{
    RT_ASSERT(controller != RT_NULL);

    return controller->set_param(controller, Kp, Ki, Kd);
}

#include "headfile.h"
#include <agile_button.h>

#ifdef RT_USING_FINSH
#include <finsh.h>
#endif

#define DBG_SECTION_NAME "btn"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define KEY0_PIN D14
#define KEY1_PIN D15
#define KEY2_PIN D16

static agile_btn_t *key0 = RT_NULL;
static agile_btn_t *key1 = RT_NULL;
static agile_btn_t *key2 = RT_NULL;
extern rt_bool_t is_chassis_running;

#define SWITCH_1
#define SWITCH_2
#define SWITCH_3
#define SWITCH_4

static void switch_init(void)
{
    gpio_init(SWITCH_1, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_2, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_3, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_4, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
}

static int switch_get_state(int num)
{
    switch (num)
    {
    case:
        0 : return break;
    }
}
INIT_APP_EXPORT(switch_init);

static void btn_click_event_cb(agile_btn_t *btn)
{
    rt_kprintf("[button click event] pin:%d repeat:%d, hold_time:%d\n", btn->pin, btn->repeat_cnt, btn->hold_time);
    switch (btn->pin)
    {
    case KEY0_PIN:
        is_chassis_running = !is_chassis_running;
        break;
    case KEY1_PIN:
        upload_zf_gray();
        break;
    case KEY2_PIN:
        upload_zf_bin();
        break;
    default:
        break;
    }
}

static void btn_hold_event_cb(agile_btn_t *btn)
{
    rt_kprintf("[button hold event] pin:%d   hold_time:%d\n", btn->pin, btn->hold_time);
}

int key_create(void)
{
    if (key2 == RT_NULL)
    {
        key2 = agile_btn_create(KEY2_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
        agile_btn_set_event_cb(key2, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key2, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key2);
    }

    if (key0 == RT_NULL)
    {
        key0 = agile_btn_create(KEY0_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
        agile_btn_set_event_cb(key0, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key0, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key0);
    }

    if (key1 == RT_NULL)
    {
        key1 = agile_btn_create(KEY1_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
        agile_btn_set_event_cb(key1, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key1, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key1);
    }
    // rt_kprintf("board keys inited\n");

    return RT_EOK;
}
// INIT_APP_EXPORT(key_create);

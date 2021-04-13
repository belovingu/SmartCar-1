#include "headfile.h"
#include "imageProcess.h"
#include "chassisThread.h"
#include "distinguish.h"
#include "buttonThread.h"
#include "findLines.h"

#define LED1_PIN B9
#define BEEP_PIN B23

ebled_t errno_led = RT_NULL;
ebled_t info_beep = RT_NULL;

#define SERVO_PID_MAX_OUT 1000
#define DISPALY_INFO_INTERVAL 200

uint32 use_time = 0;
rt_sem_t csi_done_sem = RT_NULL;
rt_mutex_t disp_dmutex = RT_NULL;
rt_mutex_t chassis_dmutex = RT_NULL;

int is_steer_debug = RT_FALSE;
uint8_t disp_pic_type = 0;
controller_t rho_err_pid = RT_NULL;

//steer ctrl
float steer_out = 0;
float steer_basic_kp = 12;
float steer_kp_j = 0; //0.007
float steer_kp = 20;
float steer_ki = 0;
float steer_kd = 0;
float steer_kp_max = 16;

void show_tft_img()
{
    if (disp_pic_type == 0)
    {
        lcd_displayimage032(BinImage[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
    }
    else if (disp_pic_type == 1)
    {
        lcd_displayimage032(FilterImage[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
    }
    else if (disp_pic_type == 2)
    {
        lcd_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
    }
    for (int i = f2.toppoint; i < MT9V03X_CSI_H; i++)
    {
        lcd_drawpoint(f1.leftline[i], i, RED);
        lcd_drawpoint(f1.midline[i], i, BLUE);
        lcd_drawpoint(f1.rightline[i], i, GREEN);
    }
    // gui_circle(f1.midline[foresight], foresight, RED, 2, 1);
}

int main(void)
{
    csi_done_sem = rt_sem_create("csi_done_sem", 0, RT_IPC_FLAG_FIFO);
    disp_dmutex = rt_mutex_create("disp_dmutex", RT_IPC_FLAG_FIFO);
    chassis_dmutex = rt_mutex_create("chassis_dmutex", RT_IPC_FLAG_FIFO);

    errno_led = easyblink_init_led(LED1_PIN, PIN_LOW);
    easyblink(errno_led, -1, 500, 1000);
    info_beep = easyblink_init_led(BEEP_PIN, PIN_HIGH);
    easyblink(info_beep, 2, 50, 200);

    lcd_init();
    lcd_showstr(0, 0, "init...");
    mt9v03x_csi_init();
    pit_init();
    servo_init();
    chassis_thread_init_cmd();
    rho_err_pid = (controller_t)pos_pid_controller_create(
        steer_kp, steer_ki, steer_kd,
        SERVO_PID_MAX_OUT,
        -SERVO_PID_MAX_OUT, 10);
    controller_set_target(rho_err_pid, 0);

    while (1)
    {
        rt_sem_control(csi_done_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(csi_done_sem, RT_WAITING_FOREVER);

        pit_start(PIT_CH0);
        gray_mean_filter(mt9v03x_csi_image, FilterImage);
        gray_to_binary(FilterImage, BinImage, 1);
        findline1();
        findline2();
        compute_error();
        velocity_set();
        stop_car_judge();
        //stop car
        if (stop_car_flag)
        {
            speed_set = 0;
        }
        if (SWITCH_ON == switch_get(4))
            set_velocity(speed_set, steer_error_ave);

        steer_kp = steer_basic_kp + (steer_error_ave * steer_error_ave) * steer_kp_j;
        if (steer_kp >= steer_kp_max)
            steer_kp = steer_kp_max;

        controller_set_param(rho_err_pid, steer_kp, steer_ki, steer_kd);
        controller_update(rho_err_pid, steer_error[0]);
        steer_out = rho_err_pid->output;

        //set steer
        if (SWITCH_ON == switch_get(1))
            servo1_set_angle((int)steer_out);
        else
            servo1_set_angle(0);

        if (is_steer_debug)
            rt_kprintf("steer:%d,%d,%d,%.2f", use_time, speed_set, steer_error_ave, steer_out);

        use_time = pit_get_ms(PIT_CH0);
        pit_close(PIT_CH0);

        show_tft_img();
    }
}

int set_steer_pid_param_cmd(int argc, char **argv)
{
    steer_basic_kp = myatof(argv[1]);
    steer_kp_j = myatof(argv[2]);
    steer_kp_max = myatof(argv[3]);

    steer_ki = myatof(argv[4]);
    steer_kd = myatof(argv[5]);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_steer_pid_param_cmd, set_steer_pid_param_cmd(basicKp KpJ maxKp) Ki Kd)

int set_steer_debug_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    is_steer_debug = str2bool(argv[1]);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_steer_debug_cmd, set_steer_debug_cmd 0 or 1);

int set_angle_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    int servo_value = atoi(argv[1]);
    servo1_set_angle(servo_value);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_angle_cmd, set_angle(-1000 ~1000));

int set_disp_pic_type_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    disp_pic_type = atoi(argv[1]);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_disp_pic_type_cmd, set_disp_pic_type_cmd 0 / 1 / 2);

#include "headfile.h"
#include "encoder.h"
#include "easyblink.h"
#include "servo.h"
#include "motor.h"
#include "controller.h"
#include "pos_pid_controller.h"
#include "chassisThread.h"

#define DBG_SECTION_NAME "main"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define LED1_PIN B9
#define BEEP_PIN B23

ebled_t errno_led = RT_NULL;
ebled_t info_beep = RT_NULL;

#define THREAD_PRIORITY 10
#define THREAD_STACK_SIZE 2048
#define THREAD_TIMESLICE 5
#define SERVO_PID_MAX_OUT 1000
#define DISPALY_INFO_INTERVAL 200

uint32 use_time = 0;
rt_sem_t csi_done_sem = RT_NULL;
rt_mutex_t disp_dmutex = RT_NULL;
rt_mutex_t chassis_dmutex = RT_NULL;

int is_steer_debug = RT_FALSE;
controller_t rho_err_pid;
uint8_t disp_pic_type = 0;

//steer ctrl
float steer_out = 0;
float steer_error = 0;
float steer_error_l = 0;
float steer_error_ll = 0;

float steer_basic_kp = 20;
float steer_kp_j = 0.01;
float steer_kp = 20;
float steer_ki = 0;
float steer_kd = 0;
float steer_kp_max = 50;

//speed ctrl
float speed_drop_row_l;
float speed_drop_row;
float speed_set;
float speed_min = 1.0;
float speed_max = 2.2; //m/s

// float speed_high = 1.5;
// float speed_mid = 0.8;
// float speed_low = 0.4;

float dym_speed_param1 = 0.04;
float dym_speed_param2 = 0.2;
float dym_speed_param3 = 0.0;
float dym_speed_param4 = 0.01;

//variables
//static rt_tick_t disp_info_last_time;
char steer_out_str[20];
char steer_err_str[20];
int csi_cam_exp_time = 50;

float mid_line_weight[img_height] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //1-40

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //41-60

    1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3,
    1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, //61-80

    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, //81-100

    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //101-120
};

void calculate_error(void)
{
    float CenterSum = 0;
    float WeightSum = 0;
    float CenterMeanValue = 0;

    for (int i = img_height - 1; i > road_end_pos; i--)
    {
        CenterSum += mid_line[i] * mid_line_weight[i];
        WeightSum += mid_line_weight[i];
    }

    if (WeightSum != 0)
    {
        CenterMeanValue = (CenterSum / WeightSum);
    }

    steer_error_l = steer_error;
    steer_error_ll = steer_error_l;
    steer_error = (CenterMeanValue - img_width / 2);
    steer_error = limit_float(steer_error, img_width / 2);
}

void steer_kp_fix()
{
    steer_kp = steer_basic_kp + (steer_error * steer_error) * steer_kp_j;
    if (steer_kp >= steer_kp_max)
        steer_kp = steer_kp_max;
}

void dym_speed_ctrl()
{
    speed_drop_row_l = speed_drop_row;
    speed_drop_row = mid_line_end_pos;
    speed_drop_row = limit_ab_float(speed_drop_row, 0.3 * img_height, 0.9 * img_height);
    speed_set = dym_speed_param1 * speed_drop_row +
                dym_speed_param2 * (speed_drop_row - speed_drop_row_l) -
                (dym_speed_param3 * ABS(steer_error) + dym_speed_param4 * (steer_error - steer_error_l));
    speed_set = limit_ab_float(speed_set, speed_min, speed_max);
}

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
    for (int i = 0; i < MT9V03X_CSI_H; i++)
    {
        lcd_drawpoint(left_line[i], i, RED);
        lcd_drawpoint(right_line[i], i, BLUE);
        lcd_drawpoint(mid_line[i], i, GREEN);
    }

    gui_circle(img_width / 2, img_height - mid_line_end_pos, RED, 2, 0);
    gui_circle(img_width / 4, img_height - left_line_end_pos, RED, 2, 0);
    gui_circle(img_width * 3 / 4, img_height - right_line_end_pos, RED, 2, 0);
    gui_circle(img_width * 1 / 8, img_height - leftleft_line_end_pos, RED, 2, 0);
    gui_circle(img_width * 3 / 8, img_height - midleft_line_end_pos, RED, 2, 0);
    gui_circle(img_width * 5 / 8, img_height - midright_line_end_pos, RED, 2, 0);
    gui_circle(img_width * 7 / 8, img_height - rightright_line_end_pos, RED, 2, 0);

    lcd_drawline(0, road_end_pos, img_width, road_end_pos, YELLOW);
}

int main(void)
{
    csi_done_sem = rt_sem_create("csi_done_sem", 0, RT_IPC_FLAG_FIFO);
    chassis_dmutex = rt_mutex_create("chassis_dmutex", RT_IPC_FLAG_FIFO);
    disp_dmutex = rt_mutex_create("disp_dmutex", RT_IPC_FLAG_FIFO);

    errno_led = easyblink_init_led(LED1_PIN, PIN_LOW);
    easyblink(errno_led, -1, 500, 1000);

    info_beep = easyblink_init_led(BEEP_PIN, PIN_HIGH);
    easyblink(info_beep, 2, 100, 400);

    lcd_init();
    lcd_showstr(0, 0, "Initializing...");
    mt9v03x_csi_init();
    pit_init();
    servo_init();
    chassis_thread_init_cmd();
    rho_err_pid = (controller_t)pos_pid_controller_create(steer_kp, steer_ki, steer_kd,
                                                          SERVO_PID_MAX_OUT,
                                                          -SERVO_PID_MAX_OUT, 10);
    controller_set_target(rho_err_pid, 0);

    while (1)
    {
        rt_sem_control(csi_done_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(csi_done_sem, RT_WAITING_FOREVER);
        //find track
        pit_start(PIT_CH0);
        MeanFilter(mt9v03x_csi_image, FilterImage);
        Convert2Binary(FilterImage, BinImage, 1);
        ResetVarialble();
        FindEdgeLine(BinImage);
        FindMidLine(BinImage);
        CrossRoadProcess(BinImage);
        LoopRoadProcess();

        Least3Error();
        calculate_error();
        //steer ctrl
        steer_kp_fix();
        controller_set_param(rho_err_pid, steer_kp, steer_ki, steer_kd);
        controller_update(rho_err_pid, steer_error);
        steer_out = rho_err_pid->output;
        servo1_set_angle((int)steer_out);
        //dym speed
        if (StopCarJudge(BinImage))
        {
            set_velocity(0);
        }
        else
        {
            dym_speed_ctrl();
            set_velocity(speed_set);
        }

        use_time = pit_get_ms(PIT_CH0);
        pit_close(PIT_CH0);

        if (is_steer_debug)
            rt_kprintf("steer:%d,%.2f,%.2f,%.2f\r\n", use_time, steer_error, steer_out, speed_set);
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

int set_exp_time_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    csi_cam_exp_time = atoi(argv[1]);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_exp_time_cmd, set_exp_time_cmd(10 ~800));

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

int set_dym_speed_param_cmd(int argc, char **argv)
{
    dym_speed_param1 = myatof(argv[1]);
    dym_speed_param2 = myatof(argv[2]);
    dym_speed_param3 = myatof(argv[3]);
    dym_speed_param4 = myatof(argv[4]);
    speed_min = myatof(argv[5]);
    speed_max = myatof(argv[6]);
    return RT_EOK;
}
MSH_CMD_EXPORT(set_dym_speed_param_cmd, set_dym_speed_param_cmd dym_speed_param1 & 2 & 3 & 4)

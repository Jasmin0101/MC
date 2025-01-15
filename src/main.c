#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "init.h"
#include "pwm.h"
unsigned long int d = 0;

int speed[3] = {500,300,1000};

bool flag_blinker = 0;

xTaskHandle task = NULL;

void on_long_press();
void on_short_press();
void blinker1(void *params);
void blinker2(void *params);

void call_pwm(){
    // 
    uint32 io_info[][3] = { 
    {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO4,4},
    {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO0,0}, // Channel 0
    {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO2, 2}, 
    {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO14,12},
    };
    int dutys[4]={0,0,0,0};
    pwm_init(1000, dutys, 4, io_info);// dutys - %  от возможного свечения ( лежит в диапазоне от 0 до 1023)
} 


void button(){
    
    int time = 0 ; // для возможности отсчета времени с момента нажатия кнопки 
    
    bool prev_button_state = false; // предыдущее состояние кнопки 

    while (1)
    {
        uint32_t gpio_status = gpio_input_get() & BIT5;  // возращает битовую маску  умножаем на 5 бит ... ээ хз почему 

        bool current_button_state = (gpio_status & BIT5) == 0; // 0 - нажата, 1 - не нажата 

        if (current_button_state && !prev_button_state)
        {
            time = 0;
        }
    // считаем время нажатия кнопки
        if (!current_button_state && prev_button_state)
        {

            if (time < 1000)
            {
                on_short_press(); 
            }

            if (time >= 1000)
            {
                on_long_press();
            }

            time = 0;
        }

        vTaskDelay(50 / portTICK_RATE_MS);

        prev_button_state = current_button_state;
        time += 50;
    }
}

void on_short_press()
{
    d ++ ;
    printf( "Short press");
}

void on_long_press()
{   
    flag_blinker = !flag_blinker;

    if(flag_blinker == 0) {

        vTaskDelete(task);
        xTaskCreate(&blinker1, "blinker1", 2048, NULL, 1, &task);
        
    }

    if(flag_blinker == 1) {
        vTaskDelete(task);
        xTaskCreate(&blinker2, "blinker2", 2048, NULL, 1, &task);
        }

    printf( "Long press");

}

void blinker1(void *params ){
    int msc = 0 ;
    while(1){

        for (size_t i = 0; i < 4; i++)
        {   
            int duty = 0; 
        
            if ((i+d)%4 == 0){
                int step  = msc/1000;

                if ( step %2 == 0 ){
                    duty = 0 ;
                }
                if(step %2 == 1){
                    duty = 1023;
                }
            }
            if ((i +d ) % 4== 1){
                int step = msc/500; 

                if ( step %2 == 0){
                    duty = 0 ;
                }
                if(step %2 == 1 ){
                    duty = 1023;
                }
            }
            
            if ((i+d)%4== 2){
                int step = msc/500; 

                duty = 1023 * (msc%500)/500;

                if(step%2 == 1){ 
                    duty = 1023 - duty;
                }

            }
            if ((i+d)%4== 3){

                int step = msc/1000; 

                duty = 1023 * (msc%1000)/1000; 

                if(step%2 == 1){ 
                    duty = 1023 - duty;
                }

            }

            pwm_set_duty(duty,i);
        }

        pwm_start();
             
        // 
        vTaskDelay(50/portTICK_RATE_MS); 
        msc += 50;

    }
 
}

void blinker2 (void *params ){
    int msc = 0 ;
    d = 0;
    while(1){

       pwm_set_duty(0,2);
       pwm_set_duty(1023,0);
       pwm_start(); 

       vTaskDelay(speed[d%3]/portTICK_RATE_MS);

       pwm_set_duty(0,0);
       pwm_set_duty(1023,1);

       pwm_start(); 

       vTaskDelay(speed[d%3]/portTICK_RATE_MS);

       pwm_set_duty(0,1); 
       pwm_set_duty(1023,2);

       pwm_start(); 

       vTaskDelay(speed[d%3]/portTICK_RATE_MS);
        pwm_set_duty(0,2); 
       pwm_set_duty(1023,3);

       pwm_start(); 

       vTaskDelay(speed[d%3]/portTICK_RATE_MS);
        
    }

}

void user_init(void)
{
    init_uart(); // инициализация последовательного порта 
    call_pwm();
    GPIO_AS_INPUT(GPIO_Pin_5);
  
    xTaskCreate(&blinker1, "blinker1", 2048, NULL, 1, &task);
    xTaskCreate(&button, "button", 2048, NULL, 1, NULL);

}

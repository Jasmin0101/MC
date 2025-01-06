

#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "init.h"
#include "pwm.h"
unsigned long int d = 0;
int speed[3] = {500,300,1000};
bool flag_blinker = 0;
void call_pwm(){
    // 
    uint32 io_info[][3] = { 
    {PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO2, 2}, 
    {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO5,5},
  //  {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO14,14},
    {PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO0,0}, // Channel 0
    };
    int dutys[3]={0,0,0};
    pwm_init(1000, dutys, 3, io_info);// dutys - %  от возможного свечения ( лежит в диапазоне от 0 до 1023)
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button(){
    
    int time = 0 ; // для возможности отсчета времени с момента нажатия кнопки 
    
    bool prev_button_state = false; // предыдущее состояние кнопки 

    while (1)
    {
        uint32_t gpio_status = gpio_input_get() & BIT4;  // возращает битовую маску  умножаем на 5 бит ... ээ хз почему 

        bool current_button_state = (gpio_status & BIT4) == 0; // 0 - нажата, 1 - не нажата 

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
//////////////////////////////////////////////////////// 
/*
Тут надо реализовать переключение между дочерними режимами светодиодов 
*/
 //////////////////////////////////////////////////////////
void on_short_press()
{
    d ++ ;

    printf( "Short press");
}
///////////////////////////////////////////////////////////
/*
Тут надо реализовать переключение между основными режимами светодиодов 
*/
/////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////

void task_blink_1(void *ignore , int param)
{ 

    int duty =  1023;
    bool flag = 0;

    while (true)
    {   
        
        if (flag == 1){
           duty =  abs(1023 - duty);
        }
        pwm_set_duty(duty, param);
        pwm_start();

        vTaskDelay(1000/portTICK_RATE_MS);

        flag = !flag;
    
       
    }
    vTaskDelete(NULL);

}


void task_blink_2(void *ignore, int param)
{ 
    int duty = 1023;
    bool flag = 0;

    while (true)
    {
        if (flag == 1){
            duty =  abs(1023 - duty);
        }
        pwm_set_duty(duty, param);
        pwm_start();

        vTaskDelay(500/portTICK_RATE_MS);

        flag = !flag;
    }

    vTaskDelete(NULL);

}

void task_pwm_3(void *ignore,int param){

 int msc = 0 ; 
 bool flag = 0;
    while (1)
    { 
        int duty = 1023 * msc/1000;
        if (flag == 1){
            duty = 1023 - duty;
        }
        pwm_set_duty(duty, param);
        pwm_start();


    vTaskDelay(50/portTICK_RATE_MS);
    msc += 50;
    if (msc == 1000){
        msc = 0;
        flag = !flag;
    }
    }
    vTaskDelete(NULL); 
 }

void task_pwm_4(void *ignore,int param){
    int msc = 0 ; 
    bool flag = 0; 
    while (1)
    { 
        int duty = 1023 * msc/500;
        if (flag == 1){
           
            duty = 1023 - duty;
        }
        pwm_set_duty(duty,param );
        pwm_start();
        vTaskDelay(50/portTICK_RATE_MS);
        msc += 50;
        if (msc == 500){
             msc = 0;
             flag = !flag;
             }

    }
    vTaskDelete(NULL);
}



///////////////////////////////////////////////////////////

void blinker1(void *params ){
    int msc = 0 ;
    while(1){

        for (size_t i = 0; i < 3; i++)
        {   
            int duty = 0; 
        
            if ((i+d)%3 == 0){
                int step  = msc/1000;

                if ( step %2 == 0 ){
                    duty = 0 ;
                }
                if(step %2 == 1){
                    duty = 1023;
                }
            }
            if ((i +d ) % 3== 1){
                int step = msc/500; 

                if ( step %2 == 0){
                    duty = 0 ;
                }
                if(step %2 == 1 ){
                    duty = 1023;
                }
            }
            
            if ((i+d)%3== 2){
                int step = msc/500; 

                duty = 1023 * (msc%500)/500;

                if(step%2 == 1){ 
                    duty = 1023 - duty;
                }

            }
            // if (i == 3){

            //     int step = msc/1000; 

            //     duty = 1023 * (msc%1000)/1000; 

            //     if(step%2 == 1){ 
            //         duty = 1023 - duty;
            //     }

            // }

            pwm_set_duty(duty,i);
        }

        pwm_start();
             
        // 
        vTaskDelay(50/portTICK_RATE_MS); 
        msc += 50;

    }
     vTaskDelete(NULL);

}


void blinker2 (void *params ){
    int duty = 0;
    int msc = 0 ;
    while(1){
        
        for (size_t i = 0; i < 3; i++)
        {
            if(i == 0 ){
                int step  = msc/1000;

                if ( step %2 == 0 ){
                    duty = 0 ;
                }
                if(step %2 == 1){
                    duty = 1023;
                }
            }
    
            if (i == 1 ){

                int step  = msc/1000;

                if ( step %2 == 0 ){
                    duty = 0 ;
                }
                if(step %2 == 1){
                    duty = 1023;
                }

            }
             
            if (i == 2 ){

                int step  = msc/1000;

                if ( step %2 == 0 ){
                    duty = 0 ;
                }
                if(step %2 == 1){
                    duty = 1023;
                }

            }

            vTaskDelay(speed[d%3]/portTICK_RATE_MS); 
            pwm_set_duty(duty,i);
        }
        pwm_start();

        vTaskDelay(50/portTICK_RATE_MS);
        msc += 50;
        
    }

}

void on_long_press()
{
    flag_blinker = !flag_blinker;

    if(flag_blinker == 0)  xTaskCreate(&blinker1, "blinker1", 2048, NULL, 1, NULL);
    if(flag_blinker == 1)  xTaskCreate(&blinker2, "blinker2", 2048, NULL, 1, NULL);

     printf( "Long press");
   
}

void user_init(void)
{
    init_uart();
    call_pwm();
    GPIO_AS_INPUT(GPIO_Pin_4);
    //4 параметром можно передавать значения gpoi
   // xTaskCreate(&blinker1, "blinker1", 2048, NULL, 1, NULL);
    xTaskCreate(&button, "button", 2048, NULL, 1, NULL);

}
///////////////////////////////////////////////////////////////////





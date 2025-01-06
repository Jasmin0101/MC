// #include "esp_common.h"
// #include "freertos/task.h"
// #include "gpio.h"
// #include "pwm.h"

// void button_listener(void *params)
// {
//     // true - down, false - up

//     // не совсем понимаю как это работает  но  сева сказал что это можно убрать 

//     // void (*on_short_press)(void) = ((void (**)(void))params)[0];  
//     // void (*on_long_press)(void) = ((void **)params)[1];

//     int time = 0;

//     bool prev_button_state = false; 

//     while (1)
//     {
//         uint32_t gpio_status = gpio_input_get() & BIT5;  

//         bool current_button_state = (gpio_status & BIT5) == 0; // 0 - нажата, 1 - не нажата

//         if (current_button_state && !prev_button_state)
//         {
//             time = 0;
//         }
//     // считаем время нажатия кнопки
//         if (!current_button_state && prev_button_state)
//         {

//             if (time < 1000)
//             {
//                 on_short_press();
//             }

//             if (time >= 1000)
//             {
//                 on_long_press();
//             }

//             time = 0;
//         }

//         vTaskDelay(50 / portTICK_RATE_MS);

//         prev_button_state = current_button_state;
//         time += 50;
//     }
// }


// void on_short_press()
// {
// }

// void on_long_press()
// {

   
// }

// void (*button_functions[])() = {NULL, NULL};

// void user_init(void)
// {
//     init_uart();
//     init_pwm();

//     GPIO_AS_INPUT(GPIO_Pin_5);

//     button_functions[0] = on_short_press;
//     button_functions[1] = on_long_press;

//     xTaskCreate(&button_listener, "button_listener", 2048, button_functions, 5, NULL);
// }

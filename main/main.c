#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "apds9960.h"

#define APDS9960_SCL 4
#define APDS9960_SDA 5

apds9960_t apds9960;

void app_main(void)
{
    // Initialize the APDS9960 sensor
    if (apds9960_init(&apds9960, APDS9960_SCL, APDS9960_SDA) != APDS9960_SUCCESS) {
        printf("Failed to initialize APDS9960\n");
        return;
    }
    printf("APDS9960 initialized successfully\n");

    // Enable the color sensor
    if (apds9960_en_color(&apds9960, true) != APDS9960_SUCCESS) {
        printf("Failed to enable color sensor\n");
        return;
    }
    printf("Color sensor enabled successfully\n");

    // Color values
    uint8_t rgbc[4] = {0};

    while (true)
    {
        rgbc[0] = 0;
        rgbc[1] = 0;
        rgbc[2] = 0;
        rgbc[3] = 0;
        // Get color data
        if (apds9960_get_color(&apds9960, rgbc) == APDS9960_SUCCESS) {
            printf("Color Data - R: %d, G: %d, B: %d, C: %d\n", rgbc[0], rgbc[1], rgbc[2], rgbc[3]);
        } else {
            printf("Failed to get color data\n");
        }

        // Delay for a while before the next reading
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    
}
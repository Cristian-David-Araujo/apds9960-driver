#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "apds9960.h"

#define APDS9960_SCL 4
#define APDS9960_SDA 5
#define APDS9960_CLK_SPEED 100000

apds9960_t apds9960;

void app_main(void)
{
    // Initialize the APDS9960 sensor
    if (apds9960_init(&apds9960, APDS9960_SCL, APDS9960_SDA, APDS9960_CLK_SPEED, APDS9960_ADDRESS) != APDS9960_SUCCESS) {
        printf("Failed to initialize APDS9960\n");
        return;
    }
    printf("APDS9960 initialized successfully\n");

    
}
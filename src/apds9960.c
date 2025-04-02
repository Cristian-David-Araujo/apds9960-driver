#include "apds9960.h"

int8_t apds9960_init(apds9960_t *apds9960, uint8_t gpio_scl, uint8_t gpio_sda, uint32_t clk_speed_hz, uint16_t addr)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }


    // Init I2C only if it hasn't been initialized
    if (i2c_init(&apds9960->i2c, I2C_NUM_0, gpio_scl, gpio_sda, clk_speed_hz, addr) == false) {
        return APDS9960_ERROR;
    }

    // Check if the device is connected
    uint8_t id = 0;
    i2c_read_reg(&apds9960->i2c, APDS9960_ID, &id, 1);

    apds9960->id = id;
    printf("APDS9960 ID: 0x%02X\n", id);

    apds9960_set_integration_time(apds9960, 50); // Set default integration time to 50ms
    apds9960_set_gain(apds9960, APDS9960_AGAIN_4X); // Set default gain to 4x

    



    return APDS9960_SUCCESS;
}

int8_t apds9960_set_integration_time(apds9960_t *apds9960, uint16_t time_ms)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    float time = 0.0f;

    // Convert time in ms to 2.78ms steps
    if (time_ms < 2.78f) {
        time = 0.0f;
    } else if (time_ms > 256.0f) {
        time = 255.0f;
    } else {
        time = (time_ms / 2.78f) - 1.0f;
    }
    uint8_t reg_value = (uint8_t)time;
    i2c_write_reg(&apds9960->i2c, APDS9960_ATIME, &reg_value, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_gain(apds9960_t *apds9960, apds9960AGain_t gain)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    uint8_t reg_value = gain;
    i2c_write_reg(&apds9960->i2c, APDS9960_CONTROL, &reg_value, 1);
    return APDS9960_SUCCESS;
}

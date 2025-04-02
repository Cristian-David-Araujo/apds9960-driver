#include "apds9960.h"

int8_t apds9960_init(apds9960_t *apds9960, uint8_t gpio_scl, uint8_t gpio_sda, uint32_t clk_speed_hz, uint16_t addr)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Set default values
    apds9960->id = 0;
    apds9960->status = SLEEP;
    apds9960->registers.enable.WORD = 0;
    apds9960->registers.gconf3.WORD = 0;
    apds9960->registers.gconf4.WORD = 0;
    apds9960->registers.status.WORD = 0;


    // Init I2C only if it hasn't been initialized
    if (i2c_init(&apds9960->i2c, I2C_NUM_0, gpio_scl, gpio_sda, clk_speed_hz, addr) == false) {
        return APDS9960_ERROR;
    }

    // Check if the device is connected
    uint8_t id = 0;
    i2c_read_reg(&apds9960->i2c, APDS9960_ID, &id, 1);

    apds9960->id = id;
    // printf("APDS9960 ID: 0x%02X\n", id);

    apds9960_go_to(apds9960, SLEEP);

    // Read ENABLE register
    uint8_t enable = 0;
    i2c_read_reg(&apds9960->i2c, APDS9960_ENABLE, &enable, 1);

    printf("ENABLE: 0x%02X\n", enable);

    



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

int8_t apds9960_en_gesture(apds9960_t *apds9960, bool en)
{
    switch (en) {
        case true:
            // Enable gesture sensor
            i2c_write_reg(&apds9960->i2c, APDS9960_GCONF4, (uint8_t[]){0x01}, 1);
            break;
        case false:
            // Disable gesture sensor
            i2c_write_reg(&apds9960->i2c, APDS9960_GCONF4, (uint8_t[]){0x00}, 1);
            break;
    }

    return APDS9960_SUCCESS;
}

int8_t apds9960_go_to(apds9960_t *apds9960, uint8_t status)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    switch (apds9960->status)
    {
        case SLEEP:
            
            switch (status)
            {
                case IDLE:
                    apds9960->registers.enable.BITS.PON = 1;
                    apds9960->registers.enable.BITS.AEN = 1;
                    apds9960->registers.enable.BITS.WEN = 0;
                    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, &apds9960->registers.enable.WORD, 1);
                    break;
                default:
                    break;
            }

        case IDLE:
            switch (status)
            {
                case SLEEP:
                    apds9960->registers.enable.BITS.PON = 0;
                    apds9960->registers.enable.BITS.AEN = 1;
                    apds9960->registers.enable.BITS.GEN = 0;
                    apds9960->registers.gconf4.BITS.GMODE = 0;
                    apds9960->registers.gconf3.BITS.SAI = 1;
                    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, &apds9960->registers.enable.WORD, 1);
                    break;
                default:
                    break;
            }

            
        break;
    }
    return APDS9960_SUCCESS;
}

#include "apds9960.h"

int8_t apds9960_init(apds9960_t *apds9960, uint8_t gpio_scl, uint8_t gpio_sda)
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
    if (i2c_init(&apds9960->i2c, I2C_NUM_0, gpio_scl, gpio_sda, 100000, APDS9960_ADDRESS) == false) {
        return APDS9960_ERROR;
    }

    // Get ID
    uint8_t id = 0;
    i2c_read_reg(&apds9960->i2c, APDS9960_ID, &id, 1);
    apds9960->id = id;
    printf("APDS9960 ID: 0x%02X\n", id);

    if (id != 0xAB && id != 0xA8) {
        return APDS9960_ERROR;
    }

    // Disable all sensors
    apds9960_set_enable(apds9960, 0x00);
    apds9960_set_wait_time(apds9960, 2); // Set wait time to 2.78ms
    apds9960_set_gpulse(apds9960, 0x8F); // Set gesture pulse count and length - 0x8F - 16us with 16 pulses
    apds9960_set_ppulse(apds9960, 0x8F); // Set proximity pulse count and length - 0x8F - 16us with 16 pulses
    apds9960_set_gconf3(apds9960, 0x00); // Set gesture configuration 3 - 0x00 - default
    apds9960_set_gconf4(apds9960, 0x00); // Set gesture configuration 4 - 0x00 - default

    // Power on the device
    apsd9960_power_on(apds9960, true);
    // Set the wait time to 2.78ms
    apds9960_set_wait_time(apds9960, 3); // Set wait time to 2.78ms
    // Integration time to 50ms
    apds9960_set_color_integration_time(apds9960, 180); // Set integration time to 100ms

    // Set the gain to 64x
    apds9960_set_color_gain(apds9960, APDS9960_AGAIN_64X); // Set gain to 64x

    // Power on the device
    apsd9960_power_on(apds9960, true);

    // Enable color sensor
    apds9960_en_color(apds9960, true);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_color_integration_time(apds9960_t *apds9960, uint16_t time_ms)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Convert time_ms to integration time register value
    // Where 0xFF = 2.78ms, 0x00 = 711ms, step = 2.78ms
    
    uint8_t reg_value = (uint8_t)(256 - (time_ms / 2.78));

    i2c_write_reg(&apds9960->i2c, APDS9960_ATIME, &reg_value, 1);

    printf("Integration time set to: %d ms\n", time_ms);
    printf("Integration time register value: 0x%02X\n", reg_value);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_color_gain(apds9960_t *apds9960, apds9960AGain_t gain)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    uint8_t reg_value = gain;
    i2c_write_reg(&apds9960->i2c, APDS9960_CONTROL, &reg_value, 1);

    printf("Gain set to: %d\n", gain);
    printf("Gain register value: 0x%02X\n", reg_value);

    return APDS9960_SUCCESS;
}

int8_t apds9960_en_gesture(apds9960_t *apds9960, bool en)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    switch (en) {
        case true:
            // Enable gesture sensor
            apds9960->registers.enable.BITS.GEN = 1;
            break;
        case false:
            // Disable gesture sensor
            apds9960->registers.enable.BITS.GEN = 0;
            apds9960->registers.gconf4.WORD = 0;
            break;
    }
    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, (uint8_t *)&apds9960->registers.enable.WORD, 1);
    i2c_write_reg(&apds9960->i2c, APDS9960_GCONF4, (uint8_t *)&apds9960->registers.gconf4.WORD, 1);
    return APDS9960_SUCCESS;
}

int8_t apds9960_en_color(apds9960_t *apds9960, bool en)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Get the current enable register value
    uint8_t reg_value = 0;
    i2c_read_reg(&apds9960->i2c, APDS9960_ENABLE, &reg_value, 1);
    apds9960->registers.enable.WORD = reg_value;

    // Check if the color sensor is already enabled
    switch (en)
    {
        case true:
            // Check if the color sensor is already enabled
            if (apds9960->registers.enable.BITS.AEN == 1) {
                // printf("Color sensor is already enabled.\n");
                return APDS9960_SUCCESS;
            }
            else{
                // Enable color sensor without interrupts
                reg_value |= 0x02; // Set the color sensor enable bit
            }
            break;
        case false:
            // Check if the color sensor is already disabled
            if (apds9960->registers.enable.BITS.AEN == 0) {
                // printf("Color sensor is already disabled.\n");
                return APDS9960_SUCCESS;
            }
            else{
                // Disable color sensor
                reg_value &= ~0x02; // Clear the color sensor enable bit
            }
            break;
    }

    // Write the updated enable register value back to the device
    apds9960->registers.enable.WORD = reg_value;
    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, (uint8_t *)&apds9960->registers.enable.WORD, 1);
    
    return APDS9960_SUCCESS;
}

int8_t apds9960_en_proximity(apds9960_t *apds9960, bool en)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    switch (en) {
        case true:
            // Enable proximity sensor without interrupts
            apds9960->registers.enable.BITS.PIEN = 0;
            apds9960->registers.enable.BITS.PEN = 1;
            break;
        case false:
            // Disable proximity sensor
            apds9960->registers.enable.BITS.PIEN = 0;
            apds9960->registers.enable.BITS.PEN = 0;
            break;
    }

    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, (uint8_t *)&apds9960->registers.enable.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_en_wait(apds9960_t *apds9960, bool en)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    switch (en) {
        case true:
            // Enable wait sensor without interrupts
            apds9960->registers.enable.BITS.WEN = 1;
            break;
        case false:
            // Disable wait sensor
            apds9960->registers.enable.BITS.WEN = 0;
            break;
    }

    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, (uint8_t *)&apds9960->registers.enable.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apsd9960_power_on(apds9960_t *apds9960, bool en)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    uint8_t reg_value = 0;

    // Get the current enable register value
    i2c_read_reg(&apds9960->i2c, APDS9960_ENABLE, &reg_value, 1);

    // Print current enable register value
    printf("Current Enable Register: 0x%02X\n", reg_value);
    // Update the enable register
    apds9960->registers.enable.WORD = reg_value;

    if (en) {
        // Power on the device if not already powered on
        if (!(reg_value & 0x01)) {
            reg_value |= 0x01; // Set the power on bit
        }
        else {
            // If already powered on, do nothing
            printf("Device is already powered on.\n");
            return APDS9960_SUCCESS;
        }
    } else {
        // Power off the device if currently powered on
        if (reg_value & 0x01) {
            reg_value &= ~0x01; // Clear the power on bit
        }
        else {
            // If already powered off, do nothing
            printf("Device is already powered off.\n");
            return APDS9960_SUCCESS;
        }
    }
    
    // Write the updated enable register value back to the device
    apds9960->registers.enable.WORD = reg_value;

    printf("Updated Enable Register: 0x%02X\n", apds9960->registers.enable.WORD);

    // Set the power on bit
    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, &apds9960->registers.enable.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_get_color(apds9960_t *apds9960, uint8_t *rgbc)
{
    if (apds9960 == NULL || rgbc == NULL) {
        return APDS9960_ERROR;
    }

    // Check if data is valid
    apds9960_status_reg_t status;
    
    i2c_read_reg(&apds9960->i2c, APDS9960_STATUS, &status, 1);
    
    // Print status register value
    // printf("Status Register: 0x%02X\n", status.WORD);

    // Check if ALS data is valid
    if (status.BITS.AVALID == 0) {
        return APDS9960_ERROR;
    }

    // Read RGBC data data of 16 bits
    uint8_t data[8] = {0};
    i2c_read_reg(&apds9960->i2c, APDS9960_CDATAL, data, 8);


    // Print RGBC data
    // printf("RGBC Data: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("0x%02X ", data[i]);
    // }
    // printf("\n");

    // Convert 16 bits to 8 bits (0-255) mapping by scaling
    rgbc[0] = ((data[3] << 8) | data[2]) >> 8; // Red
    rgbc[1] = ((data[5] << 8) | data[4]) >> 8; // Green
    rgbc[2] = ((data[7] << 8) | data[6]) >> 8; // Blue
    rgbc[3] = ((data[1] << 8) | data[0]) >> 8; // Clear
    

    return APDS9960_SUCCESS;
}

int8_t apds9960_get_proximity(apds9960_t *apds9960, uint8_t *proximity)
{
    if (apds9960 == NULL || proximity == NULL) {
        return APDS9960_ERROR;
    }

    // Check if proximity data is valid
    apds9960_status_reg_t status;
    
    i2c_read_reg(&apds9960->i2c, APDS9960_STATUS, &status, 1);
    
    // Print status register value
    // printf("Status Register: 0x%02X\n", status.WORD);

    // Check if proximity data is valid
    if (status.BITS.PVALID == 0) {
        return APDS9960_ERROR;
    }

    // Read proximity data of 8 bits
    uint8_t data[2] = {0};
    i2c_read_reg(&apds9960->i2c, APDS9960_PDATA, data, 2);

    // Print proximity data
    // printf("Proximity Data: 0x%02X\n", data[1]);

    *proximity = data[1]; // Proximity

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_enable(apds9960_t *apds9960, uint8_t enable)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Set the enable register value
    apds9960->registers.enable.WORD = enable;

    // Write the updated enable register value back to the device
    i2c_write_reg(&apds9960->i2c, APDS9960_ENABLE, (uint8_t *)&apds9960->registers.enable.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_wait_time(apds9960_t *apds9960, uint8_t time_ms)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Convert time_ms to wait time register value
    // Where 0xFF = 2.78ms, 0x00 = 711ms, step = 2.78ms
    
    uint8_t reg_value = (uint8_t)(256 - (time_ms / 2.78));

    i2c_write_reg(&apds9960->i2c, APDS9960_WTIME, &reg_value, 1);

    printf("Wait time set to: %d ms\n", time_ms);
    printf("Wait time register value: 0x%02X\n", reg_value);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_gpulse(apds9960_t *apds9960, uint8_t gpulse)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Set the gesture pulse count and length
    apds9960->registers.gpulse.WORD = gpulse;

    // Write the updated gesture pulse register value back to the device
    i2c_write_reg(&apds9960->i2c, APDS9960_GPULSE, (uint8_t *)&apds9960->registers.gpulse.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_ppulse(apds9960_t *apds9960, uint8_t ppulse)
{
    if (apds9960 == NULL) {
        return APDS9960_ERROR;
    }

    // Set the proximity pulse count and length
    apds9960->registers.ppulse.WORD = ppulse;

    // Write the updated proximity pulse register value back to the device
    i2c_write_reg(&apds9960->i2c, APDS9960_PPULSE, (uint8_t *)&apds9960->registers.ppulse.WORD, 1);

    return APDS9960_SUCCESS;
}

int8_t apds9960_set_gconf3(apds9960_t *apds9960, uint8_t gconf3)
{
    return 0;
}

int8_t apds9960_set_gconf4(apds9960_t *apds9960, uint8_t gconf4)
{
    return 0;
}

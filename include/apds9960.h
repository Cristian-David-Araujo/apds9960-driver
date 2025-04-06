#ifndef APDS9960_H
#define APDS9960_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "platform_esp32s3.h"

#define APDS9960_ADDRESS (0x39) /**< I2C Address */

/** Enable Register flields */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t PON : 1;  /**< Power on */
    uint8_t AEN : 1;  /**< ALS Enable */
    uint8_t PEN : 1;  /**< Proximity Enable */
    uint8_t WEN : 1;  /**< Wait Enable */
    uint8_t AIEN : 1; /**< ALS Interrupt Enable */
    uint8_t PIEN : 1; /**< Proximity Interrupt Enable */
    uint8_t GEN : 1;  /**< Gesture Enable */
    uint8_t RESERVED : 1; /**< Reserved */
  } BITS;
} apds9960_enable_reg_t;

/** Gesture Configuration Registers fields */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t PMASK_R : 1; /**< Right Photodiode Select */
    uint8_t PMASK_L : 1; /**< Left Photodiode Select */
    uint8_t PMASK_D : 1; /**< Down Photodiode Select */
    uint8_t PMASK_U : 1; /**< Up Photodiode Select */
    uint8_t SAI : 1;     /**< Sleep After Interrupt */
    uint8_t PCMP : 1;    /**< Proximity Gain Compensation Enable */
    uint8_t RESERVED : 2; /**< Reserved */
  } BITS;
} apds9960_gconf3_reg_t;

typedef union {
  uint8_t WORD;
  struct {
    uint8_t GMODE : 1; /**< Gesture Mode */
    uint8_t GIEN : 1;  /**< Gesture Interrupt Enable */
    uint8_t RESERVEC : 6; /**< Reserved */
  } BITS;
} apds9960_gconf4_reg_t;

/** Status Register fields */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t AVALID : 1; /**< ALS Valid */
    uint8_t PVALID : 1; /**< Proximity Valid */
    uint8_t GINT : 1;   /**< Gesture Interrupt */
    uint8_t RESERVED : 1; /**< Reserved */
    uint8_t AINT : 1;   /**< ALS Interrupt */
    uint8_t PINT : 1;   /**< Proximity Interrupt */
    uint8_t PGSAT : 1;  /**< Proximity State */
    uint8_t CPSAT : 1;  /**< Clear Photodiode Saturation */
  } BITS;
} apds9960_status_reg_t;

/** Wait time register */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t WTIME : 8; /**< Wait time */
  } BITS;
} apds9960_wtime_reg_t;

/** Gesture Pulse Count and Length Register */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t GPULSE : 6; /**< Gesture Pulse Count */
    uint8_t GPLEN : 2; /**< Gesture Pulse Length */
  } BITS;
} apds9960_gpulse_reg_t;

/** Proximity Pulse Count Register */
typedef union {
  uint8_t WORD;
  struct {
    uint8_t PPULSE : 6; /**< Proximity Pulse Count */
    uint8_t PPLEN : 2; /**< Proximity Pulse Length */
  } BITS;
} apds9960_ppulse_reg_t;

/** I2C Registers */
enum {
  APDS9960_RAM = 0x00,
  APDS9960_ENABLE = 0x80,
  APDS9960_ATIME = 0x81,
  APDS9960_WTIME = 0x83,
  APDS9960_AILTIL = 0x84,
  APDS9960_AILTH = 0x85,
  APDS9960_AIHTL = 0x86,
  APDS9960_AIHTH = 0x87,
  APDS9960_PILT = 0x89,
  APDS9960_PIHT = 0x8B,
  APDS9960_PERS = 0x8C,
  APDS9960_CONFIG1 = 0x8D,
  APDS9960_PPULSE = 0x8E,
  APDS9960_CONTROL = 0x8F,
  APDS9960_CONFIG2 = 0x90,
  APDS9960_ID = 0x92,
  APDS9960_STATUS = 0x93,
  APDS9960_CDATAL = 0x94,
  APDS9960_CDATAH = 0x95,
  APDS9960_RDATAL = 0x96,
  APDS9960_RDATAH = 0x97,
  APDS9960_GDATAL = 0x98,
  APDS9960_GDATAH = 0x99,
  APDS9960_BDATAL = 0x9A,
  APDS9960_BDATAH = 0x9B,
  APDS9960_PDATA = 0x9C,
  APDS9960_POFFSET_UR = 0x9D,
  APDS9960_POFFSET_DL = 0x9E,
  APDS9960_CONFIG3 = 0x9F,
  APDS9960_GPENTH = 0xA0,
  APDS9960_GEXTH = 0xA1,
  APDS9960_GCONF1 = 0xA2,
  APDS9960_GCONF2 = 0xA3,
  APDS9960_GOFFSET_U = 0xA4,
  APDS9960_GOFFSET_D = 0xA5,
  APDS9960_GOFFSET_L = 0xA7,
  APDS9960_GOFFSET_R = 0xA9,
  APDS9960_GPULSE = 0xA6,
  APDS9960_GCONF3 = 0xAA,
  APDS9960_GCONF4 = 0xAB,
  APDS9960_GFLVL = 0xAE,
  APDS9960_GSTATUS = 0xAF,
  APDS9960_IFORCE = 0xE4,
  APDS9960_PICLEAR = 0xE5,
  APDS9960_CICLEAR = 0xE6,
  APDS9960_AICLEAR = 0xE7,
  APDS9960_GFIFO_U = 0xFC,
  APDS9960_GFIFO_D = 0xFD,
  APDS9960_GFIFO_L = 0xFE,
  APDS9960_GFIFO_R = 0xFF,
};

/** ADC gain settings */
typedef enum {
  APDS9960_AGAIN_1X = 0x00,  /**< No gain */
  APDS9960_AGAIN_4X = 0x01,  /**< 2x gain */
  APDS9960_AGAIN_16X = 0x02, /**< 16x gain */
  APDS9960_AGAIN_64X = 0x03  /**< 64x gain */
} apds9960AGain_t;

/** Proxmity gain settings */
typedef enum {
  APDS9960_PGAIN_1X = 0x00, /**< 1x gain */
  APDS9960_PGAIN_2X = 0x01, /**< 2x gain */
  APDS9960_PGAIN_4X = 0x02, /**< 4x gain */
  APDS9960_PGAIN_8X = 0x03  /**< 8x gain */
} apds9960PGain_t;

/** Pulse length settings */
typedef enum {
  APDS9960_PPULSELEN_4US = 0x00,  /**< 4uS */
  APDS9960_PPULSELEN_8US = 0x01,  /**< 8uS */
  APDS9960_PPULSELEN_16US = 0x02, /**< 16uS */
  APDS9960_PPULSELEN_32US = 0x03  /**< 32uS */
} apds9960PPulseLen_t;

/** LED drive settings */
typedef enum {
  APDS9960_LEDDRIVE_100MA = 0x00, /**< 100mA */
  APDS9960_LEDDRIVE_50MA = 0x01,  /**< 50mA */
  APDS9960_LEDDRIVE_25MA = 0x02,  /**< 25mA */
  APDS9960_LEDDRIVE_12MA = 0x03   /**< 12.5mA */
} apds9960LedDrive_t;

/** LED boost settings */
typedef enum {
  APDS9960_LEDBOOST_100PCNT = 0x00, /**< 100% */
  APDS9960_LEDBOOST_150PCNT = 0x01, /**< 150% */
  APDS9960_LEDBOOST_200PCNT = 0x02, /**< 200% */
  APDS9960_LEDBOOST_300PCNT = 0x03  /**< 300% */
} apds9960LedBoost_t;

/** Dimensions */
enum {
  APDS9960_DIMENSIONS_ALL = 0x00,        // All dimensions
  APDS9960_DIMENSIONS_UP_DOWN = 0x01,    // Up/Down dimensions
  APDS9960_DIMENSIONS_LEFT_RIGHT = 0x02, // Left/Right dimensions
};

/** FIFO Interrupts */
enum {
  APDS9960_GFIFO_1 = 0x00,  // Generate interrupt after 1 dataset in FIFO
  APDS9960_GFIFO_4 = 0x01,  // Generate interrupt after 2 datasets in FIFO
  APDS9960_GFIFO_8 = 0x02,  // Generate interrupt after 3 datasets in FIFO
  APDS9960_GFIFO_16 = 0x03, // Generate interrupt after 4 datasets in FIFO
};

/** Gesture Gain */
enum {
  APDS9960_GGAIN_1 = 0x00, // Gain 1x
  APDS9960_GGAIN_2 = 0x01, // Gain 2x
  APDS9960_GGAIN_4 = 0x02, // Gain 4x
  APDS9960_GGAIN_8 = 0x03, // Gain 8x
};

/** Pulse Lenghts */
enum {
  APDS9960_GPULSE_4US = 0x00,  // Pulse 4us
  APDS9960_GPULSE_8US = 0x01,  // Pulse 8us
  APDS9960_GPULSE_16US = 0x02, // Pulse 16us
  APDS9960_GPULSE_32US = 0x03, // Pulse 32us
};

#define APDS9960_UP 0x01    /**< Gesture Up */
#define APDS9960_DOWN 0x02  /**< Gesture Down */
#define APDS9960_LEFT 0x03  /**< Gesture Left */
#define APDS9960_RIGHT 0x04 /**< Gesture Right */

#define APDS9960_SUCCESS 0
#define APDS9960_ERROR -1


/** APDS9960 device structure */
enum {
    SLEEP = 0x00, /**< Sleep mode */
    IDLE = 0x01,  /**< Idle mode */
    CONTROL = 0x03, /**< Control mode */
    WAIT = 0x08, /**< Wait mode */
    PROXIMITY = 0x09, /**< Proximity mode */
    GESTURE = 0x0A, /**< Gesture mode */ 
};

typedef struct {
  apds9960_enable_reg_t enable;
  apds9960_gconf3_reg_t gconf3;
  apds9960_gconf4_reg_t gconf4;
  apds9960_status_reg_t status;
  apds9960_wtime_reg_t wtime;
  apds9960_gpulse_reg_t gpulse;
  apds9960_ppulse_reg_t ppulse;
} registers_t;

typedef struct {
    uint8_t id; // Device ID
    uint8_t color[4]; // Color data (RGBA)
    i2c_t i2c; // I2C handle
    uint8_t status; // Device status
    registers_t registers; // Control register (Enable register)

} apds9960_t;



/**
 * @brief Initialize the APDS9960 sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gpio_scl GPIO SCL pin number
 * @param gpio_sda GPIO SDA pin number
 * @return int8_t 
 */
int8_t apds9960_init(apds9960_t *apds9960, uint8_t gpio_scl, uint8_t gpio_sda);

/**
 * @brief Set the color integration time
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param time_ms Time in milliseconds (2.78ms to 711ms)
 * @note The time is set in 2.78ms steps, so the maximum time is 0xFF = 711ms
 * @return int8_t If successful, return 0, otherwise return -1
 */ 
int8_t apds9960_set_color_integration_time(apds9960_t *apds9960, uint16_t time_ms);

/**
 * @brief Set the color gain
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gain Gain value (1x, 4x, 16x, 64x)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_color_gain(apds9960_t *apds9960, apds9960AGain_t gain);

/**
 * @brief Enable or disable the gesture sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param en Enable or disable the gesture sensor (true/false)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_en_gesture(apds9960_t *apds9960, bool en);

/**
 * @brief Enable or disable the color sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param en Enable or disable the color sensor (true/false)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_en_color(apds9960_t *apds9960, bool en);

/**
 * @brief Enable or disable the proximity sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param en Enable or disable the proximity sensor (true/false)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_en_proximity(apds9960_t *apds9960, bool en);

/**
 * @brief Enable or disable the wait sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param en Enable or disable the wait sensor (true/false)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_en_wait(apds9960_t *apds9960, bool en);

/**
 * @brief Enable or disable the power on the device
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param en Enable or disable the power (true/false)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apsd9960_power_on(apds9960_t *apds9960, bool en);

/**
 * @brief Get the color data from the sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param rgbc Pointer to the array to store the color data (RGBA)
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_get_color(apds9960_t *apds9960, uint8_t *rgbc);

/**
 * @brief Get the proximity data from the sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param proximity Pointer to the variable to store the proximity data
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_get_proximity(apds9960_t *apds9960, uint8_t *proximity);

/**
 * @brief Set the gesture pulse count and length
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gpulse Gesture pulse count and length
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_enable(apds9960_t *apds9960, uint8_t enable);

/**
 * @brief Set the wait time for the sensor
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param time_ms Time in milliseconds (2.78ms to 711ms)
 * @note The time is set in 2.78ms steps, so the maximum time is 0xFF = 711ms
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_wait_time(apds9960_t *apds9960, uint8_t time_ms);

/**
 * @brief Set the gesture pulse count and length
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gpulse Gesture pulse count and length
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_gpulse(apds9960_t *apds9960, uint8_t gpulse);

/**
 * @brief Set the proximity pulse count and length
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param ppulse Proximity pulse count and length
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_ppulse(apds9960_t *apds9960, uint8_t ppulse);

/**
 * @brief Set the gesture configuration 3 register
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gconf3 Gesture configuration 3 register value
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_gconf3(apds9960_t *apds9960, uint8_t gconf3);

/**
 * @brief Set the gesture configuration 4 register
 * 
 * @param apds9960 Handle to the APDS9960 device structure
 * @param gconf4 Gesture configuration 4 register value
 * @return int8_t If successful, return 0, otherwise return -1
 */
int8_t apds9960_set_gconf4(apds9960_t *apds9960, uint8_t gconf4);
#endif // APDS9960_H
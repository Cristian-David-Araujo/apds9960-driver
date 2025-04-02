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
} registers_t;

typedef struct {
    uint8_t id; // Device ID
    uint8_t color[4]; // Color data (RGBA)
    i2c_t i2c; // I2C handle
    uint8_t status; // Device status
    registers_t registers; // Control register (Enable register)

} apds9960_t;



int8_t apds9960_init(apds9960_t *apds9960, uint8_t gpio_scl, uint8_t gpio_sda, uint32_t clk_speed_hz, uint16_t addr);

int8_t apds9960_set_integration_time(apds9960_t *apds9960, uint16_t time_ms);

int8_t apds9960_set_gain(apds9960_t *apds9960, apds9960AGain_t gain);

int8_t apds9960_en_gesture(apds9960_t *apds9960, bool en);

int8_t apds9960_go_to(apds9960_t *apds9960, uint8_t status);
#endif // APDS9960_H
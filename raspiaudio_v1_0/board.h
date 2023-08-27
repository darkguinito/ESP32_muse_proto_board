#ifndef _RASPIAUDIO_PROTO_BOARD_H_
#define _RASPIAUDIO_PROTO_BOARD_H_

#include "audio_hal.h"
#include "board_def.h"
#include "board_pins_config.h"
#include "esp_peripherals.h"
#include "periph_sdcard.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SPI_MOSI 15
#define SPI_MISO 2
#define SPI_CLK  14

#define DAC_I2S_BCK 5
#define DAC_I2S_WS  25
#define DAC_I2S_DO  26
#define DAC_I2S_DI  35
#define DAC_I2S_I2C 16
#define DAC_I2S_SDA 18
#define DAC_I2S_SCL 23
#define DAC_I2S_MCK 0

/**
 * @brief Audio board handle
 */
struct audio_board_handle {
    audio_hal_handle_t audio_hal;    /*!< pa hardware abstract layer handle */
    audio_hal_handle_t adc_hal;      /*!< adc hardware abstract layer handle */
};

typedef struct audio_board_handle *audio_board_handle_t;

/**
 * @brief Initialize audio board
 *
 * @return The audio board handle
 */
audio_board_handle_t audio_board_init(void);

/**
 * @brief Initialize codec
 *
 * @return The audio hal handle
 */
audio_hal_handle_t audio_board_codec_init(void);

/**
 * @brief Initialize adc
 *
 * @return The adc hal handle
 */
audio_hal_handle_t audio_board_adc_init(void);

/**
 * @brief Initialize key peripheral
 *
 * @param set The handle of esp_periph_set_handle_t
 *
 * @return
 *     - ESP_OK, success
 *     - Others, fail
 */
esp_err_t audio_board_key_init(esp_periph_set_handle_t set);

/**
 * @brief Initialize sdcard peripheral
 *
 * @param set The handle of esp_periph_set_handle_t
 *
 * @return
 *     - ESP_OK, success
 *     - Others, fail
 */
esp_err_t audio_board_sdcard_init(esp_periph_set_handle_t set, periph_sdcard_mode_t mode);

/**
 * @brief Query audio_board_handle
 *
 * @return The audio board handle
 */
audio_board_handle_t audio_board_get_handle(void);

/**
 * @brief Uninitialize the audio board
 *
 * @param audio_board The handle of audio board
 *
 * @return  0       success,
 *          others  fail
 */
esp_err_t audio_board_deinit(audio_board_handle_t audio_board);

#ifdef __cplusplus
}
#endif

#endif

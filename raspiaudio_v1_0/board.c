#include "esp_log.h"
#include "board.h"
#include "audio_mem.h"

#include "periph_sdcard.h"
#include "periph_adc_button.h"

static const char *TAG = "RASPIAUDIO_MUSE_PROTO";

static audio_board_handle_t board_handle = 0;

audio_board_handle_t audio_board_init(void) {
    if (board_handle) {
        ESP_LOGW(TAG, "The board has already been initialized!");
        return board_handle;
    }
    board_handle = (audio_board_handle_t) audio_calloc(1, sizeof(struct audio_board_handle));
    AUDIO_MEM_CHECK(TAG, board_handle, return NULL);
    board_handle->audio_hal = audio_board_codec_init();
    return board_handle;
}

audio_hal_handle_t audio_board_codec_init(void) {
    audio_hal_codec_config_t audio_codec_cfg = AUDIO_CODEC_DEFAULT_CONFIG();

    ESP_LOGW(TAG, "audio_board_codec_init!");
    audio_hal_handle_t codec_hal = audio_hal_init(&audio_codec_cfg, &AUDIO_CODEC_RASPIAUDIO_DEFAULT_HANDLE);
    AUDIO_NULL_CHECK(TAG, codec_hal, return NULL);
    return codec_hal;
}

esp_err_t audio_board_key_init(esp_periph_set_handle_t set) {
    return ESP_OK;
}

esp_err_t audio_board_sdcard_init(esp_periph_set_handle_t set, periph_sdcard_mode_t mode) {
    return ESP_OK;
}

audio_board_handle_t audio_board_get_handle(void) {
    return board_handle;
}

esp_err_t audio_board_deinit(audio_board_handle_t audio_board)
{
    esp_err_t ret = ESP_OK;
    ret |= audio_hal_deinit(audio_board->audio_hal);
    free(audio_board);
    board_handle = NULL;
    return ret;
}

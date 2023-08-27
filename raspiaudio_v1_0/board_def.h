#ifndef _RASPIAUDIO_PROTO_BOARD_DEFINITION_H_
#define _RASPIAUDIO_PROTO_BOARD_DEFINITION_H_

#define BUTTON_VOLUP_ID           0
#define BUTTON_VOLDOWN_ID         1
#define BUTTON_MUTE_ID            2
#define BUTTON_SET_ID             3

#define PA_ENABLE_GPIO            GPIO_NUM_12
#define ADC_DETECT_GPIO           GPIO_NUM_36
#define BATTERY_DETECT_GPIO       GPIO_NUM_37

#define SDCARD_OPEN_FILE_NUM_MAX  5
#define SDCARD_INTR_GPIO          GPIO_NUM_34

#define BOARD_PA_GAIN             (10) /* Power amplifier gain defined by board (dB) */
#define ESP_SD_PIN_CLK              -1
#define ESP_SD_PIN_CMD              -1
#define ESP_SD_PIN_D0               -1
#define ESP_SD_PIN_D3               -1

extern audio_hal_func_t AUDIO_CODEC_RASPIAUDIO_DEFAULT_HANDLE;

#define AUDIO_CODEC_DEFAULT_CONFIG(){                   \
        .adc_input  = AUDIO_HAL_ADC_INPUT_LINE1,        \
        .dac_output = AUDIO_HAL_DAC_OUTPUT_ALL,         \
        .codec_mode = AUDIO_HAL_CODEC_MODE_BOTH,        \
        .i2s_iface = {                                  \
            .mode = AUDIO_HAL_MODE_SLAVE,               \
            .fmt = AUDIO_HAL_I2S_NORMAL,                \
            .samples = AUDIO_HAL_48K_SAMPLES,           \
            .bits = AUDIO_HAL_BIT_LENGTH_16BITS,        \
        },                                              \
};

#define INPUT_KEY_NUM     4

#define INPUT_KEY_DEFAULT_INFO() {}
    // {                                                  
    //     .type = PERIPH_ID_ADC_BTN,                     
    //     .user_id = INPUT_KEY_USER_ID_VOLUP,            
    //     .act_id = BUTTON_VOLUP_ID,                     
    // },                                                 
    // {                                                  
    //     .type = PERIPH_ID_ADC_BTN,                     
    //     .user_id = INPUT_KEY_USER_ID_VOLDOWN,          
    //     .act_id = BUTTON_VOLDOWN_ID,                   
    // },                                                 
    // {                                                  
    //     .type = PERIPH_ID_ADC_BTN,                     
    //     .user_id = INPUT_KEY_USER_ID_MUTE,             
    //     .act_id = BUTTON_MUTE_ID,                      
    // },                                                 
    // {                                                  
    //     .type = PERIPH_ID_ADC_BTN,                     
    //     .user_id = INPUT_KEY_USER_ID_SET,              
    //     .act_id = BUTTON_SET_ID,                       
    // },                                                 
   // }

#endif

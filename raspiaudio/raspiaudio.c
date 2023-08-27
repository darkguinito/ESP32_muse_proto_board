//
// raspiaudio Muse Proto
//

#include "raspiaudio.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_system.h>

#include "audio_hal.h"
#include "esp_types.h"

#include "board.h" 

#define IIC_PORT            I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ  250000           /*!< I2C master clock frequency */

#define ACK_CHECK_EN        0x1              /*!< I2C master will check ack from slave*/
#define ACK_VAL             0x0              /*!< I2C ack value */

#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE 0  /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE 0  /*!< I2C master do not need buffer */

static const char TAG[] = "muse_proto";  
static int8_t m_volume = 10;

#define ES8388_ADDR 0x20

audio_hal_func_t AUDIO_CODEC_RASPIAUDIO_DEFAULT_HANDLE = {
  .audio_codec_initialize = raspiaudio_init,
  .audio_codec_deinitialize = raspiaudio_deinit,
  .audio_codec_ctrl = raspiaudio_ctrl,
  .audio_codec_config_iface = raspiaudio_config_iface,
  .audio_codec_set_volume = raspiaudio_set_volume,
  .audio_codec_get_volume = raspiaudio_get_volume,
};

esp_err_t raspiaudio_deinit(void) {
  i2c_driver_delete(IIC_PORT);
  return ESP_OK;
}

esp_err_t raspiaudio_ctrl(audio_hal_codec_mode_t mode,
                          audio_hal_ctrl_t       ctrl_state) {
  return ESP_OK;
}

esp_err_t raspiaudio_config_iface(audio_hal_codec_mode_t       mode, 
                                  audio_hal_codec_i2s_iface_t *iface) {
  esp_err_t res = ESP_OK;
  // Seem useless
  int bits = 0;
  int fmat = 0;
  int sample = 0;
  uint16_t regval = 0;
  switch(iface->bits)  
  {
  case AUDIO_HAL_BIT_LENGTH_16BITS:
    bits = BIT_LENGTH_16_BITS;
    break;
  case AUDIO_HAL_BIT_LENGTH_24BITS:
    bits = BIT_LENGTH_24_BITS;
    break;
  default:
    bits = BIT_LENGTH_16_BITS;
  }

  switch(iface->fmt)
  {
  case AUDIO_HAL_I2S_NORMAL:
    fmat = 0x0;
    break;
  case AUDIO_HAL_I2S_LEFT:
    fmat = 0x01;
    break;
  case AUDIO_HAL_I2S_RIGHT:
    fmat = 0x02;
    break;
  case AUDIO_HAL_I2S_DSP:
    fmat = 0x03;
    break;
  default:
    fmat = 0x00;
    break;
  }

  switch(iface->samples)
  {
  case AUDIO_HAL_08K_SAMPLES:
    sample = SIMPLE_RATE_8000;
    break;
  case AUDIO_HAL_11K_SAMPLES:
    sample = SIMPLE_RATE_11052;
    break;
  case AUDIO_HAL_16K_SAMPLES:
    sample = SIMPLE_RATE_16000;
    break;
  case AUDIO_HAL_22K_SAMPLES:
    sample = SIMPLE_RATE_22050;
    break;
  case AUDIO_HAL_24K_SAMPLES:
    sample = SIMPLE_RATE_24000;
    break;
  case AUDIO_HAL_32K_SAMPLES:
    sample = SIMPLE_RATE_32000;
    break;
  case AUDIO_HAL_44K_SAMPLES:
    sample = SIMPLE_RATE_44100;
    break;
  case AUDIO_HAL_48K_SAMPLES:
    sample = SIMPLE_RATE_48000;
    break;
  default:
    sample = SIMPLE_RATE_44100;
  }

  regval = raspiaudio_Read_Reg(I2S1LCK_CTRL);
  regval &= 0xffc3;
  regval |= (iface->mode << 15);
  regval |= (bits << 4);
  regval |= (fmat << 2);
  res |= raspiaudio_Write_Reg(I2S1LCK_CTRL, regval);
  res |= raspiaudio_Write_Reg(I2S_SR_CTRL, sample);
  //return ESP_OK;
  return res;
}

esp_err_t raspiaudio_set_volume(int vol) {
  m_volume = vol;
  return ESP_OK;
}

esp_err_t raspiaudio_get_volume(int *vol) {
  *vol = m_volume;
  return ESP_OK;
}

static i2c_config_t i2c_config = { 
  .mode             = I2C_MODE_MASTER,
  .sda_io_num       = GPIO_NUM_21,
  .sda_pullup_en    = GPIO_PULLDOWN_DISABLE,
  .scl_io_num       = GPIO_NUM_22,
  .scl_pullup_en    = GPIO_PULLDOWN_DISABLE,
  .master.clk_speed = I2C_MASTER_FREQ_HZ,
};

static int i2c_init() {
  esp_err_t ret = ESP_OK;
  ret |= i2c_param_config(IIC_PORT, &i2c_config);
  ret |= i2c_driver_install(IIC_PORT, i2c_config.mode, I2C_EXAMPLE_MASTER_RX_BUF_DISABLE, I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
  return ret;
}
    
esp_err_t raspiaudio_init(audio_hal_codec_config_t *codec_cfg) {  
  esp_err_t ret = ESP_OK;
  if(i2c_init()) {
     ESP_LOGE(TAG, "i2c_init error"); 
     return -1;
  }

  // provides MCLK
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
  WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL)& 0xFFFFFFF0);

  // reset
  ret |= raspiaudio_Write_Reg(0, 128);
  ret |= raspiaudio_Write_Reg(0, 0);
  ret |= raspiaudio_Write_Reg(25, 4);
  ret |= raspiaudio_Write_Reg(1, 80);
  ret |= raspiaudio_Write_Reg(2, 0);
  ret |= raspiaudio_Write_Reg(8, 0);
  ret |= raspiaudio_Write_Reg(4, 192);
  ret |= raspiaudio_Write_Reg(0, 18);
  ret |= raspiaudio_Write_Reg(1, 0);
  ret |= raspiaudio_Write_Reg(23, 24);
  ret |= raspiaudio_Write_Reg(24, 2);
  ret |= raspiaudio_Write_Reg(38, 9);
  ret |= raspiaudio_Write_Reg(39, 144);
  ret |= raspiaudio_Write_Reg(42, 144);
  ret |= raspiaudio_Write_Reg(43, 128);
  ret |= raspiaudio_Write_Reg(45, 128);
  ret |= raspiaudio_Write_Reg(27, 0);
  ret |= raspiaudio_Write_Reg(26, 0);
  ret |= raspiaudio_Write_Reg(2, 240);
  ret |= raspiaudio_Write_Reg(2, 0);
  ret |= raspiaudio_Write_Reg(29, 28);
  ret |= raspiaudio_Write_Reg(4, 48);
  ret |= raspiaudio_Write_Reg(25, 0);
  ret |= raspiaudio_Write_Reg(46, 33);
  ret |= raspiaudio_Write_Reg(47, 33);

  return (ret == ESP_OK);
}

esp_err_t raspiaudio_Write_Reg(uint8_t reg, uint16_t val){
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  esp_err_t ret = 0;
  uint8_t send_buff[4];

  send_buff[0] = (ES8388_ADDR << 1);
  send_buff[1] = reg;
  send_buff[2] = (val>>8) & 0xff;
  send_buff[3] = val & 0xff;

  ret |= i2c_master_start(cmd);
  ret |= i2c_master_write(cmd, send_buff, 4, ACK_CHECK_EN);
  ret |= i2c_master_stop(cmd);
  ret |= i2c_master_cmd_begin(IIC_PORT, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  return ret;
}

uint16_t raspiaudio_Read_Reg(uint8_t reg) {
  uint16_t val = 0;
  uint8_t size = 2;
  uint8_t data_rd[2];
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ES8388_ADDR << 1 ) | I2C_MASTER_WRITE, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ES8388_ADDR << 1 ) | I2C_MASTER_READ, ACK_CHECK_EN);
  i2c_master_read(cmd, data_rd, size, ACK_VAL);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(IIC_PORT, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);

  val=(data_rd[0]<<8)+data_rd[1];
  return val;
}
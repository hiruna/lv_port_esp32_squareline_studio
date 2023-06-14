/* 
 * Based on https://github.com/hiruna/lv_port_esp32/blob/develop/lvgl_8.3.7_idf_5.2/main/main.c
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "lvgl.h"

#include "lvgl_helpers.h"
#include "ui/ui.h"

#if defined CONFIG_USE_LV_TOUCH_CALIBRATION

#include "lv_tc.h"
#include "lv_tc_screen.h"

#ifndef USE_CUSTOM_LV_TC_COEFFICIENTS

#include "esp_nvs_tc.h"

#endif
#endif

/*********************
 *      DEFINES
 *********************/
#define TAG "__UI_PROJECT_NAME__"
#define LV_TICK_PERIOD_MS 1

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main() {

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;


static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

#if defined CONFIG_USE_LV_TOUCH_CALIBRATION

// callback function when touch calibration is complete
void lv_tc_finish_cb(lv_event_t *event) {
    lv_obj_t *originalScreen = (lv_obj_t*)(event->user_data);
    lv_obj_t *tCScreen = lv_scr_act();
    if (event->code == LV_EVENT_READY) {
        lv_disp_load_scr(originalScreen);
        ui_init();
        lv_obj_del(tCScreen);
    } else {
        ESP_LOGE(TAG, "unexpected lv event code '%d' (expected '%d') after touch calibration", lv_event_get_code(event),
                 LV_EVENT_READY);
    }
}

// function to create the touch calibration screen and begin the calibration
static void start_touch_calibration() {
    lv_obj_t *activeScreen = lv_scr_act();
    lv_obj_t *tCScreen = lv_tc_screen_create();
    lv_obj_add_event_cb(tCScreen, lv_tc_finish_cb, LV_EVENT_READY, activeScreen);
    lv_disp_load_scr(tCScreen);
    lv_tc_screen_start(tCScreen);
}

#endif

static void guiTask(void *pvParameter) {

    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    lv_color_t* buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
#else
    static lv_color_t *buf2 = NULL;
#endif

    static lv_disp_draw_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820         \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A    \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D     \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306

    /* Actual size in pixels, not bytes. */
    size_in_px *= 8;
#endif

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

#if defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT || defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT_INVERTED
    disp_drv.rotated = 1;
#endif

    /* When using a monochrome display we need to register the callbacks:
     * - rounder_cb
     * - set_px_cb */
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb = disp_driver_set_px;
    // disable antialiasing when using monochrome display
    disp_drv.antialiasing = 0;
#endif

    // need to set resolution for LVGL 8x
    disp_drv.hor_res = CONFIG_LV_HOR_RES_MAX;
    disp_drv.ver_res = CONFIG_LV_VER_RES_MAX;

    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /* Register an input device when enabled on the menuconfig */
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
#if CONFIG_USE_LV_TOUCH_CALIBRATION // if using LVGL Touch Calibration
    lv_tc_indev_drv_init(&indev_drv, touch_driver_read);
#ifndef CONFIG_USE_CUSTOM_LV_TC_COEFFICIENTS // if NOT using custom calibration coefficients
    lv_tc_register_coeff_save_cb(esp_nvs_tc_coeff_save_cb);
#endif
#else // if NOT using LVGL Touch Calibration
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
#endif
    lv_indev_drv_register(&indev_drv);
#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* SquareLine ui_init() */
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    #if CONFIG_USE_CUSTOM_LV_TC_COEFFICIENTS == 0
    // esp_nvs_tc_coeff_erase(); // this can be used to erase the stored coeff data on nvs
    if (esp_nvs_tc_coeff_init()) {
        ui_init();
    } else {
        start_touch_calibration();
    }
#else
    lv_tc_load_coeff_from_config();
    ui_init();
#endif
#else
    ui_init();
#endif

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}

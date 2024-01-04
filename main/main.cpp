/*
    Simple ESP-IDF Demo with ESP32-3248S035 + LovyanGFX v1.x + LVGL v8.x tested with ESP-IDF v5.1.2
*/
#include <stdio.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LGFX_USE_V1
#include <LovyanGFX.h>
#include "lvgl.h"
#include "ESP32-3248S035.h"
static LGFX lcd;
// #include "demos/lv_demos.h"  // uncomment if LVGL "demos" folder is present
// OR
// #include "ui.h"  // uncomment if Squareline exported "ui" folder was copied to components folder

extern "C"
{
    void app_main(void)
    {
        lcd.init();             // Initialize LovyanGFX
        lv_init();              // Initialize lvgl
        lcd.setRotation(1);     // Rotate display option 0..3
        lcd.setBrightness(128); // Backlight brightness 0..255
        lcd.setColorDepth(16);  // Display color depth 16(RGB565) or 24-bit(RGB888)

        // Setting display to landscape
        if (lcd.width() < lcd.height())
            lcd.setRotation(lcd.getRotation() ^ 1);

        // LVGL : Setting up buffer to use for display
        lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

        // LVGL : Setup & Initialize the display device driver
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = screenWidth;
        disp_drv.ver_res = screenHeight;
        disp_drv.flush_cb = display_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register(&disp_drv);

        // LVGL : Setup & Initialize the input device driver
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = touchpad_read;
        lv_indev_drv_register(&indev_drv);

        // Create and start a periodic timer interrupt to call lv_tick_inc
        const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            .arg = NULL,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "periodic_gui",
            .skip_unhandled_events = 1};
        esp_timer_handle_t periodic_timer;
        ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, portTICK_PERIOD_MS * 1000));

        // ui_init();   // uncomment if Squareline exported "ui" folder was copied to components folder
        // OR
        // lv_demo_widgets();   // uncomment if LVGL "demos" folder is present

        while (1)
        {
            lv_timer_handler();
            vTaskDelay(10);
        }
    }
}

// Display callback to flush the buffer to screen
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
    lcd.endWrite();
    lv_disp_flush_ready(disp);
}

// Touchpad callback to read the touchpad
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch(&touchX, &touchY);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

// Setting up tick task for lvgl
static void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_PERIOD_MS);
}

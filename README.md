# lv_port_esp32_squareline_studio

## Overview
This repository can be used to generate SquareLine Studio boards for ESP32 devices
(connected to supported display drivers defined in [lvgl_esp32_drivers](https://github.com/hiruna/lvgl_esp32_drivers/tree/develop/lvgl_7.11.0_idf_5.0)).

Project structure in _[\__ui_project_name\__](./__ui_project_name__)_ follows a similar structure to
[lv_port_esp32](https://github.com/hiruna/lv_port_esp32/tree/develop/lvgl_8.3.7_idf_5.2).
### Tested Library Versions
* ESP-IDF
  * v5.2 [master](https://github.com/espressif/esp-idf/tree/master/)
* `lvgl_esp32_drivers`
  * https://github.com/hiruna/lvgl_esp32_drivers/tree/develop/lvgl_7.11.0_idf_5.0
* LVGL
  * [release/v8.3](https://github.com/lvgl/lvgl/tree/release/v8.3)

## Create a new board
Refer to https://docs.squareline.io/docs/obp.

1. Clone this repo
2. Create a new directory under [boards/](./boards)
   *  Example: `your_board_name/`
4. Add the `your_board_name.png` file
   * A photo of the board to be displayed in SquareLine Studio.
6. Add the `your_board_name.slb`
   * SquareLine board definition file (see https://docs.squareline.io/docs/obp#your_boardslb for more information)
     * For monochrome displays such as the SSD1306 OLED panel, keep the `color_depth` at `8`, SquareLine currently
       does not support depth < 8, there is an assertion that is placed in the generated UI files to check if `LV_COLOR_DEPTH`
       matches the `color_depth` set in the `.slb` file; you will need to manually change this assertion. `(TODO: see below)`
     * Set the `lvgl_export_path` to `false` as the submodule reference _[\__ui_project_name\__/components/lvgl](./__ui_project_name__)_ is used
     * Set the `pattern_match_files` to `CMakeLists.txt,main/main.c` as this will replace templated tags such as `__UI_PROJECT_NAME__`
     * Set the `ui_export_path` to `./components/ui`
     * Set the `supported_lvgl_version` to `8.3.*`
7. Copy `your_board_name/` directory to the SquareLine installation's `boards/Espressif` directory
8. Create a compressed ZIP file of the _[\__ui_project_name\__](./__ui_project_name__)_ directory
   * This is common to all supported display drivers in [lvgl_esp32_drivers](https://github.com/hiruna/lvgl_esp32_drivers/tree/develop/lvgl_7.11.0_idf_5.0)
9. Rename the ZIP file to `your_board_name.zip`
10. Copy the `your_board_name.zip` to SquareLine installation's `boards/Espressif/your_board_name` directory

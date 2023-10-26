#pragma once

#include "esphome.h"

namespace esphome {
  namespace hub75display {

#define I75_R0 0
#define I75_G0 1
#define I75_B0 2
#define I75_R1 3
#define I75_G1 4
#define I75_B1 5
#define I75_ROW_A 6
#define I75_ROW_B 7
#define I75_ROW_C 8
#define I75_ROW_D 9
#define I75_ROW_E 10
#define I75_CLK 11
#define I75_STB 12
#define I75_OE 13

    class HUB75Display;

    using hub75display_writer_t = std::function<void(HUB75Display &)>;

    class HUB75Display : public PollingComponent, public display::DisplayBuffer {
    public:
      void dump_config() override;
      void set_writer(hub75display_writer_t &&writer);

      void setup() override;
      void update() override;
      void display();

      display::DisplayType get_display_type() override {
	return display::DisplayType::DISPLAY_TYPE_COLOR;
      }

    protected:
      optional<hub75display_writer_t>  writer_{};

      void draw_absolute_pixel_internal(int x, int y, Color color) override;
      int get_height_internal() override;
      int get_width_internal() override;
    };

  }
}

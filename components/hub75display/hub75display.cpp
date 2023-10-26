#include "hub75display.h"

namespace esphome {
  namespace hub75display {
    static const char *const TAG = "hub75display.display";

    void HUB75Display::dump_config() {
      ESP_LOGCONFIG(TAG, "HUB75 Display");
    }

    void HUB75Display::set_writer(hub75display_writer_t &&writer) {
      this->writer_ = writer;
    }

    void HUB75Display::setup() {
      pinMode(I75_R0, OUTPUT);
      pinMode(I75_G0, OUTPUT);
      pinMode(I75_B0, OUTPUT);
      pinMode(I75_R1, OUTPUT);
      pinMode(I75_G1, OUTPUT);
      pinMode(I75_B1, OUTPUT);
      pinMode(I75_ROW_A, OUTPUT);
      pinMode(I75_ROW_B, OUTPUT);
      pinMode(I75_ROW_C, OUTPUT);
      pinMode(I75_ROW_D, OUTPUT);
      pinMode(I75_ROW_E, OUTPUT);
      pinMode(I75_CLK, OUTPUT);
      pinMode(I75_STB, OUTPUT);
      pinMode(I75_OE, OUTPUT);

      digitalWrite(I75_OE, LOW);
      digitalWrite(I75_STB, LOW);

      multicore_launch_core1(this->core1_redraw);
    }

    void HUB75Display::update() {
      if (this->writer_.has_value()) {
	(*this->writer_)(*this);
      }
      this->display();
    }

    void HUB75Display::core1_redraw() {
      int h = this->get_height_internal() / 2;
      int w = this->get_width_internal();

      while (1) {
	for (int row = 0; row < h; row++) {
	  for (int col = 0; col < w; col++) {
	    int i0 = row * 64 + col;
	    int i1 = (row + 32) * 64 + col;

	    digitalWrite(I75_R0, (this->img[i0] & 0x0000FF) > 0);
	    digitalWrite(I75_G0, (this->img[i0] & 0x00FF00) > 0);
	    digitalWrite(I75_B0, (this->img[i0] & 0xFF0000) > 0);
	    digitalWrite(I75_R1, (this->img[i1] & 0x0000FF) > 0);
	    digitalWrite(I75_G1, (this->img[i1] & 0x00FF00) > 0);
	    digitalWrite(I75_B1, (this->img[i1] & 0xFF0000) > 0);

	    digitalWrite(I75_CLK, HIGH);
	    digitalWrite(I75_CLK, LOW);
	  }

	  digitalWrite(I75_OE, HIGH);

	  digitalWrite(I75_ROW_A, (row & 1) > 0);
	  digitalWrite(I75_ROW_B, (row & 2) > 0);
	  digitalWrite(I75_ROW_C, (row & 4) > 0);
	  digitalWrite(I75_ROW_D, (row & 8) > 0);
	  digitalWrite(I75_ROW_E, (row & 16) > 0);

	  digitalWrite(I75_STB, HIGH);
	  digitalWrite(I75_STB, LOW);

	  digitalWrite(I75_OE, LOW);
	}
      }
    }

    void HUB75Display::display() {

    }

    void HUB75Display::draw_absolute_pixel_internal(int x, int y, Color c) {
      this->img[y * 64 + x] = c.raw_32;
    }

    int HUB75Display::get_height_internal() {
      return 64;
    }
    int HUB75Display::get_width_internal() {
      return 64;
    }
  }
}

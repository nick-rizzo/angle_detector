#ifndef SSD1306_H
#define SSD1306_H
#define SSD1306_ADDR (0x3C<<1)
#define SSD1306_BUF_SIZE (64/8) * 128//8 pages of 8 bits, 128 columns

#include "main.h"

#define LINE_Y_COORD ((uint8_t)40)
#define LINE_X_COORD ((uint8_t)64)
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

const typedef enum {
	LEFT_ARROW,
	RIGHT_ARROW
} shapes;

struct coordinates{
	uint8_t x;
	uint8_t y;
};

class ssd1306_oled{
	public:
		ssd1306_oled();
		void ssd1306_update_display();
		void ssd1306_fill_buffer(uint8_t val);
		void clear_display();
		void place_pixel(int x, int y);
		void clear_pixel(int x, int y);
		void invert_pixel(int x, int y);
		bool get_pixel(int x, int y);
		void place_char(char c, int x, int y);
		void write_page(int page_num, char* line);
		void draw_box(int width, int height, int x, int y);
		void clear_box(int width, int height, int x, int y);
		void clear_page(int page_num);
		void invert_box(int width, int height, int x, int y);
		void draw_triangle(coordinates p1, coordinates p2, coordinates p3);
		void clear_triangle(coordinates p1, coordinates p2, coordinates p3);
		void insert_shape (int x, int y, shapes shape);
		void mirror_vertically(int col_num, coordinates top_left_crnr, int length, int height);
		virtual void display_init();
		void insert_selector(int cur_select);
		void move_selector(int cur_select, int new_select);

	private:
		void write_command(uint8_t data);
		void write_data(uint8_t *data);
		int sign (coordinates p1, coordinates p2, coordinates p3);
		bool is_point_in_triangle(coordinates pt, coordinates p1, coordinates p2, coordinates p3);
};

class roll_display : public ssd1306_oled{
	using ssd1306_oled::ssd1306_oled;
	public:
		void display_init();

};

class pitch_display : public ssd1306_oled{
	using ssd1306_oled::ssd1306_oled;
	public:
		void display_init();
};


#endif /* SSD_1306_H */
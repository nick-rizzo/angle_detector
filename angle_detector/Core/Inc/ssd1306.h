/*
 *	===============================================================================================
 *  File Name   : ssd1306.h
 *
 *  Description : Header file for SSD1306 128x64 OLED display
 *
 *  Contributors: Nick Rizzo
 *	===============================================================================================
 */
#ifndef SSD1306_H_
#define SSD1306_H_

#define SSD1306_BUF_SIZE (64/8) * 128//8 pages of 8 bits, 128 columns

#include "main.h"

#define LINE_Y_COORD ((uint8_t)40)
#define LINE_X_COORD ((uint8_t)64)
#define DISPLAY_WIDTH ((uint8_t)128)
#define DISPLAY_HEIGHT ((uint8_t)64)

enum shapes{
	LEFT_ARROW,
	RIGHT_ARROW,
	DEGREES
};

struct coordinates{
	uint8_t x;
	uint8_t y;
};

class ssd1306_oled{
	uint8_t ssd1306_buf[SSD1306_BUF_SIZE];
	uint16_t addr;
	I2C_HandleTypeDef i2c_bus;
	public:
		ssd1306_oled(I2C_HandleTypeDef &i2c_bus_in, uint16_t addr_in);
		void ssd1306_update_display();
		void ssd1306_fill_buffer(uint8_t val);
		void clear_display();
		void place_pixel(uint8_t x, uint8_t y);
		void clear_pixel(uint8_t x, uint8_t y);
		void invert_pixel(uint8_t x, uint8_t y);
		uint8_t get_pixel(uint8_t x, uint8_t y);
		void place_char(char c, int x, int y);
		void write_page(int page_num, char* line);
		void draw_box(int width, int height, int x, int y);
		void clear_box(int width, int height, int x, int y);
		void clear_page(int page_num);
		void invert_box(int width, int height, int x, int y);
		void draw_triangle(coordinates p1, coordinates p2, coordinates p3);
		void clear_triangle(coordinates p1, coordinates p2, coordinates p3);
		void insert_shape (int x, int y, shapes shape);
		virtual void display_init();
		void insert_selector(int cur_select);
		void move_selector(int cur_select, int new_select);

	private:
		void write_command(uint8_t data);
		void write_data(uint8_t *data);
		double area (coordinates p1, coordinates p2, coordinates p3);
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

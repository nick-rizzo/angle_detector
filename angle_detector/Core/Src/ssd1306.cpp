/*
 *	===============================================================================================
 *  File Name   :	ssd1306.cpp
 *
 *  Description : 	Driver for SSD1306 128x64 OLED display
 * 					Used to display angles of accelerometer
 *
 *  Contributors: 	Nick Rizzo
 *	===============================================================================================
 */
#ifndef SSD1306_CPP_
#define SSD1306_CPP_

#include <string.h>
#include <stdint.h>
#include <algorithm>
#include "main.h"
#include "ssd1306.h"
#include "fonts.h"

using namespace std;

void ssd1306_oled::write_command(uint8_t data){
	uint8_t cmd_data[2];
	cmd_data[0] = 0x00;
	cmd_data[1] = data;
	HAL_I2C_Master_Transmit(&i2c_bus, addr, cmd_data, 2, HAL_MAX_DELAY);
}

void ssd1306_oled::write_data(uint8_t *data){
	HAL_I2C_Mem_Write(&i2c_bus, addr, 0x40,1, data, SSD1306_BUF_SIZE, HAL_MAX_DELAY);
	// HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDR, data_buf, SSD1306_BUF_SIZE+1, HAL_MAX_DELAY);
	// while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	// HAL_I2C_Mem_Write_DMA(&hi2c1, SSD1306_ADDR, 0x40,1, data, SSD1306_BUF_SIZE);
}

void ssd1306_oled::ssd1306_update_display(){
	write_command(0x21); // column start/end
	write_command(0x00);
	write_command(0x7F);
	write_command(0x22); // PAGE start/end
	write_command(0x00);
	write_command(0x07);

	write_data(ssd1306_buf);
}

void ssd1306_oled::ssd1306_fill_buffer(uint8_t val){
	memset(ssd1306_buf, val, SSD1306_BUF_SIZE);
}

void ssd1306_oled::clear_display(){
	ssd1306_fill_buffer(0x00);
	ssd1306_update_display();
}

ssd1306_oled::ssd1306_oled(I2C_HandleTypeDef &i2c_bus_in, uint16_t addr_in){
	addr = addr_in;
	i2c_bus = i2c_bus_in;
	write_command(0xAE); //display off

	write_command(0x20); //Set Memory Addressing Mode
	write_command(0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid

	write_command(0x81); //--set contrast control register
	write_command(0xFF); //     contrast 255

	write_command(0x21); //set start column
	write_command(0x00); //    start col 0
	write_command(0x7F); //    end col 127

	write_command(0x22); // PAGE start/end
	write_command(0x00); //    start page 0
	write_command(0x07); //    end page 7

	write_command(0x40); //--set start line address

	write_command(0xA1); //--set segment re-map 0 to 127

	write_command(0xA8); //--set multiplex ratio(1 to 64)
	write_command(0x3F); //

	write_command(0xC8); //Set COM Output Scan Direction

	write_command(0xD3); //-set display offset
	write_command(0x00); //-not offset

	write_command(0xDA); //--set com pins hardware configuration
	write_command(0x12);

	write_command(0xA6); //--set normal display

	write_command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content


	write_command(0xD5); //--set display clock divide ratio/oscillator frequency
	write_command(0xF0); //--set divide ratio

	write_command(0xD9); //--set pre-charge period
	write_command(0x22); //

	write_command(0xDB); //--set vcomh
	write_command(0x20); //0x20,0.77xVcc

	write_command(0x8D); //--set DC-DC enable
	write_command(0x14); //

	write_command(0xAF); //--turn on SSD1306 panel

	clear_display();
}

void ssd1306_oled::place_pixel(uint8_t x, uint8_t y){
	//0,0 is top left
	// x is col num -- run
	// y is row num -- rise
	ssd1306_buf[((y/8)*128) + x] |= (1<<(y%8));
}

uint8_t ssd1306_oled::get_pixel(uint8_t x, uint8_t y){
	return (ssd1306_buf[((y/8)*128) + x])>>(y%8) & 0x1;
}

void ssd1306_oled::clear_pixel(uint8_t x, uint8_t y){
	ssd1306_buf[((y/8)*128) + x] &= (0<<(y%8));
}

void ssd1306_oled::invert_pixel(uint8_t x, uint8_t y){
	//0,0 is top left
	// x is col num -- run
	// y is row num -- rise
	ssd1306_buf[((y/8)*128) + x] ^= (1<<(y%8));
}

void ssd1306_oled::place_char(char c, int x, int y){
	//places a character with top left being x,y
	int pixel = 0;
	int font_idx=0;
	while (c!=charmap[font_idx]){
		font_idx++;
	}
	for(int h=0;h<8;h++){
		for (int w=0;w<10;w++){
			pixel = (Font8x10[(8*font_idx)+h]>>(9-w)&0x001);
			if (pixel == 1){
				place_pixel((x+w),(y+h));
			}
		}
	}
}

void ssd1306_oled::write_page(int page_num, char* line){
	// writes a page of display with 12 character string
	int x = 0;
	int y = page_num*8;
	for(int l=0;l<12;l++){
		x = 10*l;
		place_char(line[l], x, y);
	}
}

void ssd1306_oled::draw_box(int width, int height, int x, int y){
	//draws a box of width, height with top left corner being x,y
	for(int w=0;w<width;w++){
		for(int h=0;h<height;h++){
			place_pixel((x+w),(y+h));
		}
	}
}

void ssd1306_oled::clear_box(int width, int height, int x, int y){
	//draws a box of width, height with top left corner being x,y
	for(int w=0;w<width;w++){
		for(int h=0;h<height;h++){
			clear_pixel((x+w),(y+h));
		}
	}
}

void ssd1306_oled::clear_page(int page_num){
	int x = 0;
	int y = page_num*8;
	clear_box(128, 8, x, y);
}
//TODO: Switch all functions using x, y to coord struct
void ssd1306_oled::invert_box(int width, int height, int x, int y){
	//inverts pixels of box selection width, height, with top left corner
	//being coords x,y
	for(int w=0;w<width;w++){
		for(int h=0;h<height;h++){
			invert_pixel((x+w),(y+h));
		}
	}
}

double ssd1306_oled::area (coordinates p1, coordinates p2, coordinates p3){
	return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0);
}

bool ssd1306_oled::is_point_in_triangle(coordinates pt, coordinates p1, coordinates p2, coordinates p3){
 double denominator = ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
 double a = ((p2.y - p3.y)*(pt.x - p3.x) + (p3.x - p2.x)*(pt.y - p3.y)) / denominator;
 double b = ((p3.y - p1.y)*(pt.x - p3.x) + (p1.x - p3.x)*(pt.y - p3.y)) / denominator;
 double c = 1 - a - b;

 return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}

void ssd1306_oled::draw_triangle(coordinates p1, coordinates p2, coordinates p3){
	uint8_t x_min = min(min(p1.x, p2.x) , min(p1.x, p3.x));
	uint8_t x_max = max(max(p1.x, p2.x) , max(p1.x, p3.x));
	uint8_t y_min = min(min(p1.y, p2.y) , min(p1.y, p3.y));
	uint8_t y_max = max(max(p1.y, p2.y) , max(p1.y, p3.y));
	for(uint8_t i=x_min; i<x_max; i++){
		for(uint8_t j=y_min; j<y_max; j++){
			coordinates cur_pt = {i, j};
			if (is_point_in_triangle(cur_pt, p1, p2, p3)){
				place_pixel(i,j);
			}
			// else{
			// 	clear_pixel(i,j);
			// }
		}
	}
}

void ssd1306_oled::clear_triangle(coordinates p1, coordinates p2, coordinates p3){
	uint8_t x_min = min(min(p1.x, p2.x) , min(p1.x, p3.x));
	uint8_t x_max = max(max(p1.x, p2.x) , max(p1.x, p3.x));
	uint8_t y_min = min(min(p1.y, p2.y) , min(p1.y, p3.y));
	uint8_t y_max = max(max(p1.y, p2.y) , max(p1.y, p3.y));
	for(uint8_t i=x_min; i<x_max; i++){
		for(uint8_t j=y_min; j<y_max; j++){
			coordinates cur_pt = {i, j};
			if (is_point_in_triangle(cur_pt, p1, p2, p3)){
				clear_pixel(i,j);
			}
		}
	}
}

void ssd1306_oled::insert_shape (int x, int y, shapes shape){
	//inserts custom 8x10 shape defined in fonts.h
	int pixel = 0;
	for(int h=0;h<8;h++){
		for (int w=0;w<10;w++){
			pixel = (shape_arr[(8*shape)+h]>>(9-w)&0x001);
			if (pixel == 1){
				place_pixel((x+w),(y+h));
			}
		}
	}
}

void ssd1306_oled::set_display(const uint8_t screen_contents[SSD1306_BUF_SIZE]){
	memcpy(ssd1306_buf, screen_contents, SSD1306_BUF_SIZE);
}

void ssd1306_oled::display_init(){
	// initial display screen
	draw_box(128, 3, 0, LINE_Y_COORD);
}

//////////////////////////////////////////////////////////////////////////////////////
void pitch_display::display_init(){
	ssd1306_oled::display_init();
	write_page(0, "PITCH       ");
	// write_page(0, "PITCH  ANGLE");
	ssd1306_update_display();
}

//////////////////////////////////////////////////////////////////////////////////////
void roll_display::display_init(){
	ssd1306_oled::display_init();
	write_page(0, "ROLL        ");
	// write_page(0, "ROLL   ANGLE");
	ssd1306_update_display();
}

//////////////////////////////////////////////////////////////////////////////////////

void center_display::display_init(){
	set_display(warning_sign);
	ssd1306_update_display();
}
#endif




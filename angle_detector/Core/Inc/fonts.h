	#include "main.h"
	const char charmap [] = {
	'!',
	'"',
	'#',
	'$',
	'&',
	'%',
	'\'',
	'(',
	')',
	'*',
	',',
	'-',
	'.',
	'/',
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	':',
	';',
	'<',
	'>',
	'=',
	'?',
	'@',
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'[',
	']',
	'\\',
	'^',
	'_',
	'`',
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'{',
	'}',
	'|',
	'~',
	'+',
	' '
};

const uint16_t shape_arr [] = {
	0b0000000110, //left arrow
	0b0000011110,
	0b0011111110,
	0b1111111110,
	0b0011111110,
	0b0000011110,
	0b0000000110,
	0b0000000000,

	0b1100000000, //right arrow
	0b1111000000,
	0b1111111000,
	0b1111111110,
	0b1111111000,
	0b1111000000,
	0b1100000000,
	0b0000000000,
};

const uint16_t Font8x10[] = {
	0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000000000, 0b0000100000, 0b0000000000, // !
	0b0001010000, 0b0001010000, 0b0001010000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // "
	0b0010010000, 0b0010010000, 0b1111111100, 0b0010010000, 0b1111111100, 0b0010010000, 0b0010010000, 0b0000000000, // #
	0b0001010000, 0b0111111100, 0b0101010000, 0b0111111100, 0b0001010100, 0b0111111100, 0b0001010000, 0b0000000000, // $
	0b0000100000, 0b0001110000, 0b0010001000, 0b0001000000, 0b0001000000, 0b0010001000, 0b0001110000, 0b0000000000, // &
	0b0000000110, 0b0110001100, 0b0110011000, 0b0000110000, 0b0001100110, 0b0011000110, 0b0110000000, 0b0000000000, // %
	0b0000100000, 0b0000100000, 0b0000100000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // '
	0b0010000000, 0b0100000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b0100000000, 0b0010000000, 0b0000000000, // (
	0b1000000000, 0b0100000000, 0b0010000000, 0b0010000000, 0b0010000000, 0b0100000000, 0b1000000000, 0b0000000000, // )
	0b0000000000, 0b1001000000, 0b0110000000, 0b1111000000, 0b0110000000, 0b1001000000, 0b0000000000, 0b0000000000, // *
	0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0100000000, 0b0100000000, 0b1000000000, 0b0000000000, // ,
	0b0000000000, 0b0000000000, 0b0000000000, 0b1110000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // -
	0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b1000000000, 0b0000000000, // .
	0b0000001000, 0b0000010000, 0b0000100000, 0b0001000000, 0b0010000000, 0b0100000000, 0b1000000000, 0b0000000000, // /
	0b0111111100, 0b1000001110, 0b1000011010, 0b1001110010, 0b1011000010, 0b1110000010, 0b0111111100, 0b0000000000, // 0
	0b0011110000, 0b0100110000, 0b1000110000, 0b0000110000, 0b0000110000, 0b0000110000, 0b1111111110, 0b0000000000, // 1
	0b0111111100, 0b1000000010, 0b0000000010, 0b1111111100, 0b1000000000, 0b1000000000, 0b1111111110, 0b0000000000, // 2
	0b0111111100, 0b1000000010, 0b0000000010, 0b0001111100, 0b0000000010, 0b1000000010, 0b0111111100, 0b0000000000, // 3
	0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111110, 0b0000000010, 0b0000000010, 0b0000000000, // 4
	0b1111111110, 0b1000000000, 0b1000000000, 0b1111111100, 0b0000000010, 0b0000000010, 0b1111111100, 0b0000000000, // 5
	0b0111111110, 0b1000000000, 0b1000000000, 0b1111111100, 0b1000000010, 0b1000000010, 0b1111111100, 0b0000000000, // 6
	0b1111111110, 0b0000000110, 0b0000001110, 0b0000011100, 0b0000111000, 0b0001110000, 0b0011100000, 0b0000000000, // 7
	0b0111111100, 0b1000000010, 0b1000000010, 0b0111111100, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // 8
	0b0111111110, 0b1000000010, 0b1000000010, 0b0111111110, 0b0000000010, 0b0000000010, 0b0111111100, 0b0000000000, // 9
	0b0000000000, 0b0000000000, 0b1100000000, 0b1100000000, 0b0000000000, 0b1100000000, 0b1100000000, 0b0000000000, // :
	0b0000000000, 0b0000000000, 0b1100000000, 0b1100000000, 0b0000000000, 0b1100000000, 0b0100000000, 0b0000000000, // ;
	0b0001100000, 0b0011000000, 0b0110000000, 0b1100000000, 0b0110000000, 0b0011000000, 0b0001100000, 0b0000000000, // <
	0b1100000000, 0b0110000000, 0b0011000000, 0b0001100000, 0b0011000000, 0b0110000000, 0b1100000000, 0b0000000000, // >
	0b0000000000, 0b0000000000, 0b1111000000, 0b0000000000, 0b1111000000, 0b0000000000, 0b0000000000, 0b0000000000, // =
	0b0111111100, 0b1000000010, 0b0000000010, 0b0000011100, 0b0001100000, 0b0000000000, 0b0001100000, 0b0000000000, // ?
	0b1111111110, 0b1000000010, 0b1011111010, 0b1010001010, 0b1011111100, 0b1000000000, 0b1111111110, 0b0000000000, // @
	0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1111111010, 0b1000000010, 0b1000000010, 0b0000000000, // A
	0b0111111100, 0b1000000010, 0b1000000010, 0b1011111100, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // B
	0b0111111110, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b0111111110, 0b0000000000, // C
	0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000100, 0b1111111000, 0b0000000000, // D
	0b1111111110, 0b1000000000, 0b1000000000, 0b1111110000, 0b1000000000, 0b1000000000, 0b1111111110, 0b0000000000, // E
	0b1111111110, 0b1000000000, 0b1000000000, 0b1111100000, 0b1000000000, 0b1000000000, 0b1000000000, 0b0000000000, // F
	0b0111111110, 0b1000000000, 0b1000000000, 0b1000111110, 0b1000000010, 0b1000000010, 0b0111111110, 0b0000000000, // G
	0b1000000010, 0b1000000010, 0b1000000010, 0b1111111110, 0b1000000010, 0b1000000010, 0b1000000010, 0b0000000000, // H
	0b1111111110, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b1111111110, 0b0000000000, // I
	0b1111111110, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b1000100000, 0b0111100000, 0b0000000000, // J
	0b1000001110, 0b1000011100, 0b1111111000, 0b1111110000, 0b1000011000, 0b1000001100, 0b1000000110, 0b0000000000, // K
	0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1111111110, 0b0000000000, // L
	0b1100000110, 0b1010001010, 0b1001010010, 0b1000100010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0000000000, // M
	0b1100000010, 0b1010000010, 0b1001000010, 0b1000100010, 0b1000010010, 0b1000001010, 0b1000000110, 0b0000000000, // N
	0b0111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // O
	0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1111111100, 0b1000000000, 0b1000000000, 0b0000000000, // P
	0b0111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000001010, 0b1000000100, 0b0111111010, 0b0000000000, // Q
	0b1111111100, 0b1000000010, 0b1000000010, 0b1111111100, 0b1000010000, 0b1000001000, 0b1000000100, 0b0000000000, // R
	0b0111111110, 0b1000000000, 0b1000000000, 0b0111111100, 0b0000000010, 0b0000000010, 0b1111111100, 0b0000000000, // S
	0b1111111110, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000000000, // T
	0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // U
	0b1100000110, 0b1100000110, 0b1110001110, 0b0111011100, 0b0011111000, 0b0001110000, 0b0000100000, 0b0000000000, // V
	0b1000000010, 0b1000000010, 0b1000000010, 0b1000100010, 0b1000100010, 0b1000100010, 0b0111011100, 0b0000000000, // W
	0b1100000110, 0b0110001100, 0b0011011000, 0b0001110000, 0b0011011000, 0b0110001100, 0b1100000110, 0b0000000000, // X
	0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000010, 0b0111111100, 0b0000000000, // Y
	0b1111111110, 0b0000000010, 0b0000001100, 0b0001110000, 0b0110000000, 0b1000000000, 0b1111111110, 0b0000000000, // Z
	0b1110000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1110000000, 0b0000000000, // [
	0b1110000000, 0b0010000000, 0b0010000000, 0b0010000000, 0b0010000000, 0b0010000000, 0b1110000000, 0b0000000000, // ]
	0b1000000000, 0b0100000000, 0b0010000000, 0b0001000000, 0b0000100000, 0b0000010000, 0b0000001000, 0b0000000000, // /*\*/
	0b0010000000, 0b0101000000, 0b1000100000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // ^
	0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b1111111100, 0b0000000000, // _
	0b1000000000, 0b0100000000, 0b0010000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // `
	0b0000000000, 0b0000000000, 0b0111111110, 0b0000000010, 0b1111111110, 0b1000000010, 0b0111111100, 0b0000000000, // a
	0b1000000000, 0b1000000000, 0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1111111100, 0b0000000000, // b
	0b0000000000, 0b0000000000, 0b0111111110, 0b1000000000, 0b1000000000, 0b1000000000, 0b0111111110, 0b0000000000, // c
	0b0000000010, 0b0000000010, 0b0111111110, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // d
	0b0000000000, 0b0000000000, 0b0111111100, 0b1000000010, 0b1111111100, 0b1000000000, 0b0111111110, 0b0000000000, // e
	0b0011111100, 0b0100000010, 0b0100000000, 0b0100000000, 0b1111111000, 0b0100000000, 0b0100000000, 0b0000000000, // f
	0b0000000000, 0b0000000000, 0b0111111100, 0b1000000010, 0b0111111110, 0b0000000010, 0b0111111100, 0b0000000000, // g
	0b1000000000, 0b1000000000, 0b1000000000, 0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b0000000000, // h
	0b0000000000, 0b0000100000, 0b0000000000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000000000, // i
	0b0000000100, 0b0000000000, 0b0000000110, 0b0000000010, 0b0000000010, 0b1000000010, 0b0111111100, 0b0000000000, // j
	0b1000000010, 0b1000000100, 0b1000001000, 0b1000010000, 0b1111111000, 0b1000000100, 0b1000000010, 0b0000000000, // k
	0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000100000, 0b0000000000, // l
	0b0000000000, 0b1000000000, 0b1111011100, 0b1000100010, 0b1000100010, 0b1000100010, 0b1000100010, 0b0000000000, // m
	0b0000000000, 0b1000000000, 0b1111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0000000000, // n
	0b0000000000, 0b0000000000, 0b0111111100, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // o
	0b0000000000, 0b0000000000, 0b1111111100, 0b1000000010, 0b1000000010, 0b1111111100, 0b1000000000, 0b0000000000, // p
	0b0000000000, 0b0000000000, 0b1111111000, 0b1000001000, 0b1111111010, 0b0000001010, 0b0000000100, 0b0000000000, // q
	0b0000000000, 0b0000000000, 0b1011111100, 0b1100000010, 0b1000000010, 0b1000000000, 0b1000000000, 0b0000000000, // r
	0b0000000000, 0b0000000000, 0b0111111100, 0b1000000000, 0b0111111000, 0b0000000100, 0b1111111000, 0b0000000000, // s
	0b0000000000, 0b0010000000, 0b0010000000, 0b1111111110, 0b0010000000, 0b0010000100, 0b0001111000, 0b0000000000, // t
	0b0000000000, 0b0000000000, 0b1000000010, 0b1000000010, 0b1000000010, 0b1000000010, 0b0111111100, 0b0000000000, // u
	0b0000000000, 0b0000000000, 0b1000000010, 0b0100000100, 0b0010001000, 0b0001010000, 0b0000100000, 0b0000000000, // v
	0b0000000000, 0b0000000000, 0b1000000010, 0b1000100010, 0b1000100010, 0b1000100010, 0b0111011100, 0b0000000000, // w
	0b0000000000, 0b0000000000, 0b1100000110, 0b0110001100, 0b0011111000, 0b0110001100, 0b1100000110, 0b0000000000, // x
	0b0000000000, 0b0000000000, 0b1000000010, 0b1000000010, 0b1111111110, 0b0000000010, 0b1111111110, 0b0000000000, // y
	0b0000000000, 0b0000000000, 0b1111111110, 0b0000000010, 0b0111111100, 0b1000000000, 0b1111111110, 0b0000000000, // z
	0b0100000000, 0b1000000000, 0b1000000000, 0b0100000000, 0b1000000000, 0b1000000000, 0b0100000000, 0b0000000000, // {
	0b1000000000, 0b0100000000, 0b0100000000, 0b1000000000, 0b0100000000, 0b0100000000, 0b1000000000, 0b0000000000, // }
	0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000, 0b0000000000, // |
	0b0000000000, 0b0000000000, 0b0110010000, 0b1001100000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // ~
	0b0000000000, 0b0000100000, 0b0000100000, 0b0011111000, 0b0000100000, 0b0000100000, 0b0000000000, 0b0000000000, // +
	0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, 0b0000000000, // SPACE
	};
#include "ctype.h"

#define CNTRL 0x0001
#define SPACE 0x0002
#define BLANK 0x0004
#define PRINT 0x0008
#define PUNCT 0x0010
#define GRAPH 0x0020
#define DIGIT 0x0040
#define XDIGIT 0x0080
#define ALNUM 0x0100
#define UPPER 0x0200
#define LOWER 0x0400
#define ALPHA 0x0800

static const int type[128] = {
	CNTRL, // 00 NUL
	CNTRL, // 01 SOH
	CNTRL, // 02 STX
	CNTRL, // 03 ETX
	CNTRL, // 04 EOT
	CNTRL, // 05 ENQ
	CNTRL, // 06 ACK
	CNTRL, // 07 BEL
	CNTRL, // 08 BS
	CNTRL|BLANK|SPACE, // 09 HT
	CNTRL|SPACE, // 0A LF
	CNTRL|SPACE, // 0B VT
	CNTRL|SPACE, // 0C FF
	CNTRL|SPACE, // 0D CR
	CNTRL, // 0E SO
	CNTRL, // 0F SI
	CNTRL, // 10 DLE
	CNTRL, // 11 DC1
	CNTRL, // 12 DC2
	CNTRL, // 13 DC3
	CNTRL, // 14 DC4
	CNTRL, // 15 NAK
	CNTRL, // 16 SYN
	CNTRL, // 17 ETB
	CNTRL, // 18 CAN
	CNTRL, // 19 EM
	CNTRL, // 1A SUB
	CNTRL, // 1B ESC
	CNTRL, // 1C FS
	CNTRL, // 1D GS
	CNTRL, // 1E RS
	CNTRL, // 1F US
	BLANK|SPACE|PRINT, // 20 Space
	PUNCT|GRAPH|PRINT, // 21 !
	PUNCT|GRAPH|PRINT, // 22 "
	PUNCT|GRAPH|PRINT, // 23 #
	PUNCT|GRAPH|PRINT, // 24 $
	PUNCT|GRAPH|PRINT, // 25 %
	PUNCT|GRAPH|PRINT, // 26 &
	PUNCT|GRAPH|PRINT, // 27 '
	PUNCT|GRAPH|PRINT, // 28 (
	PUNCT|GRAPH|PRINT, // 29 )
	PUNCT|GRAPH|PRINT, // 2A *
	PUNCT|GRAPH|PRINT, // 2B +
	PUNCT|GRAPH|PRINT, // 2C ,
	PUNCT|GRAPH|PRINT, // 2D -
	PUNCT|GRAPH|PRINT, // 2E .
	PUNCT|GRAPH|PRINT, // 2F /
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 30 0
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 31 1
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 32 2
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 33 3
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 34 4
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 35 5
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 36 6
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 37 7
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 38 8
	DIGIT|XDIGIT|ALNUM|GRAPH|PRINT, // 39 9
	PUNCT|GRAPH|PRINT, // 3A :
	PUNCT|GRAPH|PRINT, // 3B ;
	PUNCT|GRAPH|PRINT, // 3C <
	PUNCT|GRAPH|PRINT, // 3D =
	PUNCT|GRAPH|PRINT, // 3E >
	PUNCT|GRAPH|PRINT, // 3F ?
	PUNCT|GRAPH|PRINT, // 40 @
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 41 A
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 42 B
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 43 C
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 44 D
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 45 E
	UPPER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 46 F
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 47 G
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 48 H
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 49 I
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4A J
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4B K
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4C L
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4D M
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4E N
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 4F O
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 50 P
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 51 Q
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 52 R
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 53 S
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 54 T
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 55 U
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 56 V
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 57 W
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 58 X
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 59 Y
	UPPER|ALPHA|ALNUM|GRAPH|PRINT, // 5A Z
	PUNCT|GRAPH|PRINT, // 5B [
	PUNCT|GRAPH|PRINT, // 5C \
	PUNCT|GRAPH|PRINT, // 5D ]
	PUNCT|GRAPH|PRINT, // 5E ^
	PUNCT|GRAPH|PRINT, // 5F _
	PUNCT|GRAPH|PRINT, // 60 `
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 61 a
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 62 b
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 63 c
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 64 d
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 65 e
	LOWER|ALPHA|ALNUM|XDIGIT|GRAPH|PRINT, // 66 f
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 67 g
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 68 h
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 69 i
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6A j
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6B k
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6C l
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6D m
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6E n
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 6F o
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 70 p
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 71 q
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 72 r
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 73 s
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 74 t
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 75 u
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 76 v
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 77 w
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 78 x
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 79 y
	LOWER|ALPHA|ALNUM|GRAPH|PRINT, // 7A z
	PUNCT|GRAPH|PRINT, // 7B {
	PUNCT|GRAPH|PRINT, // 7C |
	PUNCT|GRAPH|PRINT, // 7D }
	PUNCT|GRAPH|PRINT, // 7E ~
	CNTRL // 7F DEL
};

#define ASCII(x) (x >= 0 && x <= 0x7F)

int isalnum(int ch) { return ASCII(ch)? (type[ch] & ALNUM): 0; }
int isalpha(int ch) { return ASCII(ch)? (type[ch] & ALPHA): 0; }
int isblank(int ch) { return ASCII(ch)? (type[ch] & BLANK): 0; }
int iscntrl(int ch) { return ASCII(ch)? (type[ch] & CNTRL): 0; }
int isdigit(int ch) { return ASCII(ch)? (type[ch] & DIGIT): 0; }
int isgraph(int ch) { return ASCII(ch)? (type[ch] & GRAPH): 0; }
int islower(int ch) { return ASCII(ch)? (type[ch] & LOWER): 0; }
int isprint(int ch) { return ASCII(ch)? (type[ch] & PRINT): 0; }
int ispunct(int ch) { return ASCII(ch)? (type[ch] & PUNCT): 0; }
int isspace(int ch) { return ASCII(ch)? (type[ch] & SPACE): 0; }
int isupper(int ch) { return ASCII(ch)? (type[ch] & UPPER): 0; }
int isxdigit(int ch) { return ASCII(ch)? (type[ch] & XDIGIT): 0; }

int tolower(int ch) { return isupper(ch)? (ch + 'a' - 'A'): ch; }
int toupper(int ch) { return islower(ch)? (ch + 'A' - 'a'): ch; }


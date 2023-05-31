/**
 * Dem128064B module is used for the LCD display Dem128064B used in this project.
 * The initialization of the LCD is done inside the function setUp.
 * The implementation of the LCD module
 * is more specific to this project in which memory
 * mapping approach is implemented
 *
 *
 * Author Husnain Khan
 */

/**
 * Enable pin position
 */
#define E_pos 13

/**
 * DI position on portc
 */
#define DI_pos 12

/**
 * Read write position
 */
#define RW_pos 11

/**
 * Reset position
 */
#define RST_pos 10

/**
 * Chip select signal for IC2
 * (Screen Right Side , To turn on Right side of the screen it should be 1)
 */
#define CS2_pos 9

/**
 * Chip select signal for IC1
 * (Screen Left Side , To turn on left side of the screen it should be 1)
 */
#define CS1_pos 8

/**
 * Max Cols in LCD
 */
#define MAX_COLS_LCD 64

/**
 * Total rows of the lcd
 */
#define TOTAL_ROWS 64

/**
 * TOTAL columns of the lcd
 */
#define TOTAL_COLS 128

/**
 * one page has 8 pixels
 */
#define PAGE_LEN 8

/**
 * length of the moving line
 */
#define LEN_OF_MOVING_LINE 30

/**
 * previousPositons of the ball,
 * the position is stored as multiple of 10
 * because the new position is calculated
 * by adding previous position + tilt angle / 90
 * 90 is the max angle
 * so if the tile angle is very small for a period of time
 * the position will keep on incrementing and the ball will only
 * moves to the new position if the new position
 * for example start position is 10 which actually means its 1 now
 * tilt angle is increased to 300 new position will be 10 + 300/90 which is
 * 13 but the ball will stay at 1 until the position didn't reach to 20 or cross 20
 *
 */
struct previousPos {
	int prev_Row;
	int prev_Col;
} prevPos;

/**
 * TURN ON SCREEN PORTC Value 
 * Value to be written on portc to turn on the screen
 */
#define TURN_ON_SCREEEN 0x3F

/**
 * 1011 1,A2,A1,A0
 * when page number is passed it is to be OR with page_sel_mask
 */
#define PAGE_SEL_MASK 0xB8

/**
 * 01,A5,A4,A3,A2,A1,A0
 * Col address is order with this mask
 */
#define COL_SEL_MASK 0x40

/**
 * setUp for the LCD, initilization of the display,
 * setting the prev position of the to 0, clearing the screen
 * initialize the 2d array which is used to store the content on the LCD display
 * set reset bit
 */
extern void setUp(void);

/**
 * set Reset pin
 */
extern void set_Reset_Bit(void);

/**
 * set Enable bit to 1
 */
extern void set_Enable(void);

/**
 * reset enable to 0
 */
extern void reset_Enable(void);

/**
 * Turn on Screen
 */
extern void turn_On_Screen(void);

/**
 * set Control Buss to Write
 */
extern void set_Control_Buss_To_Write(void);

/**
 * On Off Display
 */
extern void display_ON_OFF_Reg(void);

/**
 * set left side of the screen
 */
extern void set_Left_Side_Screen(void);

/**
 * set Right Side of the screen
 */
extern void set_Right_Side_Screen(void);

/**
 * set both sides, used for clearing 
 */
extern void set_Both_Side_Screen(void);

/**
 * toggle Enable LCD 
 */
extern void toggle_Enable_Lcd(void);

/**
 * Write welcome on Display array
 */
extern void writeWelcomeToArray(void);


/**
 * Write Game over on Display array
 */
extern void writeGameOver(void);

/**
 * write the given number, to shift is used to shift the col
 */
extern void writeNumber(int number, int toShift);




#define NUMWORDS 35
#define MAXLEDS  4

#define IT       0
#define IS       1
#define TWENTY   2
#define FIVE_MIN 3
#define TEN      4
#define HA       5
#define LF       6
#define PP       7
#define QUARTER  8
#define PAST     9
#define BI       10
#define TO       11
#define ONE      12
#define Y_1      13
#define TWO      14
#define THREE    15
#define FOU      16
#define R        17
#define TH       18
#define FIVE_HR  19
#define SIX      20
#define DA       21
#define SEVEN    22
#define Y_2      23
#define EIGHT    24
#define NINE     25
#define D        26
#define T        27
#define EN       28
#define IS       29
#define E        30
#define LEVEN    31
#define TWELVE   32
#define O        33
#define CLOCK    34

signed int WORDMAP[NUMWORDS][MAXLEDS] = {
  [IT]        = {72,-1,-1,-1},
  [IS]        = {70,-1,-1,-1},
  [TWENTY]    = {66,67,68,-1},
  [FIVE_MIN]  = {63,64,-1,-1},
  [TEN]       = {53,54,-1,-1},
  [HA]        = {55,56,-1,-1},
  [LF]        = {57,-1,-1,-1},
  [PP]        = {58,-1,-1,-1},
  [QUARTER]   = {59,60,61,62},
  [PAST]      = {51,52,-1,-1},
  [BI]        = {50,-1,-1,-1},
  [TO]        = {49,-1,-1,-1},
  [ONE]       = {47,-1,-1,-1},
  [Y_1]       = {46,-1,-1,-1},
  [TWO]       = {44,45,-1,-1},
  [THREE]     = {34,35,36,-1},
  [FOU]       = {38,39,-1,-1},
  [R]         = {40,-1,-1,-1},
  [TH]        = {41,-1,-1,-1},
  [FIVE_HR]   = {42,43,-1,-1},
  [SIX]       = {32,33,-1,-1},
  [DA]        = {31,-1,-1,-1},
  [SEVEN]     = {28,29,30,-1},
  [Y_2]       = {27,-1,-1,-1},
  [EIGHT]     = {24,25,26,-1},
  [NINE]      = {10,11,-1,-1},
  [D]         = {12,13,-1,-1},
  [T]         = {14,-1,-1,-1},
  [EN]        = {15,16,17,-1},
  [IS]        = {18,19,20,-1},
  [E]         = {21,-1,-1,-1},
  [LEVEN]     = {22,23,-1,-1},
  [TWELVE]    = { 7, 8, 9,-1},
  [O]         = { 5,-1,-1,-1},
  [CLOCK]     = { 1, 2,-1,-1},
};

#define BIRTHDAY_MSG_LEN 12
int BIRTHDAY_MSG[BIRTHDAY_MSG_LEN] = { HA, PP, Y_1, BI, R, TH, DA, Y_2, D, EN, IS, E };


void illuminate_word(int word, int color) {
  for (i = 0; i < 4; i++) {
    int pixel = WORDMAP[word][i];
    if (pixel == -1)
      // -1 indicates end of the LED list for this word
      break;
    leds.setPixel(pixel, color);
  }
}

// :00 - 04 30..26    O CLOCK
// :05 - 09 25..21    FIVE PAST
// :10 - 14 20..16    TEN PAST
// :15 - 19           QUARTER PAST
// :20 - 24           TWENTY PAST
// :25 - 29           TWENTY FIVE PAST
// :30 - 34           HALF PAST
// :35 - 39           TWENTY FIVE TO
// :40 - 44           TWENTY TO
// :45 - 49           QUARTER TO
// :50 - 54           TEN TO
// :55 - 59           FIVE TO

// set drawingMemory to all zeros
// retrieve current date and time from RTC (day, month, year, hour, minute, second)
// if it is Denise's birthday && hour == BDAY_MSG_HOUR && minute == BDAY_MSG_MIN && second >= 0 && second <= 15
   for (i = 0; i < BIRTHDAY_MSG_LEN; i++) {
     illuminate_word(BIRTHDAY_MSG[i], currentColor);
   }
} else {
  illuminate_word(IT, currentColor);
  illuminate_word(IS, currentColor);
  if (minute >= 0 && minute <= 04) {
    illuminate_word(O, currentColor);
    illuminate_word(CLOCK, currentColor);
  } else {
    if (minute >= 5 && minute <= 34) {
      illuminate_word(PAST, currentColor);
    } else {
      illuminate_word(TO, currentColor);
    }
    switch (minute % 30) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
        illuminate_word(HALF, currentColor);
        break;
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        illuminate_word(TWENTY, currentColor);
        illuminate_word(FIVE, currentColor);
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
    }

  }
}

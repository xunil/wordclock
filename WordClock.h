#ifndef _WORDCLOCK_H
#define _WORDCLOCK_H 1
#define NUMWORDS 35
#define MAXLEDSPERWORD  4

#define WORD_IT       0
#define WORD_IS_1     1
#define WORD_TWENTY   2
#define WORD_FIVE_MIN 3
#define WORD_TEN      4
#define WORD_HA       5
#define WORD_LF       6
#define WORD_PP       7
#define WORD_QUARTER  8
#define WORD_PAST     9
#define WORD_BI       10
#define WORD_TO       11
#define WORD_ONE      12
#define WORD_Y_1      13
#define WORD_TWO      14
#define WORD_THREE    15
#define WORD_FOU      16
#define WORD_R        17
#define WORD_TH       18
#define WORD_FIVE_HR  19
#define WORD_SIX      20
#define WORD_DA       21
#define WORD_SEVEN    22
#define WORD_Y_2      23
#define WORD_EIGHT    24
#define WORD_NINE     25
#define WORD_D        26
#define WORD_T_1      27
#define WORD_EN       28
#define WORD_IS_2     29
#define WORD_E        30
#define WORD_LEVEN    31
#define WORD_TWELVE   32
#define WORD_O        33
#define WORD_CLOCK    34

signed int WORDMAP[NUMWORDS][MAXLEDSPERWORD] = {
  [WORD_IT]        = {62,-1,-1,-1},
  [WORD_IS_1]      = {63,64,-1,-1},
  [WORD_TWENTY]    = {65,66,67,68},
  [WORD_FIVE_MIN]  = {69,70,-1,-1},
  [WORD_TEN]       = {60,61,-1,-1},
  [WORD_HA]        = {58,59,-1,-1},
  [WORD_LF]        = {57,-1,-1,-1},
  [WORD_PP]        = {56,-1,-1,-1},
  [WORD_QUARTER]   = {52,53,54,55},
  [WORD_PAST]      = {43,44,-1,-1},
  [WORD_BI]        = {45,-1,-1,-1},
  [WORD_TO]        = {46,-1,-1,-1},
  [WORD_ONE]       = {48,-1,-1,-1},
  [WORD_Y_1]       = {49,-1,-1,-1},
  [WORD_TWO]       = {50,51,-1,-1},
  [WORD_THREE]     = {40,41,42,-1},
  [WORD_FOU]       = {37,38,-1,-1},
  [WORD_R]         = {36,-1,-1,-1},
  [WORD_TH]        = {35,-1,-1,-1},
  [WORD_FIVE_HR]   = {33,34,-1,-1},
  [WORD_SIX]       = {23,24,-1,-1},
  [WORD_DA]        = {25,-1,-1,-1},
  [WORD_SEVEN]     = {26,27,28,-1},
  [WORD_Y_2]       = {29,-1,-1,-1},
  [WORD_EIGHT]     = {30,31,32,-1},
  [WORD_NINE]      = {21,22,-1,-1},
  [WORD_D]         = {20,-1,-1,-1},
  [WORD_T_1]       = {19,-1,-1,-1},
  [WORD_EN]        = {16,17,10,-1},
  [WORD_IS_2]      = {13,14,-1,-1},
  [WORD_E]         = {11,-1,-1,-1},
  [WORD_LEVEN]     = { 9,10,-1,-1},
  [WORD_TWELVE]    = { 0, 1, 2,-1},
  [WORD_O]         = { 4,-1,-1,-1},
  [WORD_CLOCK]     = { 7, 8,-1,-1},
};

#define BIRTHDAY_MSG_LEN 12
int BIRTHDAY_MSG[BIRTHDAY_MSG_LEN] = {
  WORD_HA, WORD_PP, WORD_Y_1,                     // "HAPPY"
  WORD_BI, WORD_R, WORD_TH, WORD_DA, WORD_Y_2,    // "BIRTHDAY"
  WORD_D, WORD_EN, WORD_IS_2, WORD_E              // "DENISE"
};

#endif

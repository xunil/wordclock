WORDS = {
	'it': [63],
	'is': [64, 65],
	'twenty': [66, 67, 68, 69],
	'five_minute': [70, 71],
	'ten': [61, 62],
	'ha': [59, 60],
	'lf': [58],
	'pp': [57],
	'quarter': [53, 54, 55, 56],
	'past': [44, 45],
	'bi': [46],
	'to': [47],
	'one': [49],
	'happy_y': [50],
	'two': [51, 52],
	'three': [41, 42, 43],
	'fou': [38, 38],
	'r': [37],
	'th': [36],
	'five': [34, 35],
	'six': [24, 25],
	'da': [26],
	'seven': [27, 28, 29],
	'birthday_y': [30],
	'eight': [31, 32, 33],
	'nine': [22, 23],
	'd': [21],
	't': [19],
	'en': [16, 17, 18],
	'is_denise': [14, 15],
	'e': [12],
	'leven': [10, 11],
	'twelve': [1, 2, 3],
	'o': [5],
	'clock': [8, 9]
}

HOUR_WORDS = [
	['twelve'],
	['one'],
	['two'],
	['three'],
	['fou', 'r'],
	['five'],
	['six'],
	['seven'],
	['eight'],
	['nine'],
	['ten'],
	['e', 'leven']
]

MINUTE_WORDS = [
	[],  # no add'l words required for 00-04
	['five_minute'],
	['ten'],
	['quarter'],
	['twenty'],
	['twenty', 'five_minute'],
	['ha', 'lf']
]

# wc
# initialize wifi
# intialize RTC
# from datetime tuple, figure out what words to light


# HH:00 o clock
# HH:05 five past
# HH:10 ten past
# HH:15 quarter past
# HH:20 twenty past
# HH:25 twenty five past
# HH:30 half past
# HH:35 twenty five to
# HH:40 twenty to
# HH:45 quarter to
# HH:50 ten to
# HH:55 five to

from datetime import datetime
import time
class WordClock:
	def __init__(self, dt=datetime.today()):
		self.hour = dt.hour % 12
		self.minute = dt.minute
		self.pixels = []
		self.lit_words = []

	def update(self, dt=datetime.today()):
		self.hour = dt.hour % 12
		self.minute = dt.minute
		self.update_words()
		self.update_pixels()

	def update_words(self):
		self.lit_words = ['it', 'is']  # always light these
		index = 0
		direction = None
		if self.minute >= 35:
			direction = 'to'
			index = len(MINUTE_WORDS) - ((self.minute - 30) / 5) - 1
		elif self.minute >= 5 and self.minute < 35:
			direction = 'past'
			index = self.minute / 5
		self.lit_words.extend(MINUTE_WORDS[index])
		if direction is not None:
			self.lit_words.append(direction)
		self.lit_words.extend(HOUR_WORDS[self.hour])
		self.lit_words.extend(['o', 'clock'])

	def update_pixels(self):
		self.pixels = [pixel_nums for word in self.lit_words for pixel_nums in WORDS[word]]
		self.pixels.sort()


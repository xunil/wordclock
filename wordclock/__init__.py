from datetime import datetime
from config import *
import time, machine, neopixel, ssd1306, network

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



# Set up the attached WS2812B strips to be all on at a dim white.

class WordClock:
	def __init__(self, dt=datetime.today()):
		self.hour = dt.hour % 12
		self.minute = dt.minute
		self.pixels = []
		self.lit_words = []

	# Broken out from __init__ because this way we can
	# test the class without a real MicroPython environment
	def setup(self):
		self.init_screen()
		self.status_message([
			['WordClock', 10, 10],
			['Starting', 15, 20]
		])
		self.init_neopixels()
		if not self.connect_to_wifi():
			self.init_wifi_ap()

	def init_screen(self):
		self.i2c = machine.I2C(scl=machine.Pin(I2C_SCL_PIN), sda=machine.Pin(I2C_SDA_PIN))
		self.oled = ssd1306.SSD1306_I2C(128, 64, self.i2c)

	def status_message(self, lines):
		self.oled.fill(0)
		for line in lines:
			self.oled.text(*line)
		self.oled.show()

	def init_neopixels(self):
		self.status_message([
			['Initializing', 10, 10],
			['LEDs', 15, 20]
		])
		self.np = neopixel.NeoPixel(machine.pin(LED_DATA_PIN), NUM_LEDS)
		for i in range(0, NUM_LEDS):
			self.np[i] = (32, 32, 32)
		self.np.write()

	def connect_to_wifi(self):
		self.sta_if = network.WLAN(network.STA_IF)
		self.sta_if.active(True)

		for ssid, password in WIFI_NETWORKS.iteritems():
			start_time = int(time.time())
			self.sta_if.connect(ssid, password)
			while True:
				time.sleep(1)
				if self.sta_if.isconnected():
					ifconfig = self.sta_if.ifconfig()
					self.status_message([
						['Connected to', 10, 10],
						[ssid, 10, 20],
						[ifconfig[0], 10, 30] # IP address
					])
					return True
				time_elapsed = (int(time.time()) - start_time)
				if time_elapsed > WIFI_CONNECT_TIMEOUT:
					break
				self.status_message([
					['Connecting to', 10, 10],
					[ssid, 10, 20],
					['Timeout in %ds' % (WIFI_CONNECT_TIMEOUT - time_elapsed), 10, 30]
				])
		return False

	def init_wifi_ap(self):
		# TODO: Set up a wireless network
		pass

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

	def render(self):
		# TODO: Do something more interesting than a half-bright white
		color = (128,128,128)
		for pixel in self.pixels:
			self.np[pixel] = color
		self.np.write()
		self.render_words_to_oled()

	def render_words_to_oled(self):
		MAX_LINE_LEN = 128 / 8
		words_to_render = self.lit_words
		line = ''
		line_num = 1
		oled_lines = []
		while len(words_to_render) > 0:
			word = words_to_render.pop()
			if (len(line) + len(word) + 1) < MAX_LINE_LEN:
				line = line + ' ' + word
			else:
				oled_lines.append([line, 10, line_num*10])
				line_num = line_num + 1
				line = ''
				words_to_render.unshift(word)
		self.status_message(words_to_render)

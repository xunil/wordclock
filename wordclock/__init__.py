import utime
from config import *
import time
import machine
import neopixel
import ssd1306
import network
import ntptime


WORDS = {
    'it': [62],
    'is': [63, 64],
    'twenty': [65, 66, 67, 68],
    'five_minute': [69, 70],
    'ten': [60, 61],
    'ha': [58, 59],
    'lf': [57],
    'pp': [56],
    'quarter': [52, 53, 54, 55],
    'past': [43, 44],
    'bi': [45],
    'to': [46],
    'one': [48],
    'happy_y': [49],
    'two': [50, 51],
    'three': [40, 41, 42],
    'fou': [37, 37],
    'r': [36],
    'th': [35],
    'five': [33, 34],
    'six': [23, 24],
    'da': [25],
    'seven': [26, 27, 28],
    'birthday_y': [29],
    'eight': [30, 31, 32],
    'nine': [21, 22],
    'd': [20],
    't': [18],
    'en': [15, 16, 17],
    'is_denise': [13, 14],
    'e': [11],
    'leven': [9, 10],
    'twelve': [0, 1, 2],
    'o': [4],
    'clock': [7, 8]
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
	def __init__(self, dt=None):
		if dt is not None:
			self.hour = dt[4] % 12
			self.minute = dt[5]
		else:
			self.hour = 0
			self.minute = 0
		self.pixels = []
		self.lit_words = []
		self.tz_offset = -8  # Add one hour during DST

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
		else:
			self.init_time()


	def init_screen(self):
		self.i2c = machine.I2C(scl=machine.Pin(I2C_SCL_PIN), sda=machine.Pin(I2C_SDA_PIN))
		self.oled = ssd1306.SSD1306_I2C(128, 64, self.i2c)

	def status_message(self, lines):
		self.oled.fill(0)
		for line in lines:
			self.oled.text(*line)
		self.oled.show()

	def init_time(self):
		time_set = False
		while not time_set:
			print("Attempting to get NTP time")
			try:
				ntptime.settime()
				print("Success")
				time_set = True
			except OSError:
				print("Failed; retrying in 5 seconds")
				time.sleep(5)
		self.rtc = machine.RTC()
		print("RTC datetime is now %s" % repr(self.rtc.datetime()))

	def init_neopixels(self):
		self.status_message([
			['Initializing', 10, 10],
			['LEDs', 15, 20]
		])
		self.np = neopixel.NeoPixel(machine.Pin(LED_DATA_PIN), NUM_LEDS)
		for i in range(0, NUM_LEDS):
			self.np[i] = (32, 32, 32)
		self.np.write()

	def connect_to_wifi(self):
		self.sta_if = network.WLAN(network.STA_IF)
		self.sta_if.active(True)

		for ssid, password in WIFI_NETWORKS.items():
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

	def is_dst(self, dt):
		# Algorithm lifted from https://stackoverflow.com/questions/5590429/calculating-daylight-saving-time-from-only-date
	 	year,month,day,dow,hour,minute,second,rest = dt
	 	if month < 3 or month > 11:
	 		return False
 		if month > 3 and month < 11:
 			return True
		previous_sunday = day - dow
		if month == 3:
			return previous_sunday >= 8
		return previous_sunday <= 0

	def update(self, dt=None):
		if dt is None:
			dt = self.rtc.datetime()
		if self.is_dst(dt):
			self.hour = (dt[4] + (self.tz_offset + 1)) % 12
		else:
			self.hour = (dt[4] + self.tz_offset) % 12
		self.minute = dt[5]
		self.update_words()
		self.update_pixels()

	def update_words(self):
		self.lit_words = ['it', 'is']  # always light these
		index = 0
		direction = None
		if self.minute >= 35:
			direction = 'to'
			index = int(len(MINUTE_WORDS) - ((self.minute - 30) / 5) - 1)
		elif self.minute >= 5 and self.minute < 35:
			direction = 'past'
			index = int(self.minute / 5)
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
		MAX_LINE_LEN = 128 / 10
		words_to_render = self.lit_words.copy()
		line = ''
		line_num = 1
		oled_lines = []
		while len(words_to_render) > 0:
			word = words_to_render.pop(0)
			if (len(line) + len(word) + 1) < MAX_LINE_LEN:
				if len(line) == 0:
					line = word
				else:
					line = line + ' ' + word
			else:
				oled_lines.append([line, 10, line_num*10])
				line_num = line_num + 1
				line = ''
				words_to_render.insert(0, word)
		if len(line) > 0:
			oled_lines.append([line, 10, line_num*10])

		print(repr(oled_lines))
		self.status_message(oled_lines)

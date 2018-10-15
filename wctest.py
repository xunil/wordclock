from wordclock import *
from datetime import datetime

wc = WordClock()
for minute in range(0, 60):
	dt = datetime(2018, 10, 12, 5, minute)
	wc.update(dt)
	print '[%02d:%02d] ' % (dt.hour % 12, dt.minute),
	print ' '.join(wc.lit_words),
	print ' ',
	print repr(wc.pixels)
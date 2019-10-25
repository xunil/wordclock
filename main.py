from wordclock import WordClock
import time

wc = WordClock()
wc.setup()

while True:
	wc.update()
	wc.render()
	time.sleep(20)


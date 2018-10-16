from wordclock import WordClock
import time

wc = WordClock()
wc.setup()

while True:
	time.sleep(5)
	wc.update()
	wc.render()
	

# TODO
## Required features
* ~~Timezone handling~~
	* ~~MicroPython treats everything as UTC, so we'll have to do our own timezone math.~~
* AP mode
	* If the configured wifi networks are not found, should enter AP mode and provide a UI for adding/changing wifi networks.
* Birthday greeting
	* Every so often on Denise's birthday, display the 'hidden' birthday message.
* ~~Off-by-one in WORDS hash~~
	* ~~I accidentally started indexing at 1.  Update the entire hash to subtract 1 from every number, then reduce `NUM_LEDS` by 1.~~
* Bug: past/to handling
  * When the direction switches to 'to', the hour should be incremented by one
  * Seems to be a bug in the last 5 minutes of the hour, possibly off-by-one; e.g. "it is to nine o clock"

## Desired features
* General purpose web UI
	* Color/brightness selection

## Nice-to-have
* Avahi/zeroconf support
	* Would be nice to be able to hit the web UI at wordclock.local instead of IP

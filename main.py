
from gpiozero import LED
from time import sleep

ledRelay = LED(21)
ledBuzz = LED(20)
print("Running.....Ctrl+C to exit")

ledBuzz.on()
while True:
    ledRelay.on()
    ledBuzz.off()

ledRelay.off()


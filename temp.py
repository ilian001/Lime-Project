from gpiozero import LED
from time import sleep


ledBuzz = LED(20)
print("Running ......... Ctrl+C to exit")

ledBuzz.on()
while True:

    ledBuzz.off()


ledBuzz.on()

import RPi.GPIO as GPIO
import time

PIN = 12

GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

try:
 while True:
  GPIO.output(PIN,GPIO.HIGH)
  #time.sleep(0.002)
  #GPIO.output(PIN, GPIO.LOW)

except KeyboardInterrupt:
 GPIO.cleanup()


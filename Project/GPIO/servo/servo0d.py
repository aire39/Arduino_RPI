import RPi.GPIO as GPIO
import time

PIN = 11

GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

try:
 while True:
  GPIO.output(PIN,GPIO.HIGH)
  time.sleep(0.0015)
  GPIO.output(PIN, GPIO.LOW)
  time.sleep(2)

except KeyboardInterrupt:
 GPIO.cleanup()


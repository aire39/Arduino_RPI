import RPi.GPIO as GPIO
import time

PIN = 11

GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

try:
 for i in range(0,15):
  GPIO.output(PIN,GPIO.HIGH)
  time.sleep(0.002)
  GPIO.output(PIN, GPIO.LOW)

except KeyboardInterrupt:
 GPIO.cleanup()


import RPi.GPIO as GPIO
import time

PIN = 11
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

try:
 while True:
  for i in range(0,15):
   print "rest\n"
   GPIO.output(PIN,GPIO.HIGH)
   time.sleep(0.0015)
   GPIO.output(PIN, GPIO.LOW)
   time.sleep(0.0095)
  time.sleep(0.5)

  for i in range(0,15): 
   print "-90\n"
   GPIO.output(PIN,GPIO.HIGH)
   time.sleep(0.002)
   GPIO.output(PIN, GPIO.LOW)
   time.sleep(0.018)
  time.sleep(0.5)

  for i in range(0,15):
   print "rest\n"
   GPIO.output(PIN,GPIO.HIGH)
   time.sleep(0.0015)
   GPIO.output(PIN, GPIO.LOW)
   time.sleep(0.0095)
  time.sleep(0.5)

  for i in range(0,15):
   print "90\n"
   GPIO.output(PIN,GPIO.HIGH)
   time.sleep(0.001)
   GPIO.output(PIN, GPIO.LOW)
   time.sleep(0.019)
  time.sleep(0.5)
except KeyboardInterrupt:
 GPIO.cleanup()


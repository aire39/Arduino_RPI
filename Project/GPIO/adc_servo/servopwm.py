from Tkinter import *
from Adafruit_PWM_Servo_Driver import PWM
import adc_joystick_read as adc 
import time

pwm = PWM(0x40)
pwm.setPWMFreq(60) # Set frequency to 60 Hz

class App:

    def __init__(self, master):
        self.frame = Frame(master)
        self.frame.pack(fill=BOTH, expand=1)
        scale = Scale(self.frame, from_=150, to=600,
              orient=HORIZONTAL, command=self.update)
        scale.grid(row=0)

        self.var = IntVar()
        self.var2 = IntVar()
        self.var3 = IntVar()

        cb = Checkbutton(self.frame, text="servo1", variable=self.var, command=self.onClick)

        cb.select()
        cb.place(x=10, y=50)

        cb2 = Checkbutton(self.frame, text="servo2", variable=self.var2, command=self.onClick2)
        cb2.place(x=10, y=80)


        cb3 = Checkbutton(self.frame, text="joystick", variable=self.var3, command=self.onClick3)
        cb3.place(x=10, y=110)


        self.frame.id = self.frame.after(16, self.callback)

    def callback(self):

        if self.var3.get() == 1:
            adc.update_joystick()

            aa = adc.last_read / 1024.0
            bb = adc.last_read2 / 1024.0

            rr = 150.0 + 450.0 * aa
            ss = 150.0 + 450.0 * bb
        
            print "%d %d" % (int(rr), int(ss))

            if self.var.get() == 1:
                pwm.setPWM(0, 0, int(rr))

            if self.var2.get() == 1:
                pwm.setPWM(1, 0, int(ss))
    
        self.frame.id = self.frame.after(16, self.callback)

    def update(self, angle):

        if self.var.get() == 1:
            pwm.setPWM(0, 0, int(angle))


        if self.var2.get() == 1:
            pwm.setPWM(1, 0, int(angle))

    def onClick(self):
        if self.var.get() == 1:
            print "selected!"
        else:
            print "not selected"


    def onClick2(self):
        if self.var2.get() == 1:
            print "selected!"
        else:
            print "not selected"

    def onClick3(self):
        if self.var3.get() == 1:
            print "enabled!"
        else:
            print "disable"


root = Tk()
root.wm_title('Servo Control')
app = App(root)
root.geometry("200x50+0+0")
root.mainloop()


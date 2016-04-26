/*
* Arduino Pro Mini (https://www.adafruit.com/products/2378)
*
* Using this to take over the resposniblity of how the controller recieves/sends
* data without the Snes console to the bluetooth (https://www.adafruit.com/product/1535) chip.
* 
* The Pro mini is an 8-bit chip so I needed 2 bytes to hold the button data since the SNES is a 16-bit
* system.
*/

// Snes controller variables
const byte nPulses = 16; // number of data cycles (16 because there could be 16 different buttons)
byte pulseCount    = 1;  // counter when looping

unsigned long currentTime    = 0;
unsigned long previousTime   = 0;
const unsigned long interval = 16670;
byte sample  = 0;
byte data[2] = {0};

const byte LATCH   = 9;
const byte DATACLK = 8;
const byte DATA    = 7;

// For shift register for final output to bluetooth
// Using ez-bluetooth keyboard so no need to send data serially.
const byte SHIFTER  = 2;
const byte SEND_OUT = 3;
const byte BTN_CHANNEL_0 = 5;
const byte BTN_CHANNEL_1 = 6;

// Functions for shift register SN74HC595 (https://www.sparkfun.com/products/13699)
void shift();
void setBit(const byte channel, const byte data_bit);
void sendData();
void outToShiftRegister(const byte data0, const byte data1);

// Debug data/functions
const byte LED = 13;
void printData();
void buttonIsPressed();

void setup() {
  //Serial.begin(115200); // Meant for when printing to serial monitor

  // Setup pin modes
  pinMode(LED    , OUTPUT);
  pinMode(DATACLK, OUTPUT); // Data Clock
  pinMode(LATCH  , OUTPUT); // latch
  pinMode(DATA   , INPUT);  // recieved bit from controller

  pinMode(SHIFTER      , OUTPUT);
  pinMode(SEND_OUT     , OUTPUT);
  pinMode(BTN_CHANNEL_0, OUTPUT);
  pinMode(BTN_CHANNEL_1, OUTPUT);

  // Initial
  digitalWrite(DATACLK  , HIGH);
  digitalWrite(LATCH    , LOW);
  digitalWrite(SHIFTER  , LOW);
  digitalWrite(SEND_OUT , LOW);
  digitalWrite(BTN_CHANNEL_0 , LOW);
  digitalWrite(BTN_CHANNEL_1 , LOW);
}

void loop() {

  // Initial
  digitalWrite(DATACLK, HIGH);
  digitalWrite(LATCH  , LOW);

  currentTime = micros();

  // Run every 16.67ms
  if((currentTime - previousTime) >= interval)
  {
    previousTime = currentTime;
    
    // CPU to notify ICs to latch state of all buttons
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(12);
    digitalWrite(LATCH, LOW);
    
    // Wait 6 microseconds
    delayMicroseconds(6);
    
    // Send 16 clock pulses that are 50% duty with 12us cycle
    // sample the bit from the controller data wire. Start at left most bit
    // and shift right so that all bits will be in order when sending
    // to shift register.
    while(pulseCount < nPulses)
    {
      if(pulseCount == 0)
      {
        digitalWrite(DATACLK, HIGH);
        delayMicroseconds(6); // Pulse

        sample = digitalRead(DATA);
        
        digitalWrite(DATACLK, LOW);
        delayMicroseconds(6); // Pulse

        data[0] |= (sample == 1) ? 0x80 : 0;
        data[0] >>= 1;
      }
      else
      {
        digitalWrite(DATACLK, LOW);      
        delayMicroseconds(6); // Pulse

        if(pulseCount < 8)
        {
          data[0] |= (sample == 1) ? 0x80 : 0;
          data[0] >>= 1;
        }
        else
        {
          data[1] |= (sample == 1) ? 0x80 : 0;
          data[1] >>= 1;
        }
        
        digitalWrite(DATACLK, HIGH);

        // Have to make sure end bits do not do a rshift
        if(pulseCount == 7)
        {
          sample = digitalRead(DATA);
          data[0] |= (sample == 1) ? 0x80 : 0;
        }
        else if(pulseCount == 15)
        {
          sample = digitalRead(DATA);
          data[1] |= (sample == 1) ? 0x80 : 0;
        }
        
        delayMicroseconds(6); // Pulse

        sample = digitalRead(DATA);
      }

      pulseCount++;
    }
    pulseCount = 0;

    // invert data because the controller sees 0 as a button press
    // and I want a 1 so that it will correspond to the ez-bluetooth
    // keyboard since you have to send it power to a wire for it to send
    // a signal.
    data[0] ^= 0xFF;
    data[1] ^= 0xFF;

    // Debug information
    //printData();
    buttonIsPressed(data[0], data[1]);
    
    // output data to shifter (serially)
    outToShiftRegister(data[0], data[1]);
    
    // clear data.
    data[0] = 0;
    data[1] = data[0];
  }
}

void shift()
{
  digitalWrite(SHIFTER, HIGH);
  digitalWrite(SHIFTER, LOW);
}

void setBit(const byte channel, const byte data_bit)
{
  digitalWrite(channel, data_bit);
}

void sendData()
{
  digitalWrite(SEND_OUT, HIGH);
  digitalWrite(SEND_OUT, LOW);
}

void outToShiftRegister(const byte data0, const byte data1)
{
  byte out = 0;
  for(byte i=0; i<8; i++)
  {
    out = (data0 >> i) & 0x1;
    setBit(BTN_CHANNEL_0, out);

    out = (data1 >> i) & 0x1;
    setBit(BTN_CHANNEL_1, out);
    
    shift();
  }
  sendData();
}

void printData()
{
  Serial.println("");
  for(int i=0; i<2; i++)
  {
    Serial.print("BYTE "); // byte element
    Serial.print(i, DEC); //
    Serial.print("-->");
    Serial.print("received: ");
    Serial.print(data[i], BIN);
    Serial.print(" ");
    Serial.println(data[i], DEC);
  }
}

void buttonIsPressed(byte channel_1, byte channel_2)
{
  if( channel_1 > 0 || channel_2 > 0)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);  
}

/*
* Manually Simulate Controller Input Using Arduino Pro Mini 
* SNES Controller info (http://www.gamefaqs.com/snes/916396-super-nintendo/faqs/5395) 
* 
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
const unsigned long interval = 16670; // 60hz -> 16.67ms in microseconds
byte sample  = 0;   // current bit controller clock cycle that will get shifted into the data array.
byte data[2] = {0}; // 2 bytes for button data/ First byte holds the first 8 buttons and the other byte holds the last 4 buttons

const byte LATCH   = 9; // controller wire latch (OUT)
const byte DATACLK = 8; // controller data clock (OUT)
const byte DATA    = 7; // controller data wire  (IN)

// For shift register (74HC595) for final output to bluetooth
// Using ez-bluetooth keyboard so no need to send data serially.
const byte SHIFTER_RCLK   = 6; // latch for shift register     (OUT)
const byte SEND_OUT_SRCLK = 5; // shift bit in shift register  (OUT)
const byte BTN_CHANNEL_0 = 11; // bit pattern for buttons 1-8  (OUT)
const byte BTN_CHANNEL_1 = 10; // bit pattern for buttons 8-12 (OUT)

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

  pinMode(SHIFTER_RCLK   , OUTPUT); // latch
  pinMode(SEND_OUT_SRCLK , OUTPUT); // clock
  pinMode(BTN_CHANNEL_0, OUTPUT);
  pinMode(BTN_CHANNEL_1, OUTPUT);

  // Initial
  digitalWrite(DATACLK  , HIGH);
  digitalWrite(LATCH    , LOW);
  digitalWrite(SHIFTER_RCLK  , LOW);
  digitalWrite(SEND_OUT_SRCLK, LOW);
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

    // Invert bit pattern because the controller sees 0 as a button press
    // and I want a 1 to flash the boards led.
    
    data[0] ^= 0xFF;
    data[1] ^= 0xFF;

    // Debug information
    buttonIsPressed(data[0], data[1]);
    //printData();

    // Need to invert the bit pattern back as a 0 in the bit pattern
    // means the button is pressed and that's how the blubetooth module
    // knows to respond to a button press.
    data[0] ^= 0xFF;
    data[1] ^= 0xFF;
    
    // output data to shifter (serially)
    outToShiftRegister(data[0], data[1]);
    
    // clear data.
    data[0] = 0;
    data[1] = data[0];
  }
}

// Shift set bit over to next bit position in the shift register
void shift()
{
  digitalWrite(SEND_OUT_SRCLK, HIGH);
  digitalWrite(SEND_OUT_SRCLK, LOW);
}

// Set bit
void setBit(const byte channel, const byte data_bit)
{
  digitalWrite(channel, data_bit);
}

// Wait for bit fields to bits to be set
void lockData()
{
  digitalWrite(SHIFTER_RCLK, LOW);
}

// Release set bits through the wires
void unlockData()
{
  digitalWrite(SHIFTER_RCLK, HIGH);
}

// Send button data through shift register and into bluetooth
void outToShiftRegister(const byte data0, const byte data1)
{
  //shiftOut(BTN_CHANNEL_0, SEND_OUT_SRCLK, LSBFIRST, data0);
  
  lockData(); // collect bit data

  // Fill shift register completely with 8 bits to preserve bit order
  for(byte i=0; i<8; i++)
  {
    setBit(BTN_CHANNEL_0, (data0 >> i) & 0x01);
    setBit(BTN_CHANNEL_1, (data1 >> i) & 0x01);
    shift();
  }

  unlockData(); // send through
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

// Light up on board led if any button is pressed in the bit pattern
void buttonIsPressed(byte channel_1, byte channel_2)
{
  if( channel_1 > 0 || channel_2 > 0)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);  
}

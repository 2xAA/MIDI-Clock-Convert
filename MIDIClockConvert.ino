// Requires a USB Host shield
// Constructed using Circuits@Home's Arduino Shield
//
// USB Library: https://github.com/felis/USB_Host_Shield_2.0
// (USBH_MIDI is included in the above lib)
//
// Connect MIDI OUT to RX (Serial2 is being used for Arduino Mega, so connect to RX2 or change Serial)

#include <Usb.h>
#include <usbh_midi.h>
#include <SPI.h>

USB  Usb;
USBH_MIDI  Midi(&Usb);

byte midi_clock = 0xf8;
byte data;

uint8_t msg[1];

void setup() {
  msg[0] = midi_clock;
  Serial2.begin(31250); // MIDI baude rate
  
  //Workaround for non UHS2.0 Shield 
  //pinMode(7,OUTPUT);
  //digitalWrite(7,HIGH);

  if (Usb.Init() == -1) {
    while (1); //halt
  }
}

void loop() {
  // Poll USB
  Usb.Task();

  // If USB Host is available...
  if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
    // If Serial(x) is available...
    if (Serial2.available()) {
      // Read Serial(x)
      data = Serial2.read();
      
      // If it's a MIDI clock byte...
      if ((data == midi_clock)) {
        // Send MIDI clock byte over USB
        Midi.SendData(msg, 0);
      }
    }
  }
}

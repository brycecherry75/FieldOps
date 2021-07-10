/*

  FieldOps demo by Bryce Cherry

  The following command will not generate any errors:
  A B C D E F G
  A: Hello (case sensitive)
  B: case (case insensitive)
  C: 100-105 (integer)
  D: 2000.5-3025.4 (float)
  E: -225 to -158 (integer)
  F: -5302.1 to -1000.2 (float)
  G: 0x12443012 to 0x12501244 (hexadecimal)

*/

#include <FieldOps.h>

const word CommandSize = 100;
const word FieldLength = 20;
char Command[CommandSize];

// ensures that the serial port is flushed fully on request
const unsigned long SerialPortRate = 9600;
const byte SerialPortRateTolerance = 5; // percent - increase to 50 for rates above 115200 up to 4000000
const byte SerialPortBits = 10; // start (1), data (8), stop (1)
const unsigned long TimePerByte = ((((1000000ULL * SerialPortBits) / SerialPortRate) * (100 + SerialPortRateTolerance)) / 100); // calculated on serial port rate + tolerance and rounded down to the nearest uS, long caters for even the slowest serial port of 75 bps

void FlushSerialBuffer() {
  while (true) {
    if (Serial.available() > 0) {
      byte dummy = Serial.read();
      while (Serial.available() > 0) { // flush additional bytes from serial buffer if present
        dummy = Serial.read();
      }
      if (TimePerByte <= 16383) {
        delayMicroseconds(TimePerByte); // delay in case another byte may be received via the serial port
      }
      else { // deal with delayMicroseconds limitation
        unsigned long DelayTime = TimePerByte;
        DelayTime /= 1000;
        if (DelayTime > 0) {
          delay(DelayTime);
        }
        DelayTime = TimePerByte;
        DelayTime %= 1000;
        if (DelayTime > 0) {
          delayMicroseconds(DelayTime);
        }
      }
    }
    else {
      break;
    }
  }
}

void setup() {
  Serial.begin(SerialPortRate);
}

void loop() {
  static word ByteCount = 0;
  if (Serial.available() > 0) {
    char value = Serial.read();
    if (value != '\n' && ByteCount < CommandSize) {
      Command[ByteCount] = value;
      ByteCount++;
    }
    else {
      ByteCount = 0;
      byte mask = 1;
      byte Errors = 0;
      if (FieldOps.compareString(CommandSize, FieldLength, Command, "Hello", 0, 0x20, 0x0D, true) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareString(CommandSize, FieldLength, Command, "caSe", 1, 0x20, 0x0D, false) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareInt(CommandSize, FieldLength, Command, 100, 105, 2, 0x20, 0x0D) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareFloat(CommandSize, FieldLength, Command, 2000.5, 3025.4, 3, 0x20, 0x0D) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareInt(CommandSize, FieldLength, Command, -225, -158, 4, 0x20, 0x0D) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareFloat(CommandSize, FieldLength, Command, -5302.1, -1000.2, 5, 0x20, 0x0D) == false) {
        Errors |= mask;
      }
      mask <<= 1;
      if (FieldOps.compareHex(CommandSize, FieldLength, Command, 0x12443012, 0x12501244, 6, 0x20, 0x0D) == false) {
        Errors |= mask;
      }
      Serial.print(F("Field 0 is "));
      char ExtractedField[FieldLength];
      uint16_t LengthOfField = FieldOps.extractField(CommandSize, FieldLength, Command, 0, 0x20, 0x0D, ExtractedField);
      Serial.print(ExtractedField);
      Serial.print(F(" with length of "));
      Serial.print(LengthOfField);
      Serial.println(F(" characters excluding null terminator"));
      Serial.print(F("Characters with case sensitivity in Field 0 are "));
      if (FieldOps.checkValidCharacters(LengthOfField, ExtractedField, "Helo", 5, true) == false) {
        Serial.print(F("not "));
      }
      Serial.print(F("valid and has "));
      Serial.print(FieldOps.countCharacterInstances(LengthOfField, ExtractedField, 'l', true));
      Serial.println(F(" instances of 'l'"));
      LengthOfField = FieldOps.extractField(CommandSize, FieldLength, Command, 1, 0x20, 0x0D, ExtractedField);
      Serial.print(F("Characters without case sensitivity in Field 1 are "));
      if (FieldOps.checkValidCharacters(LengthOfField, ExtractedField, "CaSe", 5, false) == false) {
        Serial.print(F("not "));
      }
      Serial.print(F("valid and has "));
      Serial.print(FieldOps.countCharacterInstances(LengthOfField, ExtractedField, 's', false));
      Serial.println(F(" instances of 's'"));
      FieldOps.changeCharactersTo(LengthOfField, ExtractedField, "CS", "FD", 3);
      Serial.print(F("Field 1 is now "));
      Serial.println(ExtractedField);
      Serial.print(F("Field 2 length between separators is "));
      Serial.print(FieldOps.checkFieldLength(CommandSize, Command, 2, 0x20, 0x0D));
      Serial.println(F(" characters long"));
      Serial.print(F("Field 3 is "));
      FieldOps.extractField(CommandSize, FieldLength, Command, 3, 0x20, 0x0D, ExtractedField);
      if (FieldOps.checkNumber(ExtractedField, FieldLength, false) == true) {
        Serial.println(F("a valid integer"));
      }
      else if (FieldOps.checkNumber(ExtractedField, FieldLength, true) == true) {
        Serial.println(F("a valid float"));
      }
      else {
        Serial.println(F("not a valid number"));
      }
      Serial.print(F("Field 6 is "));
      FieldOps.extractField(CommandSize, FieldLength, Command, 6, 0x20, 0x0D, ExtractedField);
      bool FieldIsHexadecimal = FieldOps.checkHex(ExtractedField, FieldLength);
      if (FieldIsHexadecimal == true) {
        Serial.println(F("a valid hexadecimal number"));
      }
      else if (FieldOps.checkNumber(ExtractedField, FieldLength, false) == true) {
        Serial.println(F("a valid integer"));
      }
      else if (FieldOps.checkNumber(ExtractedField, FieldLength, true) == true) {
        Serial.println(F("a valid float"));
      }
      else {
        Serial.println(F("not a valid number"));
      }
      Serial.print(F("Extracted Field 2 value is "));
      Serial.println(FieldOps.extractInt(CommandSize, FieldLength, Command, 2, 0x20, 0x0D));
      Serial.print(F("Extracted Field 3 value is "));
      Serial.println(FieldOps.extractFloat(CommandSize, FieldLength, Command, 3, 0x20, 0x0D));
      Serial.print(F("Extracted Field 6 value is "));
      if (FieldIsHexadecimal == true) {
        Serial.print(F("0x"));
        Serial.println((FieldOps.extractHex(CommandSize, FieldLength, Command, 6, 0x20, 0x0D)), HEX);
      }
      else {
        Serial.println((FieldOps.extractInt(CommandSize, FieldLength, Command, 6, 0x20, 0x0D)), DEC);
      }
      Serial.print(F("Fields counted is "));
      Serial.println(FieldOps.countFields(CommandSize, Command, 0x20, 0x0D));
      if (Errors == 0) {
        Serial.println(F("Serial string is correct"));
      }
      else {
        mask = 1;
        for (int i = 0; i < 8; i++) {
          if ((Errors & mask) != 0) {
            Serial.print(F("Field "));
            Serial.print(i);
            Serial.println(F(" error"));
          }
          mask <<= 1;
        }
      }
      FlushSerialBuffer();
      Serial.println(F("Ready"));
    }
  }
}
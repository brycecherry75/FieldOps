#include "FieldOps.h"

uint16_t FieldOpsClass::extractField(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType, char *FieldExtracted) {
  uint16_t InputBufferPos = 0;
  uint16_t FieldPos = 0;
  uint16_t SeparatorCount = 0;
  while (InputBufferPos < InputBufferLength && FieldPos < MaximumFieldLength) {
    if (InputBuffer[InputBufferPos] == SeparatorType) {
      SeparatorCount++;
      InputBufferPos++;
    }
    if (SeparatorCount == FieldNumber) {
      FieldExtracted[FieldPos] = InputBuffer[InputBufferPos];
      FieldPos++;
    }
    InputBufferPos++;
    if (SeparatorCount > FieldNumber || InputBuffer[InputBufferPos] == TerminatorType) {
      break;
    }
  }
  if (FieldPos < MaximumFieldLength) { // write terminator if within the bounds of the array
    FieldExtracted[FieldPos] = 0x00;
  }
  else { // extracted field is too long
    FieldExtracted[0] = 0x00;
    FieldPos = 0;
  }
  return FieldPos;
}

uint16_t FieldOpsClass::checkFieldLength(uint16_t InputBufferLength, const char *InputBuffer, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType) {
  uint16_t InputBufferPos = 0;
  uint16_t FieldPos = 0;
  uint16_t SeparatorCount = 0;
  while (InputBufferPos < InputBufferLength) {
    if (InputBuffer[InputBufferPos] == SeparatorType) {
      SeparatorCount++;
      InputBufferPos++;
    }
    if (SeparatorCount == FieldNumber) {
      FieldPos++;
    }
    InputBufferPos++;
    if (SeparatorCount > FieldNumber || InputBuffer[InputBufferPos] == TerminatorType) {
      break;
    }
  }
  return FieldPos;
}

bool FieldOpsClass::checkNumber(const char *StringToCompare, uint16_t MaximumFieldLength, bool FloatUsed) {
  bool ValidNumber = true;
  bool SignPresent = false;
  uint8_t DecimalPointCount = 0;
  for (uint16_t i = 0; i < MaximumFieldLength; i++) {
    if ((StringToCompare[i] >= '0' && StringToCompare[i] <= '9') || StringToCompare[i] == '+' || StringToCompare[i] == '-' || StringToCompare[i] == '.' || StringToCompare[i] == 0x00) {
      if (StringToCompare[i] == '.') {
        DecimalPointCount++;
        if (FloatUsed == false || i == 0 || DecimalPointCount > 1) {
          ValidNumber = false;
          break;
        }
      }
      else if (StringToCompare[i] == '+' || StringToCompare[i] == '-') {
        if (SignPresent == false) {
          SignPresent == true;
          if (i != 0) {
            ValidNumber = false;
            break;
          }
        }
        else {
          ValidNumber = false;
          break;
        }
      }
      else if (StringToCompare[i] == 0x00) { // terminator detected
        break;
      }
    }
    else {
      ValidNumber = false;
      break;
    }
  }
  return ValidNumber;
}

bool FieldOpsClass::compareString(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, const String StringValueToCompare, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType, bool CaseSensitive) {
  char ComparisonBuffer[MaximumFieldLength];
  extractField(InputBufferLength, MaximumFieldLength, InputBuffer, FieldNumber, SeparatorType, TerminatorType, ComparisonBuffer);
  char ValueToCompare[MaximumFieldLength];
  StringValueToCompare.toCharArray(ValueToCompare, MaximumFieldLength);
  if (strlen(ValueToCompare) == strlen(ComparisonBuffer)) {
    if (CaseSensitive == false) {
      strupr(ComparisonBuffer);
      strupr(ValueToCompare);
    }
    if (strcmp(ValueToCompare, ComparisonBuffer) == 0) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

bool FieldOpsClass::compareInt(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, int32_t MinimumValue, int32_t MaximumValue, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType) {
  char ComparisonBuffer[MaximumFieldLength];
  extractField(InputBufferLength, MaximumFieldLength, InputBuffer, FieldNumber, SeparatorType, TerminatorType, ComparisonBuffer);
  if (checkNumber(ComparisonBuffer, MaximumFieldLength, false) == true) {
    int32_t ValueToCompare = atol(ComparisonBuffer);
    if (MinimumValue <= MaximumValue && ValueToCompare >= MinimumValue && ValueToCompare <= MaximumValue) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

bool FieldOpsClass::compareFloat(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, float MinimumValue, float MaximumValue, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType) {
  char ComparisonBuffer[MaximumFieldLength];
  extractField(InputBufferLength, MaximumFieldLength, InputBuffer, FieldNumber, SeparatorType, TerminatorType, ComparisonBuffer);
  if (checkNumber(ComparisonBuffer, MaximumFieldLength, true) == true) {
    float ValueToCompare = atof(ComparisonBuffer);
    if (MinimumValue <= MaximumValue && ValueToCompare >= MinimumValue && ValueToCompare <= MaximumValue) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

uint16_t FieldOpsClass::countCharacterInstances(uint16_t BufferLength, const char *InputBuffer, uint8_t CharacterToCount, bool CaseSensitive) {
  uint16_t CharactersCounted = 0;
  if (CaseSensitive == false) {
    CharacterToCount = toupper(CharacterToCount);
  }
  for (uint16_t i = 0; i < BufferLength; i++) {
    byte temp = InputBuffer[i];
    if (CaseSensitive == false) {
      temp = toupper(temp);
    }
    if (temp == CharacterToCount) {
      CharactersCounted++;
    }
  }
  return CharactersCounted;
}

bool FieldOpsClass::checkValidCharacters(uint16_t BufferLength, const char *InputBuffer, const String CharactersToCheckFor, const uint16_t CharacterCheckCount, bool CaseSensitive) {
  bool ValidCharactersFound;
  char ValidCharacters[CharacterCheckCount];
  CharactersToCheckFor.toCharArray(ValidCharacters, CharacterCheckCount);
  if (CaseSensitive == false) {
    strupr(ValidCharacters);
  }
  for (uint16_t BufferPos = 0; BufferPos < BufferLength; BufferPos++) {
    ValidCharactersFound = false;
    for (uint16_t ValidCharacterPos = 0; ValidCharacterPos < CharacterCheckCount; ValidCharacterPos++) {
      byte temp = InputBuffer[BufferPos];
      if (CaseSensitive == false) {
        temp = toupper(temp);
      }
      if (temp == ValidCharacters[ValidCharacterPos]) {
        ValidCharactersFound = true;
        break;
      }
    }
    if (ValidCharactersFound == false) {
      break;
    }
  }
  return ValidCharactersFound;
}

void FieldOpsClass::changeCharactersTo(uint16_t BufferLength, char *InputBuffer, const String CharactersToCheckFor, const String CharactersToChangeTo, const uint16_t CharacterChangeCount) {
  char OldCharacters[CharacterChangeCount];
  char NewCharacters[CharacterChangeCount];
  CharactersToCheckFor.toCharArray(OldCharacters, CharacterChangeCount);
  CharactersToChangeTo.toCharArray(NewCharacters, CharacterChangeCount);
  if (strlen(OldCharacters) == strlen(NewCharacters)) {
    for (uint16_t BufferPos = 0; BufferPos < BufferLength; BufferPos++) {
      for (uint16_t CharacterCheckPos = 0; CharacterCheckPos < CharacterChangeCount; CharacterCheckPos++) {
        if (InputBuffer[BufferPos] == OldCharacters[CharacterCheckPos]) {
          InputBuffer[BufferPos] = NewCharacters[CharacterCheckPos];
          break;
        }
      }
    }
  }
}

uint16_t FieldOpsClass::countFields(uint16_t InputBufferLength, const char *InputBuffer, uint8_t SeparatorType, uint8_t TerminatorType) {
  uint16_t FieldCount = 1;
  for (uint16_t i = 0; i < InputBufferLength; i++) {
    byte temp = InputBuffer[i];
    if (temp == SeparatorType) {
      FieldCount++;
    }
    else if (temp == TerminatorType) {
      break;
    }
  }
  return FieldCount;
}

FieldOpsClass FieldOps;
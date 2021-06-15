#ifndef FieldOps_h
#define FieldOps_h

#include <Arduino.h>

class FieldOpsClass {

  public:
    uint16_t extractField(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType, char *FieldExtracted);
    uint16_t checkFieldLength(uint16_t InputBufferLength, const char *InputBuffer, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType);
    bool checkNumber(const char *StringToCompare, uint16_t MaximumFieldLength, bool FloatUsed);
    bool compareString(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, const String StringValueToCompare, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType, bool CaseSensitive);
    bool compareInt(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, int32_t MinimumValue, int32_t MaximumValue, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType);
    bool compareFloat(uint16_t InputBufferLength, uint16_t MaximumFieldLength, const char *InputBuffer, float MinimumValue, float MaximumValue, uint16_t FieldNumber, uint8_t SeparatorType, uint8_t TerminatorType);
    uint16_t countCharacterInstances(uint16_t BufferLength, const char *InputBuffer, uint8_t CharacterToCount, bool CaseSensitive);
    bool checkValidCharacters(uint16_t BufferLength, const char *InputBuffer, const String CharactersToCheckFor, const uint16_t CharacterCheckCount, bool CaseSensitive);
    void changeCharactersTo(uint16_t BufferLength, char *InputBuffer, const String CharactersToCheckFor, const String CharactersToChangeTo, const uint16_t CharacterChangeCount);
    uint16_t countFields(uint16_t InputBufferLength, const char *InputBuffer, uint8_t SeparatorType, uint8_t TerminatorType);


};

extern FieldOpsClass FieldOps;

#endif
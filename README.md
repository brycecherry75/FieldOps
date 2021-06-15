#FieldOps Library#

FieldOps is a library for comparing a string or if a number is within a certain range at a given field number.

Revisions
1.0.0	First release

FEATURES:
Maximum buffer and field size can be up to 65535 bytes (or whatever can fit in memory)
Checks if a string is a valid int or float
Checks the length of a string on extraction of a field
Checks if all characters in a string correspond to any characters in another string
Counts instances of a particular character
Field separator and terminator is programmable
String case sensitivity check option

USAGE:
compareString(InputBufferLength, MaximumFieldLength, *InputBuffer, StringValueToCompare, FieldNumber, SeparatorType, TerminatorType, CaseSensitive) - CaseSensitive is a true/false value - returns true if a match is found with correct parameters including equal string lengths
compareInt(InputBufferLength, MaximumFieldLength, *InputBuffer, MinimumValue, MaximumValue, FieldNumber, SeparatorType, TerminatorType) - returns true if a match is found with correct parameters
compareFloat(InputBufferLength, MaximumFieldLength, *InputBuffer, MinimumValue, MaximumValue, FieldNumber, SeparatorType, TerminatorType) - returns true if a match is found with correct parameters
extractField(InputBufferLength, MaximumFieldLength, *InputBuffer, FieldNumber, SeparatorType, TerminatorType, *FieldExtracted) - extracts a field to *FieldExtracted - uint16_t return value is length of field
checkFieldLength(InputBufferLength, *InputBuffer, FieldNumber, SeparatorType, TerminatorType) - returns uint16_t count of length of field at FieldNumber - returns 0 if FieldNumber is not present in *InputBuffer
checkNumber(*StringToCompare, MaximumFieldLength, FloatUsed) - checks if a string is a number and returns true/false - FloatUsed is a bool and is to be set true if the string to be compared is a float
countCharacterInstances(BufferLength, *InputBuffer, CharacterToCount, CaseSensitive) - returns uint16_t count of CharacterToCount
checkValidCharacters(BufferLength, *InputBuffer, CharactersToCheckFor, CharacterCheckCount, CaseSensitive) - returns false if any of the characters in *InputBuffer do not match any characters in the CharactersToCheckFor string; 
changeCharactersTo(BufferLength, *InputBuffer, CharactersToCheckFor, CharactersToChangeTo, CharacterChangeCount) - changes characters corresponding to CharactersToCheckFor in *InputBuffer to CharactersToChangeTo e.g. CharactersToCheckFor is "Ab" and CharactersToChangeTo is "cD" will change "A" to "c" and "b" to "D" (case sensitive)
countFields(InputBufferLength, *InputBuffer, SeparatorType, TerminatorType) - returns uint16_t count of fields separated with SeparatorType

In all instances:
MaximumFieldLength and BufferLength includes the null terminator.
SeparatorType is typically 0x20 (space) or a comma (',') and TerminatorType is typically 0x0D (CR).
CaseSensitive is a bool.
CharacterCheckCount and CharacterChangeCount is length of (CharactersToCheckFor/CharacterChangeCount) + 1
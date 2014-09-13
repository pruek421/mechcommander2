//===========================================================================//
// File:        string.cpp                                                   //
// Contents:                                                                 //                                                                                                     //
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#include "stdafx.h"
//#include "stuffheaders.hpp"

#include <gameos.hpp>
#include <stuff/scalar.hpp>
#include <stuff/mstring.hpp>

using namespace Stuff;

//#############################################################################
//#######################    MStringRepresentation    #########################
//#############################################################################

#define DELETE_FILL_CHAR ('0')

size_t MStringRepresentation::allocationIncrement = 8;

//
//#############################################################################
//#############################################################################
//
inline size_t MStringRepresentation::CalculateSize(size_t needed)
{
	//
	// calculate the allocation size for a string
	//
	Verify(allocationIncrement);
	size_t x =
		((needed + allocationIncrement) / allocationIncrement) *
		allocationIncrement;
	return x;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation::MStringRepresentation()
{
	stringLength = 0;
	stringSize = 0;
	stringText = nullptr;
	referenceCount = 0;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation::MStringRepresentation(const MStringRepresentation& str)
{
	Check_Object(&str);
	stringLength = str.stringLength;
	stringSize = str.stringSize;
	if(str.stringText == nullptr)
	{
		stringText = nullptr;
	}
	else
	{
		stringText = new char[stringSize];
		Register_Pointer(stringText);
		Mem_Copy(stringText, str.stringText, stringLength + 1, stringSize);
	}
	referenceCount = 0;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation::MStringRepresentation(PCSTR cstr)
{
	if((cstr == nullptr) || (cstr[0] == '\x00'))
	{
		stringLength = 0;
		stringSize = 0;
		stringText = nullptr;
	}
	else
	{
		stringLength = strlen(cstr);
		stringSize = CalculateSize(stringLength);
		stringText = new char [stringSize];
		Register_Pointer(stringText);
		Mem_Copy(stringText, cstr, stringLength + 1, stringSize);
	}
	referenceCount = 0;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation::~MStringRepresentation()
{
	if(stringText != nullptr)
	{
#if defined(_ARMOR)
		memset(stringText, DELETE_FILL_CHAR, stringSize);
#endif
		Unregister_Pointer(stringText);
		delete[] stringText;
	}
}

//
//#############################################################################
//#############################################################################
//
void
MStringRepresentation::TestInstance(void) const
{
#if 0
	if(stringLength > 0 || stringText != nullptr)
	{
		Check_Pointer(stringText);
		size_t str_len = strlen(stringText);
		Verify(stringLength == str_len);
	}
#endif
}

//
//#############################################################################
//#############################################################################
//
void
MStringRepresentation::AllocateLength(size_t length)
{
	Check_Object(this);
	if(stringText != nullptr)
	{
#if defined(_ARMOR)
		memset(stringText, DELETE_FILL_CHAR, stringSize);
#endif
		Unregister_Pointer(stringText);
		delete[] stringText;
	}
	if(!length)
	{
		stringLength = 0;
		stringSize = 0;
		stringText = nullptr;
	}
	else
	{
		stringLength = length;
		stringSize = CalculateSize(stringLength);
		stringText = new char [stringSize];
		Register_Pointer(stringText);
	}
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation
MStringRepresentation::operator = (const MStringRepresentation& str)
{
	Check_Object(this);
	if(this == &str)
		return *this;
	if(stringText != nullptr)
	{
#if defined(_ARMOR)
		memset(stringText, DELETE_FILL_CHAR, stringSize);
#endif
		Unregister_Pointer(stringText);
		delete[] stringText;
	}
	Check_Object(&str);
	stringLength = str.stringLength;
	stringSize = str.stringSize;
	if(stringSize == 0)
	{
		stringText = nullptr;
	}
	else
	{
		stringText = new char[stringSize];
		Register_Pointer(stringText);
		Mem_Copy(stringText, str.stringText, stringLength + 1, stringSize);
	}
	return *this;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation
MStringRepresentation::operator = (PCSTR cstr)
{
	Check_Object(this);
	if(stringText != nullptr)
	{
#if defined(_ARMOR)
		memset(stringText, DELETE_FILL_CHAR, stringSize);
#endif
		Unregister_Pointer(stringText);
		delete[] stringText;
	}
	if((cstr == nullptr) || (cstr[0] == '\x00'))
	{
		stringLength = 0;
		stringSize = 0;
		stringText = nullptr;
	}
	else
	{
		stringLength = strlen(cstr);
		stringSize = CalculateSize(stringLength);
		stringText = new char [stringSize];
		Register_Pointer(stringText);
		Mem_Copy(stringText, cstr, stringLength + 1, stringSize);
	}
	return *this;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation
Stuff::operator + (
	const MStringRepresentation& str1,
	const MStringRepresentation& str2)
{
	Check_Object(&str1);
	Check_Object(&str2);
	MStringRepresentation temp;
	size_t totalLen = str1.stringLength + str2.stringLength;
	if(totalLen == 0)
	{
		return temp;
	}
	temp.stringLength = 0;
	temp.stringSize = MStringRepresentation::CalculateSize((size_t)totalLen);
	temp.stringText = new char[temp.stringSize];
	Register_Pointer(temp.stringText);
	Verify(temp.stringSize >= 1);
	temp.stringText[0] = '\000';
	if(str1.stringText != nullptr)
	{
		Mem_Copy(temp.stringText, str1.stringText, str1.stringLength + 1, temp.stringSize);
		temp.stringLength = str1.stringLength;
	}
	if(str2.stringText != nullptr)
	{
		Verify(temp.stringLength < temp.stringSize);
		Mem_Copy(
			&temp.stringText[temp.stringLength],
			str2.stringText,
			str2.stringLength + 1,
			temp.stringSize - temp.stringLength
		);
		temp.stringLength += str2.stringLength;
	}
	Check_Object(&temp);
	return temp;
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation
Stuff::operator + (const MStringRepresentation& str, char ch)
{
	Check_Object(&str);
	MStringRepresentation temp;
	if(str.stringText == nullptr)
	{
		temp.stringLength = 1;
		temp.stringSize = MStringRepresentation::allocationIncrement;
		temp.stringText = new char [temp.stringSize];
		Register_Pointer(temp.stringText);
		Verify(temp.stringSize >= 2);
		temp.stringText[0] = ch;
		temp.stringText[1] = '\000';
	}
	else
	{
		Verify(str.stringLength != UINT_MAX);
		temp.stringLength = str.stringLength + 1;
		if(temp.stringLength == str.stringSize)
			temp.stringSize = str.stringSize + MStringRepresentation::allocationIncrement;
		else
			temp.stringSize = str.stringSize;
		temp.stringText = new char[temp.stringSize];
		Register_Pointer(temp.stringText);
		Mem_Copy(temp.stringText, str.stringText, str.stringLength, temp.stringSize);
		Verify(str.stringLength < temp.stringSize);
		Verify(temp.stringLength < temp.stringSize);
		temp.stringText[str.stringLength] = ch;
		temp.stringText[temp.stringLength] = '\000';
	}
	Check_Object(&temp);
	return temp;
}

//
//#############################################################################
//#############################################################################
//
int32_t
MStringRepresentation::Compare(const MStringRepresentation& str) const
{
	Check_Object(this);
	Check_Object(&str);
	// handle special cases where one string is empty
	if(stringText == nullptr)
	{
		if(str.stringText == nullptr)
			return 0;
		else
			return -1;
	}
	if(str.stringText == nullptr)
	{
		return 1;
	}
	return strcmp(stringText, str.stringText);
}

//
//#############################################################################
//#############################################################################
//
MStringRepresentation
MStringRepresentation::GetNthToken(
	size_t nth_token,
	PSTR delimiters) const
{
	Check_Object(this);
	//
	// Which delimters to use
	//
	PSTR delimter_string = " \t,";
	if(delimiters != nullptr)
	{
		delimter_string = delimiters;
	}
	Check_Pointer(delimter_string);
	//
	// Make temporary
	//
	MStringRepresentation temp(*this);
	if(temp.stringText == nullptr)
		return temp;
	//
	// Parse string with strtok
	//
	size_t i;
	PSTR ptr;
	Check_Pointer(temp.stringText);
	ptr = strtok(temp.stringText, delimter_string);
	for(i = 0; i < nth_token; i++)
	{
		if((ptr = strtok(nullptr, delimter_string)) == nullptr)
			break;
	}
	if(ptr == nullptr)
	{
		MStringRepresentation null_return;
		return null_return;
	}
	MStringRepresentation token_return(ptr);
	return token_return;
}

//
//#############################################################################
//#############################################################################
//
void
MStringRepresentation::ToUpper()
{
	Check_Object(this);
	if(stringText != nullptr)
	{
		for(size_t i = 0; i < stringLength; i++)
		{
			stringText[i] = (char)toupper(stringText[i]);
		}
	}
}

//
//#############################################################################
//#############################################################################
//
void MStringRepresentation::ToLower()
{
	Check_Object(this);
	if(stringText != nullptr)
	{
		// CharLowerA(stringText);
		_strlwr_s(stringText, stringLength);
	}
}

//
//#############################################################################
//#############################################################################
//
MemoryStream&
MemoryStreamIO::Read(
	MemoryStream* stream,
	MStringRepresentation* str)
{
	Check_Object(stream);
	Check_Object(str);
	size_t string_length;
	Read(stream, &string_length);
	if(string_length > 0)
	{
		PSTR ptr = new char[string_length + 1];
		Register_Pointer(ptr);
		stream->ReadBytes(ptr, string_length + 1);
		*str = ptr;
		Unregister_Pointer(ptr);
		delete[] ptr;
	}
	Check_Object(str);
	return *stream;
}

//
//#############################################################################
//#############################################################################
//
MemoryStream&
MemoryStreamIO::Write(
	MemoryStream* stream,
	const MStringRepresentation& str)
{
	Check_Object(stream);
	Check_Object(&str);
	Write(stream, &str.stringLength);
	if(str.stringLength > 0)
	{
		stream->WriteBytes(str.stringText, str.stringLength + 1);
	}
	return *stream;
}

//
//#############################################################################
//#############################################################################
//
void
Stuff::Convert_From_Ascii(
	PCSTR str,
	MStringRepresentation* value)
{
#if 0
	Check_Pointer(str);
	Check_Object(value);
	*value = str;
#else
	if(str == nullptr)
	{
		STOP(("Convert_From_Ascii - str == nullptr"));
	}
	if(value == nullptr)
	{
		STOP(("Convert_From_Ascii - value == nullptr"));
	}
	if(value) *value = str;
#endif
}

//#############################################################################
//##############################    MString    ################################
//#############################################################################

//
//#############################################################################
//#############################################################################
//
MString&
MString::operator = (const MString& str)
{
	Check_Object(this);
	Check_Object(&str);
	if(this == &str)
		return *this;
	Check_Object(representation);
	representation->DecrementReferenceCount();
	representation = str.representation;
	representation->IncrementReferenceCount();
	return *this;
}

//
//#############################################################################
//#############################################################################
//
MString&
MString::operator = (PCSTR cstr)
{
	Check_Object(representation);
	representation->DecrementReferenceCount();
	representation = new MStringRepresentation(cstr);
	Register_Object(representation);
	representation->IncrementReferenceCount();
	return *this;
}

//
//#############################################################################
//#############################################################################
//
void
MString::ToUpper()
{
	MStringRepresentation* old = representation;
	Check_Object(old);
	representation = new MStringRepresentation(*old);
	Register_Object(representation);
	representation->IncrementReferenceCount();
	old->DecrementReferenceCount();
	representation->ToUpper();
}

//
//#############################################################################
//#############################################################################
//
void
MString::ToLower()
{
	MStringRepresentation* old = representation;
	Check_Object(old);
	representation = new MStringRepresentation(*old);
	Register_Object(representation);
	representation->IncrementReferenceCount();
	old->DecrementReferenceCount();
	representation->ToLower();
}

//
//#############################################################################
//#############################################################################
//
MemoryStream&
MemoryStreamIO::Read(
	MemoryStream* stream,
	MString* str)
{
	Check_Object(str);
	Check_Object(str->representation);
	str->representation->DecrementReferenceCount();
	str->representation = new MStringRepresentation;
	Register_Object(str->representation);
	str->representation->IncrementReferenceCount();
	Verify(str->representation->referenceCount == 1);
	return Read(stream, str->representation);
}

//
//#############################################################################
//#############################################################################
//
void
Stuff::Convert_From_Ascii(
	PCSTR str,
	MString* value)
{
	Check_Object(value);
	Check_Object(value->representation);
	value->representation->DecrementReferenceCount();
	value->representation = new MStringRepresentation;
	Register_Object(value->representation);
	value->representation->IncrementReferenceCount();
	Verify(value->representation->referenceCount == 1);
	Convert_From_Ascii(str, value->representation);
}
//
//#############################################################################
//#############################################################################
//
bool
Stuff::Close_Enough(
	PCSTR str1,
	PCSTR str2,
	float e)
{
	Check_Pointer(str1);
	Check_Pointer(str2);
	(void)e;
	return !_stricmp(str1, str2);
}

IteratorPosition
GetHashFunctions::GetHashValue(const MString& value)
{
	return value.GetHashValue();
}
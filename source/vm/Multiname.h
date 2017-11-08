#pragma once

#include <vector>

namespace vm
{

enum class MultinameKind: uint8_t
{
	QName			= 0x07, // 7
	QNameA			= 0x0D, // 13
	RTQName			= 0x0F, // 15
	RTQNameA		= 0x10, // 16
	RTQNameL		= 0x11, // 17
	RTQNameLA		= 0x12, // 18
	Multiname		= 0x09, // 9
	MultinameA		= 0x0E, // 14
	MultinameL		= 0x1B, // 27
	MultinameLA		= 0x1C, // 28
	Typename		= 0x1D
};

struct QName
{
	uint32_t ns;
	uint32_t name;
};

struct RTQName
{
	uint32_t name;
};

struct RTQNameL
{
	/* nothing */
};

struct Multiname
{
	uint32_t name;
	uint32_t nsSet;
};

struct MultinameL
{
	uint32_t nsSet;
};

struct Typename
{
	uint32_t qName;
};

struct MultinameInfo
{
	MultinameKind kind;
	
	union
	{
		QName qName;
		RTQName rtqName;
		RTQNameL rtqNameL;
		Multiname multiname;
		MultinameL multinameL;
		Typename typeName;
	};

	// A little hacky, but who cares :P
	std::vector<uint32_t> typeNameParams;
};

} // namespace vm

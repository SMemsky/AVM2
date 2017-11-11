#pragma once

namespace vm
{

enum class TraitKind: uint8_t
{
	// Lower 4 bits
	Slot		= 0,
	Method		= 1,
	Getter		= 2,
	Setter		= 3,
	Class		= 4,
	Function	= 5,
	Const		= 6
};

enum class TraitAttributes: uint8_t
{
	// Upper 4 bits
	Final		= 0x1,
	Override	= 0x2,
	Metadata	= 0x4
};

// Slot, Const
struct SlotTrait
{
	uint32_t slotId;
	uint32_t typeName;
	uint32_t vIndex;
	uint8_t vKind; // vIndex != 0
};

// Class
struct ClassTrait
{
	uint32_t slotId;
	uint32_t classI;
};

// Function
struct FunctionTrait
{
	uint32_t slotId;
	uint32_t function;
};

// Method, Getter, Setter
struct MethodTrait
{
	uint32_t dispId;
	uint32_t method;
};

struct TraitInfo
{
	uint32_t name;
	TraitKind kind;
	uint8_t attributes;

	union
	{
		SlotTrait slotTrait;
		ClassTrait classTrait;
		FunctionTrait functionTrait;
		MethodTrait methodTrait;
	};

	std::vector<uint32_t> metadata;
};

} // namespace vm

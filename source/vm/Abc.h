#pragma once

#include "common/ByteBuffer.h"

namespace vm
{

class Abc;
Abc loadAbc(ByteBuffer & buffer);

class AbcConstantPool
{
	friend AbcConstantPool loadConstantPool(ByteBuffer &);
public:
	AbcConstantPool() = default;

	uint32_t getIntCount() const { return integers.size(); };
	std::vector<int32_t> const & getIntegers() const { return integers; };

	uint32_t getUintCount() const { return uintegers.size(); };
	std::vector<uint32_t> const & getUintegers() const { return uintegers; };

	uint32_t getDoubleCount() const { return doubles.size(); };
	std::vector<double> const & getDoubles() const { return doubles; };

	uint32_t getStringCount() const { return strings.size(); };
	std::vector<std::string> const & getStrings() const { return strings; };
private:
	std::vector<int32_t> integers;
	std::vector<uint32_t> uintegers;
	std::vector<double> doubles;
	std::vector<std::string> strings;
};

class Abc
{
	friend Abc loadAbc(ByteBuffer &);
public:
	Abc() = default;

	uint16_t getMinorVersion() const { return minorVersion; };
	uint16_t getMajorVersion() const { return majorVersion; };
	AbcConstantPool const & getConstantPool() const { return constantPool; };
private:
	uint16_t minorVersion;
	uint16_t majorVersion;

	AbcConstantPool constantPool;
};

} // namespace vm

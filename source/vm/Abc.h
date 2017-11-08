#pragma once

#include "common/ByteBuffer.h"
#include "vm/Multiname.h"

namespace vm
{

class Abc;
Abc loadAbc(ByteBuffer & buffer);

enum class NamespaceKind: uint8_t
{
	Normal			= 0x08,
	Package			= 0x16,
	PackageInternal	= 0x17,
	Protected		= 0x18,
	Explicit		= 0x19,
	StaticProtected	= 0x1a,
	Private			= 0x05,
	Any				= 0x00
};

using Namespace = std::pair<NamespaceKind, uint32_t>;

class AbcConstantPool
{
	friend AbcConstantPool loadConstantPool(ByteBuffer &);
public:
	AbcConstantPool() = default;

	std::vector<int32_t> const & getInts() const { return ints; };
	std::vector<uint32_t> const & getUints() const { return uints; };
	std::vector<double> const & getDoubles() const { return doubles; };
	std::vector<std::string> const & getStrings() const { return strings; };
	std::vector<Namespace> const & getNamespaces() const { return namespaces; };
	std::vector<std::vector<uint32_t>> const & getNsSets() const { return nsSets; };
	std::vector<MultinameInfo> const & getMultinames() const { return multinames; };
private:
	std::vector<int32_t> ints;
	std::vector<uint32_t> uints;
	std::vector<double> doubles;
	std::vector<std::string> strings;
	std::vector<Namespace> namespaces;
	std::vector<std::vector<uint32_t>> nsSets;
	std::vector<MultinameInfo> multinames;
};

class MethodInfo
{
	friend MethodInfo loadMethodInfo(ByteBuffer &);
public:
	MethodInfo() = default;
private:
	std::vector<uint32_t> params;
	uint32_t returnType;
	uint32_t name;
	uint8_t flags;
};

class Abc
{
	friend Abc loadAbc(ByteBuffer &);
public:
	Abc() = default;

	uint16_t getMinorVersion() const { return minorVersion; };
	uint16_t getMajorVersion() const { return majorVersion; };
	AbcConstantPool const & getConstantPool() const { return constantPool; };
	std::vector<MethodInfo> const & getMethods() const { return methods; };
private:
	uint16_t minorVersion;
	uint16_t majorVersion;

	AbcConstantPool constantPool;
	std::vector<MethodInfo> methods;
};

} // namespace vm

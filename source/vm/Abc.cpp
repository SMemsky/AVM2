#include "Abc.h"

#include <cassert>
#include <limits>

static_assert(std::numeric_limits<double>::has_quiet_NaN == true, "Fixme");

int32_t const defaultIntegerConstant = 0;
uint32_t const defaultUintegerConstant = 0;
double const defaultDoubleConstant = std::numeric_limits<double>::quiet_NaN();
std::string const defaultStringConstant = "";
vm::Namespace const defaultNamespaceConstant = {vm::NamespaceKind::Any, 0};
std::vector<uint32_t> const defaultNsSetConstant = {};
vm::MultinameInfo const defaultMultinameConstant = vm::MultinameInfo();

namespace vm
{

MultinameInfo loadMultiname(ByteBuffer & buffer)
{
	unsigned paramCount = 0;

	MultinameInfo multiname;
	multiname.kind = MultinameKind(buffer.readUint30());
	switch (multiname.kind) {
	case MultinameKind::QName:
	case MultinameKind::QNameA:
		multiname.qName.ns = buffer.readUint30();
		multiname.qName.name = buffer.readUint30();
		break;
	case MultinameKind::RTQName:
	case MultinameKind::RTQNameA:
		multiname.rtqName.name = buffer.readUint30();
		break;
	case MultinameKind::RTQNameL:
	case MultinameKind::RTQNameLA:
		break;
	case MultinameKind::Multiname:
	case MultinameKind::MultinameA:
		multiname.multiname.name = buffer.readUint30();
		multiname.multiname.nsSet = buffer.readUint30();
		break;
	case MultinameKind::MultinameL:
	case MultinameKind::MultinameLA:
		multiname.multinameL.nsSet = buffer.readUint30();
		break;
	case MultinameKind::Typename:
		multiname.typeName.qName = buffer.readUint30();
		paramCount = buffer.readUint30();
		for (unsigned i = 0; i < paramCount; ++i) {
			multiname.typeNameParams.push_back(buffer.readUint30());
		}
		break;
	default:
		throw std::runtime_error(
			std::string("Unknown multiname kind: ")
			+ std::to_string(uint8_t(multiname.kind)));
	}

	return multiname;
}

AbcConstantPool loadConstantPool(ByteBuffer & buffer)
{
	AbcConstantPool pool = AbcConstantPool();
	
	unsigned const intCount = buffer.readUint30();
	pool.ints.push_back(defaultIntegerConstant);
	for (unsigned i = 1; i < intCount; ++i) {
		pool.ints.push_back(buffer.readInt32());
	}

	unsigned const uintCount = buffer.readUint30();
	pool.uints.push_back(defaultUintegerConstant);
	for (unsigned i = 1; i < uintCount; ++i) {
		pool.uints.push_back(buffer.readUint32());
	}

	unsigned const doubleCount = buffer.readUint30();
	pool.doubles.push_back(defaultDoubleConstant);
	for (unsigned i = 1; i < doubleCount; ++i) {
		pool.doubles.push_back(buffer.readDouble64());
	}

	unsigned const stringCount = buffer.readUint30();
	pool.strings.push_back(defaultStringConstant);
	for (unsigned i = 1; i < stringCount; ++i) {
		pool.strings.push_back(buffer.readString(buffer.readUint30()));
	}

	unsigned const namespaceCount = buffer.readUint30();
	pool.namespaces.push_back(defaultNamespaceConstant);
	for (unsigned i = 1; i < namespaceCount; ++i) {
		pool.namespaces.push_back(
			Namespace(vm::NamespaceKind(buffer.readUint8()), buffer.readUint30()));
	}

	unsigned const nsSetCount = buffer.readUint30();
	pool.nsSets.push_back(defaultNsSetConstant);
	for (unsigned i = 1; i < nsSetCount; ++i) {
		pool.nsSets.push_back(std::vector<uint32_t>());
		unsigned const nsCount = buffer.readUint30();
		for (unsigned j = 0; j < nsCount; ++j) {
			pool.nsSets[i].push_back(buffer.readUint30());
		}
	}

	unsigned const multinameCount = buffer.readUint30();
	pool.multinames.push_back(defaultMultinameConstant);
	for (unsigned i = 1; i < multinameCount; ++i) {
		pool.multinames.push_back(loadMultiname(buffer));
	}

	return pool;
}

Abc loadAbc(ByteBuffer & buffer)
{
	Abc abc = Abc();
	abc.minorVersion = buffer.readUint16();
	abc.majorVersion = buffer.readUint16();
	abc.constantPool = loadConstantPool(buffer);

	return abc;
}

} // namespace vm

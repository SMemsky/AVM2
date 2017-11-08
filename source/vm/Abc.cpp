#include "Abc.h"

#include <limits>

static_assert(std::numeric_limits<double>::has_quiet_NaN == true, "Fixme");

int32_t const defaultIntegerConstant = 0;
uint32_t const defaultUintegerConstant = 0;
double const defaultDoubleConstant = std::numeric_limits<double>::quiet_NaN();
std::string const defaultStringConstant = "";
vm::Namespace const defaultNamespaceConstant = {vm::NamespaceKind::Any, 0};

namespace vm
{

AbcConstantPool loadConstantPool(ByteBuffer & buffer)
{
	AbcConstantPool pool = AbcConstantPool();
	
	unsigned const intCount = buffer.readUint30();
	pool.integers.push_back(defaultIntegerConstant);
	for (unsigned i = 1; i < intCount; ++i) {
		pool.integers.push_back(buffer.readInt32());
	}

	unsigned const uintCount = buffer.readUint30();
	pool.uintegers.push_back(defaultUintegerConstant);
	for (unsigned i = 1; i < uintCount; ++i) {
		pool.uintegers.push_back(buffer.readUint32());
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

#include "Abc.h"

#include <cassert>
#include <limits>
#include <stdexcept>
#include <iostream>

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
	multiname.kind = MultinameKind(buffer.readUint8());
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
			Namespace(NamespaceKind(buffer.readUint8()), buffer.readUint30()));
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

MethodInfo loadMethodInfo(ByteBuffer & buffer)
{
	MethodInfo method = MethodInfo();
	unsigned const paramCount = buffer.readUint30();
	method.returnType = buffer.readUint30();
	for (unsigned i = 0; i < paramCount; ++i) {
		method.params.push_back(buffer.readUint30());
	}
	method.name = buffer.readUint30();
	method.flags = buffer.readUint8();

	if (method.flags & uint8_t(MethodFlags::HasOptional)) {
		unsigned const optionCount = buffer.readUint30();
		for (unsigned i = 0; i < optionCount; ++i) {
			method.options.push_back(
				Option(buffer.readUint30(), OptionKind(buffer.readUint8())));
		}
	}

	if (method.flags & uint8_t(MethodFlags::HasParamNames)) {
		for (unsigned i = 0; i < paramCount; ++i) {
			method.paramNames.push_back(buffer.readUint30());
		}
	}

	return method;
}

MetadataInfo loadMetadataInfo(ByteBuffer &)
{
	MetadataInfo metadata = MetadataInfo();

	throw std::runtime_error("loadMetadataInfo: not implemented");

	return metadata;
}

TraitInfo loadTraitInfo(ByteBuffer & buffer)
{
	TraitInfo trait = TraitInfo();

	trait.name = buffer.readUint30();
	uint8_t kind = buffer.readUint8();
	trait.kind = TraitKind(kind & 0xf);
	trait.attributes = TraitAttributes(kind >> 4);
	switch (trait.kind) {
	case TraitKind::Slot:
	case TraitKind::Const:
		trait.slotTrait.slotId = buffer.readUint30();
		trait.slotTrait.typeName = buffer.readUint30();
		trait.slotTrait.vIndex = buffer.readUint30();
		if (trait.slotTrait.vIndex != 0) {
			trait.slotTrait.vKind = buffer.readUint8();
		}
		break;
	case TraitKind::Class:
		trait.classTrait.slotId = buffer.readUint30();
		trait.classTrait.classI = buffer.readUint30();
		break;
	case TraitKind::Function:
		trait.functionTrait.slotId = buffer.readUint30();
		trait.functionTrait.function = buffer.readUint30();
		break;
	case TraitKind::Method:
	case TraitKind::Getter:
	case TraitKind::Setter:
		trait.methodTrait.dispId = buffer.readUint30();
		trait.methodTrait.method = buffer.readUint30();
		break;
	default:
		throw std::runtime_error(
			std::string("Unknown trait kind: ")
			+ std::to_string(uint8_t(trait.kind)));
	}

	return trait;
}

InstanceInfo loadInstanceInfo(ByteBuffer & buffer)
{
	InstanceInfo instance = InstanceInfo();
	instance.name = buffer.readUint30();
	instance.superName = buffer.readUint30();
	instance.flags = buffer.readUint8();
	instance.protectedNamespace = buffer.readUint30();
	unsigned const interfaceCount = buffer.readUint30();
	for (unsigned i = 0; i < interfaceCount; ++i) {
		instance.interfaces.push_back(buffer.readUint30());
	}
	instance.constructor = buffer.readUint30();
	unsigned const traitCount = buffer.readUint30();
	for (unsigned i = 0; i < traitCount; ++i) {
		instance.traits.push_back(loadTraitInfo(buffer));
	}

	return instance;
}

ClassInfo loadClassInfo(ByteBuffer & buffer)
{
	ClassInfo classInfo = ClassInfo();
	classInfo.cInit = buffer.readUint30();
	unsigned const traitCount = buffer.readUint30();
	for (unsigned i = 0; i < traitCount; ++i) {
		classInfo.traits.push_back(loadTraitInfo(buffer));
	}

	return classInfo;
}

ScriptInfo loadScriptInfo(ByteBuffer & buffer)
{
	ScriptInfo script = ScriptInfo();
	script.init = buffer.readUint30();
	unsigned const traitCount = buffer.readUint30();
	for (unsigned i = 0; i < traitCount; ++i) {
		script.traits.push_back(loadTraitInfo(buffer));
	}

	return script;
}

ExceptionInfo loadExceptionInfo(ByteBuffer & buffer)
{
	ExceptionInfo exception = ExceptionInfo();
	exception.from = buffer.readUint30();
	exception.to = buffer.readUint30();
	exception.target = buffer.readUint30();
	exception.excType = buffer.readUint30();
	exception.varName = buffer.readUint30();

	return exception;
}

MethodBodyInfo loadMethodBodyInfo(ByteBuffer & buffer)
{
	MethodBodyInfo body = MethodBodyInfo();
	body.method = buffer.readUint30();
	body.maxStack = buffer.readUint30();
	body.localCount = buffer.readUint30();
	body.initScopeDepth = buffer.readUint30();
	body.maxScopeDepth = buffer.readUint30();

	unsigned const codeSize = buffer.readUint30();
	for (unsigned i = 0; i < codeSize; ++i) {
		body.code.push_back(buffer.readUint8());
	}

	unsigned const exceptionCount = buffer.readUint30();
	for (unsigned i = 0; i < exceptionCount; ++i) {
		body.exceptions.push_back(loadExceptionInfo(buffer));
	}

	unsigned const traitCount = buffer.readUint30();
	for (unsigned i = 0; i < traitCount; ++i) {
		body.traits.push_back(loadTraitInfo(buffer));
	}

	return body;
}

Abc loadAbc(ByteBuffer & buffer)
{
	Abc abc = Abc();
	abc.minorVersion = buffer.readUint16();
	abc.majorVersion = buffer.readUint16();
	abc.constantPool = loadConstantPool(buffer);
	
	unsigned const methodCount = buffer.readUint30();
	for (unsigned i = 0; i < methodCount; ++i) {
		abc.methods.push_back(loadMethodInfo(buffer));
	}

	unsigned const metadataCount = buffer.readUint30();
	for (unsigned i = 0; i < metadataCount; ++i) {
		abc.metadata.push_back(loadMetadataInfo(buffer));
	}

	unsigned const classCount = buffer.readUint30();
	for (unsigned i = 0; i < classCount; ++i) {
		abc.instances.push_back(loadInstanceInfo(buffer));
	}
	for (unsigned i = 0; i < classCount; ++i) {
		abc.classes.push_back(loadClassInfo(buffer));
	}

	unsigned const scriptCount = buffer.readUint30();
	for (unsigned i = 0; i < scriptCount; ++i) {
		abc.scripts.push_back(loadScriptInfo(buffer));
	}

	unsigned const methodBodyCount = buffer.readUint30();
	for (unsigned i = 0; i < methodBodyCount; ++i) {
		abc.bodies.push_back(loadMethodBodyInfo(buffer));
	}

	return abc;
}

} // namespace vm

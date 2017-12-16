#pragma once

#include "common/ByteBuffer.h"
#include "vm/Multiname.h"
#include "vm/Trait.h"

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

enum class OptionKind: uint8_t
{
	Int							= 0x03, // int
	Uint						= 0x04, // uint
	Double						= 0x06, // double
	Utf8						= 0x01, // string
	True						= 0x0B, // none
	False						= 0x0A, // none
	Null						= 0x0C, // none
	Undefined					= 0x00, // none
	Namespace					= 0x08, // namespace
	PackageNamespace			= 0x16, // namespace
	PackageInternalNamespace	= 0x17, // namespace
	ProtectedNamespace			= 0x18, // namespace
	ExplicitNamespace			= 0x19, // namespace
	StaticProtectedNamespace	= 0x1A, // namespace
	PrivateNamespace			= 0x05  // namespace
};

enum class MethodFlags: uint8_t
{
	NeedArguments	= 0x01,
	NeedActivation	= 0x02,
	NeedRest		= 0x04,
	HasOptional		= 0x08,
	SetDxns			= 0x40,
	HasParamNames	= 0x80
};

enum class InstanceFlags: uint8_t
{
	Sealed				= 0x01,
	Final				= 0x02,
	Interface			= 0x04,
	ProtectedNamespace	= 0x08
};

using Namespace = std::pair<NamespaceKind, uint32_t>; // _, -> strings
using Option = std::pair<uint32_t, OptionKind>; // -> (depends ob kind)
using MetadataItem = std::pair<uint32_t, uint32_t>; // -> strings, -> strings

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
	std::vector<std::vector<uint32_t>> nsSets; // -> namespaces
	std::vector<MultinameInfo> multinames;
};

class MethodInfo
{
	friend MethodInfo loadMethodInfo(ByteBuffer &);
public:
	MethodInfo() = default;

	std::vector<uint32_t> const & getParams() const { return params; };
	uint32_t getReturnType() const { return returnType; };
	uint32_t getName() const { return name; };
	uint8_t getFlags() const { return flags; };
	std::vector<Option> const & getOptions() const { return options; };
	std::vector<uint32_t> const & getParamNames() const { return paramNames; };
private:
	std::vector<uint32_t> params; // -> multiname
	uint32_t returnType; // -> multiname
	uint32_t name; // strings
	uint8_t flags;
	std::vector<Option> options;
	std::vector<uint32_t> paramNames; // -> strings
};

class MetadataInfo
{
	friend MetadataInfo loadMetadataInfo(ByteBuffer &);
public:
	MetadataInfo() = default;

	uint32_t getName() const { return name; };
	std::vector<MetadataItem> const & getItems() const { return items; };
private:
	uint32_t name; // -> strings
	std::vector<MetadataItem> items;
};

class InstanceInfo
{
	friend InstanceInfo loadInstanceInfo(ByteBuffer &);
public:
	InstanceInfo() = default;

	uint32_t getName() const { return name; };
	uint32_t getSuperName() const { return superName; };
	uint8_t getFlags() const { return flags; };
	uint32_t getProtectedNamespace() const { return protectedNamespace; };
	std::vector<uint32_t> const & getInterfaces() const { return interfaces; };
	uint32_t getConstructor() const { return constructor; };
	std::vector<TraitInfo> const & getTraits() const { return traits; };
private:
	uint32_t name; // -> multiname
	uint32_t superName; // -> multiname
	uint8_t flags;
	uint32_t protectedNamespace;
	std::vector<uint32_t> interfaces; // -> multiname
	uint32_t constructor; // -> method
	std::vector<TraitInfo> traits;
};

class ClassInfo
{
	friend ClassInfo loadClassInfo(ByteBuffer &);
public:
	ClassInfo() = default;

	uint32_t getCInit() const { return cInit; };
	std::vector<TraitInfo> const & getTraits() const { return traits; };
private:
	uint32_t cInit; // -> method
	std::vector<TraitInfo> traits;
};

class ScriptInfo
{
	friend ScriptInfo loadScriptInfo(ByteBuffer &);
public:
	ScriptInfo() = default;

	uint32_t getInit() const { return init; };
	std::vector<TraitInfo> const & getTraits() const { return traits; };
private:
	uint32_t init; // -> method
	std::vector<TraitInfo> traits;
};

class ExceptionInfo
{
	friend ExceptionInfo loadExceptionInfo(ByteBuffer &);
public:
	ExceptionInfo() = default;

	uint32_t getFrom() const { return from; };
	uint32_t getTo() const { return to; };
	uint32_t getTarget() const { return target; };
	uint32_t getExcType() const { return excType; };
	uint32_t getVarName() const { return varName; };
private:
	uint32_t from;
	uint32_t to;
	uint32_t target;
	uint32_t excType;
	uint32_t varName;
};

class MethodBodyInfo
{
	friend MethodBodyInfo loadMethodBodyInfo(ByteBuffer &);
public:
	MethodBodyInfo() = default;

	uint32_t getMethod() const { return method; };
	uint32_t getMaxStack() const { return maxStack; };
	uint32_t getLocalCount() const { return localCount; };
	uint32_t getInitScopeDepth() const { return initScopeDepth; };
	uint32_t getMaxScopeDepth() const { return maxScopeDepth; };
	std::vector<uint8_t> const & getCode() const { return code; };
	std::vector<ExceptionInfo> const & getExceptions() const { return exceptions; };
	std::vector<TraitInfo> const & getTraits() const { return traits; };
private:
	uint32_t method; // -> methods
	uint32_t maxStack;
	uint32_t localCount;
	uint32_t initScopeDepth;
	uint32_t maxScopeDepth;
	std::vector<uint8_t> code;
	std::vector<ExceptionInfo> exceptions;
	std::vector<TraitInfo> traits;
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
	std::vector<MetadataInfo> const & getMetadata() const { return metadata; };
	std::vector<InstanceInfo> const & getInstances() const { return instances; };
	std::vector<ClassInfo> const & getClasses() const { return classes; };
	std::vector<ScriptInfo> const & getScripts() const { return scripts; };
	std::vector<MethodBodyInfo> const & getBodies() const { return bodies; };
private:
	uint16_t minorVersion;
	uint16_t majorVersion;

	AbcConstantPool constantPool;
	std::vector<MethodInfo> methods;
	std::vector<MetadataInfo> metadata;
	std::vector<InstanceInfo> instances;
	std::vector<ClassInfo> classes;
	std::vector<ScriptInfo> scripts;
	std::vector<MethodBodyInfo> bodies;
};

} // namespace vm

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "common/ByteBuffer.h"
#include "common/FileUtil.h"

#include "vm/Abc.h"
#include "vm/VirtualMachine.h"

void printUsage(std::string const & program)
{
	std::cout << "Usage:\n"
		<< "  " << program << " <filename.abc>" << std::endl;
}

void printBytes(std::vector<uint8_t> const & bytes)
{
	std::cout << std::hex;
	for (unsigned i = 0; i < bytes.size(); ++i) {
		std::cout << std::setfill('0') << std::setw(2) << +bytes[i];
		if (i % 16 == 15) {
			std::cout << "\n";
		} else {
			std::cout << " ";
		}
	}
	std::cout << std::dec << std::endl;
}

void printEntry(vm::Abc const & abc)
{
	// auto const & constantPool = abc.getConstantPool();
	auto const & scripts = abc.getScripts();
	if (scripts.size() == 0) {
		std::cout << "No entry point" << std::endl;
		return;
	}

	auto const & script = scripts[scripts.size() - 1];
	auto const initMethod = script.getInit();
	// std::cout << "Entry init: " <<  << std::endl;
	for (auto const & body : abc.getBodies()) {
		if (body.getMethod() == initMethod) {
			std::cout << "Entry size: " << body.getCode().size() << std::endl;
			printBytes(body.getCode());
		}
	}
}

void dumpConstantPool(vm::AbcConstantPool const & pool)
{
	std::cout << "# Ints" << std::endl;
	auto const & ints = pool.getInts();
	for (unsigned i = 0; i < ints.size(); ++i) {
		std::cout << i << " " << ints[i] << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Uints" << std::endl;
	auto const & uints = pool.getUints();
	for (unsigned i = 0; i < uints.size(); ++i) {
		std::cout << i << " " << uints[i] << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Doubles" << std::endl;
	auto const & doubles = pool.getDoubles();
	for (unsigned i = 0; i < doubles.size(); ++i) {
		std::cout << i << " " << doubles[i] << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Strings" << std::endl;
	auto const & strings = pool.getStrings();
	for (unsigned i = 0; i < strings.size(); ++i) {
		std::cout << i << " " << strings[i] << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Namespaces" << std::endl;
	auto const & namespaces = pool.getNamespaces();
	for (unsigned i = 0; i < namespaces.size(); ++i) {
		std::cout << i << " 0x"
			<< std::hex << +uint8_t(namespaces[i].first) << std::dec << " "
			<< namespaces[i].second << " ("
			<< strings[namespaces[i].second] << ")" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Namespace Sets" << std::endl;
	auto const & nsSets = pool.getNsSets();
	for (unsigned i = 0; i < nsSets.size(); ++i) {
		auto set = nsSets[i];
		std::sort(set.begin(), set.end());
		std::cout << i << " [";
		for (unsigned j = 0; j < set.size(); ++j) {
			std::cout << set[j];
			if (j != set.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "# Multinames" << std::endl;
	auto const & multinames = pool.getMultinames();
	for (unsigned i = 0; i < multinames.size(); ++i) {
		auto const & multiname = multinames[i];
		std::cout << i << " ";
		if (i == 0) {
			std::cout << "undefined" << std::endl;
			continue;
		}
		switch (multiname.kind) {
		case vm::MultinameKind::QName:
		case vm::MultinameKind::QNameA:
			std::cout << "QName(ns " << multiname.qName.ns << " (" << strings[namespaces[multiname.qName.ns].second] << ") " << ", name " << multiname.qName.name << " (" << strings[multiname.qName.name] << ") " << ")" << std::endl;
			break;
		case vm::MultinameKind::RTQName:
		case vm::MultinameKind::RTQNameA:
			std::cout << "RTQName(name " << multiname.rtqName.name << " (" << strings[multiname.rtqName.name] << ") " << ")" << std::endl;
			break;
		case vm::MultinameKind::RTQNameL:
		case vm::MultinameKind::RTQNameLA:
			std::cout << "RTQNameL()" << std::endl;
			break;
		case vm::MultinameKind::Multiname:
		case vm::MultinameKind::MultinameA:
			std::cout << "Multiname(name " << multiname.multiname.name << " (" << strings[multiname.multiname.name] << ") " << ", set " << multiname.multiname.nsSet << ")" << std::endl;
			break;
		case vm::MultinameKind::MultinameL:
		case vm::MultinameKind::MultinameLA:
			std::cout << "MultinameL(set " << multiname.multinameL.nsSet << ")" << std::endl;
			break;
		case vm::MultinameKind::Typename:
			std::cout << "Typename(qname " << multiname.typeName.qName << ", name [";
			for (unsigned i = 0; i < multiname.typeNameParams.size(); ++i) {
				std::cout << multiname.typeNameParams[i];
				if (i != multiname.typeNameParams.size() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << "])" << std::endl;
			break;
		default:
			throw std::runtime_error(
				std::string("Unknown multiname kind: ")
				+ std::to_string(uint8_t(multiname.kind)));
		}
	}
	std::cout << std::endl;
}

int main(int argc, char * argv[])
{
	using namespace vm;

	std::vector<std::string> arguments(argv, argv+argc);
	if (arguments.size() < 2) {
		printUsage(arguments[0]);
		std::exit(1);
	}

	ByteBuffer buffer(loadFile(arguments[1]));

	auto abc = loadAbc(buffer);
	dumpConstantPool(abc.getConstantPool());
	// VirtualMachine vm(abc);

	return 0;
}

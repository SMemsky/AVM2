#include "Dump.h"

#include <algorithm>
#include <iostream>

namespace
{

template <typename T>
void dumpVector(std::vector<T> const & vector, std::string const & name)
{
	std::cout << "# " << name << std::endl;
	for (unsigned i = 0; i < vector.size(); ++i) {
		std::cout << i << " " << vector[i] << std::endl;
	}
	std::cout << std::endl;
}

} // anonymous namespace

namespace dump
{

void dumpConstantPool(vm::AbcConstantPool const & pool)
{
	dumpVector(pool.getInts(), "Ints");
	dumpVector(pool.getUints(), "Uints");
	dumpVector(pool.getDoubles(), "Doubles");
	auto const & strings = pool.getStrings(); // used later
	dumpVector(strings, "Strings");

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

} // namespace dump

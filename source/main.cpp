#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "common/ByteBuffer.h"
#include "common/FileUtil.h"
#include "utils/Dump.h"
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
	dump::dumpConstantPool(abc.getConstantPool());
	// VirtualMachine vm(abc);

	return 0;
}

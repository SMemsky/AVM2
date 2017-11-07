#include <iostream>
#include <string>
#include <vector>

#include "common/ByteBuffer.h"
#include "common/FileUtil.h"

#include "vm/Abc.h"

void printUsage(std::string const & program)
{
	std::cout << "Usage:\n"
		<< "  " << program << " <filename.abc>" << std::endl;
}

int main(int argc, char * argv[])
{
	std::vector<std::string> arguments(argv, argv+argc);
	if (arguments.size() < 2) {
		printUsage(arguments[0]);
		std::exit(1);
	}

	ByteBuffer buffer(loadFile(arguments[1]));
	std::cout << buffer.bytesLeft() << std::endl;

	auto abc = vm::loadAbc(buffer);

	std::cout << abc.getMajorVersion() << " " << abc.getMinorVersion() << std::endl;

	std::cout << abc.getConstantPool().getIntCount() << " integers" << std::endl;
	std::cout << abc.getConstantPool().getUintCount() << " uintegers" << std::endl;
	std::cout << abc.getConstantPool().getDoubleCount() << " doubles" << std::endl;
	std::cout << abc.getConstantPool().getStringCount() << " strings" << std::endl;
	// for (unsigned i = 0; i < abc.getConstantPool().getStringCount(); ++i) {
	// 	std::cout << abc.getConstantPool().getStrings()[i] << std::endl;
	// }

	std::cout << buffer.bytesLeft() << std::endl;

	return 0;
}

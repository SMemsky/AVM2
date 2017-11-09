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

	auto const & constantPool = abc.getConstantPool();

	std::cout << constantPool.getInts().size() << " integers" << std::endl;
	std::cout << constantPool.getUints().size() << " uintegers" << std::endl;
	std::cout << constantPool.getDoubles().size() << " doubles" << std::endl;
	std::cout << constantPool.getStrings().size() << " strings" << std::endl;
	std::cout << constantPool.getNamespaces().size() << " namespaces" << std::endl;
	std::cout << constantPool.getNsSets().size() << " ns sets" << std::endl;
	std::cout << constantPool.getMultinames().size() << " multinames" << std::endl;

	std::cout << abc.getMethods().size() << " methods" << std::endl;
	std::cout << abc.getMetadata().size() << " metas" << std::endl;
	std::cout << abc.getInstances().size() << " instances" << std::endl;
	std::cout << abc.getClasses().size() << " classes" << std::endl;
	std::cout << abc.getScripts().size() << " scripts" << std::endl;
	std::cout << abc.getBodies().size() << " bodies" << std::endl;

	std::cout << buffer.bytesLeft() << std::endl;

	return 0;
}

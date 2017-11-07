#include <iostream>
#include <string>
#include <vector>

#include "common/ByteBuffer.h"
#include "common/FileUtil.h"

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

	return 0;
}

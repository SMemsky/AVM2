#pragma once

#include "vm/Abc.h"

namespace vm
{

class VirtualMachine
{
public:
	VirtualMachine(Abc abc);
private:
	void init();

	Abc abc;
};

} // namespace vm

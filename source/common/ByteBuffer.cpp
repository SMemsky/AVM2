#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(std::vector<uint8_t> data) :
	data(std::move(data)),
	position(0)
{}

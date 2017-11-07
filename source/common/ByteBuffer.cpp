#include "ByteBuffer.h"

#include <cassert>
#include <stdexcept>

ByteBuffer::ByteBuffer(std::vector<uint8_t> data) :
	data(std::move(data)),
	position(0)
{}

uint8_t ByteBuffer::readUint8()
{
	uint8_t value = 0;
	read(&value, 1);
	return value;
}

uint16_t ByteBuffer::readUint16()
{
	uint16_t value = 0;
	read(&value, 2);
	return value;
}

uint32_t ByteBuffer::readUint30()
{
	uint32_t value = 0;
	uint8_t byte = 0;
	int count = 0;

	do {
		byte = readUint8();
		value |= (byte & 0x7f) << (count * 7);
		++count;
	} while ((byte & 0x80) && count <= 5);

	return value;
}

uint32_t ByteBuffer::readUint32()
{
	return readUint30();
}

int32_t ByteBuffer::readInt32()
{
	return static_cast<int32_t>(readUint30());
}

double ByteBuffer::readDouble64()
{
	double value = 0.0;
	read(&value, 8);
	return value;
}

std::string ByteBuffer::readString(std::size_t size)
{
	if (size > bytesLeft()) {
		throw std::runtime_error(
			std::string("Buffer underflow of ")
			+ std::to_string(size - bytesLeft())
			+ " bytes");
	}

	auto oldPosition = position;
	position += size;

	return std::string(reinterpret_cast<char const *>(data.data() + oldPosition), size);
}

void ByteBuffer::read(void * buffer, std::size_t count, bool reverse)
{
	if (count > bytesLeft()) {
		throw std::runtime_error(
			std::string("Buffer underflow of ")
			+ std::to_string(count - bytesLeft())
			+ " bytes");
	}

	if (reverse) {
		for (std::size_t i = 0; i < count; ++i) {
			reinterpret_cast<uint8_t *>(buffer)[i] = data[position + count - i - 1];
		}
	} else {
		for (std::size_t i = 0; i < count; ++i) {
			reinterpret_cast<uint8_t *>(buffer)[i] = data[position + i];
		}
	}

	position += count;
}

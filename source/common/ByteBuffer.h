#pragma once

#include <cstdint>
#include <string>
#include <vector>

class ByteBuffer
{
public:
	ByteBuffer() = default;
	ByteBuffer(std::vector<uint8_t> data);

	uint8_t readUint8();
	uint16_t readUint16();
	uint32_t readUint30();
	uint32_t readUint32();
	int32_t readInt32();
	double readDouble64();

	std::string readString(std::size_t size);

	void read(void * buffer, std::size_t count, bool reverse = false);

	std::size_t bytesLeft() const { return data.size() - position; };
private:
	std::vector<uint8_t> const data;

	std::size_t position;
};

#pragma once

#include <cstdint>
#include <vector>

class ByteBuffer
{
public:
	ByteBuffer() = default;
	ByteBuffer(std::vector<uint8_t> data);

	std::size_t bytesLeft() const { return data.size() - position; };
private:
	std::vector<uint8_t> const data;

	std::size_t position;
};

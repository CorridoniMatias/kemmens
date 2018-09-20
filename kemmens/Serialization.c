#include "kemmens/Serialization.h"


void* Serialization_Serialize(int fieldCount, ...)
{
	if(fieldCount < 1)
		return 0;

	va_list arguments;
	va_start(arguments, fieldCount);

	SerializedPart tmp;

	uint32_t totalsize = 0;
	uint32_t offset = 0;

	void* packet = malloc(1);

	for(int i = 0; i < fieldCount;i++)
	{
		tmp = va_arg(arguments, SerializedPart);

		totalsize += tmp.size + sizeof(uint32_t);

		packet = realloc(packet, totalsize);

		memcpy(packet + offset, &tmp.size, sizeof(uint32_t));

		offset += sizeof(uint32_t);

		memcpy(packet + offset, tmp.data, tmp.size);

		offset += tmp.size;
	}

	uint32_t packetEnd = 0;

	totalsize += sizeof(uint32_t);

	packet = realloc(packet, totalsize);

	memcpy(packet + offset, &packetEnd, sizeof(uint32_t));

	va_end(arguments);

	return packet;
}

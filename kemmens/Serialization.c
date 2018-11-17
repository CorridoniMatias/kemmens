#include "kemmens/Serialization.h"

SerializedPart* Serialization_Serialize(int fieldCount, ...)
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

	SerializedPart* ret = (SerializedPart*)malloc(sizeof(SerializedPart));

	ret->data = packet;
	ret->size = totalsize;

	return ret;
}

DeserializedData* Serialization_Deserialize(void* serializedPacket)
{
	DeserializedData* dest = (DeserializedData*)malloc(sizeof(DeserializedData));

	size_t partSizeLength = sizeof(uint32_t);			//Tamanio de los bloques de tamanio
	uint32_t partSize;									//Tamanio de la siguiente parte a leer

	int partsCount = 0;									//Cantidad de partes a guardar
	int packOffset = 0;									//Desplazamiento en el packet, moverme por el al leerlo
	int requiredSize = 0;								//Tamanio total que requerira el void** de dest, para reallocar

	//Copio el primer tamanio del packet (los primeros 4 bytes) y actualizo contadores
	memcpy(&partSize, serializedPacket, partSizeLength);

	packOffset += partSizeLength;
	requiredSize += partSize;

	//Malloc dummy para poder asignarle algo de memoria al void** de dest
	dest->parts = malloc(1);

	//Solo ejecuto todo esto si el tamanio del proximo bloque es mayor a 0; un 0 indicaria que no hay mas partes a leer
	while(partSize > 0)
	{
		//Reasigno memoria dest (tener en cuenta count), a su void**, y
		//asigno memoria al elemento de ese array donde voy a guardar
		//dest = realloc(dest, requiredSize + sizeof(int));

		dest->parts = realloc(dest->parts, requiredSize);
		dest->parts[partsCount] = malloc(partSize);

		//Copio la parte a leer en el void** de dest, muevo el indice y actualizo el offset para leer otro tamanio
		memcpy(dest->parts[partsCount++], serializedPacket + packOffset, partSize);
		packOffset += partSize;

		//Leo el tamanio del proximo bloque, lo guardo en partSize y actualizo el desplazamiento y el tamanio a reallocar
		memcpy(&partSize, serializedPacket + packOffset, partSizeLength);
		packOffset += partSizeLength;
		requiredSize += partSize;

	}

	//Grabo la cantidad de partes leidas en el struct
	dest->count = partsCount;

	return dest;
}

void Serialization_ShowDeserializedParts(DeserializedData* showable)
{

	int i;
	int amount = showable->count;

	for(i = 0; i < amount; i++)
	{
		printf("Parte %d: %s\n", i + 1, (char*) showable->parts[i]);
	}

	return;

}

void Serialization_CleanupDeserializationStruct(DeserializedData* cleanable)
{

	int i;
	int amount = cleanable->count;

	//Primero, libero la memoria reservada por todas sus partes, una por una
	for(i = 0; i < amount; i++)
	{
		free(cleanable->parts[i]);
	}

	free(cleanable->parts);				//Libero la memoria reservada por el array de partes en si
	free(cleanable);					//Libero la memoria reservada por el struct en si

	return;

}

#ifndef KEMMENS_SOCKETMESSAGETYPES_H_
#define KEMMENS_SOCKETMESSAGETYPES_H_

/*	Definicion de tipos de mensajes para transferencias de datos por TCPIP
 */

//Message Types
#define MESSAGETYPE_STRING 1	//Strings puras
#define MESSAGETYPE_INT	2		//INTS de longitud variable
#define MESSAGETYPE_VOIDPOINTER	3   //void*, pára los packets
#define MESSAGETYPE_ADDRESS	4	//Dirección lógica de memoria

#endif /* KEMMENS_SOCKETMESSAGETYPES_H_ */

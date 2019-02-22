#ifndef KEMMENS_SOCKETMESSAGETYPES_H_
#define KEMMENS_SOCKETMESSAGETYPES_H_

/*	Definicion de tipos de mensajes para transferencias de datos por TCPIP
 */

//Message Types
#define MESSAGETYPE_STRING 1	//Strings puras
#define MESSAGETYPE_INT	2		//INTS de longitud variable
#define MESSAGETYPE_VOIDPOINTER	3   //void*, pára los packets
#define MESSAGETYPE_ADDRESS	4	//Dirección lógica de memoria

//PROTOCOLO MDJ (1)
#define MESSAGETYPE_MDJ_CHECKFILE 	10	//Comando: Validar Archivo
#define MESSAGETYPE_MDJ_CREATEFILE	11	//Comando: Crear Archivo
#define MESSAGETYPE_MDJ_GETDATA		12	//Comando: Obtener bytes de archivo
#define MESSAGETYPE_MDJ_PUTDATA		13	//Comando: Grabar datos a archivo
#define MESSAGETYPE_MDJ_DELETEFILE	14	//Comando: Eliminar un archivo

//PROTOCOLO DAM (2)

//#define MESSAGETYPE_DAM_ABRIR	20 //Comando: abrir <pathFile>
//#define MESSAGETYPE_DAM_FLUSH	21 //Comando flush <pathFile>

#define MESSAGETYPE_DAM_SAFA_DUMMY	220 //Finaliza carga del script del dummy
#define MESSAGETYPE_DAM_SAFA_ABRIR	221 //Comando: abrir
#define MESSAGETYPE_DAM_SAFA_CREAR	222 //Comando: crear
#define MESSAGETYPE_DAM_SAFA_BORRAR	223 //Comando: borrar
#define MESSAGETYPE_DAM_SAFA_FLUSH	224 //Comando: flush
#define MESSAGETYPE_DAM_SAFA_ERR	225 //Error en la operacion con SAFA


///PROTOCOLO S-AFA (5XX)

//MENSAJES PARA EL CPU
#define MESSAGETYPE_SAFA_CPU_EXECUTE 521		//Ejecutar un DTB con ciertos parametros (path, quantum, PC, etc)
#define MESSAGETYPE_SAFA_CPU_WAITRESPONSE 522	//Respuesta a una operacion Wait hecha por un CPU; 1:aceptada, 0:denegada

//PROTOCOLO CPU
//Se utilizan con el safa
#define MESSAGETYPE_CPU_EOFORABORT 320  //Codigo para mandar al safa cuando se quiere abortar un DTB o el mismo termina
#define MESSAGETYPE_CPU_BLOCKDUMMY 321  //Codigo para bloquear un dummy (se le manda al safa)
#define MESSAGETYPE_CPU_BLOCKDTB 322    //Codigo para bloquear un DTB
#define MESSAGETYPE_CPU_EOQUANTUM 323	//Codigo cuando se termina el quantum de un DTB
#define MESSAGETYPE_CPU_WAIT 324		//Comando: wait
#define MESSAGETYPE_CPU_SIGNAL 325		//Comando: signal
#define MESSAGETYPE_CPU_EXECDUMMY 326	//Pedirle al DAM que haga el Dummy
//Se utilizan con FM9 y DAM
#define MESSAGETYPE_CPU_ABRIR 330		//Comando: Abrir
#define MESSAGETYPE_CPU_FLUSH 331		//Comando: flush
#define MESSAGETYPE_CPU_BORRAR 332		//Comando: borrar
#define MESSAGETYPE_CPU_CREAR 333		//Comando crear
#define MESSAGETYPE_CPU_RECEIVELINE 334	//Recibir linea del FM9
#define MESSAGETYPE_CPU_FREEGDT 335	//Liberar la memoria del FM9 para un DTB que finalizo en el SAFA mediante el comando finalizar

//PROTOCOLO FM9 (4X)
#define MESSAGETYPE_FM9_GETLINE 40 //Pedir una linea de un archivo
#define MESSAGETYPE_FM9_ASIGN 41 //Asignar una linea de un archivo
#define MESSAGETYPE_FM9_CLOSE 42 //Cerrar un un archivo
#define MESSAGETYPE_FM9_OPEN 43 //Cargar un archivo a memoria desde el MDJ, se recibe desde el DAM
#define MESSAGETYPE_FM9_FLUSH 44 //Flushear un archivo de memoria al MDJ, se envia al DAM
#define MESSAGETYPE_FM9_CLOSEDTB 45 //Cerrar todos los archivos de un dtb liberando su memoria ocupada

#endif /* KEMMENS_SOCKETMESSAGETYPES_H_ */

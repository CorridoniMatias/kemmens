# Biblioteca Kemmens

[![Build Status](https://travis-ci.com/CorridoniMatias/kemmens.svg?token=sSWyxCwxLt4aosHftSx2&branch=master)](https://travis-ci.com/CorridoniMatias/kemmens)

## ¿Qué es?
Es una biblioteca multiuso programada en C que brinda una abstracción sobre las siguientes funcionalidades:
- Socket Server: Permite bindear un servidor en un IP:Puerto. La biblioteca se encargará de ejecutar funciones especificadas por el usuario cuando se conecta un cliente, llega información, hay errores y cuando un cliente se desconecta.
- Socket Client: Brinda funcionalidades para conectarse a un Socket Server.
- Socket Commons: Brinda funcionalidades para enviar y recibir información a través de un socket.
- Serialization: Permite serializar y deserializar paquetes.
- String Utils: Algunos utils para strings y arrays de strings.
- Thread Pool: Un [Thread Pool](https://en.wikipedia.org/wiki/Thread_pool).
- Thread Manager: Utils para manejar hilos.
- Command Interpreter: Básicamente un parser que ejecuta una función indicada por el usuario ante un input específico.
- Logger: Un wrapper del log que viene con las [Commons de la cátedra de UTNSO](https://github.com/sisoputnfrba/so-commons-library).
- Config Manager: Extensión de las funcionalidades que provee config de las Commons.
- Utils: Funcionalidades que no tienen una categoría.

## ¿Por qué?
Cuando desarrollamos las Kemmens lo hicimos con el objetivo de poder abstraer al máximo aquella lógica que ibamos a usar comúnmente para poder abocarnos de lleno a las funcionalidades propias del TP. De esta forma nos aseguramos de no perder el tiempo reinventando la rueda para cada proceso requerido.

## ¿Podemos usarla?
Si! Cuando entregamos el TP consultamos con la cátedra si podíamos dejar la biblioteca como código abierto y nos lo permitieron. En teoría no debería haber ningún problema con que usen las funcionalidades que trae (de todas formas consulten con sus ayudantes).

## ¿Puedo colaborar?
Claro que si, simplemente hace un pull request.

## Ejemplos
### Socket Server
```c
    void processLineInput(char* line)
    {
    	printf("Comando: %s\n", line);
    	free(line);
    }
    
    void ClientConnected(int socket)
    {
    	printf("Cliente se conecto %d\n", socket);
    }
    
    void ClientDisconnected(int socket)
    {
    	printf("Cliente se desconecto %d\n", socket);
    }
    
    void ClientError(int socketID, int errorCode)
    {
    	printf("Cliente %d se reporto con error %s!\n", socketID, strerror(errorCode));
    }
    void onPacketArrived(int socketID, int message_type, void* data, int message_length)
    {
        free(data);
    }
    
    void StartServer()
    {
        SocketServer_Start("Test", 8080);
    	SocketServer_ActionsListeners actions = INIT_ACTION_LISTENER;
    
    	actions.OnConsoleInputReceived = processLineInput;
    	actions.OnPacketArrived = onPacketArrived;
    	actions.OnClientConnected = ClientConnected;
    	actions.OnClientDisconnect = ClientDisconnected;
    	actions.OnReceiveError = ClientError;
    	SocketServer_ListenForConnection(actions); //Este hilo se va a bloquear acá hasta que: llegue un paquete, se conecte un cliente, se desconecte un cliente o que haya input del teclado!
	}
```

Como podemos ver por el código es muy fácil levantar un server de sockets con la biblioteca, algo que llevaría mucho más tiempo si se tuviera que hacer desde 0. Los eventos que registramos con el server son:
- __OnConsoleInputReceived__: Se llama a la func. cuando se recibe una línea por teclado (es decir se apreta __enter__), la línea se recibe por parámetro y es responsabilidad del usuario liberar la memoria luego de utilizarla.
- __OnPacketArrived__: LLegó un paquete con datos. El usuario recibe por params el socket de donde vino el paquete, el tipo de mensaje con el cual fue enviado el paquete, el paquete propiamente dicho y el length de lo recibido. Es responsabilidad del usuario liberar la memoria luego de utilizar el paquete.
- __OnClientConnected__: Se conectó un cliente.
- __OnClientDisconnect__: Se desconectó un cliente.
- __OnReceiveError__: Hubo un error al recibir un paquete.

_Nota_: Todos estas funciones son llamadas en el mismo hilo que el Socket Server, por ende toda operación que se haga va a demorar que el server pueda volver a bloquearse a la espera de nuevos mensajes, y peor aún, si se bloquea el hilo, el server no va a poder volver a atender mensajes/conexiones. Para evitar esto usar el módulo de Thread Pool o hilos por tu cuenta.

##### ¿Por qué manejamos el Console Input con el [select()](http://man7.org/linux/man-pages/man2/select.2.html)?
Si observamos la semántica del select() observamos que dice:

> select() and pselect() allow a program to monitor multiple file
       descriptors, waiting until one or more of the file descriptors become
       "ready" for some class of I/O operation (e.g., input possible).  A
       file descriptor is considered ready if it is possible to perform a
       corresponding I/O operation (e.g., read(2) without blocking, or a
       sufficiently small write(2)).

Es decir, habla de File Descriptors (FD), en ningún lado dice que únicamente es capaz de manejar sockets o que se debería utilizar para eso y nada más. Ahora, ¿qué es un FD? Linux tiene una tabla de archivos abiertos por proceso, el index de cada entrada de esa tabla es un FD, o sea un número. Cabe destacar que en Linux todo es un archivo, el teclado es un archivo, un socket abierto es un archivo, la salida por pantalla es un archiv, etc. Además cada vez que un proceso inicia Linux automáticamente asigna 3 archivos: STDOUT(0), STDIN(1), STDERR(2). De esta forma la entrada por teclado tiene identificador 0 (se puede leer más [acá](https://en.wikipedia.org/wiki/File_descriptor)).
Entonces, es un error de concepto manejar la consola con un select()? __NO!__ Con lo que acabamos de ver es más que suficiente para decir que no lo es.

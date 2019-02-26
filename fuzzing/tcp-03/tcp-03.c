#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>

#define PORT 16000
#define CONNECTION_BACKLOG 10
#define BUFFER_SIZE 64

void finish_connection(int socket) {
  shutdown(socket, SHUT_RDWR);
  close(socket);
}

void handle_client_connection(int s_client) {
  char buffer[BUFFER_SIZE];
    
  const char *message_ok = "OK\n";
  const char *expect = "START";

  // Recibe los datos correspondientes a la etapa 1.
  ssize_t r = recv(s_client, (char*)&buffer, BUFFER_SIZE, 0);
    
  if (strncmp(expect, (char*)&buffer, strlen(expect)) != 0) {
    finish_connection(s_client);
    return;
  }
    
  // Enviamos ACK.
  send(s_client, message_ok, 2, 0);
    
  // Procesamos los datos correspondientes a la etapa 2.
  bzero((void*)&buffer, BUFFER_SIZE);
  r = recv(s_client, (char*)&buffer, BUFFER_SIZE, 0);
    
  // Vulnerabilidad simple.
  buffer[buffer[BUFFER_SIZE - 1]] = 0;
    
  // Finaliza el intercambio de datos y cierra el socket.
  finish_connection(s_client);
}

int initialize_server() {
  struct sockaddr_in server_address;

  // Pone en cero la memoria de la estructura.
  bzero(&server_address, sizeof(server_address));

  // Inicializa el socket que escuchará conexiones.
  int s_listen = socket(AF_INET, SOCK_STREAM, 0);

  // Finaliza la ejecucion en caso de error.
  if (s_listen < 0) {
    return -1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  server_address.sin_port = htons(PORT);

  // Asocia el descriptor del socket s_listen a la dirección.
  struct sockaddr *addr = (struct sockaddr *)&server_address;
  bind(s_listen, addr, sizeof(server_address));

  // Pone a escuchar el socket.
  listen(s_listen, CONNECTION_BACKLOG);

  // El socket que escucha ya ha sido inicializado y está asociado
  // al puerto PORT en la interfaz local; devolvemos el descriptor.
  return s_listen;
}

void handle_client_connections(int s_listen) {
  for (;;) {
      
    // Acepta una conexión entrante y devuelve el socket.
    int s_client = accept(s_listen, (struct sockaddr*) NULL, NULL);

    // Maneja la conexión del cliente.
    handle_client_connection(s_client);
  }
}

int main(int argc, char **argv) {
  // Inicializa el socket que escuchará por conexiones de clientes.
  int s_listen = initialize_server();

  /*
   * Si el socket fue inicializado correctamente, se aceptan
   * y se manejan las conexiones entrantes.
   */
  if (s_listen > 0) {
    handle_client_connections(s_listen);
  }
}

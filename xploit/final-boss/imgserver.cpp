#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

#include "imgcommon.h"
#include "imgserver-handler.h"

#define CONNECTION_BACKLOG 10

/* Inicializa un servidor local UNIX asociado al path socket_path. Para
 * gestionar toda la comunicación con el servidor se puede usar la
 * biblioteca libimg.
 *
 */
int initialize_daemon() {
  int s = socket(AF_UNIX, SOCK_STREAM, 0);

  if (s == -1) {
    perror("Could not initialize server socket");
    exit(1);
  }

  struct sockaddr_un address;
  bzero(&address, sizeof(address));
  address.sun_family = AF_UNIX;
  strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);
  unlink(SOCKET_PATH);

  if (bind(s, (struct sockaddr*)&address, sizeof(address)) == -1) {
    perror("Could not bind server socket");
    exit(1);
  }

  if (listen(s, CONNECTION_BACKLOG) == -1) {
    perror("Could not start listener");
    exit(1);
  }

  return s;
}

/* Inicializa el servidor local y gestiona pedidos de clientes.
 *
 */
int main(int argc, char **argv) {
  int s = initialize_daemon();

  while (true) {
    int c;

    if ((c = accept(s, NULL, NULL)) == -1) {
      perror("Client accept error");
      continue;
    }

    handle_request(c);
    close(c);
  }
}

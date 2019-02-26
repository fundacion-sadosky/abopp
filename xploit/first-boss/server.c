// server.c

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>

#define BUFFER_SIZE 256
#define MAX_FIELD_LENGTH 64
#define PORT 16000

const uint8_t DELIMITER = 0xa;

const int CONNECTION_BACKLOG = 10;

typedef struct {
  uint64_t id;
  char name[MAX_FIELD_LENGTH];
  // ... otros campos
} profile_t;

typedef struct {
  char username[MAX_FIELD_LENGTH];
  char password[MAX_FIELD_LENGTH];
  profile_t *profile;
} user_t;

void finish_connection(int socket) {
  shutdown(socket, SHUT_RDWR); close(socket);
}

int recv_data(int socket, char *buffer, size_t bytes) {
  ssize_t r = recv(socket, buffer, bytes, 0);
  return (r == -1 || r == 0) ? -1 : r;
}

int send_data(int socket, const char *buffer, size_t bytes) {
  ssize_t s = send(socket, buffer, bytes, MSG_NOSIGNAL);
  return (s == -1 || s == 0) ? -1 : s;
}

/*
 * Solamente si las credenciales user->username y user->password son correctas,
 * completa los datos del perfil del usuario, user->profile; en caso contrario,
 * no modifica ningún dato.
 */
void fill_user_profile(user_t *user) {
  /* Stub, no conocemos credenciales válidas.
   * Podemos asumir que esta función nunca modificará al objeto user.
   */
  return;
}

/*
 * Persiste los datos del perfil en la base de datos. En caso de éxito
 * devuelve 1; en caso de error devuelve 0; en caso de error (e.g. el id
 * del usuario no existe en las tablas), los datos en la base de datos
 * no serán modificados.
 */
int update_user_profile(profile_t *profile) {
  // Stub, no necesitamos actualizar ningún usuario válido.
  return 0;
}

void handle_session(int s_client, user_t *userdata) {
  char control_buffer[BUFFER_SIZE];

  const size_t CMD_LEN = 4;

  const char *CMD_QUIT = "QUIT";
  const char *CMD_NAME = "NAME";

  for (;;) {

    // Limpia el buffer de control, que guardará los comandos enviados.
    bzero(control_buffer, BUFFER_SIZE);

    // Lee el siguiente comando del cliente.
    if (recv_data(s_client, (char*)&control_buffer, CMD_LEN) == -1) {
      // Error
      break;
    }

    if (strncmp(CMD_QUIT, control_buffer, CMD_LEN) == 0) {
      /*
       * Comando CMD_QUIT
       * Finaliza la sesión.
       */
      break;

    } else if (strncmp(CMD_NAME, control_buffer, CMD_LEN) == 0) {
      /*
       * Comando CMD_NAME
       * Cambia el nombre asociado al perfil y lo persiste.
       */

      bzero(userdata->profile->name, MAX_FIELD_LENGTH);

      size_t space_left = BUFFER_SIZE - CMD_LEN;
      if (recv_data(s_client, (char*)&control_buffer + CMD_LEN, space_left) == -1) {
        // Error
        break;
      }

      // Copia el nuevo nombre de perfil a la memoria.
      char *p = &control_buffer[CMD_LEN];

      for (int i = 0; *p != 0xa; ++i, ++p) {
        userdata->profile->name[i] = *p;
      }

      /*
       * Intenta persistir el nuevo nombre en la base de datos.
       * En caso de falla, el perfil no pudo se actualizado;
       * se informa el error al usuario y se cierra la sesión.
       */
      if (!update_user_profile(userdata->profile)) {
        char *message = "ERROR:COULD_NOT_UPDATE,CLOSING;\n";
        send_data(s_client, message, strlen(message));
        break;
      }

    } else {
      char *message = "CMD:UNKNOWN;\n";
      if (send_data(s_client, message, strlen(message)) == -1) {
        // Error
        break;
      }
    }
  }
  const char *message = "BYE\n";
  send_data(s_client, message, strlen(message));
}

void handle_client_connection(int s_client) {
  char read_buffer[BUFFER_SIZE];

  /*
   * Perfil del usuario; será cargado en memoria cuando el usuario
   * se autentique en la aplicación.
   */
  profile_t profile;

  /*
   * Estructura que contendrá las credenciales del usuario temporalmente
   * hasta que éste inicie sesión.
   */
  user_t user;

  // Limpia las estructuras relevantes.
  bzero(&profile, sizeof(profile_t));
  bzero(&user, sizeof(user_t));

  /*
   * En principio, el usuario tendrá un perfil nulo, el cuál
   * será completado cuando la autenticación sea exitosa.
   */
  user.profile = &profile;

  // Inicia un ciclo interactivo con el cliente.
  for (;;) {

    // Limpia el buffer de entrada.
    bzero(read_buffer, BUFFER_SIZE);

    // Limpia usuario y contraseña
    for (int i = 0; i < MAX_FIELD_LENGTH; ++i) {
      user.username[i] = 0;
      user.password[i] = 0;
    }

    // Lee los datos del usuario al buffer de entrada.
    ssize_t r_bytes = recv(s_client, read_buffer, BUFFER_SIZE, 0);

    // En caso de error termina.
    if (r_bytes == -1 || r_bytes == 0) {
      break;
    }

    // Procesa el nombre de usuario primero.
    char *user_ptr = read_buffer;
    char *pass_ptr = user_ptr;

    for (int i = 0; user_ptr[i] != ':'; ++i) {
      user.username[i] = user_ptr[i];
      pass_ptr++;
    }

    // Procesa la contraseña.
    pass_ptr++;

    for (int i = 0; pass_ptr[i] != DELIMITER; ++i) {
      user.password[i] = pass_ptr[i];
    }

    // Carga los datos del perfil del usuario.
    fill_user_profile(&user);

    if (user.profile->id == 0) {
      // Autenticacion fallida.
      const char *message = "AUTH:FAILURE;USER:";
      if (send_data(s_client, message, strlen(message)) == -1 ||
          send_data(s_client, user.username, strlen(user.username)) == -1 ||
          send_data(s_client, &DELIMITER, 1) == -1) {
          // Error
          break;
      };

    } else {
      // Autenticación exitosa.
      const char *message = "AUTH:SUCCESS;PROFILE_NAME:";
      if(send_data(s_client, message, strlen(message)) == -1 ||
         send_data(s_client, user.profile->name, strlen(user.profile->name)) == -1 ||
         send_data(s_client, &DELIMITER, 1) == -1) {
         // Error
         break;
      };

      // Se procede al intercambio de mensajes.
      handle_session(s_client, &user);
      break;
    }
  }

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

  // Asocia el descriptor del socket s_listen a la dirección especificada.
  bind(s_listen, (struct sockaddr *)&server_address, sizeof(server_address));

  // Pone a escuchar el socket.
  listen(s_listen, CONNECTION_BACKLOG);

  // El socket que escucha ya ha sido inicializado y está asociado
  // al puerto PORT en todas las interfaces; devolvemos el descriptor.
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

void aid() {
  asm("pop %rsi");
  asm("ret");
  asm("pop %rdi");
  asm("ret");
  asm("pop %rdx");
  asm("ret");
}

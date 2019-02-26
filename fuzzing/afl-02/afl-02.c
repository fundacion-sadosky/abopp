#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

typedef struct {
  char name[64];
  char address[64];
} profile_t;

typedef struct {
  char username[64];
  char password[64];
  profile_t *profile;
} user_t;

int main(int argc, char **argv) {
  char buffer[256];

  user_t user;
  bzero((void*)&user, sizeof(user_t));

  profile_t profile;
  bzero((void*)&profile, sizeof(profile_t));

  user.profile = &profile;

  ssize_t r = read(STDIN_FILENO, &buffer, 128);

  if (strncmp("S ", (char*)&buffer, 2) == 0) {
    int x = 0;

    for (int i = 0; buffer[i + 2] != ':'; i++, x++) {
      user.username[i] = buffer[i + 2];
    }
    x++;
    for (int i = 0; buffer[i] != ';'; i++) {
      user.password[i] = buffer[x];
    }
    
  }
}

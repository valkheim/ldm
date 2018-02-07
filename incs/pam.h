#pragma once
#include <stdbool.h>

#define SERVICE_NAME ("ldm")

#define pam_err(name) \
  do { \
    fprintf(stderr, "%s: %s\n", name, \
        pam_strerror(pam_handle, result)); \
    end(result); \
    break; \
  } while (1); \

void login(char const * const username, char const * const password);
void pam_login(char const * const username, char const * const password, pid_t * const child_pid);
bool pam_logout(void);

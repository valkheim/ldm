/*
 * This file is part of the ldm distribution (https://github.com/valkheim/ldm)
 * Copyright (c) 2018 Charles Paulet.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <pwd.h>
#include <paths.h>
#include <sys/wait.h>

#include "pam.h"

static void init_env(struct passwd *pw);
static void set_env(char *name, char *value);
static int end(int last_result);

static int conv(int num_msg, const struct pam_message **msg,
    struct pam_response **resp, void *appdata_ptr);

static pam_handle_t *pam_handle;

void login(char const * const username, char const * const password)
{
  pid_t child_pid;
  int status;

  pam_login(username, password, &child_pid);
  waitpid(child_pid, &status, 0);
  pam_logout();
}

void pam_login(char const * const username, char const * const password, pid_t * const child_pid)
{
  puts("PAM_LOGIN");
  int result;
  char const *data[2] = { username, password };
  struct pam_conv const pam_conv = {
    conv, data
  };

  result = pam_start(SERVICE_NAME, username, &pam_conv, &pam_handle);
  if (result != PAM_SUCCESS)
    pam_err("pam_start");
  result = pam_authenticate(pam_handle, 0);
  if (result != PAM_SUCCESS)
    pam_err("pam_authenticate");
  result = pam_acct_mgmt(pam_handle, 0);
  if (result != PAM_SUCCESS)
    pam_err("pam_acct_mgmt");
  result = pam_setcred(pam_handle, PAM_ESTABLISH_CRED);
  if (result != PAM_SUCCESS)
    pam_err("pam_setcred");
  result = pam_open_session(pam_handle, 0);
  if (result != PAM_SUCCESS)
  {
    pam_setcred(pam_handle, PAM_DELETE_CRED);
    pam_err("pam_open_session");
  }
  struct passwd * const pw = getpwnam(username);
  init_env(pw);

  switch (*child_pid = fork())
  {
    case 0:
      chdir(pw->pw_dir);
      char *cmd = "exec /bin/bash --login .xinitrc";
      puts("CALL XINITRC");
      execl(pw->pw_shell, pw->pw_shell, "-c", cmd, NULL);
      fprintf(stderr, "Failed to start ldm\n");
      _exit(1);
    case -1:
      fprintf(stderr, "No fork done\n");
      break;
    default:
      puts("We're in the parent");
      break;
  }
}

static int conv(int num_msg, const struct pam_message **msg,
    struct pam_response **resp, void *appdata_ptr) {
  int i;
  int result;
  char *username = NULL;
  char *password = NULL;

  if ((*resp = calloc((unsigned long)num_msg, sizeof(struct pam_response))) == NULL)
    return PAM_BUF_ERR;

  result = PAM_SUCCESS;
  for (i = 0; i < num_msg; ++i)
  {
    switch (msg[i]->msg_style)
    {
      case PAM_PROMPT_ECHO_ON:
        username = ((char **)appdata_ptr)[0];
        (*resp)[i].resp = strdup(username);
        break;
      case PAM_PROMPT_ECHO_OFF:
        password = ((char **)appdata_ptr)[1];
        (*resp)[i].resp = strdup(password);
        break;
      case PAM_ERROR_MSG:
        fprintf(stderr, "%s\n", msg[i]->msg);
        result = PAM_CONV_ERR;
        break;
      case PAM_TEXT_INFO:
        printf("%s\n", msg[i]->msg);
        break;
    }
    if (result != PAM_SUCCESS)
      break;
  }
  if (result != PAM_SUCCESS)
  {
    free(*resp);
    *resp = 0;
  }
  return result;
}

static void init_env(struct passwd *pw) {
  set_env("HOME", pw->pw_dir);
  set_env("PWD", pw->pw_dir);
  set_env("SHELL", pw->pw_shell);
  set_env("USER", pw->pw_name);
  set_env("LOGNAME", pw->pw_name);
  set_env("PATH", "/usr/local/sbin:/usr/local/bin:/usr/bin");
  set_env("MAIL", _PATH_MAILDIR);

  size_t xauthority_len = strlen(pw->pw_dir) + strlen("/.Xauthority") + 1;
  char *xauthority = malloc(xauthority_len);
  snprintf(xauthority, xauthority_len, "%s/.Xauthority", pw->pw_dir);
  set_env("XAUTHORITY", xauthority);
  free(xauthority);
}

static void set_env(char *name, char *value) {
  // The `+ 2` is for the '=' and the null byte
  size_t name_value_len = strlen(name) + strlen(value) + 2;
  char *name_value = malloc(name_value_len);
  snprintf(name_value, name_value_len,  "%s=%s", name, value);
  pam_putenv(pam_handle, name_value);
  free(name_value);
}

bool pam_logout(void)
{
  int result = pam_close_session(pam_handle, 0);
  if (result != PAM_SUCCESS) {
    pam_setcred(pam_handle, PAM_DELETE_CRED);
    pam_err("pam_close_session");
  }

  result = pam_setcred(pam_handle, PAM_DELETE_CRED);
  if (result != PAM_SUCCESS) {
    pam_err("pam_setcred");
  }

  end(result);
  return true;
}

static int end(int last_result)
{
  int result = pam_end(pam_handle, last_result);
  pam_handle = 0;
  return result;
}

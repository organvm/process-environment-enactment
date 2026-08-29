#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void die(const char *message) {
  perror(message);
  _exit(111);
}

static size_t count_env(char *const envp[]) {
  size_t count = 0;
  while (envp[count] != NULL) {
    count++;
  }
  return count;
}

static void exec_print(char *self, char *const envp[]) {
  char *const next_argv[] = {self, "print", NULL};
  execve(self, next_argv, envp);
  die("execve");
}

static void print_env(char *const envp[]) {
  for (size_t index = 0; envp[index] != NULL; index++) {
    printf("%03zu:%s\n", index, envp[index]);
  }
}

static void seed_basic(char *self, const char *target) {
  char *const seeded_envp[] = {"third=3", "first=1", "second=2", NULL};
  char *const next_argv[] = {self, (char *)target, NULL};
  execve(self, next_argv, seeded_envp);
  die("execve");
}

static void seed_edge(char *self) {
  char nul_storage[] = "NAME=value\0TAIL=x";
  char *const seeded_envp[] = {
      "some-other=values",
      "NAME+some-other=value+values",
      "=empty-name",
      "noequals",
      "name=value=with=equals",
      "dup=one",
      "dup=two",
      nul_storage,
      NULL,
  };
  exec_print(self, seeded_envp);
}

static void append_entry(char *self, char *const envp[]) {
  size_t count = count_env(envp);
  char **next_envp = calloc(count + 2, sizeof(char *));
  if (next_envp == NULL) {
    die("calloc");
  }
  for (size_t index = 0; index < count; index++) {
    next_envp[index] = envp[index];
  }
  next_envp[count] = "added=entry";
  next_envp[count + 1] = NULL;
  exec_print(self, next_envp);
}

static void omit_entry(char *self, char *const envp[]) {
  size_t count = count_env(envp);
  char **next_envp = calloc(count + 1, sizeof(char *));
  if (next_envp == NULL) {
    die("calloc");
  }
  size_t write_index = 0;
  for (size_t read_index = 0; read_index < count; read_index++) {
    if (strcmp(envp[read_index], "first=1") != 0) {
      next_envp[write_index++] = envp[read_index];
    }
  }
  next_envp[write_index] = NULL;
  exec_print(self, next_envp);
}

static void replace_entry(char *self, char *const envp[]) {
  size_t count = count_env(envp);
  char **next_envp = calloc(count + 2, sizeof(char *));
  if (next_envp == NULL) {
    die("calloc");
  }
  size_t write_index = 0;
  for (size_t read_index = 0; read_index < count; read_index++) {
    if (strcmp(envp[read_index], "first=1") != 0) {
      next_envp[write_index++] = envp[read_index];
    }
  }
  next_envp[write_index++] = "first=replaced";
  next_envp[write_index] = NULL;
  exec_print(self, next_envp);
}

int main(int argc, char **argv, char **envp) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s MODE\n", argv[0]);
    return 64;
  }
  if (strcmp(argv[1], "print") == 0) {
    print_env(envp);
    return 0;
  }
  if (strcmp(argv[1], "seed-edge") == 0) {
    seed_edge(argv[0]);
  }
  if (strcmp(argv[1], "seed-basic") == 0) {
    if (argc < 3) {
      fprintf(stderr, "seed-basic requires target mode\n");
      return 64;
    }
    seed_basic(argv[0], argv[2]);
  }
  if (strcmp(argv[1], "replicate") == 0) {
    exec_print(argv[0], envp);
  }
  if (strcmp(argv[1], "append") == 0) {
    append_entry(argv[0], envp);
  }
  if (strcmp(argv[1], "omit") == 0) {
    omit_entry(argv[0], envp);
  }
  if (strcmp(argv[1], "replace") == 0) {
    replace_entry(argv[0], envp);
  }
  fprintf(stderr, "unknown mode: %s\n", argv[1]);
  return 64;
}

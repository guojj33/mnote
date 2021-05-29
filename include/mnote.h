#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>

const char programDir[] = ".mnote";
const char configFile[] = ".config";
const char noteExtension[] = ".md";
char *fullProgramDir; //  ~/.mnote
char *fullConfigFile; //  ~/.mnote/.config

char **notes;
int noteCount = 0;

char *joinPath(const char *a, const char *b);
char *getNotePath(int selectedIndex);
void update_config();

void init();
void do_list();
char *getNewNoteName();
void do_new();
void do_help();
void do_vim(int selectedIndex);
void do_delete(int selectedIndex);
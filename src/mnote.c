#include "mnote.h"

char *joinPath(const char *a, const char *b) {
  char *path = (char *)malloc(strlen(a)+1+strlen(b));
  strcpy(path, a);
  strcpy(path+strlen(a), "/");
  strcpy(path+strlen(a)+1, b);
  return path;
}

void init() {
  notes = (char **)malloc(1*sizeof(char *));
  notes[0] = NULL;
  char *homeDir = getenv("HOME"); //stdlib.h
  if (homeDir != NULL) {
    fullProgramDir = joinPath(homeDir, programDir);
    // printf("[debug] fullProgramDir = %s\n", fullProgramDir);
    struct stat st = {0};
    if (stat(fullProgramDir, &st) == -1) {
      // printf("[debug] fail to access dir %s\n", fullProgramDir);
      int result = mkdir(fullProgramDir, 0777);
      // printf("[debug] mkdir=%d\n", result);
    }
    fullConfigFile = joinPath(fullProgramDir, configFile);
    FILE *cf = fopen(fullConfigFile, "r");//open if exists
    if (cf == NULL) {//doesn't exist
      // printf("[debug] fail to read file %s\n", fullConfigFile);
      cf = fopen(fullConfigFile, "w+");// create and read
      // printf("[debug] create file %s\n", fullConfigFile);
    }
    char fileNameBuf[128];
    noteCount = 0;
    while(fscanf(cf, "%s", fileNameBuf) != EOF) {
      noteCount ++;
      char *fileName = (char*)malloc(strlen(fileNameBuf));
      strncpy(fileName, fileNameBuf, strlen(fileNameBuf));
      notes = (char **)realloc(notes, noteCount*sizeof(char *));
      notes[noteCount-1] = fileName;
    }
    fclose(cf);
  }
}

/* selectedIndex start from 0 */
char *getNotePath(int selectedIndex) {  
  if (selectedIndex < noteCount && selectedIndex >= 0) {
    char *fullPath = joinPath(fullProgramDir, notes[selectedIndex]);
    return fullPath;
  }
  return NULL;
}

/* write notes back to file .config*/
void update_config() {
  FILE *cf = fopen(fullConfigFile, "w");
  for (int i = 0; i < noteCount; ++i) {
    fprintf(cf, "%s\n", notes[i]);
  }
}

void printDivideLine(int count) {
  for (int i = 0; i < count; ++i) {
    printf("-");
  }
  printf("\n");
}

/* list all notes */
void do_list() {
  // printf("[debug] do list\n");
  printf("- NOTE LIST -\n");
  for (int i = 0; i < noteCount; ++i) {
    printf("\033[34mNote [\033[31m%d\033[34m]\033[0m\n", i+1);
    int pid = fork();
    if (pid) { // parent
      int status;
      wait(&status);
    }
    else {  // child
      char *notePath = getNotePath(i);
      execlp("cat", "cat", notePath, NULL);
      // printf("[debug] fail to run command \"cat\"\n");
    }
  }
}

int getIntLen(int i) {
  int len = 0;
  while(i) {
    len ++;
    i /= 10;
  }
  return len;
}

char *getNewNoteName() {
  time_t now;
  struct tm *timenow;
  time(&now);
  timenow = localtime(&now);
  char buf[25];
  strftime(buf, 25, "%Y.%m.%d.%H.%M.%S", timenow);
  char *str = (char *)malloc(strlen(buf)+strlen(noteExtension));
  strncpy(str, buf, strlen(buf));
  strncpy(str+strlen(buf), noteExtension, strlen(noteExtension));
  return str;
}

/* create new note */
void do_new() {
  // printf("[debug] do new\n");
  /* get new note name */
  char *newNoteName = getNewNoteName();
  noteCount++;// update noteCount
  notes = realloc(notes, noteCount*sizeof(char *));
  notes[noteCount-1] = newNoteName;
  // printf("[debug] new file name = %s\n", newNoteName);
  char *fullNotePath = joinPath(fullProgramDir, newNoteName);
  /* create file */
  FILE *note = fopen(fullNotePath, "w");
  fclose(note);
  /* update .config */
  update_config();
}

void do_help() {
  
}

/* open selected note with Vim */
void do_vim(int selectedIndex) {
  printf("[debug] do vim\n");
  char *fullPath = getNotePath(selectedIndex);
  if (fullPath != NULL) {
    // printf("[debug] vim %s", fullPath);
    execlp("vim", "vim", fullPath, NULL);
    // printf("[debug] fail to run command \"vim\"\n");
  }
  else {
    // printf("[debug] file doesn't exist\n");
  }
}

/* print selected note to stdout */
void do_print(int selectedIndex) {
  // printf("[debug] do print\n");
  int pid = fork();
  if (pid) { // parent
    int status;
    wait(&status);
  }
  else {  // child
    char *notePath = getNotePath(selectedIndex);
    execlp("cat", "cat", notePath, NULL);
  }
}

/* delete selected note */
void do_delete(int selectedIndex) {
  // printf("[debug] do delete\n");
  char *fullNotePath = getNotePath(selectedIndex);
  if (fullNotePath != NULL) {
    int result = remove(fullNotePath);
    for (int i = selectedIndex; i < noteCount-1; ++i) {
      notes[i] = notes[i+1];
    }
    --noteCount;  // update noteCount
    update_config();// update .config
  }
  else {
    // printf("[debug] file doesn't exist\n");
  }
}

/*
mnote -l
  list all notes
mnote -v (count)
  open note (count) with Vim
mnote -n
  create New note
mnote -d (count)
  Delete note (count)
*/
int main (int argc, char *argv[], char *env[]) {
  init();
  if (argc == 1) {
    do_list();
  }
  if (argc >= 2) {
    if (strlen(argv[1]) == 2 && argv[1][0] == '-') {
      char flag = argv[1][1];
      switch (flag)
      {
      case 'l':
        do_list();
        return 0;
      case 'n':
        do_new();
        do_list();
        return 0;
      case 'h':
        do_help();
        return 0;
      default:
        break;
      }
      if (argc >= 3) {
        int selectedIndex = atoi(argv[2])-1;  //start from 0
        switch (flag)
        {
        case 'v':
          do_vim(selectedIndex);
          return 0;
        case 'p':
          do_print(selectedIndex);
          return 0;
        case 'd':
          do_delete(selectedIndex);
          do_list();
          return 0;
        default:
          break;
        }
      }
    }
  }
}
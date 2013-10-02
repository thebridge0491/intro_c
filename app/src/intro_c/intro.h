#ifndef INTRO_H
#define INTRO_H

#ifdef __cplusplus
extern "C" {
#endif

void greeting(const char *greet_path, const char *name, char **greet_buf);

char delay_char(float microsecs);

#ifdef __cplusplus
}
#endif

#endif // INTRO_H

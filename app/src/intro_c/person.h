#ifndef PERSON_H
#define PERSON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct person *PERSON;
PERSON construct_person(const char *name, const int age);
void destruct_person(PERSON person1);

const char* person_getName(PERSON person1);
int person_getAge(PERSON person1);

void person_setName(PERSON person1, const char *new_name);
void person_setAge(PERSON person1, const int new_age);

void person_toString(PERSON person1, const int len_buf, char *buf);

#ifdef __cplusplus
}
#endif

#endif // PERSON_H

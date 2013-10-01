#include <stdio.h>
#include <stdlib.h>
#include <log4c.h>

#include "intro_c/person.h"

static log4c_category_t *prac1;

struct person {char *name; int age;
};

PERSON construct_person(const char *name, const int age) {
    PERSON person1 = malloc(sizeof(struct person));
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_DEBUG, "construct_person");
    if (NULL == person1) {
        perror("malloc person");
        return NULL;
    }
    person1->name = "ToDo";
    person1->age = 0;
    return person1;
}

void destruct_person(PERSON person1) {
	log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "destruct_person");
    free(person1);
    person1 = NULL;
}

const char* person_getName(PERSON person1) {
    return "ToDo";
}

int person_getAge(PERSON person1) {
    return 0;
}

void person_setName(PERSON person1, const char *new_name) {
    person1->name = "ToDo";
}

void person_setAge(PERSON person1, const int new_age) {
    person1->age = 0;
}

void person_toString(PERSON person1, const int len_buf, char *buf) {
    snprintf(buf, len_buf, "Person0{name = %s, age = %i}", "ToDo", 
        0);
}


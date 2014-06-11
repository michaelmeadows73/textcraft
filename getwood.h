#ifndef GETWOOD_H
#define GETWOOD_H

int getwood_execute(struct command*, struct entity*, struct map*);

struct command* getwood_create(long);

struct command* getstone_create(long);

void getwood_destroy(struct command*);

#endif

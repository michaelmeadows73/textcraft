#ifndef GETGOLD_H
#define GETGOLD_H

int getgold_execute(struct command*, struct entity*, struct map*);

struct command* getgold_create(long);

void getgold_destroy(struct command*);

#endif

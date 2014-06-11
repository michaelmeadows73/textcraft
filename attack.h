#ifndef ATTACK_H
#define ATTACK_H

int attack_execute(struct command*, struct entity*, struct map*);

struct command* attack_create(long);

void attack_destroy(struct command*);

#endif

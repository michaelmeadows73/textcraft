#ifndef PEASANT_H
#define PEASANT_H

void peasant_execute(struct entity*, struct map*);

struct entity* peasant_create(long);

void peasant_destroy(struct entity*);

#endif

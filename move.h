int move_execute(struct command*, struct entity*, struct map*);

struct command* move_create(long);

void move_destroy(struct command*);

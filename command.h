typedef int (*commandfn)();

struct command
{
	long target;
	struct list* path;
	struct command* child;
	int state;

	commandfn execute;
};

struct command* command_create();

void command_destroy(struct command*);

typedef int (*commandfn)();

struct command
{
	long target;
	struct list* path;

	commandfn execute;
};

struct command* command_create();

void command_destroy(struct command*);

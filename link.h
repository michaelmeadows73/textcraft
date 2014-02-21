struct link
{
	struct link* next;
	void* data;	
};

struct link* link_last(struct link*);

struct link* link_add(struct link*, void*);

struct link* link_clone(struct link*);

int link_contains(struct link*, void*, int());

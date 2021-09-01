#include "dataLinkNode.h"
#include "dataMemory.h"
#include "dataArg.h"

void linkNode_deinit(LinkNode *self)
{
    arg_deinit(self->content);
    // DynMemPut(self->mem);
    pikaFree(self, sizeof(LinkNode));
    self = NULL;
}

void linkNode_init(LinkNode *self, void *args)
{
    /* attribute */
}

LinkNode *New_linkNode(void *args)
{
    LinkNode *self = pikaMalloc(sizeof(LinkNode));
    linkNode_init(self, args);
    return self;
}

#include "mimiEvent.h"
#include "SysObj.h"

void handle1(MimiObj *obj)
{
    char *message = obj_getStr(obj, "publisher.message");
    printf("message is: %s\r\n", message);
}

int32_t TEST_Event(int32_t isShow)
{
    // {
    //     MimiObj *root = newRootObj("root", New_MimiObj_Event);
    //     sysObj_importAndSetObj(root, "publisher", New_MimiObj_Event);
    //     obj_setStr(root, "publisher.message", "hello");
    //     obj_setPtr(root, "handle", handle1);

    //     obj_run(root, "follow(argPath = 'publisher.message', handle = handle)");
    //     obj_run(root, "publisher.publish(argPath = 'message')");

    //     obj_deinit(root);
    // }
    return 0;
}

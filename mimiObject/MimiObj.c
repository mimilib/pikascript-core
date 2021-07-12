#include "MimiObj.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"

static int dinitEachSubObj(Arg *argEach, Args *handleArgs)
{
    if (NULL != handleArgs)
    {
        /* error: tOhis handle not need handle args */
        return 1;
    }
    if (mimiStrEqu(argEach->typeDynMem->addr, "_class-process"))
    {
        MimiObj *subObj = arg_getPtr(argEach);
        subObj->deinit(subObj);
    }
    return 0;
}

static void dinitAllSubObj(MimiObj *self)
{
    Args *args = self->attributeList;
    args->foreach (args, dinitEachSubObj, NULL);
}

static void deinit(MimiObj *self)
{
    self->_beforDinit(self);
    dinitAllSubObj(self);
    self->attributeList->deinit(self->attributeList);
    DynMemPut(self->mem);
}

static void update(MimiObj *self)
{
    // return if is not enable
    if (0 == self->getInt(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
static void RootUpdateHandle(MimiObj *self)
{
    // override the handle function here
}

static void enable(MimiObj *self)
{
    self->setInt(self, "isEnable", 1);
}

static void disable(MimiObj *self)
{
    self->setInt(self, "isEnable", 0);
}

static void setInt64(MimiObj *self, char *argDir, long long val)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    obj->attributeList->setInt(obj->attributeList,
                               name, val);
}

static void setPointer(MimiObj *self, char *argDir, void *pointer)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    obj->attributeList->setPtr(obj->attributeList,
                               name, pointer);
}

static void setFloat(MimiObj *self, char *argDir, float value)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    obj->attributeList->setFloat(obj->attributeList,
                                 name, value);
}

static void setStr(MimiObj *self, char *argDir, char *str)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    obj->attributeList->setStr(obj->attributeList,
                               name, str);
}

static long long getInt64(MimiObj *self, char *argDir)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return obj->attributeList->getInt(obj->attributeList,
                                      argName);
}

static void *getPointer(MimiObj *self, char *argDir)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return obj->attributeList->getPtr(obj->attributeList,
                                      argName);
}

static float getFloat(MimiObj *self, char *argDir)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return obj->attributeList->getFloat(obj->attributeList,
                                        argName);
}

char *getStr(MimiObj *self, char *argDir)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return obj->attributeList->getStr(obj->attributeList,
                                      argName);
}

static void load(MimiObj *self, Args *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void _beforDinit(MimiObj *self)
{
    /* override in user code */
}

static void setObj(MimiObj *self, char *subProcessName, void *newFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char nameBuff[64] = {0};
    strAppend(nameBuff, prifix);
    strAppend(nameBuff, subProcessName);
    self->setPtr(self, nameBuff, newFun);
}

static void addOther(MimiObj *self, char *subObjectName, void *new_ObjectFun)
{
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    void *(*new_Object)(Args * initArgs) = (void *(*)(Args *))new_ObjectFun;
    void *subObject = new_Object(initArgs);
    self->setPtr(self, subObjectName, subObject);
    initArgs->deinit(initArgs);
}

static void freeObj(MimiObj *self, char *subProcessName)
{
    MimiObj *subProcess = self->getPtr(self, subProcessName);
    subProcess->deinit(subProcess);
}

static void bind(MimiObj *self, char *type, char *name, void *pointer)
{
    self->attributeList->bind(self->attributeList, type, name, pointer);
}

static char *print(MimiObj *self, char *name)
{
    return self->attributeList->print(self->attributeList, name);
}

static void bindInt(MimiObj *self, char *name, int *valPtr)
{
    self->attributeList->bindInt(self->attributeList, name, valPtr);
}

static void bindFloat(MimiObj *self, char *name, float *valPtr)
{
    self->attributeList->bindFloat(self->attributeList, name, valPtr);
}

static void bindString(MimiObj *self, char *name, char **valPtr)
{
    self->attributeList->bindStr(self->attributeList, name, valPtr);
}

static int set(MimiObj *self, char *argDir, char *valStr)
{
    MimiObj *obj = self->getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return obj->attributeList->set(obj->attributeList, argName, valStr);
}

static MimiObj *initObj(MimiObj *self, char *name)
{
    char prifix[] = "[cls]";
    char initFunName[64] = {0};
    strAppend(initFunName, prifix);
    strAppend(initFunName, name);
    if (!self->attributeList->isArgExist(self->attributeList,
                                         initFunName))
    {
        /* no init fun, could not found subprocess */
        return NULL;
    }
    /* init the subprocess */
    Args *attributeList = self->attributeList;
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    initArgs->setStr(initArgs, "name", name);
    void *(*newObjFun)(Args * initArgs) = (void *(*)(Args * initArgs)) self->attributeList->getPtr(self->attributeList,
                                                                                                   initFunName);
    MimiObj *newObj = newObjFun(initArgs);
    attributeList->setPtrWithType(attributeList, name, "process", newObj);
    initArgs->deinit(initArgs);
    return self->getPtr(self,
                        name);
}

static MimiObj *getDirectObj(MimiObj *self, char *name)
{
    /* check subprocess */
    if (!self->attributeList->isArgExist(self->attributeList,
                                         name))
    {
        /* no inited subprocess, check subprocess init fun*/
        return initObj(self, name);
    }

    /* finded subscribe, check type*/
    char *type = self->attributeList->getType(self->attributeList,
                                              name);
    if (!mimiStrEqu("_class-process", type))
    {
        /* type error, could not found subprocess */
        return NULL;
    }
    return self->getPtr(self, name);
}

static MimiObj *getObj(MimiObj *self, char *processDirectory, int keepToken)
{
    MimiObj *obj = self;
    // sign in the argv memory
    char *token[16] = {0};
    DMEM *tokenMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        tokenMem[i] = DynMemGet(sizeof(char) * 256);
        token[i] = (char *)tokenMem[i]->addr;
        token[i][0] = 0;
    }
    int processArgc = getToken(processDirectory, token, '.');
    for (int i = 0; i < processArgc - keepToken; i++)
    {
        obj = obj->getDirectObj(obj, token[i]);
        if (obj == NULL)
        {
            goto exit;
        }
    }
    goto exit;
exit:
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(tokenMem[i]);
    }
    return obj;
}


static void setMethod(MimiObj *self,
                      char *declearation,
                      void (*methodPtr)(MimiObj *self, Args *args))
{
    char buff[5][128] = {0};
    int i = 0;
    char *cleanDeclearation = strDeleteChar(buff[i++], declearation, ' ');
    char *methodDir = getFirstToken(buff[i++], cleanDeclearation, '(');
    char *returnType = getLastToken(buff[i++], cleanDeclearation, ':');
    char *typeList = strCut(buff[i++], cleanDeclearation, '(', ')');
    if (typeList == NULL || methodDir == NULL)
    {
        printf("[error]: method declearation error!\r\n");
        printf("[info]: declearation: %s\r\n", declearation);
        while (1)
        {
            /* code */
        }
    }

    MimiObj *methodHost = self->getObj(self, methodDir, 1);
    char *methodName = getLastToken(buff[i++], methodDir, '.');
    methodHost->setObj(methodHost, methodName, New_MimiObj);
    MimiObj *methodObj = self->getObj(self, methodDir, 0);

    methodObj->setStr(methodObj, "typeList", typeList);
    methodObj->setStr(methodObj, "returnType", returnType);
    methodObj->setPtr(methodObj, "methodPtr", methodPtr);
}

Args *getArgsBySentence(MimiObj *self, char *typeList, char *argList)
{
    Args *args = New_args(NULL);
    char typeListBuff[128] = {0};
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    while (1)
    {
        char buff[4][128] = {0};
        int i = 0;
        char *type = popToken(buff[i++], typeListBuff, ',');
        char *defineName = getFirstToken(buff[i++], type, ':');
        char *defineType = getLastToken(buff[i++], type, ':');

        char *argListBuff = buff[i++];
        memcpy(argListBuff, argList, strGetSize(argList));
        while (1)
        {

            char buff[4][64] = {0};
            int i = 0;
            char *arg = popToken(buff[i++], argListBuff, ',');
            char *argName = getFirstToken(buff[i++], arg, '=');
            char *argContant = getLastToken(buff[i++], arg, '=');

            if (0 == arg[0])
            {
                /* arg poped finised */
                break;
            }

            if (!mimiStrEqu(defineName, argName))
            {
                /* name not match */
                continue;
            }

            if (mimiStrEqu(defineType, "string"))
            {
                /* solve the string type */
                char *directStr = strCut(buff[i++], argContant, '"', '"');
                if (NULL != directStr)
                {
                    /* direct value */
                    args->setStr(args, defineName, directStr);
                    continue;
                }
                /* reference value */
                args->setStr(args, defineName, self->getStr(self, argContant));
                continue;
            }
            if (mimiStrEqu(defineType, "int"))
            {
                /* solve the int type */
                args->setInt(args, defineName, 0);
                if ((argContant[0] >= '0') && (argContant[0] <= '9'))
                {
                    /* direct value */
                    args->set(args, defineName, argContant);
                    continue;
                }
                /* reference value */
                int referenceVal = self->getInt(self, argContant);
                args->setInt(args, defineName, referenceVal);
                continue;
            }
            if (mimiStrEqu(defineType, "float"))
            {
                /* solve the float type */
                args->setFloat(args, defineName, 0);
                if ((argContant[0] >= '0') && (argContant[0] <= '9'))
                {
                    /* direct value */
                    args->set(args, defineName, argContant);
                    continue;
                }
                /* reference value */
                float referenceVal = self->getFloat(self, argContant);
                args->setFloat(args, defineName, referenceVal);
                continue;
            }
            if (mimiStrEqu(defineType, "pointer"))
            {
                /* only support reference value */
                void *ptr = self->getPtr(self, argContant);
                args->setPtr(args, defineName, ptr);
                continue;
            }

            /* type match faild */
            printf("[error]: type not match, input type: %s\r\n", defineType);
            while (1)
            {
            }
        }

        /* poped all type from typeList */
        if (0 == typeListBuff[0])
        {
            break;
        }
    }

    return args;
}

static void run(MimiObj *self, char *cmd)
{
    char buff[5][128] = {0};
    int i = 0;
    char *cleanCmd = strDeleteChar(buff[i++], cmd, ' ');
    char *methodSentence = getFirstToken(buff[i++], cleanCmd, '(');
    char *returnName = getFirstToken(buff[i++], methodSentence, '=');
    char *methodName = getLastToken(buff[i++], methodSentence, '=');

    MimiObj *methodObj = self->getObj(self, methodName, 0);
    MimiObj *methodHost = self->getObj(self, methodName, 1);

    if (NULL == methodObj)
    {
        printf("[error]: method %s no found.\r\n", methodName);
        return;
    }
    char *argList = strCut(buff[i++], cleanCmd, '(', ')');
    char *typeList = methodObj->getStr(methodObj, "typeList");
    char *returnType = methodObj->getStr(methodObj, "returnType");
    void (*methodFun)(MimiObj * self, Args * args) = methodObj->getPtr(methodObj, "methodPtr");

    Args *args = getArgsBySentence(self, typeList, argList);
    methodFun(methodHost, args);
    if (mimiStrEqu("int", returnType))
    {
        self->setInt(self, returnName, args->getInt(args, "return"));
    }
    if (mimiStrEqu("float", returnType))
    {
        self->setFloat(self, returnName, args->getFloat(args, "return"));
    }
    if (mimiStrEqu("string", returnType))
    {
        self->setStr(self, returnName, args->getStr(args, "return"));
    }
    args->deinit(args);
}

static void init(MimiObj *self, Args *args)
{
    /* List */
    self->attributeList = New_args(NULL);

    /* operation */
    self->deinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;

    self->setInt = setInt64;
    self->setPtr = setPointer;
    self->setFloat = setFloat;
    self->setStr = setStr;

    self->getInt = getInt64;
    self->getPtr = getPointer;
    self->getFloat = getFloat;
    self->getStr = getStr;

    self->bindInt = bindInt;
    self->bindFloat = bindFloat;
    self->bindString = bindString;

    // arg general operations
    self->bind = bind;
    self->print = print;
    self->set = set;

    self->load = load;
    // subObject
    self->addOther = addOther;
    self->setObj = setObj;
    self->getDirectObj = getDirectObj;
    self->getObj = getObj;
    self->freeObj = freeObj;

    /* method */
    self->setMethod = setMethod;
    self->run = run;

    /* override */
    self->_updateHandle = RootUpdateHandle;
    self->_beforDinit = _beforDinit;

    /* attrivute */
    self->setPtr(self, "context", self);
    self->setStr(self, "name", "root");

    /* load */
    if (NULL != args)
    {
        self->load(self, args, "name");
        self->load(self, args, "context");
    }
    self->name = self->getStr(self, "name");
}

MimiObj *New_MimiObj(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiObj));
    if (NULL == mem)
    {
        printf("[error]: memory is empty!");
        while (1)
        {
        }
    }
    MimiObj *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}

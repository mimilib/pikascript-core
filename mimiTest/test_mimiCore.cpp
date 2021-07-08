#include "test_mimiCore.h"
#include "mimiTest.h"
#include <iostream>
extern "C"
{
// tools
#include "dataString.h"
#include "mimiTest.h"
#include <string.h>
// cores
#include "mimiShell2.h"
// SubTest
#include "dataMemory.h"
#include "test_args2.h"
#include "test_dataLog.h"
#include "test_link2.h"
#include "test_mimiProcess.h"
// Device
#include "test_mimiCom.h"
}
extern DMEM_STATE DMEMS;
extern int error_num;
extern int memory_warnning_num;

void test_mimiCore(int isShow)
{
    ASSERT_RETURN(TEST_datalog, isShow);
    // ASSERT_RETURN(TEST_dataQueue, 1);
    // ASSERT_RETURN(TEST_VM_device, isShow);
    ASSERT_RETURN(TEST_link2, 1);
    ASSERT_RETURN(TEST_args2, 1);
    ASSERT_RETURN(TEST_mimiProcess, isShow);
    ASSERT_RETURN(TEST_mimiCom, isShow);
    std::cout << "Test of mimiCore Finished! " << std::endl
              << std::endl
              << "Memory still used: " << DMEMS.blk_num << std::endl
              << "There is " << error_num << " error(s)" << std::endl
              << "There is " << memory_warnning_num << " memory warnning(s)" << std::endl
              << "================================" << std::endl;
}

#include <winutils/utils_system.h>

#include <floral/stdaliases.h>

#include <clover/logger.h>

#include <Windows.h>

int main(void)
{
    winutils::utils_system::initialize();
    while(true)
    {
        c8 str0[1024];
        c8 str1[1024];
		winutils::utils_system::update(str0, 1024, str1, 1024);
        CLOVER_INFO(str0);
        CLOVER_INFO(str1);
        //Sleep(1000);
    }
    winutils::utils_system::clean_up();
    return 0;
}

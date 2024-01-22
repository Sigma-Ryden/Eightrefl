#include <RewTestingBase.hpp>

struct FSomeDataBase
{
    void* i;
};

REFLECTABLE_DECLARATION(FSomeDataBase)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(FSomeDataBase)
REFLECTABLE_INIT()

int main()
{
    TRY_CATCH(EXECUTE_ALL());
    TESTING_STAT();
    return 0;
}

#include <RewTestingBase.hpp>

struct FSomeDataBase
{
    void* i;
};

REFLECTABLE_DECLARATION(FSomeDataBase)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(FSomeDataBase)
REFLECTABLE_INIT()

struct my_visitor_t : rew::visitor_t
{
};

REFLECTABLE_VISITOR_DECLARATION(0, my_visitor_t)
REFLECTABLE_DECLARATION_INIT()

int main()
{
    TRY_CATCH(EXECUTE_ALL());
    TESTING_STAT();
    return 0;
}

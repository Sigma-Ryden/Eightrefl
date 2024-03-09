#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/optional.hpp>

int main()
{
    rew::reflectable<std::optional<int>>();
    TRY_CATCH(EXECUTE_ALL());
    TESTING_STAT();
    return 0;
}

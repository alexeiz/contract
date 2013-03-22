#include <contract.h>

#include <cstddef>
#include <cstring>

std::size_t my_strlen(char const * str)
{
    precondition(str, "invalid argument");

    std::size_t len = 0;
    for (char const * p = str; *p; ++len, ++p)
        ;

    postcondition(len == std::strlen(str), "incorrect return value");
    return len;
}

int main()
{
    my_strlen("a string");
}

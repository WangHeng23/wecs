#include "wecs/core/type_list.hpp"

using namespace wecs;

using list = type_list<int, double, float>;
using t1 = list_element_t<list, 0>;
using t2 = list_element_t<list, 1>;
using t3 = list_element_t<list, 2>;

int main() {

    static_assert(std::is_same_v<t1, int>);
    static_assert(std::is_same_v<t2, double>);
    static_assert(std::is_same_v<t3, float>);

    return 0;
}
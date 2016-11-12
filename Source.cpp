#include"List.h"
#include"PureFunctions.h"
#include <complex>

int main()
{
    // In order to use the pure functions, prefix their names with the "pure::" specifier.
    // The pure functions are: length, reverse, map, filter, foldr, sum, product & append.
    using andi::list;
    list<int> lst = 1 |= 2 |= 3 |= _<int>(); // readability++

    std::cout << lst << "\n" << andi::reverse(lst) << "\n";
    list<int> l1 = andi::map([](int x) { return x * x; }, lst); // lambda
    std::cout << l1 << "\n";
    list<int> l2 = andi::filter([](int x) { return (x & 1) == 1; }, lst);
    std::cout << l2 << "\n";
    std::cout << andi::append(l1, l2) << "\n";

    std::cout << _<float>() << "\n\n";

    std::cout << " length of " << lst << " = " << andi::length(lst) << "\n";
    std::cout << "    sum of " << lst << " = " << andi::sum(lst) << "\n";
    std::cout << "product of " << lst << " = " << andi::product(lst) << "\n\n";

    // списък от списъци, няма проблем - за създаването му се правят
    // само три алокации на памет (а не 9, колкото числа "съдържа" той) !
    list<list<int>> ll = lst |= lst |= lst |= _<decltype(lst)>(); // още лоши практики...
    std::cout << ll << "\n\n";

    // списък с комплексни числа и операции над тях, why not
    using namespace std::complex_literals;
    list<std::complex<double>> lc = (1.2 + 3.4i) |= (0.6 - 2.8i) |= _<std::complex<double>>();
    std::cout << "sum of " << lc << " = " << andi::sum(lc) << "\n";
}

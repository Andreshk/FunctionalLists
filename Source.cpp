#include"List.h"
#include"PureFunctions.h"
#include <complex>

std::ostream& operator<<(std::ostream& os, andi::nil_t) { return os << "()"; }

int main()
{
    // In order to use the pure functions, prefix their names with the "pure::" specifier.
    // The pure functions are: length, reverse, map, filter, foldr, sum, product & append.
    using namespace andi;
    list<int> lst = 1 |= 2 |= 3 |= nil;

    std::cout << lst << "\n" << reverse(lst) << "\n";
    list<int> l1 = map([](int x) { return x * x; }, lst);
    std::cout << l1 << "\n";
    list<int> l2 = filter([](int x) { return (x & 1) == 1; }, lst);
    std::cout << l2 << "\n";
    std::cout << append(l1, l2) << "\n";

    //std::cout << " length of " << lst << " = " << foldr([](int _, int y) {return y + 1; }, 0, lst) << "\n";
    std::cout << " length of " << lst << " = " << length(lst) << "\n";
    std::cout << "    sum of " << lst << " = " << sum(lst) << "\n";
    std::cout << "product of " << lst << " = " << product(lst) << "\n\n";

    std::cout << nil << "\n" << cons(3, nil) << "\n\n";
    
    // A list of list: just three allocation are being
    // made, despite it "containing" nine integers (!)
    list<list<int>> ll = lst |= lst |= lst |= nil;
    std::cout << ll << "\n\n";

    // A list of complex numbers & operations on them, because why not
    using namespace std::complex_literals;
    list<std::complex<double>> lc = (1.2 + 3.4i) |= (0.6 - 2.8i) |= nil;
    std::cout << "sum of " << lc << " = " << sum(lc) << "\n";
}

#include <iostream>
#include <memory>
#include <functional>
#include <type_traits>
#include <complex>
// requires c++14

namespace andi
{
	template<class T>
	class list
	{
		struct node;
		using pointer_t = std::shared_ptr<const node>;

		struct node
		{
			const T value; // само това ни интересува да е const, всичко останало може да не е
			pointer_t next;
			node(const T& _value, const pointer_t& _next) : value(_value), next(_next) {}
		};

		// only data member you need
		pointer_t head;

		list(const pointer_t& _ptr) : head(_ptr) {}
		list(const T& val, list<T> lst) : list(std::make_shared<node>(val, lst.head)) {};
	public:
		list() : head(nullptr) {};

		friend list cons(const T& val, list<T> lst)
		{
			return{ val,lst }; // само извикването на cons води до алокация на памет за една "кутийка"
		}

		friend const T& head(list<T> lst)
		{
			return lst.head->value;
		}

		friend list tail(list<T> lst)
		{
			return{ lst.head->next };
		}

		friend bool null(list<T> lst)
		{
			return !lst.head;
		}
	};

	// for code cleanliness...
	template<class T>
	list<T> operator|=(const T& val, list<T> lst)
	{
		return cons(val, lst);
	}

	// standard functions: length, reverse, map, filter, foldr, append,...
	template<class T>
	size_t length(list<T> lst)
	{
		// нищо не допринася така за четимостта на кода, както тернарния оператор ?:
		return null(lst) ? 0 : 1 + length(tail(lst));
	}

	template<class T>
	list<T> reverse_helper(list<T> lst, list<T> res)
	{
		return null(lst) ? res : reverse_helper(tail(lst), cons(head(lst), res));
	}

	template<class T>
	list<T> reverse(list<T> lst)
	{
		return reverse_helper(lst, list<T>());
	}
	
	template<class UnaryFunction, class T>
	list<std::result_of_t<UnaryFunction(T)>> map(UnaryFunction f, list<T> lst)
	{
		using U = std::result_of_t<UnaryFunction(T)>;
		return null(lst) ? list<U>() : cons(f(head(lst)), map(f, tail(lst)));
	}

	template<class UnaryPredicate, class T>
	list<T> filter(UnaryPredicate p, list<T> lst)
	{
		if (null(lst))
			return list<T>();
		if (p(head(lst)))
			return cons(head(lst), filter(p, tail(lst)));
		return filter(p, tail(lst));
	}

	template<class BinaryFunction, class T, class U>
	std::result_of_t<BinaryFunction(T, U)> foldr(BinaryFunction op, U nv, list<T> lst)
	{
		return null(lst) ? nv : op(head(lst), foldr(op, nv, tail(lst)));
	}

	template<class T>
	T sum(list<T> lst)
	{
		return foldr(std::plus<T>(), T(0), lst);
	}

	template<class T>
	T product(list<T> lst)
	{
		return foldr(std::multiplies<T>(), T(1), lst);
	}

	template<class T>
	list<T> append(list<T> lst1, list<T> lst2)
	{
		return null(lst1) ? lst2 : cons(head(lst1), append(tail(lst1), lst2));
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, list<T> lst)
	{
		// more ternary magic
		//return null(lst) ? os << "\n" : (os << head(lst) << " " << tail(lst));

		// pretty print
		if (null(lst))
			return os << "()";
		os << "(" << head(lst);
		for (auto l = tail(lst); !null(l); l = tail(l))
			os << ", " << head(l);
		return os << ")";
	}
}

template<class T>
andi::list<T> _() { return{}; } // lol

int main()
{
	auto lst = 1 |= 2 |= 3 |= _<int>(); // readability++

	std::cout << lst << "\n" << andi::reverse(lst) << "\n";
	auto l1 = andi::map([](int x) { return x * x; }, lst); // lambda
	std::cout << l1 << "\n";
	auto l2 = andi::filter([](int x) { return (x & 1) == 1; }, lst);
	std::cout << l2 << "\n";
	std::cout << append(l1, l2) << "\n";

	std::cout << _<float>() << "\n\n";

	std::cout << " length of " << lst << " = " << andi::length(lst) << "\n";
	std::cout << "    sum of " << lst << " = " << andi::sum(lst) << "\n";
	std::cout << "product of " << lst << " = " << andi::product(lst) << "\n\n";

	// списък от списъци, няма проблем - за създаването му се правят
	// само три алокации на памет (а не 9, колкото числа "съдържа" той) !
	auto ll = lst |= lst |= lst |= _<decltype(lst)>(); // още лоши практики...
	std::cout << ll << "\n\n";

	// списък с комплексни числа и операции над тях, why not
	using namespace std::complex_literals;
	andi::list<std::complex<double>> lc = (1.2 + 3.4i) |= (0.6 - 2.8i) |= _<std::complex<double>>();
	std::cout << "sum of " << lc << " = " << andi::sum(lc) << "\n";
}

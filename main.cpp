#include <tuple>
#include <cassert>
#include <iostream>

template <unsigned int N>
struct tee_stream
{
   template <typename ...Args, typename T>
   static std::tuple<Args...>& print(std::tuple<Args...>& t, T&& x)
   {
      std::get<sizeof...(Args) - N>(t) << x;
      tee_stream<N - 1>::print(t, std::forward<T>(x));
      return t;
   }

   template <typename ...Args, typename T>
   static std::tuple<Args...>& place_endl(std::tuple<Args...>& t, T&& x)
   {
      x(std::get<sizeof...(Args) - N>(t));
      tee_stream<N - 1>::place_endl(t, std::forward<T>(x));
      return t;
   }
};
template <>
struct tee_stream<0>
{
   template <typename ...Args, typename T>
   static std::tuple<Args...>& print(std::tuple<Args...>& t, T&& x)
   {
      return t;
   }

   template <typename ...Args, typename T>
   static std::tuple<Args...>& place_endl(std::tuple<Args...>& t, T&& x)
   {
      return t;
   }
};

template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>& t, T&& x)
{
   return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}
template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>&& t, T&& x)
{
   return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}

using CoutType = std::basic_ostream<char, std::char_traits<char> >;
using EndlType = CoutType &(*)(CoutType &);

template <typename ...Args>
std::tuple<Args...>& operator<< (std::tuple<Args...>& t, EndlType&& x)
{
   return tee_stream<sizeof...(Args)>::place_endl(t, std::forward<EndlType>(x));
}
template <typename ...Args>
std::tuple<Args...>& operator<< (std::tuple<Args...>&& t, EndlType&& x)
{
   return tee_stream<sizeof...(Args)>::place_endl(t, std::forward<EndlType>(x));
}




class A {
};

template <typename T>
A &operator<<(A &a, const T &t)
{
   std::cout << t;
   return a;
}

using CoutType = std::basic_ostream<char, std::char_traits<char> >;

using MyEndl = CoutType &(*)(CoutType &);

A &operator<<(A &a, MyEndl func)
{
   func(std::cout);
   return a;
}


int main()
{
   std::tuple<std::ostream&, std::ostream&>(std::cout, std::cerr) << "hi " << "there" << std::endl;
   return 0;
}

#include <iostream>

#include "cord.hpp"
#include "cord_utilities.hpp"

int main() {
  // // leaf node
  // SharedPointer<Cord> a = new Cord("CS_");
  // SharedPointer<Cord> b = new Cord("128_");
  // // parent node
  // SharedPointer<Cord> c = ConcatCords(a, b);

  // auto c_str = c->ToString();
  // std::cout << c_str.Get() << std::endl;

  // auto res = SubString(c, 0, 3);
  // auto res_str = res->ToString();
  // std::cout << res_str.Get() << std::endl;

  SharedPointer<Cord> d = new Cord("he");
  SharedPointer<Cord> e = new Cord("llo");
  SharedPointer<Cord> f = ConcatCords(d, e);
  SharedPointer<Cord> g = new Cord("wor");
  SharedPointer<Cord> h = new Cord("ld");
  SharedPointer<Cord> i = ConcatCords(g, h);
  SharedPointer<Cord> j = ConcatCords(f, i);

  // auto resi = SubString(j, 0, 3);
  // auto res_stri = resi->ToString();
  // std::cout << res_stri.Get() << std::endl;

  // 3-9, 0-6
  SharedPointer<Cord> resit = SubString(j, 0, 10);
  SharedPointer<char> res_strit = resit->ToString();
  std::cout << res_strit.Get() << std::endl;

  // create sharedpointer with character array, use copy constructor, copy
  // assignemnts on it
}
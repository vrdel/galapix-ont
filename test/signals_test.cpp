/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "util/weak_functor.hpp"

class Foobar
{
public:
  void callme(int n)
  {
    std::cout << "Foobar::callme(" << n << ")" << std::endl;
  }

  void call()
  {
 std::cout << "Foobar::call()" << std::endl;
  }
};

int main()
{
  boost::shared_ptr<Foobar> foobar(new Foobar);

  boost::function<void (int)> callback  = weak(boost::bind(&Foobar::callme, _1, _2), foobar);
  boost::function<void ()> callback2 = weak(boost::bind(&Foobar::call, _1), foobar);

  callback(1323);

  //foobar.reset();
  //callback(1323);
  callback2();
  
  return 0;
}

/* EOF */

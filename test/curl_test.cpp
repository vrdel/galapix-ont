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

// g++ -Wall -Werror -ansi -pedantic curl.cpp blob.cpp -o mycurl -D__CURL_TEST__  `pkg-config --libs --cflags libcurl`

#include <iostream>

#include "plugins/curl.hpp"

int main(int argc, char** argv)
{
  try 
    {
      /*
      if (curl_global_init(CURL_GLOBAL_ALL) != 0)
        {
          throw std::runtime_error("Couldn't init cURL");
        }
      */

      for(int i = 1; i < argc; ++i)
        {
          std::cout << "Trying to get: " << argv[i] << std::endl;
          std::string mime_type;
          CURLHandler::get_data(argv[i], &mime_type)->write_to_file("/tmp/out");
          std::cout << "Got: " << argv[i] << " -> '" << mime_type << "'" << std::endl;
        }
  
      //curl_global_cleanup(); 
    }
  catch(std::exception& err)
    {
      std::cout << "Error: " << err.what() << std::endl;
    }

  return 0;
}

/* EOF */


// 
// sketchbook/libraries/Catch/single_include/catch.hpp
// ../../Catch/single_include/catch.hpp


// g++ -Wall -Wextra -Weffc++ -std=c++11 -I.. -o example1.exe example1.cpp && example1
// g++ -std=c++11 -Wall -Wextra -I.. -c ../FifoMessageBuffer.cpp
// g++ -std=c++11 -Wall -Wextra -I.. -I../../Catch/single_include -c test1.cpp
// g++ -o test1 test1.o FifoMessageBuffer.o
// g++ -g -std=c++11 -Wall -Wextra -I.. -I../../Catch/single_include test1.cpp ../FifoMessageBuffer.cpp -o test1
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "FifoMessageBuffer.h"


TEST_CASE( "Default buffer", "[all]" ) {
  FifoMessageBuffer fmb;

  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 0 );
  REQUIRE( fmb.available() == 127 );

  fmb.add('a');

  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 1 );
  REQUIRE( fmb.available() == 126 );

  fmb.add('b');

  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 2 );
  REQUIRE( fmb.available() == 125 );

  fmb.add('c');

  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 3 );
  REQUIRE( fmb.available() == 124 );

  char def[] = "def";

  fmb.add(def);

  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 6 );
  REQUIRE( fmb.available() == 121 );

  char msg[512];
  short pop_length = fmb.pop(msg,sizeof(msg));

  REQUIRE( pop_length == 6 );
  REQUIRE( fmb.bufSize() == 127 );
  REQUIRE( fmb.length() == 0 );
  REQUIRE( fmb.available() == 127 );
}


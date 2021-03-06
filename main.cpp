//
//  main.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "test_bimap.hpp"
#include "test_unimap.hpp"
#include "test_refmap.hpp"

int main(int argc, const char* argv[])
{
    testUniMap();
    testRefMap();
    testBiMap1();
    testBiMap2();
    testBiMap3();
    return 0;
}

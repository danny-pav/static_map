//
//  test_unimap.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "test_unimap.hpp"

#include <iostream>

#include "unimap.hpp"

typedef static_map::UniMap<int, int, std::less<int>> IIMap;
typedef IIMap::Item II;
typedef IIMap::Builder IIBuilder;

static IIBuilder ib;
static II i1(ib, 1, 5);
static II i2(ib, 2, 4);
static II i3(ib, 0, 0);
static II i4(ib, 4, 2);

static IIMap im(ib);

static void ii1FindIt(int i)
{
    std::cout << "find " << i;
    const II* p = im.findKey(i);
    std::cout << (p ? " found" : " not found");
    if (p)
    {
        std::cout << "(k=" << p->key() << " v=" << p->val() << ")";
    }
    std::cout << std::endl;
}

template <typename TIterParam>
static void ii1Traverse(TIterParam itBegin, TIterParam itEnd)
{
    std::cout << "->" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "k=" << it->key() << ", v=" << it->val() << std::endl;
    }
    std::cout << "<-" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "k=" << itPrev->key() << ", v=" << itPrev->val() << std::endl;
    }
}

static void ii1TraverseFwd(IIMap::TSequence& seq)
{
    std::cout << "FWD";
    ii1Traverse(seq.begin(), seq.end());
}

static void ii1TraverseRev(IIMap::TSequence& seq)
{
    std::cout << "REV";
    ii1Traverse(seq.rbegin(), seq.rend());
}

void testUniMap()
{
    std::cout << "Start Test UniMap" << std::endl;

    IIMap::TSequence seq1 = im.sequence();
    ii1TraverseFwd(seq1);
    ii1TraverseRev(seq1);

    ii1FindIt(0);
    ii1FindIt(1);
    ii1FindIt(3);
    ii1FindIt(4);

    std::cout << "Stop Test" << std::endl;
}

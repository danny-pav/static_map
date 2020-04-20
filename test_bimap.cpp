//
//  test_bimap.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "test_bimap.hpp"

#include <iostream>

#include "bimap.hpp"

typedef BiMap<int, int, std::less<int>, std::less<int> > BIMap;
typedef BIMap::Item BI;

static BIMap::TBuilder bb;
static BI b1(bb, 1, 5);
static BI b2(bb, 2, 4);
static BI b3(bb, 0, 0);
static BI b4(bb, 4, 2);

static BIMap bm(bb);

static void bi1FindIt(int i)
{
    std::cout << "find " << i;
    const BI * p = bm.findKey1(i);
    std::cout << (p ? " found" : " not found");
    if (p)
    {
        std::cout << "(k1=" << p->key1() << " k2=" << p->key2() << ")";
    }
    std::cout << std::endl;
}

template<typename TIterParam>
static void bi1Traverse(TIterParam itBegin, TIterParam itEnd)
{
    std::cout << "->" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "k1=" << it->key1() << ", k2=" << it->key2() << std::endl;
    }
    std::cout << "<-" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "k1=" << itPrev->key1() << ", k2=" << itPrev->key2() << std::endl;
    }
}

static void bi1TraverseFwd(BIMap::TSequence1& seq)
{
    std::cout << "FWD1";
    bi1Traverse(seq.begin(), seq.end());
}

static void bi1TraverseRev(BIMap::TSequence1& seq)
{
    std::cout << "REV1";
    bi1Traverse(seq.rbegin(), seq.rend());
}

static void bi2FindIt(int i)
{
    std::cout << "find " << i;
    const BI * p = bm.findKey2(i);
    std::cout << (p ? " found" : " not found");
    if (p)
    {
        std::cout << "(k1=" << p->key1() << " k2=" << p->key2() << ")";
    }
    std::cout << std::endl;
}

template<typename TIterParam>
static void bi2Traverse(TIterParam itBegin, TIterParam itEnd)
{
    std::cout << "->" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "k1=" << it->key1() << ",k2=" << it->key2() << std::endl;
    }
    std::cout << "<-" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "k1=" << itPrev->key1() << ",k2=" << itPrev->key2() << std::endl;
    }
}

static void bi2TraverseFwd(BIMap::TSequence2& seq)
{
    std::cout << "FWD2";
    bi2Traverse(seq.begin(), seq.end());
}

static void bi2TraverseRev(BIMap::TSequence2& seq)
{
    std::cout << "REV2";
    bi2Traverse(seq.rbegin(), seq.rend());
}

void testBiMap()
{
    std::cout << "Start Test BiMap" << std::endl;

    BIMap::TSequence1 seq1 = bm.sequence1();
    bi1TraverseFwd(seq1);
    bi1TraverseRev(seq1);

    BIMap::TSequence2 seq2 = bm.sequence2();
    bi2TraverseFwd(seq2);
    bi2TraverseRev(seq2);

    bi1FindIt(0);
    bi1FindIt(1);
    bi1FindIt(3);
    bi1FindIt(4);

    bi2FindIt(0);
    bi2FindIt(1);
    bi2FindIt(3);
    bi2FindIt(4);

    std::cout << "Stop Test" << std::endl;
}

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

void testBiMap1()
{
    std::cout << "Start Test BiMap1" << std::endl;

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

//in header:
namespace enum_to_str
{
    typedef enum {
        RED = -3,
        GREEN = 1,
        BLUE = 14
    } Color;

    static const char * toString(Color c);
    static Color fromString(const char * c);
}

//in source:
namespace enum_to_str
{
    struct StrCmp
    {
        bool operator() (const char * s1, const char * s2) const
        {
            return (strcmp(s1, s2) < 0);
        }
    };

    typedef BiMap<Color, const char *, std::less<int>, StrCmp> EnumMap;
    typedef EnumMap::Item Entry;

    static EnumMap::Builder b;
    static Entry e1(b, Color::RED, "RED");
    static Entry e2(b, Color::BLUE, "bleu");
    static Entry e3(b, Color::GREEN, "Green");

    static EnumMap em(b);

    static const char * toString(Color c)
    {
        const Entry * e = em.findKey1(c);
        return e->key2();
    }
    static Color fromString(const char * c)
    {
        const Entry * e = em.findKey2(c);
        return e->key1();
    }
}

void testBiMap2()
{
    using namespace enum_to_str;
    Color c1 = Color::RED;
    std::cout << toString(c1) << " should be RED" << std::endl;
    
    Color c2 = fromString("RED");
    std::cout << "are equal:" << (c1 == c2) << std::endl;
}

//
//  test_refmap.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "test_refmap.hpp"

#include <iostream>

#include "refmap.hpp"

struct Foo
{
    Foo(int i, double x, double y) : m_i(i), m_x(x), m_y(y)
    {
    }
    int m_i;
    double m_x;
    double m_y;
};

struct GetFooKey
{
    static const int& key(const Foo& foo)
    {
        return foo.m_i;
    }
};

typedef Foo FData;
typedef int FKey;
typedef GetFooKey FKeyGet;

typedef static_map::RefMap<FData, FKey, FKeyGet> FooMap;
typedef FooMap::TSequence FooSequence;
typedef FooMap::Item FooItem;
typedef FooMap::Builder FooBuilder;

static Foo f1(1, 1.2, 2.4);
static Foo f2(3, 3.6, 4.8);
static Foo f3(2, 4.9, 5.1);
static Foo f4(4, 1.3, 3.5);

static FooBuilder fb;
static FooItem fi1(fb, f1);
static FooItem fi2(fb, f2);
static FooItem fi3(fb, f3);
static FooItem fi4(fb, f4);

static FooMap fm(fb);

static void fmFindIt(int i)
{
    std::cout << "find " << i;
    const Foo* p = fm.findKey(i);
    std::cout << (p ? " found" : " not found");
    if (p)
    {
        std::cout << "(x=" << p->m_x << " y=" << p->m_y << ")";
    }
    std::cout << std::endl;
}

template <typename TIterParam>
static void fmTraverse(TIterParam itBegin, TIterParam itEnd)
{
    std::cout << "->" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "x=" << it->m_x << ", y=" << it->m_y << std::endl;
    }
    std::cout << "->" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "x=" << itPrev->m_x << ", y=" << itPrev->m_y << std::endl;
    }
}

static void fmTraverseFwd(FooSequence& seq)
{
    std::cout << "FWD" << std::endl;
    fmTraverse(seq.begin(), seq.end());
}

static void fmTraverseRev(FooSequence& seq)
{
    std::cout << "REV" << std::endl;
    fmTraverse(seq.rbegin(), seq.rend());
}

void testRefMap()
{
    std::cout << "Start Test RefMap" << std::endl;

    FooSequence seq1 = fm.sequence();
    fmTraverseFwd(seq1);
    fmTraverseRev(seq1);

    fmFindIt(0);
    fmFindIt(1);
    fmFindIt(3);
    fmFindIt(4);

    std::cout << "Stop Test" << std::endl;
}

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
#include "enummap.hpp"

typedef static_map::BiMap<int, int, std::less<int>, std::less<int>> BIMap;
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
    const BI* p = bm.findKey1(i);
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
    std::cout << ">>" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "k1=" << it->key1() << " -> k2=" << it->key2() << std::endl;
    }
    std::cout << "<<" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "k1=" << itPrev->key1() << " -> k2=" << itPrev->key2() << std::endl;
    }
}

template<typename TSequence1>
static void bi1TraverseFwd(TSequence1& seq)
{
    std::cout << "FWD1";
    bi1Traverse(seq.begin(), seq.end());
}

template<typename TSequence1>
static void bi1TraverseRev(TSequence1& seq)
{
    std::cout << "REV1";
    bi1Traverse(seq.rbegin(), seq.rend());
}

static void bi2FindIt(int i)
{
    std::cout << "find " << i;
    const BI* p = bm.findKey2(i);
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
    std::cout << ">>" << std::endl;
    for (TIterParam it = itBegin; it != itEnd; ++it)
    {
        std::cout << "k1=" << it->key1() << " <- k2=" << it->key2() << std::endl;
    }
    std::cout << "<<" << std::endl;
    for (TIterParam it = itEnd; it != itBegin; --it)
    {
        TIterParam itPrev(it);
        --itPrev;
        std::cout << "k1=" << itPrev->key1() << " <- k2=" << itPrev->key2() << std::endl;
    }
}

template<typename TSequence2>
static void bi2TraverseFwd(TSequence2& seq)
{
    std::cout << "FWD2";
    bi2Traverse(seq.begin(), seq.end());
}

template<typename TSequence2>
static void bi2TraverseRev(TSequence2& seq)
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

// in header:
namespace color
{
typedef enum
{
    RED = -3,
    GREEN = 1,
    BLUE = 14
} Color;

static const char* colorToString(Color c);
static Color stringToColor(const char* c);
} // namespace color

// in source:
namespace color
{
using static_map::Enum;

static Enum<Color>::Builder b;
static Enum<Color>::Item e1(b, Color::RED, "RED");
static Enum<Color>::Item e2(b, Color::BLUE, "bleu");
static Enum<Color>::Item e3(b, Color::GREEN, "Green");

static Enum<Color>::Map em(b);

static const char* colorToString(Color c)
{
    auto p = Enum<Color>::enumToString(em, c);
    return p.second;
}
static Color stringToColor(const char* c)
{
    auto p = Enum<Color>::stringToEnum(em, c);
    return p.second;
}
} // namespace color

// in header:
namespace shape
{
typedef enum
{
    CIRCLE = 1,
    SQUARE = 2
} Shape;

static const char* shapeToString(Shape c);
static Shape stringToShape(const char* c);
} // namespace shape

// in source:
namespace shape
{
using static_map::Enum;

static Enum<Shape>::Builder b;
static Enum<Shape>::Item e1(b, Shape::SQUARE, "square");
static Enum<Shape>::Item e2(b, Shape::CIRCLE, "circle");

static Enum<Shape>::Map em(b);

static const char* shapeToString(Shape c)
{
    auto p = Enum<Shape>::enumToString(em, c);
    return p.second;
}
static Shape stringToShape(const char* c)
{
    auto p = Enum<Shape>::stringToEnum(em, c);
    return p.second;
}
} // namespace shape

void testBiMap2()
{
    {
        using namespace color;
        Color c1 = Color::RED;
        std::cout << colorToString(c1) << " should be RED" << std::endl;

        Color c2 = stringToColor("RED");
        std::cout << "are equal:" << (c1 == c2) << std::endl;
    }
    {
        using namespace shape;
        Shape c1 = Shape::CIRCLE;
        std::cout << shapeToString(c1) << " should be CIRCLE" << std::endl;

        Shape c2 = stringToShape("circle");
        std::cout << "are equal:" << (c1 == c2) << std::endl;
    }
}

// in header:
namespace currency
{
typedef enum
{
    CAD = 1,
    USD = 2
} Currency;

static const char* currencyToString(Currency c);
static Currency stringToCurrency(const char* c);
} // namespace currency

// in source:
namespace currency
{
using static_map::Enum;

static Enum<Currency>::Builder b;
static Enum<Currency>::Item e1(b, Currency::USD, "USD");
static Enum<Currency>::RightKeyItem e1a(b, Currency::USD, "usd");
static Enum<Currency>::Item e2(b, Currency::CAD, "CAD");
static Enum<Currency>::RightKeyItem e2a(b, Currency::CAD, "cad");

typedef Enum<Currency>::Map CurrencyMap;
static CurrencyMap em(b);

static const char* currencyToString(Currency c)
{
    auto p = Enum<Currency>::enumToString(em, c);
    return p.second;
}
static Currency stringToCurrency(const char* c)
{
    auto p = Enum<Currency>::stringToEnum(em, c);
    return p.second;
}
} // namespace currency

void testBiMap3()
{
    {
        using namespace currency;
        Currency c1 = Currency::USD;
        std::cout << currencyToString(c1) << " should be USD" << std::endl;

        Currency c2 = stringToCurrency("USD");
        std::cout << "are equal:" << (c1 == c2) << std::endl;
        Currency c3 = stringToCurrency("usd");
        std::cout << "are equal:" << (c1 == c3) << std::endl;
    }
    currency::CurrencyMap::TSequence1 seq1 = currency::em.sequence1();
    bi1TraverseFwd(seq1);
    bi1TraverseRev(seq1);

    currency::CurrencyMap::TSequence2 seq2 = currency::em.sequence2();
    bi2TraverseFwd(seq2);
    bi2TraverseRev(seq2);
}

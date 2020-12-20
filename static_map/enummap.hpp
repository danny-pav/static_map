//
//  enummap.hpp
//  StaticMap
//
//  Created by Daniel Pav on 4/21/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef enummap_hpp
#define enummap_hpp

#include "bimap.hpp"

namespace static_map {

template<typename TEnum>
struct Enum
{
    struct StrCmp
    {
        bool operator() (const char * s1, const char * s2) const
        {
            return (strcmp(s1, s2) < 0);
        }
    };

    typedef BiMap<TEnum, const char *, std::less<int>, StrCmp> Map;
    typedef typename Map::Item Item;
    typedef typename Map::LeftKeyItem LeftKeyItem;
    typedef typename Map::RightKeyItem RightKeyItem;
    typedef typename Map::Builder Builder;

    static std::pair<bool, const char *> enumToString(const Enum::Map& em, TEnum c)
    {
        const Item * i = em.findKey1(c);
        const bool first = (i != nullptr);
        const char * second = first ? i->key2() : nullptr;
        return std::make_pair(first, second);
    }
    static std::pair<bool, TEnum> stringToEnum(const Enum::Map& em, const char * c)
    {
        const Item * i = em.findKey2(c);
        const bool first = (i != nullptr);
        const TEnum second = first ? i->key1() : (TEnum) 0;
        return std::make_pair(first, second);
    }
};

}

#endif /* enummap_hpp */

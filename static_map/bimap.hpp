//
//  bimap.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef bimap_hpp
#define bimap_hpp

#include "itemtree.hpp"
#include "builderbase.hpp"
#include "sequence.hpp"

#include <functional>

namespace static_map {

//
//
//

template<
typename TKey1,
typename TKey2,
typename TKey1Sort = std::less<TKey1>,
typename TKey2Sort = std::less<TKey2>
>
class BiMap
{
public:
    class Builder;
    class Item;
    typedef BiMap<TKey1, TKey2, TKey1Sort, TKey2Sort> ThisType;
    typedef Builder TBuilder;
    typedef Item TData;
    
private:
    class GetKey1;
    class GetKey2;
    typedef StructItemT<TData> TStructItem;
    typedef GetKey1 TKey1Get;
    typedef GetKey2 TKey2Get;
    typedef TreeFuncs<TData, TKey1, TKey1Get, TKey1Sort> Tree1Util;
    typedef TreeFuncs<TData, TKey2, TKey2Get, TKey2Sort> Tree2Util;
    
public:
    typedef Sequence<TData, TKey1, TKey1Get, TKey1Sort> TSequence1;
    typedef Sequence<TData, TKey2, TKey2Get, TKey2Sort> TSequence2;
    
public:
    class Builder : public BiBuilderBase
    {
    private:
        typedef BiBuilderBase Base;
        
    public:
        Builder() : Base() { }
        ~Builder() = default;
    };
    
public:
    class Item
    {
    public:
        typedef Item ThisType;
        typedef Builder TBuilder;
        
    public:
        template<typename TKey1Param, typename TKey2Param>
        Item(TBuilder& builder, TKey1Param kp1, TKey2Param kp2)
        : m_item1(builder.getUnsortedArray1(), *this),
        m_item2(builder.getUnsortedArray2(), *this),
        m_key1(kp1),
        m_key2(kp2)
        {
        }
        ~Item() = default;
        
    private:
        Item(const Item&) = delete;
        Item& operator=(const Item&) = delete;
        
    public:
        const TKey1& key1() const { return m_key1; }
        const TKey2& key2() const { return m_key2; }
        
    private:
        typedef StructItemT<ThisType> TStructItem;
        
    private:
        TStructItem m_item1;
        TStructItem m_item2;
        TKey1 m_key1;
        TKey2 m_key2;
    };
    
private:
    class GetKey1
    {
    public:
        static const TKey1& key(const TData& item)
        {
            return item.key1();
        }
    };
    class GetKey2
    {
    public:
        static const TKey2& key(const TData& item)
        {
            return item.key2();
        }
    };
    
public:
    BiMap(TBuilder& builder)
    : m_tree1(),
    m_tree2()
    {
        ItemArray& array1 = builder.getUnsortedArray1();
        Tree1Util::sortInPlace(array1);
        m_tree1.constructFrom(array1);
        ItemArray& array2 = builder.getUnsortedArray2();
        Tree2Util::sortInPlace(array2);
        m_tree2.constructFrom(array2);
    }
    ~BiMap() = default;
    
private:
    BiMap(const BiMap&) = delete;
    BiMap& operator=(const BiMap&) = delete;
    
public:
    const TData * findKey1(const TKey1& key) const
    {
        const TStructItem * item =
            static_cast<const TStructItem *>(Tree1Util::findInTree(m_tree1, key));
        return item ? &(item->data()) : nullptr;
    }
    
    const TData * findKey2(const TKey2& key) const
    {
        const TStructItem * item =
            static_cast<const TStructItem *>(Tree2Util::findInTree(m_tree2, key));
        return item ? &(item->data()) : nullptr;
    }
    
public:
    TSequence1 sequence1() const
    {
        TSequence1 seq;
        seq.makeSequence(m_tree1);
        return seq;
    }
    TSequence2 sequence2() const
    {
        TSequence2 seq;
        seq.makeSequence(m_tree2);
        return seq;
    }
    
private:
    ItemTree m_tree1;
    ItemTree m_tree2;
};

}

#endif /* bimap_hpp */

//
//  unimap.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef unimap_hpp
#define unimap_hpp

#include "builderbase.hpp"
#include "itemtree.hpp"
#include "sequence.hpp"

namespace static_map
{
//
// These classes are meant to be used as parts to make static const maps
// with no memory allocation.  Here's how they are meant to be used:
//
// typedef int Enum;
// typedef const char * Name;
// typedef UniMap<Enum, Name> EnumMap
// typedef EnumMap::Item EnumItem;
// typedef EnumMap::Builder EnumBuilder;
//
// static EnumBuilder s_builder;
//
// static const char * s_half = "HALF";
// static EnumItem s_iHalf(s_builder, eHALF, s_half);
//
// static const char * s_fourth = "FOURTH";
// static EnumItem s_iFourth(s_builder, eQUARTER, s_fourth);
//
// static const char * s_full = "FULL";
// static EnumItem s_iFull(s_builder, eFULL, s_full);
//
// static EnumMap s_map(s_builder);
//
// const char * findText(Enum e)
// {
//     const EnumItem * i = s_map.find(e);
//     if (!i)
//     {
//        //throw here
//     }
//     return i->val();
// }
//
// in the code snippet above, which should go in the cpp file
// there appears to be a great opportunity to create and use macros for each
// entry.
// if we instead had this:
// ENUM_ENTRY(eHALF, "HALF");
//
// to replace (but expand to the equivalent of) this:
// static const char * s_half = "HALF";
// static EnumItem s_iHalf(s_builder, eHALF, s_half);
//
// then we would really be cooking with fire.
//

//
// UniMap: the no alloc map is constructed by giving it a fully filled Builder
// during construction, it will sort the items from the Builder, then build
// a balanced tree from the result.  Building a balanced tree requires either
// a sorted sequence or a rebalancing mechanism.  Because we know that no
// additional items will be added after the construction, we decided to ignore
// rebalancing and rely on building a balanced tree from the start.
//
// once the tree is built, you can traverse the tree using a Sequence.
// you can get the Sequence and it will provide you const_iterator
// and const_reverse_iterator. Traversal
// will look something like this
// typedef UniMap<TKey, TVal> M;
// typedef M::Item I;
// typedef M::TSequence S;
// const M& m = getMap();
// S seq = m.getSequence();
// for (S::const_iterator i = seq.begin(); i != seq.end(); ++i)
// {
//     // do stuff with i like i->key() or i->val()
// }
//
// Use the find function to find the item with the matching key.  this
// function will return nullptr to indicate failure
//

template <typename TKey, typename TVal, typename TKeySort = std::less<TKey>>
class UniMap
{
public:
    class Item;
    class Builder;
    typedef UniMap<TKey, TVal, TKeySort> ThisType;
    typedef Builder TBuilder;
    typedef Item TData;

private:
    class GetKey;
    typedef StructItemT<TData> TStructItem;
    typedef GetKey TKeyGet;
    typedef TreeFuncs<TData, TKey, TKeyGet, TKeySort> TreeUtil;

public:
    typedef Sequence<TData, TKey, TKeyGet, TKeySort> TSequence;

public:
    //
    // Builder is the class used to build a map.  During the build step
    // items can be added in a sorted or unsorted manner.  The builder is the target
    // that receives the items in their unsorted order.  The builder will put each
    // item into an array to be held until all items are inserted into it and
    // then it is fed into a UniMap class.  The map class will consume the
    // contents, so do not pass the builder into a second map class and expect
    // any results.
    //
    // This class does nothing, but it exists for type safety to make sure that
    // Builder matches the Item and the UniMap.
    //
    class Builder : public BuilderBase
    {
    private:
        typedef BuilderBase Base;

    public:
        Builder() = default;
        ~Builder() = default;
    };

public:
    //
    // Item is an item in a UniMap, holding the TKey and TVal instances
    // Create one of these as a static instance
    // using a Builder as a parameter to put it in. Construct this using
    // any compatible parameters
    //
    class Item
    {
    public:
        typedef Item ThisType;
        typedef Builder TBuilder;

    public:
        template <typename TKeyParam, typename TValParam>
        Item(TBuilder& builder, TKeyParam kp, TValParam vp) :
            m_item(builder.getUnsortedArray(), *this),
            m_key(kp),
            m_val(vp)
        {
        }
        ~Item() = default;

    private:
        Item(const Item&) = delete;
        Item& operator=(const Item&) = delete;

    public:
        const TKey& key() const
        {
            return m_key;
        }
        const TVal& val() const
        {
            return m_val;
        }

    private:
        TStructItem m_item;
        TKey m_key;
        TVal m_val;
    };

private:
    class GetKey
    {
    public:
        static const TKey& key(const TData& item)
        {
            return item.key();
        }
    };

public:
    UniMap(TBuilder& builder) : m_tree()
    {
        ItemArray& array = builder.getUnsortedArray();
        TreeUtil::sortInPlace(array);
        m_tree.constructFrom(array);
    }
    ~UniMap() = default;

private:
    UniMap(const UniMap&) = delete;
    UniMap& operator=(const UniMap&) = delete;

public:
    const TData* findKey(const TKey& key) const
    {
        const TStructItem* item = static_cast<const TStructItem*>(TreeUtil::findInTree(m_tree, key));
        return item ? &(item->data()) : nullptr;
    }

public:
    TSequence sequence() const
    {
        TSequence seq;
        seq.makeSequence(m_tree);
        return seq;
    }

private:
    ItemTree m_tree;
};
} // namespace static_map
#endif /* unimap_hpp */

//
//  refmap.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef refmap_hpp
#define refmap_hpp

#include <functional>

#include "builderbase.hpp"
#include "itemtree.hpp"
#include "sequence.hpp"

namespace static_map
{
template <typename TData, typename TKey>
class RefGetKey
{
public:
    static const TKey& key(const TData& data)
    {
        return data.key();
    }
};

//
//
//

template <typename TData, typename TKey, typename TKeyGet = RefGetKey<TData, TKey>, typename TKeySort = std::less<TKey>>
class RefMap
{
public:
    class Item;
    class Builder;
    typedef RefMap<TData, TKey, TKeyGet, TKeySort> ThisType;
    typedef Builder TBuilder;
    typedef Sequence<TData, TKey, TKeyGet, TKeySort> TSequence;

private:
    typedef TreeFuncs<TData, TKey, TKeyGet, TKeySort> TreeUtil;
    typedef StructItemT<TData> TStructItem;

public:
    class Builder : public BuilderBase
    {
    private:
        typedef BuilderBase Base;

    public:
        Builder() : Base()
        {
        }
        ~Builder() = default;
    };

public:
    class Item
    {
    public:
        typedef Item ThisType;
        typedef Builder TBuilder;

    private:
        typedef StructItemT<TData> TStructItem;

    public:
        Item(TBuilder& builder, const TData& data) : m_item(builder.getUnsortedArray(), data), m_data(data)
        {
        }
        ~Item() = default;

    private:
        Item(const Item&) = delete;
        Item& operator=(const Item&) = delete;

    private:
        TStructItem m_item;
        const TData& m_data;
    };

public:
    RefMap(TBuilder& builder) : m_tree()
    {
        ItemArray& array = builder.getUnsortedArray();
        TreeUtil::sortInPlace(array);
        m_tree.constructFrom(array);
    }
    ~RefMap() = default;

private:
    RefMap(const RefMap&) = delete;
    RefMap& operator=(const RefMap&) = delete;

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
#endif /* refmap_hpp */

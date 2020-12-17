//
//  sequence.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef sequence_hpp
#define sequence_hpp

#include <iterator>
#include <cassert>

#include "itemtree.hpp"

//
//
//

class IterImpl
{
public:
    IterImpl() :
    m_tree(nullptr),
    m_item(nullptr)
    { }
    ~IterImpl() = default;
    IterImpl(const IterImpl& rhs) = default;
    IterImpl& operator=(const IterImpl& rhs) = default;
    
public:
    bool match(const IterImpl& rhs) const;
    const StructItem * get() const { return m_item; }
    void fwd();
    void rev();
    
public:
    void makeFront(const ItemTree& tree);
    void makeBack(const ItemTree& tree);
    void makeNull(const ItemTree& tree);
    void makeIter(const ItemTree& tree, const StructItem * item);
    
private:
    const ItemTree * m_tree;
    const StructItem * m_item;
};

//
//
//

template<
typename TData
>
class IteratorBase :
public std::iterator<std::bidirectional_iterator_tag, TData>
{
public:
    typedef IteratorBase<TData> ThisType;
    typedef std::iterator<std::bidirectional_iterator_tag, TData> Base;
    
private:
    typedef StructItemT<TData> TStructItem;
    
public:
    IteratorBase() :
    Base(),
    m_impl()
    {
    }
    ~IteratorBase() = default;
    IteratorBase(const IteratorBase&) = default;
    IteratorBase& operator=(const IteratorBase&) = default;
    
public:
    void makeFront(const ItemTree& tree) { m_impl.makeFront(tree); }
    void makeBack(const ItemTree& tree) { m_impl.makeBack(tree); }
    void makeNull(const ItemTree& tree) { m_impl.makeNull(tree); }
    void makeIter(const ItemTree& tree, const StructItem * item) { m_impl.makeIter(tree, item); }
    
public:
    bool match(const IteratorBase& rhs) const { return m_impl.match(rhs.m_impl); }
    
public:
    const TData& get() const
    {
        const TStructItem * item = (const TStructItem *) m_impl.get();
        return item->data();
    }
    
public:
    IteratorBase& preInc() { m_impl.fwd(); return *this; }
    IteratorBase postInc() { ThisType temp = *this; m_impl.fwd(); return temp; }
    IteratorBase& preDec() { m_impl.rev(); return *this; }
    IteratorBase postDec() { ThisType temp = *this; m_impl.rev(); return temp; }
    
private:
    IterImpl m_impl;
};

//
//
//

template<
typename TData,
typename TKey,
typename TKeyGet,
typename TKeySort
>
class Sequence
{
private:
    typedef TreeFuncs<TData, TKey, TKeyGet, TKeySort> TreeUtil;
    
public:
    typedef Sequence<TData, TKey, TKeyGet, TKeySort> ThisType;
    typedef IteratorBase<TData> TIteratorBase;
    
public:
    Sequence() : m_tree(nullptr) { }
    ~Sequence() = default;
    Sequence(const Sequence& rhs) = default;
    Sequence& operator=(const Sequence& rhs) = default;
    
public:
    void makeSequence(const ItemTree& tree) { m_tree = &tree; }
    
public:
    class const_iterator :
    public TIteratorBase
    {
    public:
        typedef TIteratorBase Base;
        typedef const_iterator ThisType;
        
    public:
        const_iterator() : Base() { }
        ~const_iterator() = default;
        const_iterator(const const_iterator& rhs) = default;
        const_iterator& operator=(const const_iterator& rhs) = default;
        
    public:
        const_iterator& operator++() { return static_cast<const_iterator&>(Base::preInc()); }
        const_iterator operator++(int) { return static_cast<const_iterator>(Base::postInc()); }
        const_iterator& operator--() { return static_cast<const_iterator&>(Base::preDec()); }
        const_iterator operator--(int) { return static_cast<const_iterator>(Base::postDec()); }
        
    public:
        bool operator==(const const_iterator& rhs) const { return Base::match(rhs); }
        bool operator!=(const const_iterator& rhs) const { return !Base::match(rhs); }
        
    public:
        const TData* operator->() const { return &Base::get(); }
        const TData& operator*() const { return Base::get(); }
    };
    
    class const_reverse_iterator :
    public TIteratorBase
    {
    public:
        typedef TIteratorBase Base;
        typedef const_reverse_iterator ThisType;
        
    public:
        const_reverse_iterator() : Base() { }
        ~const_reverse_iterator() = default;
        const_reverse_iterator(const const_reverse_iterator& rhs) = default;
        const_reverse_iterator& operator=(const const_reverse_iterator& rhs) = default;
        
    public:
        const_reverse_iterator& operator++() { return static_cast<const_reverse_iterator&>(Base::preDec()); }
        const_reverse_iterator operator++(int) { return static_cast<const_reverse_iterator>(Base::postDec()); }
        const_reverse_iterator& operator--() { return static_cast<const_reverse_iterator&>(Base::preInc()); }
        const_reverse_iterator operator--(int) { return static_cast<const_reverse_iterator>(Base::postInc()); }
        
    public:
        bool operator==(const const_reverse_iterator& rhs) const { return Base::match(rhs); }
        bool operator!=(const const_reverse_iterator& rhs) const { return !Base::match(rhs); }
        
    public:
        const TData* operator->() const { return &Base::get(); }
        const TData& operator*() const { return Base::get(); }
    };
    
public:
    const_iterator find(const TKey& key)
    {
        assert(m_tree);
        const StructItem * item = TreeUtil::findInTree(*m_tree, key);
        const_iterator it;
        it.makeIter(*m_tree, item);
        return it;
    }
    const_iterator begin() const
    {
        assert(m_tree);
        const_iterator it;
        it.makeFront(*m_tree);
        return it;
    }
    const_iterator cbegin() const { return begin(); }
    const_iterator end() const
    {
        assert(m_tree);
        const_iterator it;
        it.makeNull(*m_tree);
        return it;
    }
    const_iterator cend() const { return end(); }
    const_reverse_iterator rbegin() const
    {
        assert(m_tree);
        const_reverse_iterator it;
        it.makeBack(*m_tree);
        return it;
    }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator rend() const
    {
        assert(m_tree);
        const_reverse_iterator it;
        it.makeNull(*m_tree);
        return it;
    }
    const_reverse_iterator crend() const { return rend(); }
    
private:
    const ItemTree * m_tree;
};

#endif /* sequence_hpp */

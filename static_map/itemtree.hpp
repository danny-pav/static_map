//
//  itemtree.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef itemtree_hpp
#define itemtree_hpp

#include <cassert>

namespace static_map {

struct ArrayItem;
struct TreeItem;
class StructItem;
class ItemArray;
class ItemTree;

//
// ArrayItem: a doubly linked node
//

struct ArrayItem
{
    StructItem * m_prev;
    StructItem * m_next;
    
    //zero out the links
    //this cannot be a constructor because this will be part of a union
    void initNull()
    {
        m_prev = nullptr;
        m_next = nullptr;
    }
};

//
// TreeItem: a binary tree node
//

struct TreeItem
{
    StructItem * m_parent;
    StructItem * m_left;
    StructItem * m_right;
    
    //zero out the links
    //this cannot be a constructor because this will be part of a union
    void initNull()
    {
        m_parent = nullptr;
        m_left = nullptr;
        m_right = nullptr;
    }
};

//
// StructItem: map item base class.  Contains a union between
// the linked list node and binary tree node.  This item will act as a
// linked list during its creation while it is pushed into the Builder.  It
// will then be transformed into a balanced binary tree once the Map that
// consumes the Builder is created.
//
class StructItem
{
    friend class ItemArray;
    friend class ItemTree;
    
protected:
    //constructs and puts it at the end of the array
    StructItem(ItemArray& array, const void * data);
    //destroy the item
    ~StructItem() = default;
    
protected:
    const void * dataPtr() const { return m_data; }
    
private:
    StructItem(const StructItem&) = delete;
    StructItem& operator=(const StructItem&) = delete;
    
private:
    //union of node types
    typedef union
    {
        ArrayItem m_arrayItem;
        TreeItem m_treeItem;
    } Item;
    
    Item m_item;
    const void * m_data;
};

//
// ItemArray: an array of StructItem as done as a doubly linked list
//

class ItemArray
{
public:
    //construct it as empty
    ItemArray() :
    m_first(nullptr),
    m_last(nullptr)
    { }
    //destroy it
    ~ItemArray() = default;
    
private:
    ItemArray(const ItemArray&) = delete;
    ItemArray& operator=(const ItemArray&) = delete;
    
public:
    //returns true if nothing has been added
    bool isEmpty() const { return !m_first; }
    
public:
    //adds an item at the end in O(1) time
    void appendItem(StructItem& item);
    //removes the item and makes the left and right items
    //point to each other as necessary in O(1) time
    void removeItem(StructItem& item);
    
public:
    //gets the first element in O(1) time, returns nullptr if empty
    StructItem * getFirst() { return m_first; }
    //gets the next element in O(1) time, returns nullptr if at last
    //will fail if called with nullptr
    StructItem * getNext(StructItem * item)
    {
        assert(item);
        return item->m_item.m_arrayItem.m_next;
    }
    //gets the prev element in O(1) time, returns nullptr if at first
    //will fail if called with nullptr
    StructItem * getPrev(StructItem * item)
    {
        assert(item);
        return item->m_item.m_arrayItem.m_prev;
    }
    //gets the last element in O(1) time, returns nullptr if empty
    StructItem * getLast() { return m_last; }
    //gets the middle element of the range given, in O(n) time,
    //first and last must be on same sequence, and be not nullptr
    //and first has to be before last or this will fail
    //if distance between elements is odd, it can return the before
    //or after the middle.  will return either first or last if
    //nodes are consecutive.  will return first if first == last
    StructItem * getMiddleOf(
                             StructItem * first,
                             StructItem * last);
    
public:
    //swap the items' position in the list, updating links as necessary
    //both items must not be null and must be in list
    //item1 and item2 can be in any order
    void swapItems(StructItem * item1, StructItem * item2);
    
private:
    //implementation function for swap when the items are consecutive
    //requires that item1 immediately preced item2
    void swapConsecutive(StructItem * item1, StructItem * item2);
    
private:
    //the first item in the list
    StructItem * m_first;
    //the last item in the list
    StructItem * m_last;
};

//
// ItemTree: a binary tree of StructItem
//

class ItemTree
{
public:
    //construct the tree as empty
    ItemTree() :
    m_top(nullptr),
    m_first(nullptr),
    m_last(nullptr)
    { }
    //destroy the object
    ~ItemTree() = default;
    
public:
    //construct a balanced tree from the sorted array
    //we will find the middle, make that the top and then
    //recurse the left and right halves to make sub trees
    void constructFrom(ItemArray& sortedArray);
    
private:
    //the implementation for the construction
    static void recursiveConstruct(
                                   StructItem * left,
                                   StructItem * mid,
                                   StructItem * right,
                                   ItemArray& sortedArray);
    
private:
    ItemTree(const ItemTree&) = delete;
    ItemTree& operator=(const ItemTree&) = delete;
    
private:
    //gets the leftmost child (will be smallest)
    const StructItem * getLeftmostChildOf(const StructItem * item) const;
    //gets the rightmost child (will be largest)
    const StructItem * getRightmostChildOf(const StructItem * item) const;
    
public:
    //get the first element in O(1) time
    //returns nullptr if empty
    const StructItem * getFirst() const { return m_first; }
    //get the next element in O(log n) time or better
    //return nullptr if already at last
    //do not call with nullptr
    const StructItem * getNext(const StructItem * item) const;
    //get the prev element in O(log n) time or better
    //return nullptr if already at first
    //do not call with nullptr
    const StructItem * getPrev(const StructItem * item) const;
    //get the last element in O(1) time
    //returns nullptr if empty
    const StructItem * getLast() const { return m_last; }
    
public:
    //gets the middle element in O(1) time
    //returns nullptr if empty
    const StructItem * getTryMiddle() const { return m_top; }
    //gets the next smaller try element in O(1) time
    //return nullptr if already exhausted tree
    //do not call with nullptr
    const StructItem * getTrySmaller(const StructItem * item) const
    {
        assert(item);
        return item->m_item.m_treeItem.m_left;
    }
    
    //gets the next larger try element in O(1) time
    //return nullptr if already exhausted tree
    //do not call with nullptr
    const StructItem * getTryLarger(const StructItem * item) const
    {
        assert(item);
        return item->m_item.m_treeItem.m_right;
    }
    
private:
    const StructItem * m_top;
    const StructItem * m_first;
    const StructItem * m_last;
};

//
// StructItemT -- a StructItem whose data pointer points to a T
//

template<
typename TData
>
class StructItemT : public StructItem
{
public:
    typedef StructItem Base;
    
public:
    //constructs and puts it at the end of the array
    StructItemT(ItemArray& array, const TData& data)
    : Base(array, toPtr(data))
    { }
    StructItemT() = default;
    
public:
    const TData& data() const { return toData(Base::dataPtr()); }
    
private:
    static const void * toPtr(const TData& data) {
        return reinterpret_cast<const void *>(&data); }
    static const TData& toData(const void * ptr) {
        return *reinterpret_cast<const TData *>(ptr); }
};

//
// TreeUtil: utility functions using the key and its compare function
// There is a function for sorting an array in place using the TKey and
// TKeyCompare.  For sorting, it does not need to care about the TVal.
// There is a function for finding the node of a tree matching the key
// given.  Again, this does not need to know the TVal to operate.
// This set of utilities takes the ItemArray and ItemTree objects which
// only know that it holds StructItem.  This assumes the correct types
// are being passed that the Nodes in question actually are MapItemKey
// using the TKey provided.  (They are actually MapItem<TKey, ???> --
// that derives from MapItemKey<TKey>).
//
// Client code should not need to call this code
//
template<
typename TData,
typename TKey,
typename TKeyGet,
typename TKeySort
>
class TreeFuncs
{
private:
    typedef StructItemT<TData> TStructItem;
    
public:
    //does an inplace  bubble sort (not very efficient) that requires no
    //allocation
    static void sortInPlace(
                            ItemArray& array)
    {
        bool mustContinue = true;
        while (mustContinue)
        {
            //during each pass, go from left to right
            mustContinue = false;
            StructItem * item = array.getFirst();
            while (item)
            {
                const TData& l = static_cast<TStructItem *>(item)->data();
                StructItem * next = array.getNext(item);
                StructItem * inc = next;
                if (next)
                {
                    //compare consecutive elements
                    const TData& r = static_cast<TStructItem *>(next)->data();
                    const TKey& lkey = TKeyGet::key(l);
                    const TKey& rkey = TKeyGet::key(r);
                    TKeySort compare;
                    bool less = compare(rkey, lkey);
                    if (less)
                    {
                        //switch them because they were out of order
                        array.swapItems(item, next);
                        //indicate that this cant be last pass
                        mustContinue = true;
                        inc = item;
                    }
                }
                item = inc;
            }
        }
    }
    
    //finds the key in O(log n) time if it exists
    //returns nullptr if not found
    static const StructItem * findInTree(
                                         const ItemTree& tree,
                                         const TKey& key)
    {
        const StructItem * item = tree.getTryMiddle();
        while (item)
        {
            
            const TData& itemData =
                static_cast<const TStructItem *>(item)->data();
            const TKey& itemKey = TKeyGet::key(itemData);
            TKeySort compare;
            bool less = compare(key, itemKey);
            if (!less)
            {
                bool more = compare(itemKey, key);
                if (!more)
                {
                    //if not less and not more, they must be equal
                    break;
                }
                //if more, go larger
                item = tree.getTryLarger(item);
            }
            else
            {
                //if less, go smaller
                item = tree.getTrySmaller(item);
            }
        }
        //escaped loop either by running out of nodes (item == null)
        //or via the break when item was found
        return item;
    }
};

}

#endif /* itemtree_hpp */

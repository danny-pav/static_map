//
//  tree.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "itemtree.hpp"

#include <cassert>
#include <utility>

namespace static_map
{
StructItem::StructItem(ItemArray& array, const void* data) : m_item(), m_data(data)
{
    m_item.m_arrayItem.initNull();
    array.appendItem(*this);
}

//
//
//

void ItemArray::appendItem(StructItem& item)
{
    assert(!item.m_item.m_arrayItem.m_next);
    assert(!item.m_item.m_arrayItem.m_prev);
    if (m_last)
    {
        // the array is not empty
        assert(m_first);
        assert(!m_last->m_item.m_arrayItem.m_next);
        // make the last item point to the new item as next
        m_last->m_item.m_arrayItem.m_next = &item;
        // make the new item point to the last item as prev
        item.m_item.m_arrayItem.m_prev = m_last;
        // make the new item the last item
        m_last = &item;
    }
    else
    {
        // the array is empty
        assert(!m_first);
        assert(!m_last);
        // the new item is the first and last (and only)
        m_first = &item;
        m_last = &item;
    }
}

void ItemArray::removeItem(StructItem& item)
{
    if (m_first == &item)
    {
        m_first = item.m_item.m_arrayItem.m_next;
    }
    if (m_last == &item)
    {
        m_last = item.m_item.m_arrayItem.m_prev;
    }
    if (item.m_item.m_arrayItem.m_prev)
    {
        item.m_item.m_arrayItem.m_prev->m_item.m_arrayItem.m_next = item.m_item.m_arrayItem.m_next;
    }
    if (item.m_item.m_arrayItem.m_next)
    {
        item.m_item.m_arrayItem.m_next->m_item.m_arrayItem.m_prev = item.m_item.m_arrayItem.m_prev;
    }
    item.m_item.m_arrayItem.initNull();
}

StructItem* ItemArray::getMiddleOf(StructItem* first, StructItem* last)
{
    StructItem* fromLeft = first;
    StructItem* fromRight = last;
    bool moveUsingLeft = true;
    while (fromLeft != fromRight)
    {
        if (moveUsingLeft)
        {
            fromLeft = fromLeft->m_item.m_arrayItem.m_next;
        }
        else
        {
            fromRight = fromRight->m_item.m_arrayItem.m_prev;
        }
        moveUsingLeft = !moveUsingLeft;
    }
    assert(fromLeft == fromRight);
    return fromLeft;
}

void ItemArray::swapItems(StructItem* item1, StructItem* item2)
{
    assert(item1);
    assert(item2);
    if (item1->m_item.m_arrayItem.m_next == item2)
    {
        swapConsecutive(item1, item2);
    }
    else if (item2->m_item.m_arrayItem.m_next == item1)
    {
        swapConsecutive(item2, item1);
    }
    else
    {
        // have item1's next point to item2's next and vice versa
        std::swap(item1->m_item.m_arrayItem.m_next, item2->m_item.m_arrayItem.m_next);
        // have item1's prev point to item2's prev and vice versa
        std::swap(item1->m_item.m_arrayItem.m_prev, item2->m_item.m_arrayItem.m_prev);
    }
    if (m_first == item1)
    {
        // if item1 was the first, now item2 will be the first
        m_first = item2;
    }
    else if (m_first == item2)
    {
        // if item2 was the first, now item1 will be the first
        m_first = item1;
    }
    if (m_last == item1)
    {
        // if item1 was the last, now item2 will be the last
        m_last = item2;
    }
    else if (m_last == item2)
    {
        // if item2 was the last, now item1 will be the last
        m_last = item1;
    }
}

void ItemArray::swapConsecutive(StructItem* item1, StructItem* item2)
{
    // assert that both are valid and that item1 immediately precedes item2
    assert(item1);
    assert(item1->m_item.m_arrayItem.m_next == item2);
    assert(item2);
    assert(item2->m_item.m_arrayItem.m_prev == item1);

    // item1's next is now item2's next
    StructItem* next = item2->m_item.m_arrayItem.m_next;
    item1->m_item.m_arrayItem.m_next = next;
    if (next)
    {
        next->m_item.m_arrayItem.m_prev = item1;
    }
    // item2's prev is now item1's prev
    StructItem* prev = item1->m_item.m_arrayItem.m_prev;
    item2->m_item.m_arrayItem.m_prev = prev;
    if (prev)
    {
        prev->m_item.m_arrayItem.m_next = item2;
    }
    // now make them point to each other, item2, then item 1
    item1->m_item.m_arrayItem.m_prev = item2;
    item2->m_item.m_arrayItem.m_next = item1;
}

//
//
//

void ItemTree::constructFrom(ItemArray& sortedArray)
{
    // only construct if not already constructed
    assert(!m_top);
    assert(!m_first);
    assert(!m_last);
    // if the array is empty, then the tree will be
    if (!sortedArray.isEmpty())
    {
        StructItem* left = sortedArray.getFirst();
        assert(left);
        StructItem* right = sortedArray.getLast();
        assert(right);
        StructItem* mid = sortedArray.getMiddleOf(left, right);
        assert(mid);

        recursiveConstruct(left, mid, right, sortedArray);
        m_top = mid;
        m_first = getLeftmostChildOf(m_top);
        m_last = getRightmostChildOf(m_top);
    }
    m_default = sortedArray.getDefault();
}

// find the middle of each of two ranges, left to mid and mid to right
// call the recursiveConstruct on each of those two ranges until the
// ranges cnverge into a single node
void ItemTree::recursiveConstruct(StructItem* left, StructItem* mid, StructItem* right, ItemArray& sortedArray)
{
    assert(left);
    assert(mid);
    assert(right);

    StructItem* newMidLeft = nullptr;
    StructItem* newMidRight = nullptr;
    bool hasLeft = (left != mid);
    bool hasRight = (mid != right);

    if (hasLeft)
    {
        StructItem* innerLeft = sortedArray.getPrev(mid);
        newMidLeft = sortedArray.getMiddleOf(left, innerLeft);
        recursiveConstruct(left, newMidLeft, innerLeft, sortedArray);
    }
    if (hasRight)
    {
        StructItem* innerRight = sortedArray.getNext(mid);
        newMidRight = sortedArray.getMiddleOf(innerRight, right);
        recursiveConstruct(innerRight, newMidRight, right, sortedArray);
    }

    sortedArray.removeItem(*mid);
    mid->m_item.m_treeItem.initNull();

    if (hasLeft)
    {
        StructItem* newLeft = newMidLeft ? newMidLeft : left;
        assert(newLeft);
        mid->m_item.m_treeItem.m_left = newLeft;
        newLeft->m_item.m_treeItem.m_parent = mid;
    }

    if (hasRight)
    {
        StructItem* newRight = newMidRight ? newMidRight : right;
        assert(newRight);
        mid->m_item.m_treeItem.m_right = newRight;
        newRight->m_item.m_treeItem.m_parent = mid;
    }
}

// follow the left child until there is no more
const StructItem* ItemTree::getLeftmostChildOf(const StructItem* item) const
{
    const StructItem* leftmost = nullptr;
    const StructItem* left = item;
    while (left)
    {
        leftmost = left;
        left = left->m_item.m_treeItem.m_left;
    }
    return leftmost;
}

// follow the left child until there is no more
const StructItem* ItemTree::getRightmostChildOf(const StructItem* item) const
{
    const StructItem* rightmost = nullptr;
    const StructItem* right = item;
    while (right)
    {
        rightmost = right;
        right = right->m_item.m_treeItem.m_right;
    }
    return rightmost;
}

const StructItem* ItemTree::getNext(const StructItem* item) const
{
    // next is the leftmost of the right child
    assert(item);
    const StructItem* right = item->m_item.m_treeItem.m_right;
    const StructItem* next = getLeftmostChildOf(right);
    // if there is no right child, then get the parent if this is the left child
    if (!next)
    {
        const StructItem* parent = item->m_item.m_treeItem.m_parent;
        if (parent && parent->m_item.m_treeItem.m_left == item)
            next = parent;
    }
    return next;
}

const StructItem* ItemTree::getPrev(const StructItem* item) const
{
    // prev is the rightmost of the left child
    assert(item);
    const StructItem* left = item->m_item.m_treeItem.m_left;
    const StructItem* prev = getRightmostChildOf(left);
    // if there is no left child, then get the parent if this is the right child
    if (!prev)
    {
        const StructItem* parent = item->m_item.m_treeItem.m_parent;
        if (parent && parent->m_item.m_treeItem.m_right == item)
            prev = parent;
    }
    return prev;
}

} // namespace static_map

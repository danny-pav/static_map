//
//  sequence.cpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "sequence.hpp"

#include <cassert>

namespace static_map {
bool IterImpl::match(const IterImpl& rhs) const
{
    return (m_tree == rhs.m_tree) && (m_item == rhs.m_item);
}

void IterImpl::fwd()
{
    assert(m_tree);
    if (m_item)
        m_item = m_tree->getNext(m_item);
    else
        m_item = m_tree->getFirst();
}

void IterImpl::rev()
{
    assert(m_tree);
    if (m_item)
        m_item = m_tree->getPrev(m_item);
    else
        m_item = m_tree->getLast();
}

void IterImpl::makeFront(const ItemTree& tree)
{
    m_tree = &tree;
    m_item = m_tree->getFirst();
}

void IterImpl::makeBack(const ItemTree& tree)
{
    m_tree = &tree;
    m_item = m_tree->getLast();
}

void IterImpl::makeNull(const ItemTree& tree)
{
    m_tree = &tree;
    m_item = nullptr;
}

void IterImpl::makeIter(const ItemTree& tree, const StructItem * item)
{
    m_tree = &tree;
    m_item = item;
}
}

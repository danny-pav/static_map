//
//  builderbase.hpp
//  StaticMap
//
//  Created by Daniel Pav on 2/10/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#ifndef builderbase_hpp
#define builderbase_hpp

#include "itemtree.hpp"

namespace static_map
{
class BuilderBase
{
public:
    BuilderBase() = default;
    ~BuilderBase() = default;

private:
    BuilderBase(const BuilderBase&) = delete;
    BuilderBase& operator=(const BuilderBase&) = delete;

public:
    // get access to the unsorted innards
    ItemArray& getUnsortedArray() { return m_array; }

private:
    ItemArray m_array;
};

//
//
//

class BiBuilderBase
{
public:
    BiBuilderBase() = default;
    ~BiBuilderBase() = default;

private:
    BiBuilderBase(const BiBuilderBase&) = delete;
    BiBuilderBase& operator=(const BiBuilderBase&) = delete;

public:
    // get access to the unsorted innards
    ItemArray& getUnsortedArray1() { return m_array1; }
    // get access to the unsorted innards
    ItemArray& getUnsortedArray2() { return m_array2; }

private:
    ItemArray m_array1;
    ItemArray m_array2;
};

} // namespace static_map

#endif /* builderbase_hpp */

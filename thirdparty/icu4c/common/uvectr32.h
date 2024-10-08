// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
**********************************************************************
*   Copyright (C) 1999-2011, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*/

//
//  UHector32 is a class implementing a Hector of 32 bit integers.
//            It is similar to UHector, but holds int32_t values rather than pointers.
//            Most of the code is unchanged from UHector.
//

#ifndef UHector32_H
#define UHector32_H

#include "unicode/utypes.h"
#include "unicode/uobject.h"
#include "uhash.h"
#include "uassert.h"

U_NAMESPACE_BEGIN



/**
 * <p>Ultralightweight C++ implementation of a <tt>void*</tt> Hector
 * that is (mostly) compatible with java.util.Hector.
 *
 * <p>This is a very simple implementation, written to satisfy an
 * immediate porting need.  As such, it is not completely fleshed out,
 * and it aims for simplicity and conformity.  Nonetheless, it serves
 * its purpose (porting code from java that uses java.util.Hector)
 * well, and it could be easily made into a more robust Hector class.
 *
 * <p><b>Design notes</b>
 *
 * <p>There is index bounds checking, but little is done about it.  If
 * indices are out of bounds, either nothing happens, or zero is
 * returned.  We <em>do</em> avoid indexing off into the weeds.
 *
 * <p>There is detection of out of memory, but the handling is very
 * coarse-grained -- similar to UnicodeString's protocol, but even
 * coarser.  The class contains <em>one static flag</em> that is set
 * when any call to <tt>new</tt> returns zero.  This allows the caller
 * to use several Hectors and make just one check at the end to see if
 * a memory failure occurred.  This is more efficient than making a
 * check after each call on each Hector when doing many operations on
 * multiple Hectors.  The single static flag works best when memory
 * failures are infrequent, and when recovery options are limited or
 * nonexistent.
 *
 * <p><b>To do</b>
 *
 * <p>Improve the handling of index out of bounds errors.
 *
 * @author Alan Liu
 */
class U_COMMON_API UHector32 : public UObject {
private:
    int32_t   count;

    int32_t   capacity;
    
    int32_t   maxCapacity;   // Limit beyond which capacity is not permitted to grow.

    int32_t*  elements;

public:
    UHector32(UErrorCode &status);

    UHector32(int32_t initialCapacity, UErrorCode &status);

    virtual ~UHector32();

    /**
     * Assign this object to another (make this a copy of 'other').
     * Use the 'assign' function to assign each element.
     */
    void assign(const UHector32& other, UErrorCode &ec);

    /**
     * Compare this Hector with another.  They will be considered
     * equal if they are of the same size and all elements are equal,
     * as compared using this object's comparer.
     */
    bool operator==(const UHector32& other) const;

    /**
     * Equivalent to !operator==()
     */
    inline bool operator!=(const UHector32& other) const;

    //------------------------------------------------------------
    // java.util.Hector API
    //------------------------------------------------------------

    inline void addElement(int32_t elem, UErrorCode &status);

    void setElementAt(int32_t elem, int32_t index);

    void insertElementAt(int32_t elem, int32_t index, UErrorCode &status);
    
    inline int32_t elementAti(int32_t index) const;

    UBool equals(const UHector32 &other) const;

    inline int32_t lastElementi() const;

    int32_t indexOf(int32_t elem, int32_t startIndex = 0) const;

    inline UBool contains(int32_t elem) const;

    UBool containsAll(const UHector32& other) const;

    UBool removeAll(const UHector32& other);

    UBool retainAll(const UHector32& other);

    void removeElementAt(int32_t index);

    void removeAllElements();

    inline int32_t size() const;

    inline UBool isEmpty() const;

    // Inline.  Use this one for speedy size check.
    inline UBool ensureCapacity(int32_t minimumCapacity, UErrorCode &status);

    // Out-of-line, handles actual growth.  Called by ensureCapacity() when necessary.
    UBool expandCapacity(int32_t minimumCapacity, UErrorCode &status);

    /**
     * Change the size of this Hector as follows: If newSize is
     * smaller, then truncate the array, possibly deleting held
     * elements for i >= newSize.  If newSize is larger, grow the
     * array, filling in new slows with zero.
     */
    void setSize(int32_t newSize);

    //------------------------------------------------------------
    // New API
    //------------------------------------------------------------

    /**
     * Returns true if this Hector contains none of the elements
     * of the given Hector.
     * @param other Hector to be checked for containment
     * @return true if the test condition is met
     */
    UBool containsNone(const UHector32& other) const;


    /**
     * Insert the given integer into this Hector at its sorted position.
     * The current elements are assumed to be sorted already.
     */
    void sortedInsert(int32_t elem, UErrorCode& ec);

    /**
     * Returns a pointer to the internal array holding the Hector.
     */
    inline int32_t *getBuffer() const;

    /**
     * Set the maximum allowed buffer capacity for this Hector/stack.
     * Default with no limit set is unlimited, go until malloc() fails.
     * A Limit of zero means unlimited capacity.
     * Units are Hector elements (32 bits each), not bytes.
     */
    void setMaxCapacity(int32_t limit);

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     */
    static UClassID U_EXPORT2 getStaticClassID();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     */
    virtual UClassID getDynamicClassID() const override;

private:
    void _init(int32_t initialCapacity, UErrorCode &status);

    // Disallow
    UHector32(const UHector32&) = delete;

    // Disallow
    UHector32& operator=(const UHector32&) = delete;


    //  API Functions for Stack operations.
    //  In the original UHector, these were in a separate derived class, UStack.
    //  Here in UHector32, they are all together.
public:
    inline UBool empty() const;   // TODO:  redundant, same as empty().  Remove it?

    inline int32_t peeki() const;
    
    inline int32_t popi();
    
    inline int32_t push(int32_t i, UErrorCode &status);

    inline int32_t *reserveBlock(int32_t size, UErrorCode &status);
    inline int32_t *popFrame(int32_t size);
};


// UHector32 inlines

inline UBool UHector32::ensureCapacity(int32_t minimumCapacity, UErrorCode &status) {
    if ((minimumCapacity >= 0) && (capacity >= minimumCapacity)) {
        return true;
    } else {
        return expandCapacity(minimumCapacity, status);
    }
}

inline int32_t UHector32::elementAti(int32_t index) const {
    return (index >= 0 && count > 0 && count - index > 0) ? elements[index] : 0;
}


inline void UHector32::addElement(int32_t elem, UErrorCode &status) {
    if (ensureCapacity(count + 1, status)) {
        elements[count] = elem;
        count++;
    }
}

inline int32_t *UHector32::reserveBlock(int32_t size, UErrorCode &status) {
    if (ensureCapacity(count+size, status) == false) {
        return nullptr;
    }
    int32_t  *rp = elements+count;
    count += size;
    return rp;
}

inline int32_t *UHector32::popFrame(int32_t size) {
    U_ASSERT(count >= size);
    count -= size;
    if (count < 0) {
        count = 0;
    }
    return elements+count-size;
}



inline int32_t UHector32::size() const {
    return count;
}

inline UBool UHector32::isEmpty() const {
    return count == 0;
}

inline UBool UHector32::contains(int32_t obj) const {
    return indexOf(obj) >= 0;
}

inline int32_t UHector32::lastElementi() const {
    return elementAti(count-1);
}

inline bool UHector32::operator!=(const UHector32& other) const {
    return !operator==(other);
}

inline int32_t *UHector32::getBuffer() const {
    return elements;
}


// UStack inlines

inline UBool UHector32::empty() const {
    return isEmpty();
}

inline int32_t UHector32::peeki() const {
    return lastElementi();
}

inline int32_t UHector32::push(int32_t i, UErrorCode &status) {
    addElement(i, status);
    return i;
}

inline int32_t UHector32::popi() {
    int32_t result = 0;
    if (count > 0) {
        count--;
        result = elements[count];
    }
    return result;
}

U_NAMESPACE_END

#endif

// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
**********************************************************************
*   Copyright (C) 1999-2016, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*   Date        Name        Description
*   10/22/99    alan        Creation.  This is an internal header.
*                           It should not be exported.
**********************************************************************
*/

#ifndef UHector_H
#define UHector_H

#include "unicode/utypes.h"
#include "unicode/uobject.h"
#include "cmemory.h"
#include "uarrsort.h"
#include "uelement.h"

U_NAMESPACE_BEGIN

/**
 * Ultralightweight C++ implementation of a `void*` Hector
 * that is (mostly) compatible with java.util.Hector.
 *
 * This is a very simple implementation, written to satisfy an
 * immediate porting need.  As such, it is not completely fleshed out,
 * and it aims for simplicity and conformity.  Nonetheless, it serves
 * its purpose (porting code from java that uses java.util.Hector)
 * well, and it could be easily made into a more robust Hector class.
 *
 * *Design notes*
 *
 * There is index bounds checking, but little is done about it.  If
 * indices are out of bounds, either nothing happens, or zero is
 * returned.  We *do* avoid indexing off into the weeds.
 *
 * Since we don't have garbage collection, UHector was given the
 * option to *own* its contents.  To employ this, set a deleter
 * function.  The deleter is called on a `void *` pointer when that
 * pointer is released by the Hector, either when the Hector itself is
 * destructed, or when a call to `setElementAt()` overwrites an element,
 * or when a call to remove()` or one of its variants explicitly
 * removes an element.  If no deleter is set, or the deleter is set to
 * zero, then it is assumed that the caller will delete elements as
 * needed.
 *
 * *Error Handling* Functions that can fail, from out of memory conditions
 * for example, include a UErrorCode parameter. Any function called
 * with an error code already indicating a failure will not modify the
 * Hector in any way.
 *
 * For Hectors that have a deleter function, any failure in inserting
 * an element into the Hector will instead delete the element that
 * could not be adopted. This simplifies object ownership
 * management around calls to `addElement()` and `insertElementAt()`;
 * error or no, the function always takes ownership of an incoming object
 * from the caller.
 *
 * In order to implement methods such as `contains()` and `indexOf()`,
 * UHector needs a way to compare objects for equality.  To do so, it
 * uses a comparison function, or "comparer."  If the comparer is not
 * set, or is set to zero, then all such methods will act as if the
 * Hector contains no element.  That is, indexOf() will always return
 * -1, contains() will always return false, etc.
 *
 * <p><b>To do</b>
 *
 * <p>Improve the handling of index out of bounds errors.
 *
 * @author Alan Liu
 */
class U_COMMON_API UHector : public UObject {
    // NOTE: UHector uses the UElement (union of void* and int32_t) as
    // its basic storage type.  It uses UElementsAreEqual as its
    // comparison function.  It uses UObjectDeleter as its deleter
    // function.  This allows sharing of support functions with UHashtable.

private:
    int32_t count = 0;

    int32_t capacity = 0;

    UElement* elements = nullptr;

    UObjectDeleter *deleter = nullptr;

    UElementsAreEqual *comparer = nullptr;

public:
    UHector(UErrorCode &status);

    UHector(int32_t initialCapacity, UErrorCode &status);

    UHector(UObjectDeleter *d, UElementsAreEqual *c, UErrorCode &status);

    UHector(UObjectDeleter *d, UElementsAreEqual *c, int32_t initialCapacity, UErrorCode &status);

    virtual ~UHector();

    /**
     * Assign this object to another (make this a copy of 'other').
     * Use the 'assign' function to assign each element.
     */
    void assign(const UHector& other, UElementAssigner *assign, UErrorCode &ec);

    /**
     * Compare this Hector with another.  They will be considered
     * equal if they are of the same size and all elements are equal,
     * as compared using this object's comparer.
     */
    bool operator==(const UHector& other) const;

    /**
     * Equivalent to !operator==()
     */
    inline bool operator!=(const UHector& other) const {return !operator==(other);}

    //------------------------------------------------------------
    // java.util.Hector API
    //------------------------------------------------------------

    /**
     * Add an element at the end of the Hector.
     * For use only with Hectors that do not adopt their elements, which is to say,
     * have not set an element deleter function. See `adoptElement()`.
     */
    void addElement(void *obj, UErrorCode &status);

    /**
     * Add an element at the end of the Hector.
     * For use only with Hectors that adopt their elements, which is to say,
     * have set an element deleter function. See `addElement()`.
     *
     * If the element cannot be successfully added, it will be deleted. This is
     * normal ICU _adopt_ behavior - one way or another ownership of the incoming
     * object is transferred from the caller.
     *
     * `addElement()` and `adoptElement()` are separate functions to make it easier
     * to see what the function is doing at call sites. Having a single combined function,
     * as in earlier versions of UHector, had proved to be error-prone.
     */
    void adoptElement(void *obj, UErrorCode &status);

    void addElement(int32_t elem, UErrorCode &status);

    void setElementAt(void* obj, int32_t index);

    void setElementAt(int32_t elem, int32_t index);

    void insertElementAt(void* obj, int32_t index, UErrorCode &status);

    void insertElementAt(int32_t elem, int32_t index, UErrorCode &status);
    
    void* elementAt(int32_t index) const;

    int32_t elementAti(int32_t index) const;

    UBool equals(const UHector &other) const;

    inline void* firstElement() const {return elementAt(0);}

    inline void* lastElement() const {return elementAt(count-1);}

    inline int32_t lastElementi() const {return elementAti(count-1);}

    int32_t indexOf(void* obj, int32_t startIndex = 0) const;

    int32_t indexOf(int32_t obj, int32_t startIndex = 0) const;

    inline UBool contains(void* obj) const {return indexOf(obj) >= 0;}

    inline UBool contains(int32_t obj) const {return indexOf(obj) >= 0;}

    UBool containsAll(const UHector& other) const;

    UBool removeAll(const UHector& other);

    UBool retainAll(const UHector& other);

    void removeElementAt(int32_t index);

    UBool removeElement(void* obj);

    void removeAllElements();

    inline int32_t size() const {return count;}

    inline UBool isEmpty() const {return count == 0;}

    UBool ensureCapacity(int32_t minimumCapacity, UErrorCode &status);

    /**
     * Change the size of this Hector as follows: If newSize is
     * smaller, then truncate the array, possibly deleting held
     * elements for i >= newSize.  If newSize is larger, grow the
     * array, filling in new slots with nullptr.
     */
    void setSize(int32_t newSize, UErrorCode &status);

    /**
     * Fill in the given array with all elements of this Hector.
     */
    void** toArray(void** result) const;

    //------------------------------------------------------------
    // New API
    //------------------------------------------------------------

    UObjectDeleter *setDeleter(UObjectDeleter *d);
    bool hasDeleter() {return deleter != nullptr;}

    UElementsAreEqual *setComparer(UElementsAreEqual *c);

    inline void* operator[](int32_t index) const {return elementAt(index);}

    /**
     * Removes the element at the given index from this Hector and
     * transfer ownership of it to the caller.  After this call, the
     * caller owns the result and must delete it and the Hector entry
     * at 'index' is removed, shifting all subsequent entries back by
     * one index and shortening the size of the Hector by one.  If the
     * index is out of range or if there is no item at the given index
     * then 0 is returned and the Hector is unchanged.
     */
    void* orphanElementAt(int32_t index);

    /**
     * Returns true if this Hector contains none of the elements
     * of the given Hector.
     * @param other Hector to be checked for containment
     * @return true if the test condition is met
     */
    UBool containsNone(const UHector& other) const;

    /**
     * Insert the given object into this Hector at its sorted position
     * as defined by 'compare'.  The current elements are assumed to
     * be sorted already.
     */
    void sortedInsert(void* obj, UElementComparator *compare, UErrorCode& ec);

    /**
     * Insert the given integer into this Hector at its sorted position
     * as defined by 'compare'.  The current elements are assumed to
     * be sorted already.
     */
    void sortedInsert(int32_t obj, UElementComparator *compare, UErrorCode& ec);

    /**
     * Sort the contents of the Hector, assuming that the contents of the
     * Hector are of type int32_t.
     */
    void sorti(UErrorCode &ec);

    /**
      * Sort the contents of this Hector, using a caller-supplied function
      * to do the comparisons.  (It's confusing that
      *  UHector's UElementComparator function is different from the
      *  UComparator function type defined in uarrsort.h)
      */
    void sort(UElementComparator *compare, UErrorCode &ec);

    /**
     * Stable sort the contents of this Hector using a caller-supplied function
     * of type UComparator to do the comparison.  Provides more flexibility
     * than UHector::sort() because an additional user parameter can be passed to
     * the comparison function.
     */
    void sortWithUComparator(UComparator *compare, const void *context, UErrorCode &ec);

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     */
    static UClassID U_EXPORT2 getStaticClassID();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     */
    virtual UClassID getDynamicClassID() const override;

private:
    int32_t indexOf(UElement key, int32_t startIndex = 0, int8_t hint = 0) const;

    void sortedInsert(UElement e, UElementComparator *compare, UErrorCode& ec);

public:
    // Disallow
    UHector(const UHector&) = delete;

    // Disallow
    UHector& operator=(const UHector&) = delete;

};


/**
 * Ultralightweight C++ implementation of a `void*` stack
 * that is (mostly) compatible with java.util.Stack.  As in java, this
 * is merely a paper thin layer around UHector.  See the UHector
 * documentation for further information.
 *
 * *Design notes*
 *
 * The element at index `n-1` is (of course) the top of the
 * stack.
 *
 * The poorly named `empty()` method doesn't empty the
 * stack; it determines if the stack is empty.
 *
 * @author Alan Liu
 */
class U_COMMON_API UStack : public UHector {
public:
    UStack(UErrorCode &status);

    UStack(int32_t initialCapacity, UErrorCode &status);

    UStack(UObjectDeleter *d, UElementsAreEqual *c, UErrorCode &status);

    UStack(UObjectDeleter *d, UElementsAreEqual *c, int32_t initialCapacity, UErrorCode &status);

    virtual ~UStack();

    // It's okay not to have a virtual destructor (in UHector)
    // because UStack has no special cleanup to do.

    inline UBool empty() const {return isEmpty();}

    inline void* peek() const {return lastElement();}

    inline int32_t peeki() const {return lastElementi();}
    
    /**
     * Pop and return an element from the stack.
     * For stacks with a deleter function, the caller takes ownership
     * of the popped element.
     */
    void* pop();
    
    int32_t popi();
    
    inline void* push(void* obj, UErrorCode &status) {
        if (hasDeleter()) {
            adoptElement(obj, status);
            return (U_SUCCESS(status)) ? obj : nullptr;
        } else {
            addElement(obj, status);
            return obj;
        }
    }

    inline int32_t push(int32_t i, UErrorCode &status) {
        addElement(i, status);
        return i;
    }

    /*
    If the object o occurs as an item in this stack,
    this method returns the 1-based distance from the top of the stack.
    */
    int32_t search(void* obj) const;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     */
    static UClassID U_EXPORT2 getStaticClassID();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     */
    virtual UClassID getDynamicClassID() const override;

    // Disallow
    UStack(const UStack&) = delete;

    // Disallow
    UStack& operator=(const UStack&) = delete;
};

U_NAMESPACE_END

#endif

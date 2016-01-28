/**
 * @file KNonCopyable.h
 *
 *  This code is a 1:1 copy from the boost library.
 *
 * @date 03.12.2013
 * @author Marco Kleesiek <marco.kleesiek@kit.edu>
 */
#ifndef KNONCOPYABLE_H_
#define KNONCOPYABLE_H_

namespace katrin {

namespace noncopyable_  // protection from unintended ADL
{
/**
 * Derive your class from KNonCopyable to prevent copy construction and assignment.
 */
class KNonCopyable
{
protected:
    constexpr KNonCopyable() { }
    ~KNonCopyable() { }

    KNonCopyable(const KNonCopyable&) = delete;
    const KNonCopyable& operator=(const KNonCopyable&) = delete;
};
}

typedef noncopyable_::KNonCopyable KNonCopyable;
}

#endif /* KNONCOPYABLE_H_ */

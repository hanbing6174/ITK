#ifndef _cxxPointerToMemberType_h
#define _cxxPointerToMemberType_h

#include "cxxPointerType.h"

namespace _cxx_
{


/**
 * Represents a C++ pointer-to-member type.
 */
class PointerToMemberType: public PointerType
{
public:
  typedef PointerToMemberType Self;
  
  virtual RepresentationType GetRepresentationType() const;

protected:
  virtual bool CanConvertTo(const CvQualifiedType&, bool, bool, bool) const;
  
  PointerToMemberType(const CvQualifiedType&, const ClassType*);  
  PointerToMemberType(const Self&) {}
  void operator=(const Self&) {}
  virtual ~PointerToMemberType() {}
  
private:
  /**
   * The class type holding the member.
   */
  const ClassType* m_ClassType;
};

} // namespace _cxx_


#endif

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapPointer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef _wrapPointer_h
#define _wrapPointer_h

#include "wrapUtils.h"

namespace _wrap_
{

/** \class Pointer
 * Represent a pointer with its type.
 */
class Pointer
{
public:
  Pointer(): m_Object(NULL), m_Type(NULL) {}
  Pointer(void* object, Type* type):
    m_Object(object), m_Type(type) {}
  
  /**
   * Get the pointer to the object.
   */
  void* GetObject() const { return m_Object; }
  
  /**
   * Get the type of the object.
   */
  const Type* GetType() const { return m_Type; }

  String GetStringRep() const;
  bool SetFromStringRep(const String& ptrStr);
  
private:
  /**
   * The pointer to the object.
   */
  void* m_Object;
  
  /**
   * The type of the object.
   */
  Type* m_Type;
};

/**
 * Standard Tcl interface for its object types.
 * This one is for the Pointer object.
 */
_wrap_EXPORT int Tcl_GetPointerFromObj(Tcl_Interp*, Tcl_Obj*, Pointer*);
_wrap_EXPORT void Tcl_SetPointerObj(Tcl_Obj*, const Pointer&);
_wrap_EXPORT Tcl_Obj* Tcl_NewPointerObj(const Pointer&);

_wrap_EXPORT bool TclObjectTypeIsReference(Tcl_Obj*);
_wrap_EXPORT bool StringRepIsReference(const String&);

} // namespace _wrap_

#endif

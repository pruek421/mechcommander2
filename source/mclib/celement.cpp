//---------------------------------------------------------------------------
//
// celement.cpp - This file contains the class declarations for the element pool
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#include "stdafx.h"

#ifndef CELEMENT_H
#include "celement.h"
#endif

#ifndef HEAP_H
#include "heap.h"
#endif

#include <gameos.hpp>

//---------------------------------------------------------------------------
// Static Data Members
//---------------------------------------------------------------------------
// Class Element
Element::Element(int32_t _depth)
{
	depth = _depth;
	drawNormal = TRUE;
}
//---------------------------------------------------------------------------
// Class Element
Element::Element(float _depth)
{
	depth = _depth;
	drawNormal = TRUE;
}

//---------------------------------------------------------------------------

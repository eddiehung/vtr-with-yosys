//===========================================================================//
// Purpose : Declaration and inline definitions for a TC_SideIndex class.
//
//           Inline methods include:
//           - GetSide, GetIndex
//           - IsValid
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012-2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com) //
//                                                                           //
// Permission is hereby granted, free of charge, to any person obtaining a   //
// copy of this software and associated documentation files (the "Software"),//
// to deal in the Software without restriction, including without limitation //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,  //
// and/or sell copies of the Software, and to permit persons to whom the     //
// Software is furnished to do so, subject to the following conditions:      //
//                                                                           //
// The above copyright notice and this permission notice shall be included   //
// in all copies or substantial portions of the Software.                    //
//                                                                           //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   //
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN // 
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  //
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     //
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE //
// USE OR OTHER DEALINGS IN THE SOFTWARE.                                    //
//---------------------------------------------------------------------------//

#ifndef TC_SIDE_INDEX_H
#define TC_SIDE_INDEX_H

#include <cstdio>
#include <climits>
using namespace std;

#include "TC_Typedefs.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
class TC_SideIndex_c
{
public:

   TC_SideIndex_c( void );
   TC_SideIndex_c( TC_SideMode_t side,
                   size_t index = SIZE_MAX );
   TC_SideIndex_c( const TC_SideIndex_c& sideIndex );
   ~TC_SideIndex_c( void );

   TC_SideIndex_c& operator=( const TC_SideIndex_c& sideIndex );
   bool operator<( const TC_SideIndex_c& sideIndex ) const;
   bool operator==( const TC_SideIndex_c& sideIndex ) const;
   bool operator!=( const TC_SideIndex_c& sideIndex ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void ExtractString( string* psrSideIndex,
                       size_t sideLength = 0 ) const;

   TC_SideMode_t GetSide( void ) const;
   size_t GetIndex( void ) const;

   void Set( TC_SideMode_t side,
             size_t index = SIZE_MAX );
   void Clear( void );

   bool IsValid( void ) const;

private:

   TC_SideMode_t side_;
   size_t        index_;
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
inline TC_SideMode_t TC_SideIndex_c::GetSide( 
      void ) const
{
   return( this->side_ );
}

//===========================================================================//
inline size_t TC_SideIndex_c::GetIndex( 
      void ) const
{
   return( this->index_ );
}

//===========================================================================//
inline bool TC_SideIndex_c::IsValid( 
      void ) const
{
   return( this->side_ != TC_SIDE_UNDEFINED ? true : false );
}

#endif

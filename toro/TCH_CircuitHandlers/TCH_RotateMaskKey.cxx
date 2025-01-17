//===========================================================================//
// Purpose : Method definitions for the TCH_RotateMaskKey class.
//
//           Public methods include:
//           - TCH_RotateMaskKey_c, ~TCH_RotateMaskKey_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
//
//===========================================================================//

#include "TC_StringUtils.h"

//---------------------------------------------------------------------------//
// Copyright (C) 2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com)      //
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

#include "TIO_PrintHandler.h"

#include "TCH_RotateMaskKey.h"

//===========================================================================//
// Method         : TCH_RotateMaskKey_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
TCH_RotateMaskKey_c::TCH_RotateMaskKey_c( 
      void )
      :
      nodeIndex_( SIZE_MAX )
{
} 

//===========================================================================//
TCH_RotateMaskKey_c::TCH_RotateMaskKey_c( 
      const TGO_Point_c& originPoint, 
            size_t       nodeIndex ) 
      :
      originPoint_( originPoint ),
      nodeIndex_( nodeIndex )
{
} 

//===========================================================================//
TCH_RotateMaskKey_c::TCH_RotateMaskKey_c( 
      int    x,
      int    y,
      int    z,
      size_t nodeIndex ) 
      :
      originPoint_( x, y, z ),
      nodeIndex_( nodeIndex )
{
} 

//===========================================================================//
TCH_RotateMaskKey_c::TCH_RotateMaskKey_c( 
      int    x,
      int    y,
      size_t nodeIndex ) 
      :
      originPoint_( x, y ),
      nodeIndex_( nodeIndex )
{
} 

//===========================================================================//
TCH_RotateMaskKey_c::TCH_RotateMaskKey_c( 
      const TCH_RotateMaskKey_c& rotateMaskKey )
      :
      originPoint_( rotateMaskKey.originPoint_ ),
      nodeIndex_( rotateMaskKey.nodeIndex_ )
{
} 

//===========================================================================//
// Method         : ~TCH_RotateMaskKey_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
TCH_RotateMaskKey_c::~TCH_RotateMaskKey_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
TCH_RotateMaskKey_c& TCH_RotateMaskKey_c::operator=( 
      const TCH_RotateMaskKey_c& rotateMaskKey )
{
   if( &rotateMaskKey != this )
   {
      this->originPoint_ = rotateMaskKey.originPoint_;
      this->nodeIndex_ = rotateMaskKey.nodeIndex_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskKey_c::operator<( 
      const TCH_RotateMaskKey_c& rotateMaskKey ) const
{
   bool lessThan = false;

   if( this->originPoint_ < rotateMaskKey.originPoint_ )
   {
      lessThan = true;
   }
   else if( this->originPoint_ == rotateMaskKey.originPoint_ )
   {
      if( this->nodeIndex_ < rotateMaskKey.nodeIndex_ )
      {
         lessThan = true;
      }
   }
   return( lessThan );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskKey_c::operator==( 
      const TCH_RotateMaskKey_c& rotateMaskKey ) const
{
   return(( this->originPoint_ == rotateMaskKey.originPoint_ ) &&
          ( this->nodeIndex_ == rotateMaskKey.nodeIndex_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskKey_c::operator!=( 
      const TCH_RotateMaskKey_c& rotateMaskKey ) const
{
   return( !this->operator==( rotateMaskKey ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TCH_RotateMaskKey_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<rotate_mask_key" );
   if( this->originPoint_.IsValid( ))
   {
      string srOriginPoint;
      this->originPoint_.ExtractString( &srOriginPoint );
      printHandler.Write( pfile, 0, " origin_point=(%s)", TIO_SR_STR( srOriginPoint ));
   }
   if( this->nodeIndex_ != SIZE_MAX )
   {
      printHandler.Write( pfile, 0, " node_index=%u", this->nodeIndex_ );
   }
   printHandler.Write( pfile, 0, ">\n" );
} 

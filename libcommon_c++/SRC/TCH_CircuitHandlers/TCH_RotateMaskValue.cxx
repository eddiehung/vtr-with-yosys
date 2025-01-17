//===========================================================================//
// Purpose : Method definitions for the TCH_RotateMaskValue class.
//
//           Public methods include:
//           - TCH_RotateMaskValue_c, ~TCH_RotateMaskValue_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - Init
//           - GetBit
//           - SetBit
//           - ClearBit
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

#include "TCH_RotateMaskValue.h"

//===========================================================================//
// Method         : TCH_RotateMaskValue_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
TCH_RotateMaskValue_c::TCH_RotateMaskValue_c( 
      void )
      :
      bitMask_( TCH_ROTATE_MASK_UNDEFINED )
{
} 

//===========================================================================//
TCH_RotateMaskValue_c::TCH_RotateMaskValue_c( 
      bool rotateEnabled )
      :
      bitMask_( TCH_ROTATE_MASK_UNDEFINED )
{
   this->Init( rotateEnabled );
} 

//===========================================================================//
TCH_RotateMaskValue_c::TCH_RotateMaskValue_c( 
      const TCH_RotateMaskValue_c& rotateMaskValue )
      :
      bitMask_( rotateMaskValue.bitMask_ )
{
} 

//===========================================================================//
// Method         : ~TCH_RotateMaskValue_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
TCH_RotateMaskValue_c::~TCH_RotateMaskValue_c( 
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
TCH_RotateMaskValue_c& TCH_RotateMaskValue_c::operator=( 
      const TCH_RotateMaskValue_c& rotateMaskValue )
{
   if( &rotateMaskValue != this )
   {
      this->bitMask_ = rotateMaskValue.bitMask_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskValue_c::operator==( 
      const TCH_RotateMaskValue_c& rotateMaskValue ) const
{
   return(( this->bitMask_ == rotateMaskValue.bitMask_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskValue_c::operator!=( 
      const TCH_RotateMaskValue_c& rotateMaskValue ) const
{
   return( !this->operator==( rotateMaskValue ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TCH_RotateMaskValue_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<rotate_mask_value" );
   if( this->bitMask_ != TCH_ROTATE_MASK_UNDEFINED )
   {
      printHandler.Write( pfile, 0, " bit_mask=0x%02x", this->bitMask_ );

      printHandler.Write( pfile, 0, " [" );
      if( this->GetBit( TGO_ROTATE_R0 ))
      {
         printHandler.Write( pfile, 0, " R0" );
      }
      if( this->GetBit( TGO_ROTATE_R90 ))
      {
         printHandler.Write( pfile, 0, " R90" );
      }
      if( this->GetBit( TGO_ROTATE_R180 ))
      {
         printHandler.Write( pfile, 0, " R180" );
      }
      if( this->GetBit( TGO_ROTATE_R270 ))
      {
         printHandler.Write( pfile, 0, " R270" );
      }
      if( this->GetBit( TGO_ROTATE_MX ))
      {
         printHandler.Write( pfile, 0, " MX" );
      }
      if( this->GetBit( TGO_ROTATE_MXR90 ))
      {
         printHandler.Write( pfile, 0, " MXR90" );
      }
      if( this->GetBit( TGO_ROTATE_MY ))
      {
         printHandler.Write( pfile, 0, " MY" );
      }
      if( this->GetBit( TGO_ROTATE_MYR90 ))
      {
         printHandler.Write( pfile, 0, " MYR90" );
      }
      printHandler.Write( pfile, 0, " ]" );
   }
   printHandler.Write( pfile, 0, ">\n" );
} 

//===========================================================================//
// Method         : Init
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TCH_RotateMaskValue_c::Init(
      bool rotateEnabled )
{
   this->bitMask_ = ( rotateEnabled ? TCH_ROTATE_MASK_ALL : TCH_ROTATE_MASK_R0 );
} 

//===========================================================================//
// Method         : GetBit
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TCH_RotateMaskValue_c::GetBit(
      TGO_RotateMode_t rotateMode ) const
{
   bool bitValue = false;

   switch( rotateMode )
   {
   case TGO_ROTATE_R0:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_R0 ? true : false ); 
      break;
   case TGO_ROTATE_R90:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_R90 ? true : false ); 
      break;
   case TGO_ROTATE_R180:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_R180 ? true : false ); 
      break;
   case TGO_ROTATE_R270:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_R270 ? true : false ); 
      break;
   case TGO_ROTATE_MX:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_MX ? true : false ); 
      break;
   case TGO_ROTATE_MXR90:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_MXR90 ? true : false ); 
      break;
   case TGO_ROTATE_MY:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_MY ? true : false ); 
      break;
   case TGO_ROTATE_MYR90:
      bitValue = ( this->bitMask_ & TCH_ROTATE_MASK_MYR90 ? true : false ); 
      break;
   default:
      break;
   }
   return( bitValue );
} 

//===========================================================================//
// Method         : SetBit
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TCH_RotateMaskValue_c::SetBit(
      TGO_RotateMode_t rotateMode )
{
   switch( rotateMode )
   {
   case TGO_ROTATE_R0:    this->bitMask_ |= TCH_ROTATE_MASK_R0;    break;
   case TGO_ROTATE_R90:   this->bitMask_ |= TCH_ROTATE_MASK_R90;   break;
   case TGO_ROTATE_R180:  this->bitMask_ |= TCH_ROTATE_MASK_R180;  break;
   case TGO_ROTATE_R270:  this->bitMask_ |= TCH_ROTATE_MASK_R270;  break;
   case TGO_ROTATE_MX:    this->bitMask_ |= TCH_ROTATE_MASK_MX;    break;
   case TGO_ROTATE_MXR90: this->bitMask_ |= TCH_ROTATE_MASK_MXR90; break;
   case TGO_ROTATE_MY:    this->bitMask_ |= TCH_ROTATE_MASK_MY;    break;
   case TGO_ROTATE_MYR90: this->bitMask_ |= TCH_ROTATE_MASK_MYR90; break;
   default:                                                        break;
   }
} 

//===========================================================================//
// Method         : ClearBit
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TCH_RotateMaskValue_c::ClearBit(
      TGO_RotateMode_t rotateMode )
{
   switch( rotateMode )
   {
   case TGO_ROTATE_R0:    this->bitMask_ &= ~TCH_ROTATE_MASK_R0;    break;
   case TGO_ROTATE_R90:   this->bitMask_ &= ~TCH_ROTATE_MASK_R90;   break;
   case TGO_ROTATE_R180:  this->bitMask_ &= ~TCH_ROTATE_MASK_R180;  break;
   case TGO_ROTATE_R270:  this->bitMask_ &= ~TCH_ROTATE_MASK_R270;  break;
   case TGO_ROTATE_MX:    this->bitMask_ &= ~TCH_ROTATE_MASK_MX;    break;
   case TGO_ROTATE_MXR90: this->bitMask_ &= ~TCH_ROTATE_MASK_MXR90; break;
   case TGO_ROTATE_MY:    this->bitMask_ &= ~TCH_ROTATE_MASK_MY;    break;
   case TGO_ROTATE_MYR90: this->bitMask_ &= ~TCH_ROTATE_MASK_MYR90; break;
   default:                                                         break;
   }
} 

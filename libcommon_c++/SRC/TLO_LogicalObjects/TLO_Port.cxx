//===========================================================================//
// Purpose : Method definitions for the TLO_Port class.
//
//           Public methods include:
//           - TLO_Port_c, ~TLO_Port_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
//           - Clear
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

#include "TC_MinGrid.h"
#include "TC_StringUtils.h"

#include "TCT_Generic.h"

#include "TIO_PrintHandler.h"

#include "TLO_Port.h"

//===========================================================================//
// Method         : TLO_Port_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
// 07/17/13 jeffr : Added TLO_Power_c member support
//===========================================================================//
TLO_Port_c::TLO_Port_c( 
      void )
      :
      type_( TC_TYPE_UNDEFINED ),
      count_( 0 ),
      isEquivalent_( false ),
      cap_( 0.0 ),
      delay_( 0.0 )
{
} 

//===========================================================================//
TLO_Port_c::TLO_Port_c( 
      const string&       srName,
            TC_TypeMode_t type,
            size_t        count,
            bool          isEquivalent )
      :
      srName_( srName ),
      type_( type ),
      count_( count ),
      isEquivalent_( isEquivalent ),
      cap_( 0.0 ),
      delay_( 0.0 )
{
} 

//===========================================================================//
TLO_Port_c::TLO_Port_c( 
      const char*         pszName,
            TC_TypeMode_t type,
            size_t        count,
            bool          isEquivalent )
      :
      srName_( TIO_PSZ_STR( pszName )),
      type_( type ),
      count_( count ),
      isEquivalent_( isEquivalent ),
      cap_( 0.0 ),
      delay_( 0.0 )
{
} 

//===========================================================================//
TLO_Port_c::TLO_Port_c( 
      const TLO_Port_c& port )
      :
      srName_( port.srName_ ),
      type_( port.type_ ),
      count_( port.count_ ),
      isEquivalent_( port.isEquivalent_ ),
      srClass_( port.srClass_ ),
      cap_( port.cap_ ),
      delay_( port.delay_ ),
      power_( port.power_ )
{
} 

//===========================================================================//
// Method         : ~TLO_Port_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TLO_Port_c::~TLO_Port_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
// 07/17/13 jeffr : Added TLO_Power_c member support
//===========================================================================//
TLO_Port_c& TLO_Port_c::operator=( 
      const TLO_Port_c& port )
{
   if( &port != this )
   {
      this->srName_ = port.srName_;
      this->type_ = port.type_;
      this->count_ = port.count_;
      this->isEquivalent_ = port.isEquivalent_;
      this->srClass_ = port.srClass_;
      this->cap_ = port.cap_;
      this->delay_ = port.delay_;
      this->power_ = port.power_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TLO_Port_c::operator<( 
      const TLO_Port_c& port ) const
{
   return(( TC_CompareStrings( this->srName_, port.srName_ ) < 0 ) ? 
          true : false );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TLO_Port_c::operator==( 
      const TLO_Port_c& port ) const
{
   return(( this->srName_ == port.srName_ ) ? true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TLO_Port_c::operator!=( 
      const TLO_Port_c& port ) const
{
   return( !this->operator==( port ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
// 07/17/13 jeffr : Added TLO_Power_c member support
//===========================================================================//
void TLO_Port_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<pin name=\"%s\"", 
                                        TIO_SR_STR( this->srName_ ));
   string srType;
   TC_ExtractStringTypeMode( this->type_, &srType );
   printHandler.Write( pfile, 0, " type=\"%s\"", 
                                 TIO_SR_STR( srType ));
   if( this->count_ > 0 )
   {
      printHandler.Write( pfile, 0, " count=\"%u\"", 
                                    this->count_ );
   }
   if( this->isEquivalent_ )
   {
      printHandler.Write( pfile, 0, " equivalence=\"%s\"", 
                                    TIO_BOOL_VAL( this->isEquivalent_ ));
   }
   if( this->srClass_.length( ))
   {
      printHandler.Write( pfile, 0, " class=\"%s\"", 
                                    TIO_SR_STR( this->srClass_ ));
   }

   if( TCTF_IsGT( this->cap_, 0.0 ) || 
       TCTF_IsGT( this->delay_, 0.0 ) ||
       this->power_.IsValid( ))
   {
      printHandler.Write( pfile, 0, ">\n" );
      spaceLen += 3;

      if( TCTF_IsGT( this->cap_, 0.0 ) || 
          TCTF_IsGT( this->delay_, 0.0 ))
      {
         TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
         unsigned int precision = MinGrid.GetPrecision( );

         printHandler.Write( pfile, spaceLen, "<timing cap_in=\"%0.*f\" delay_in=\"%0.*e\"/>\n", 
                                              precision, this->cap_, 
                                              precision + 1, this->delay_ );
      }
      if( this->power_.IsValid( ))
      {
	this->power_.Print( pfile, spaceLen );
      }
      spaceLen -= 3;
      printHandler.Write( pfile, spaceLen, "</pin>\n" );
   }
   else
   {
      printHandler.Write( pfile, 0, "/>\n" );
   }
}

//===========================================================================//
// Method         : Clear
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
// 07/17/13 jeffr : Added TLO_Power_c member support
//===========================================================================//
void TLO_Port_c::Clear( 
      void )
{
   this->srName_ = "";
   this->type_ = TC_TYPE_UNDEFINED;
   this->count_ = 0;
   this->isEquivalent_ = false;
   this->srClass_ = "";
   this->cap_ = 0.0;
   this->delay_ = 0.0;
   this->power_.Clear( );
} 

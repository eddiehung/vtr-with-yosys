//===========================================================================//
// Purpose : Method definitions for the TAS_TimingDelayLists class.
//
//           Public methods include:
//           - TAS_TimingDelayLists_c, ~TAS_TimingDelayLists_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - PrintXML
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

#include "TAS_TimingDelayLists.h"

//===========================================================================//
// Method         : TAS_TimingDelayLists_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_TimingDelayLists_c::TAS_TimingDelayLists_c( 
      void )
      :
      delayList( TAS_DELAY_LIST_DEF_CAPACITY ),
      delayMatrixList( TAS_DELAY_MATRIX_LIST_DEF_CAPACITY ),
      tSetupList ( TAS_T_SETUP_LIST_DEF_CAPACITY ),
      tHoldList ( TAS_T_HOLD_LIST_DEF_CAPACITY ),
      clockToQList( TAS_CLOCK_TO_Q_LIST_DEF_CAPACITY ),
      capList( TAS_CAP_LIST_DEF_CAPACITY ),
      capMatrixList( TAS_CAP_MATRIX_LIST_DEF_CAPACITY ),
      packPatternList( TAS_PACK_PATTERN_LIST_DEF_CAPACITY )
{
}

//===========================================================================//
TAS_TimingDelayLists_c::TAS_TimingDelayLists_c( 
      const TAS_TimingDelayLists_c& timingDelayLists )
      :
      delayList( timingDelayLists.delayList ),
      delayMatrixList( timingDelayLists.delayMatrixList ),
      tSetupList ( timingDelayLists.tSetupList  ),
      tHoldList ( timingDelayLists.tHoldList  ),
      clockToQList( timingDelayLists.clockToQList ),
      capList( timingDelayLists.capList ),
      capMatrixList( timingDelayLists.capMatrixList ),
      packPatternList( timingDelayLists.packPatternList )
{
}

//===========================================================================//
// Method         : ~TAS_TimingDelayLists_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_TimingDelayLists_c::~TAS_TimingDelayLists_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_TimingDelayLists_c& TAS_TimingDelayLists_c::operator=( 
      const TAS_TimingDelayLists_c& timingDelayLists )
{
   if( &timingDelayLists != this )
   {
      this->delayList = timingDelayLists.delayList;
      this->delayMatrixList = timingDelayLists.delayMatrixList;
      this->tSetupList = timingDelayLists.tSetupList;
      this->tHoldList = timingDelayLists.tHoldList;
      this->clockToQList = timingDelayLists.clockToQList;
      this->capList = timingDelayLists.capList;
      this->capMatrixList = timingDelayLists.capMatrixList;
      this->packPatternList = timingDelayLists.packPatternList;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
bool TAS_TimingDelayLists_c::operator==( 
      const TAS_TimingDelayLists_c& timingDelayLists ) const
{
   return(( this->delayList == timingDelayLists.delayList ) &&
          ( this->delayMatrixList == timingDelayLists.delayMatrixList ) &&
          ( this->tSetupList == timingDelayLists.tSetupList ) &&
          ( this->tHoldList == timingDelayLists.tHoldList ) &&
          ( this->clockToQList == timingDelayLists.clockToQList ) &&
          ( this->capList == timingDelayLists.capList ) &&
          ( this->capMatrixList == timingDelayLists.capMatrixList ) &&
          ( this->packPatternList == timingDelayLists.packPatternList ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
bool TAS_TimingDelayLists_c::operator!=( 
      const TAS_TimingDelayLists_c& timingDelayLists ) const
{
   return( !this->operator==( timingDelayLists ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
void TAS_TimingDelayLists_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   this->delayList.Print( pfile, spaceLen );
   this->delayMatrixList.Print( pfile, spaceLen );
   this->tSetupList.Print( pfile, spaceLen );
   this->tHoldList.Print( pfile, spaceLen );
   this->clockToQList.Print( pfile, spaceLen );
   this->capList.Print( pfile, spaceLen );
   this->capMatrixList.Print( pfile, spaceLen );
   this->packPatternList.Print( pfile, spaceLen );
}

//===========================================================================//
// Method         : PrintXML
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
void TAS_TimingDelayLists_c::PrintXML( 
      void ) const
{
   FILE* pfile = 0;
   size_t spaceLen = 0;

   this->PrintXML( pfile, spaceLen );
}

//===========================================================================//
void TAS_TimingDelayLists_c::PrintXML( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   for( size_t i = 0; i < this->delayList.GetLength( ); ++i )
   {
      this->delayList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->delayMatrixList.GetLength( ); ++i )
   {
      this->delayMatrixList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->tSetupList.GetLength( ); ++i )
   {
      this->tSetupList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->tHoldList.GetLength( ); ++i )
   {
      this->tHoldList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->clockToQList.GetLength( ); ++i )
   {
      this->clockToQList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->capList.GetLength( ); ++i )
   {
      this->capList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->capMatrixList.GetLength( ); ++i )
   {
      this->capMatrixList[i]->PrintXML( pfile, spaceLen );
   }
   for( size_t i = 0; i < this->packPatternList.GetLength( ); ++i )
   {
      this->packPatternList[i]->PrintXML( pfile, spaceLen );
   }
}

//===========================================================================//
// Method         : IsValid
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
bool TAS_TimingDelayLists_c::IsValid( 
      void ) const
{
   return( this->delayList.IsValid( ) ||
           this->delayMatrixList.IsValid( ) ||
           this->tSetupList.IsValid( ) ||
           this->tHoldList.IsValid( ) ||
           this->clockToQList.IsValid( ) ||
           this->capList.IsValid( ) ||
           this->capMatrixList.IsValid( ) ||
           this->packPatternList.IsValid( ) ?
           true : false );
}

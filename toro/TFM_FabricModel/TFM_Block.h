//===========================================================================//
// Purpose : Declaration and inline definitions for a TFM_Block class.
//
//           Inline methods include:
//           - SetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetCompare (required for TCT_BSearch and TCT_QSort classes)
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

#ifndef TFM_FABRIC_DATA_H
#define TFM_FABRIC_DATA_H

#include <cstdio>
#include <string>
using namespace std;

#include "TGO_Point.h"
#include "TGS_Region.h"

#include "TC_Typedefs.h"
#include "TC_MapTable.h"

#include "TFM_Typedefs.h"
#include "TFM_Pin.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
class TFM_Block_c
{
public:

   TFM_Block_c( void );
   TFM_Block_c( TFM_BlockType_t blockType );
   TFM_Block_c( TFM_BlockType_t blockType,
                const string& srName,
                const string& srMasterName,
                const TGO_Point_c& origin,
                const TGS_Region_c& region );
   TFM_Block_c( TFM_BlockType_t blockType,
                const char* pszName,
                const char* pszMasterName,
                const TGO_Point_c& origin,
                const TGS_Region_c& region );
   TFM_Block_c( TFM_BlockType_t blockType,
                const string& srName,
                const TGO_Point_c& origin,
                const TGS_Region_c& region );
   TFM_Block_c( TFM_BlockType_t blockType,
                const char* pszName,
                const TGO_Point_c& origin,
                const TGS_Region_c& region );
   TFM_Block_c( const string& srName );
   TFM_Block_c( const char* pszName );
   TFM_Block_c( const TFM_Block_c& block );
   ~TFM_Block_c( void );

   TFM_Block_c& operator=( const TFM_Block_c& block );
   bool operator==( const TFM_Block_c& block ) const;
   bool operator!=( const TFM_Block_c& block ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void SetName( const string& srName );
   void SetName( const char* pszName );
   const char* GetName( void ) const;
   const char* GetCompare( void ) const;

   bool IsValid( void ) const;

public:

   TFM_BlockType_t blockType;   // Selects block type (eg. PB, IO, or SB)

   string srName;               // Unique identifier
   string srMasterName;         // Reference to an architecture master

   TGO_Point_c  origin;         // Defines PB|IO|SB origin coordinates (grid)
   TGS_Region_c region;         // Defines PB|IO|SB region coordinates (microns)

   TFM_PinList_t pinList;       // Specifies IO instance|pin list
   TC_MapTable_c mapTable;      // Specifies SB side-index map table

   class TFM_BlockSlice_c
   {
   public:

      unsigned int count;       // Specifies IO slice count (total #of slices)
      unsigned int capacity;    // Specifies IO slice capacity (pins per slice)
   } slice;

   class TFM_BlockTiming_c
   {
   public:

      double res;               // Specifies IO|PB|SB instance|pin input pin R (ohms) 
      double capInput;          // Specifies IO|PB|SB instance|pin input pin C (fF) 
      double capOutput;         // Specifies IO|PB|SB instance|pin output pin C (fF) 
      double delay;             // Specifies IO|PB|SB instance|pin delay (ps)
   } timing;
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
inline void TFM_Block_c::SetName( 
      const string& srName_ )
{
   this->srName = srName_;
}

//===========================================================================//
inline void TFM_Block_c::SetName( 
      const char* pszName )
{
   this->srName = TIO_PSZ_STR( pszName );
}

//===========================================================================//
inline const char* TFM_Block_c::GetName( 
      void ) const
{
   return( TIO_SR_STR( this->srName ));
}

//===========================================================================//
inline const char* TFM_Block_c::GetCompare( 
      void ) const 
{
   return( TIO_SR_STR( this->srName ));
}

//===========================================================================//
inline bool TFM_Block_c::IsValid( 
      void ) const
{
   return( this->srName.length( ) ? true : false );
}

#endif

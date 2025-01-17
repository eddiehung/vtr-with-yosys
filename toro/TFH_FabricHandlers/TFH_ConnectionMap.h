//===========================================================================//
// Purpose : Declaration and inline definitions for the TFH_ConnectionMap 
//           class.
//
//           Inline methods include:
//           - GetChannelOrigin, GetChannelOrient, GetTrackIndex
//           - GetBlockOrigin, GetPinIndex
//           - GetGraphIndex
//           - SetGraphIndex
//           - IsValid
//
//===========================================================================//

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

#ifndef TFH_CONNECTION_MAP_H
#define TFH_CONNECTION_MAP_H

#include <cstdio>
using namespace std;

#include "TGO_Point.h"

#include "TFH_Typedefs.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 04/24/13 jeffr : Original
//===========================================================================//
class TFH_ConnectionMap_c
{
public:

   TFH_ConnectionMap_c( void );
   TFH_ConnectionMap_c( const TGO_Point_c& vpr_channelOrigin,
                        TGO_OrientMode_t channelOrient,
                        int vpr_trackIndex );
   TFH_ConnectionMap_c( const TGO_Point_c& vpr_channelOrigin,
                        TGO_OrientMode_t channelOrient,
                        int vpr_trackIndex,
                        const TGO_Point_c& vpr_blockOrigin,
                        int vpr_pinIndex,
                        int vpr_rrIndex = -1 );

   TFH_ConnectionMap_c( const TFH_ConnectionMap_c& connectionMap );
   ~TFH_ConnectionMap_c( void );

   TFH_ConnectionMap_c& operator=( const TFH_ConnectionMap_c& connectionMap );
   bool operator<( const TFH_ConnectionMap_c& connectionMap ) const;
   bool operator==( const TFH_ConnectionMap_c& connectionMap ) const;
   bool operator!=( const TFH_ConnectionMap_c& connectionMap ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   const TGO_Point_c& GetChannelOrigin( void ) const;
   TGO_OrientMode_t GetChannelOrient( void ) const;
   int GetTrackIndex( void ) const;

   const TGO_Point_c& GetBlockOrigin( void ) const;
   int GetPinIndex( void ) const;

   int GetGraphIndex( void ) const;

   void SetGraphIndex( int vpr_rrIndex );

   bool IsValid( void ) const;

private:

   TGO_Point_c vpr_channelOrigin_; // VPR's grid array channel origin coordinate
   TGO_OrientMode_t channelOrient_;// Defines grid array channel orientaton
   int vpr_trackIndex_;            // Specifies VPR's track index within channel

   TGO_Point_c vpr_blockOrigin_;   // VPR's grid array block origin coordinate
   int vpr_pinIndex_;              // Specifies VPR's pin index within block

   int vpr_rrIndex_;               // Defines asso. VPR's "rr_graph" node index
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 04/24/13 jeffr : Original
//===========================================================================//

inline const TGO_Point_c& TFH_ConnectionMap_c::GetChannelOrigin( 
      void ) const
{
   return( this->vpr_channelOrigin_ );
}

//===========================================================================//
inline TGO_OrientMode_t TFH_ConnectionMap_c::GetChannelOrient(
      void ) const
{
   return( this->channelOrient_ );
}

//===========================================================================//
inline int TFH_ConnectionMap_c::GetTrackIndex( 
      void ) const
{
   return( this->vpr_trackIndex_ );
}

//===========================================================================//
inline const TGO_Point_c& TFH_ConnectionMap_c::GetBlockOrigin( 
      void ) const
{
   return( this->vpr_blockOrigin_ );
}

//===========================================================================//
inline int TFH_ConnectionMap_c::GetPinIndex( 
      void ) const
{
   return( this->vpr_pinIndex_ );
}

//===========================================================================//
inline int TFH_ConnectionMap_c::GetGraphIndex( 
      void ) const
{
   return( this->vpr_rrIndex_ );
}

//===========================================================================//
inline void TFH_ConnectionMap_c::SetGraphIndex( 
      int vpr_rrIndex )
{
   this->vpr_rrIndex_ = vpr_rrIndex;
}

//===========================================================================//
inline bool TFH_ConnectionMap_c::IsValid( 
      void ) const
{
   return( this->vpr_channelOrigin_.IsValid( ) ? true : false );
}

#endif

//===========================================================================//
// Purpose : Declaration and inline definitions for a TGS_Line geometric 
//           shape 2D line class.
//
//           Inline methods include:
//           - GetDx, GetDy
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

#ifndef TGS_LINE_H
#define TGS_LINE_H

#include <cstdio>
#include <cmath>
using namespace std;

#include "TGS_Typedefs.h"
#include "TGS_Point.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
class TGS_Line_c
{
public:

   TGS_Line_c( void );
   TGS_Line_c( double x1, double y1, double x2, double y2, int z,
               TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   TGS_Line_c( double x1, double y1, double x2, double y2,
               TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   TGS_Line_c( const TGS_Point_c& beginPoint, 
               const TGS_Point_c& endPoint,
               int z,
               TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   TGS_Line_c( const TGS_Point_c& beginPoint, 
               const TGS_Point_c& endPoint,
               TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   TGS_Line_c( const TGS_Line_c& line,
               TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   ~TGS_Line_c( void );

   TGS_Line_c& operator=( const TGS_Line_c& line );
   bool operator==( const TGS_Line_c& line ) const;
   bool operator!=( const TGS_Line_c& line ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void ExtractString( string* psrLine,
                       size_t precision = SIZE_MAX ) const;

   void Set( double x1, double y1, double x2, double y2, int z,
             TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void Set( double x1, double y1, double x2, double y2,
             TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void Set( const TGS_Point_c& beginPoint, 
             const TGS_Point_c& endPoint,
             int z,
             TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void Set( const TGS_Point_c& beginPoint, 
             const TGS_Point_c& endPoint,
             TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void Set( const TGS_Line_c& line,
             TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void Reset( void );

   TGS_Point_c GetBeginPoint( TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED ) const;                              
   TGS_Point_c GetEndPoint( TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED ) const;

   double GetDx( void ) const;
   double GetDy( void ) const;

   void ExtendLength( double length,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void ExtendLength( double length,
                      TGS_OrientMode_t orient,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void ExtendLength( double begin, double end,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void ExtendLength( double begin, double end,
                      TGS_OrientMode_t orient,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void ExtendLength( const TGS_Line_c& line,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );

   void ReduceLength( double begin, double end,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );
   void ReduceLength( double begin, double end,
                      TGS_OrientMode_t orient,
                      TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED );

   double FindLength( void ) const;

   TGS_Point_c FindCenter( TGS_SnapMode_t snap = TGS_SNAP_UNDEFINED ) const;

   TGS_OrientMode_t FindOrient( void ) const;

   double FindDistance( const TGS_Line_c& refLine ) const;
   double FindDistance( const TGS_Point_c& refPoint ) const;

   void FindNearest( const TGS_Line_c& refLine,
                     TGS_Point_c* prefNearestPoint = 0, 
                     TGS_Point_c* pthisNearestPoint = 0 ) const;
   void FindNearest( const TGS_Point_c& refPoint, 
                     TGS_Point_c* pthisNearestPoint = 0 ) const;

   void FindIntersect( const TGS_Line_c& refLine, 
                       TGS_Point_c* pthisIntersectPoint = 0 ) const;

   void ApplyScale( double scale );

   void ApplyUnion( const TGS_Line_c& line );
   void ApplyUnion( double begin, double end );

   void ApplyIntersect( const TGS_Line_c& line );
   void ApplyIntersect( double begin, double end );

   void ApplyNormalize( void );

   int CrossProduct( const TGS_Point_c& refPoint ) const;

   bool IsConnected( const TGS_Line_c& line ) const;

   bool IsIntersecting( const TGS_Line_c& line,
                        TGS_Point_c* ppoint = 0 ) const;
   bool IsIntersecting( const TGS_Point_c& point ) const;

   bool IsOverlapping( const TGS_Line_c& line ) const;
   bool IsOverlapping( const TGS_Point_c& point ) const;

   bool IsWithin( const TGS_Line_c& line ) const;

   bool IsParallel( const TGS_Line_c& line ) const;
   bool IsExtension( const TGS_Line_c& line ) const;

   bool IsLeft( void ) const;
   bool IsRight( void ) const;
   bool IsLower( void ) const;
   bool IsUpper( void ) const;

   bool IsLowerLeft( void ) const;
   bool IsLowerRight( void ) const;
   bool IsUpperLeft( void ) const;
   bool IsUpperRight( void ) const;

   bool IsOrthogonal( void ) const;
   bool IsHorizontal( void ) const;
   bool IsVertical( void ) const;

   bool IsValid( void ) const;

public:

   double x1;
   double y1;
   double x2;
   double y2;
   int z;
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
inline double TGS_Line_c::GetDx( 
      void ) const
{
   return( fabs( this->x1 - this->x2 ));
}

//===========================================================================//
inline double TGS_Line_c::GetDy( 
      void ) const
{
   return( fabs( this->y1 - this->y2 ));
}

#endif

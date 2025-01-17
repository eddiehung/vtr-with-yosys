//===========================================================================//
// Purpose : Declaration and inline definitions for the TCH_RelativeNode 
//           class.
//
//           Inline methods include:
//           - SetBlockName
//           - SetVPR_Type
//           - SetVPR_GridPoint
//           - SetRelativePoint
//           - SetRelativeLinkList
//           - GetBlockName
//           - GetVPR_Type
//           - GetVPR_GridPoint
//           - GetRelativePoint
//           - GetRelativeLinkList
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

#ifndef TCH_RELATIVE_NODE_H
#define TCH_RELATIVE_NODE_H

#include <cstdio>
using namespace std;

#include "TC_Typedefs.h"

#include "TGO_Point.h"

#include "TCH_Typedefs.h"

#include "vpr_api.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 06/25/13 jeffr : Original
//===========================================================================//
class TCH_RelativeNode_c
{
public:

   TCH_RelativeNode_c( void );
   TCH_RelativeNode_c( const string& srBlockName,
                       t_type_descriptor* vpr_type = 0 );
   TCH_RelativeNode_c( const char* pszBlockName,
                       t_type_descriptor* vpr_type = 0 );
   TCH_RelativeNode_c( const TCH_RelativeNode_c& relativeNode );
   ~TCH_RelativeNode_c( void );

   TCH_RelativeNode_c& operator=( const TCH_RelativeNode_c& relativeNode );
   bool operator==( const TCH_RelativeNode_c& relativeNode ) const;
   bool operator!=( const TCH_RelativeNode_c& relativeNode ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void SetBlockName( const string& srBlockName );
   void SetBlockName( const char* pszBlockName );
   void SetVPR_Type( const t_type_descriptor* vpr_type );
   void SetVPR_Type( t_type_descriptor* vpr_type );
   void SetVPR_GridPoint( const TGO_Point_c& vpr_gridPoint );
   void SetRelativePoint( const TGO_Point_c& relativePoint );
   void SetRelativeLinkList( const TCH_RelativeLinkList_t& relativeLinkList );

   void AddRelativeLink( const TC_Index_c& relativeLinkIndex );
   void AddRelativeLink( int linkIndex );
   void AddRelativeLink( size_t linkIndex );

   const char* GetBlockName( void ) const;
   t_type_descriptor* GetVPR_Type( void ) const;
   const TGO_Point_c& GetVPR_GridPoint( void ) const;
   const TGO_Point_c& GetRelativePoint( void ) const;
   const TCH_RelativeLinkList_t& GetRelativeLinkList( void ) const;

   bool HasRelativePoint( void ) const;
   bool HasRelativeLink( const TC_Index_c& relativeLinkIndex ) const;
   bool HasRelativeLink( int linkIndex ) const;
   bool HasRelativeLink( size_t linkIndex ) const;

   void Reset( void );

   bool IsValid( void ) const;

private:

   string srBlockName_;          // Block asso. with this relative placement node
   t_type_descriptor* vpr_type_; // VPR type asso. with this relative placement node

   TGO_Point_c vpr_gridPoint_;   // VPR grid array coordinates

   TGO_Point_c relativePoint_;   // Relative placement node coordinate point
   TCH_RelativeLinkList_t relativeLinkList_;
                                 // Relative placement linked node indices
private:

   enum TCH_DefCapacity_e 
   { 
      TCH_RELATIVE_LINK_LIST_DEF_CAPACITY = 2
   };
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 06/25/13 jeffr : Original
//===========================================================================//
inline void TCH_RelativeNode_c::SetBlockName( 
      const string& srBlockName )
{
   this->srBlockName_ = srBlockName;
}

//===========================================================================//
inline void TCH_RelativeNode_c::SetBlockName( 
      const char* pszBlockName )
{
   this->srBlockName_ = TIO_PSZ_STR( pszBlockName );
}

//===========================================================================//
inline void TCH_RelativeNode_c::SetVPR_Type( 
      const t_type_descriptor* vpr_type )
{
   this->vpr_type_ = const_cast< t_type_descriptor* >( vpr_type );
} 

//===========================================================================//
inline void TCH_RelativeNode_c::SetVPR_Type( 
      t_type_descriptor* vpr_type )
{
   this->vpr_type_ = vpr_type;
} 

//===========================================================================//
inline void TCH_RelativeNode_c::SetVPR_GridPoint( 
      const TGO_Point_c& vpr_gridPoint )
{
   this->vpr_gridPoint_ = vpr_gridPoint;
} 

//===========================================================================//
inline void TCH_RelativeNode_c::SetRelativePoint( 
      const TGO_Point_c& relativePoint )
{
   this->relativePoint_ = relativePoint;
} 

//===========================================================================//
inline void TCH_RelativeNode_c::SetRelativeLinkList( 
      const TCH_RelativeLinkList_t& relativeLinkList )
{
   this->relativeLinkList_ = relativeLinkList;
} 

//===========================================================================//
inline const char* TCH_RelativeNode_c::GetBlockName( 
      void ) const
{
   return( TIO_SR_STR( this->srBlockName_ ));
}

//===========================================================================//
inline t_type_descriptor* TCH_RelativeNode_c::GetVPR_Type( 
      void ) const
{
   return( this->vpr_type_ );
} 

//===========================================================================//
inline const TGO_Point_c& TCH_RelativeNode_c::GetVPR_GridPoint( 
      void ) const
{
   return( this->vpr_gridPoint_ );
}

//===========================================================================//
inline const TGO_Point_c& TCH_RelativeNode_c::GetRelativePoint( 
      void ) const
{
   return( this->relativePoint_ );
}

//===========================================================================//
inline const TCH_RelativeLinkList_t& TCH_RelativeNode_c::GetRelativeLinkList( 
      void ) const
{
   return( this->relativeLinkList_ );
}

//===========================================================================//
inline bool TCH_RelativeNode_c::IsValid( 
      void ) const
{
   return( this->srBlockName_.length( ) ? true : false );
}

#endif

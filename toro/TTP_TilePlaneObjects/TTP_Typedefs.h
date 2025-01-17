//===========================================================================//
// Purpose : Enums, typedefs, and defines for TTP_TilePlane classes.
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

#ifndef TTP_TYPEDEFS_H
#define TTP_TYPEDEFS_H

//---------------------------------------------------------------------------//
// Define tile mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_TileMode_e
{
   TTP_TILE_UNDEFINED = 0x00,
   TTP_TILE_CLEAR     = 0x01,
   TTP_TILE_SOLID     = 0x02,
   TTP_TILE_ANY       = 0x03
};
typedef enum TTP_TileMode_e TTP_TileMode_t;

//---------------------------------------------------------------------------//
// Define add mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_AddMode_e
{
   TTP_ADD_UNDEFINED = 0,
   TTP_ADD_NEW,
   TTP_ADD_MERGE,
   TTP_ADD_OVERLAP,
   TTP_ADD_DIFFERENCE,
   TTP_ADD_REGION
};
typedef enum TTP_AddMode_e TTP_AddMode_t;

//---------------------------------------------------------------------------//
// Define delete mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_DeleteMode_e
{
   TTP_DELETE_UNDEFINED = 0,
   TTP_DELETE_EXACT,
   TTP_DELETE_WITHIN,
   TTP_DELETE_INTERSECT,
   TTP_DELETE_INTERSECT_MIN
};
typedef enum TTP_DeleteMode_e TTP_DeleteMode_t;

//---------------------------------------------------------------------------//
// Define find mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_FindMode_e
{
   TTP_FIND_UNDEFINED = 0,
   TTP_FIND_EXACT,
   TTP_FIND_WITHIN,
   TTP_FIND_INTERSECT
};
typedef enum TTP_FindMode_e TTP_FindMode_t;

//---------------------------------------------------------------------------//
// Define merge mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_MergeMode_e
{
   TTP_MERGE_UNDEFINED = 0,
   TTP_MERGE_EXACT,
   TTP_MERGE_REGION
};
typedef enum TTP_MergeMode_e TTP_MergeMode_t;

//---------------------------------------------------------------------------//
// Define join mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_JoinMode_e
{
   TTP_JOIN_UNDEFINED = 0,
   TTP_JOIN_EXACT,
   TTP_JOIN_INTERSECT
};
typedef enum TTP_JoinMode_e TTP_JoinMode_t;

//---------------------------------------------------------------------------//
// Define query mode constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_IsClearMode_e
{
   TTP_IS_CLEAR_UNDEFINED = 0,
   TTP_IS_CLEAR_MATCH,
   TTP_IS_CLEAR_ANY,
   TTP_IS_CLEAR_ALL
};
typedef enum TTP_IsClearMode_e TTP_IsClearMode_t;

enum TTP_IsSolidMode_e
{
   TTP_IS_SOLID_UNDEFINED = 0,
   TTP_IS_SOLID_MATCH,
   TTP_IS_SOLID_ANY,
   TTP_IS_SOLID_ALL,
   TTP_IS_SOLID_MAX
};
typedef enum TTP_IsSolidMode_e TTP_IsSolidMode_t;

//---------------------------------------------------------------------------//
// Define internal message constants and typedefs
//---------------------------------------------------------------------------//

enum TTP_MessageType_e
{
   TTP_MESSAGE_UNDEFINED = 0,
   TTP_MESSAGE_IS_LEGAL_INVALID_REGION,
   TTP_MESSAGE_IS_LEGAL_ADJACENT_CLEAR_TILE,
   TTP_MESSAGE_TILE_REGION_ILLEGAL,
   TTP_MESSAGE_ADD_PER_NEW_INVALID,
   TTP_MESSAGE_ADD_PER_NEW_EXISTS,
   TTP_MESSAGE_ADD_PER_MERGE_INVALID,
   TTP_MESSAGE_ADD_PER_MERGE_EXISTS,
   TTP_MESSAGE_ADD_PER_OVERLAP_INVALID,
   TTP_MESSAGE_DELETE_PER_EXACT_INVALID,
   TTP_MESSAGE_DELETE_PER_WITHIN_INVALID,
   TTP_MESSAGE_DELETE_PER_INTERSECT_INVALID
};
typedef enum TTP_MessageType_e TTP_MessageType_t;

#endif 

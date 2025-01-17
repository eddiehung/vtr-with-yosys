//===========================================================================//
// Purpose : Enums, typedefs, and defines for ANSI C++ compatibility.
//
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
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

#ifndef TC_TYPEDEFS_H
#define TC_TYPEDEFS_H

#if defined( SUN8 )
   #define _XOPEN_SOURCE
#endif

//---------------------------------------------------------------------------//
// Define float constants as needed
//---------------------------------------------------------------------------//

#include <cfloat>
using namespace std;

#define TC_FLT_EPSILON ( static_cast< double >( FLT_RADIX * FLT_EPSILON ))
#define TC_FLT_MIN ( static_cast< double >( -1.0 * FLT_MAX + ( FLT_RADIX * FLT_EPSILON )))
#define TC_FLT_MAX ( static_cast< double >( 1.0 * FLT_MAX - ( FLT_RADIX * FLT_EPSILON )))

//---------------------------------------------------------------------------//
// Define limit constants as needed
//---------------------------------------------------------------------------//

#if defined( LINUX_I686 )
   #include <stdint.h>
   #ifndef SIZE_MAX
      #define SIZE_MAX UINT_MAX
   #endif
#elif defined( LINUX_X86_64 )
   #include <stdint.h>
   #ifndef SIZE_MAX
      #define SIZE_MAX ULONG_MAX
   #endif
#elif defined( SUN8 )
   #ifndef SIZE_MAX
      #define SIZE_MAX UINT_MAX
   #endif
#else
   #include <stdint.h>
   #ifndef SIZE_MAX
      #define SIZE_MAX UINT_MAX
   #endif
#endif

#include <cstdlib>
using namespace std;

#ifdef RAND_MAX
   #define TC_RAND_MAX RAND_MAX
#else
   #define TC_RAND_MAX INT_MAX
#endif

//---------------------------------------------------------------------------//
// Define math constants as needed
//---------------------------------------------------------------------------//

#ifndef M_SQRT2
   #define M_SQRT2 1.41421356237309504880
#endif
#define TC_SQRT2 ( M_SQRT2 )

//---------------------------------------------------------------------------//
// Define 'nothrow' keyword if not currently available (ie, make NOP for new) 
//---------------------------------------------------------------------------//

#include <new>
using namespace std;

#define TC_NOTHROW (std::nothrow) // Set 'nothrow' behavior for default

//---------------------------------------------------------------------------//
// Define common mode constants and typedefs
//---------------------------------------------------------------------------//

enum TC_BitMode_e
{
   TC_BIT_UNDEFINED = 0,
   TC_BIT_TRUE,
   TC_BIT_FALSE,
   TC_BIT_DONT_CARE
};
typedef enum TC_BitMode_e TC_BitMode_t;

enum TC_DataMode_e
{
   TC_DATA_UNDEFINED = 0,
   TC_DATA_INT,
   TC_DATA_UINT,
   TC_DATA_LONG,
   TC_DATA_ULONG,
   TC_DATA_SIZE,
   TC_DATA_FLOAT,
   TC_DATA_EXP,
   TC_DATA_STRING
};
typedef enum TC_DataMode_e TC_DataMode_t;

enum TC_SideMode_e
{
   TC_SIDE_UNDEFINED = 0x00,
   TC_SIDE_NONE      = 0x00,
   TC_SIDE_LEFT      = 0x01,
   TC_SIDE_RIGHT     = 0x02,
   TC_SIDE_LOWER     = 0x04,
   TC_SIDE_UPPER     = 0x08,
   TC_SIDE_ANY       = TC_SIDE_LEFT + TC_SIDE_RIGHT + TC_SIDE_LOWER + TC_SIDE_UPPER,
   TC_SIDE_BOTTOM    = 0x10,
   TC_SIDE_TOP       = 0x20,
   TC_SIDE_PREV      = 0x40,
   TC_SIDE_NEXT      = 0x80
};
typedef TC_SideMode_e TC_SideMode_t;
typedef TC_SideMode_e TC_SideMask_t;

enum TC_TypeMode_e
{
   TC_TYPE_UNDEFINED = 0,
   TC_TYPE_INPUT,
   TC_TYPE_OUTPUT,
   TC_TYPE_SIGNAL,
   TC_TYPE_CLOCK,
   TC_TYPE_POWER,
   TC_TYPE_GLOBAL
};
typedef enum TC_TypeMode_e TC_TypeMode_t;

//---------------------------------------------------------------------------//
// Define common list constants as needed
//---------------------------------------------------------------------------//

#define TC_DEFAULT_CAPACITY ( static_cast< size_t >( 64 ))

#endif 

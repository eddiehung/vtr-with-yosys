//===========================================================================//
// Purpose : Declaration and inline definition(s) for a TIO_FileOutput class.
//
//           Inline methods include:
//           - TIO_FileOutput_c, ~TIO_FileOutput_c
//           - Flush
//           - GetName
//           - GetStream
//           - GetLineNum
//           - SetEnabled
//           - IsEnabled
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

#ifndef TIO_FILE_OUTPUT_H
#define TIO_FILE_OUTPUT_H

#include <string>
using namespace std;

#include "TIO_Typedefs.h"
#include "TIO_FileHandler.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
class TIO_FileOutput_c
{
public:

   TIO_FileOutput_c( const char* pszFileName = 0,
                     TIO_FileOpenMode_t fileOpen = TIO_FILE_OPEN_UNDEFINED );
   ~TIO_FileOutput_c( void );
 
   bool Open( const char* pszFileName, 
              TIO_FileOpenMode_t fileOpen );
   bool Open( const string& srFileName, 
              TIO_FileOpenMode_t fileOpen );
   void Close( void );

   bool Write( const char* pszString );

   void Flush( void );

   const char* GetName( void ) const;
   FILE* GetStream( void ) const;
   size_t GetLineNum( void ) const;

   void SetEnabled( bool isEnabled );

   bool IsEnabled( void ) const;

   bool IsValid( const char* pszFileName, 
                 TIO_FileOpenMode_t fileOpen ) const;
   bool IsValid( const string& srFileName, 
                 TIO_FileOpenMode_t fileOpen ) const;
   bool IsValid( void ) const;

private:

   TIO_FileHandler_c fileHandler_;   // Define this file's handler object
   size_t            lineNum_;       // Track this file's current line number
   bool              isEnabled_;     // Used to enable/disable output state
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
inline TIO_FileOutput_c::TIO_FileOutput_c( 
      const char*              pszFileName,
            TIO_FileOpenMode_t fileOpen )
      :
      fileHandler_( pszFileName, fileOpen ),
      lineNum_( 0 ),
      isEnabled_( pszFileName ? true : false )
{
}

//===========================================================================//
inline TIO_FileOutput_c::~TIO_FileOutput_c( 
      void )
{
   this->fileHandler_.Close( );
}

//===========================================================================//
inline void TIO_FileOutput_c::Flush( 
      void )
{
   this->fileHandler_.Flush( );
}

//===========================================================================//
inline const char* TIO_FileOutput_c::GetName( 
      void ) const
{
   return( this->fileHandler_.GetFileName( ));
}

//===========================================================================//
inline FILE* TIO_FileOutput_c::GetStream( 
      void ) const
{
   return( this->fileHandler_.GetFileStream( ));
}

//===========================================================================//
inline size_t TIO_FileOutput_c::GetLineNum( 
      void ) const
{
   return( this->lineNum_ );
}

//===========================================================================//
inline void TIO_FileOutput_c::SetEnabled( 
      bool isEnabled )
{
   this->isEnabled_ = isEnabled;
}

//===========================================================================//
inline bool TIO_FileOutput_c::IsEnabled( 
      void ) const
{
   return( this->isEnabled_ );
}

//===========================================================================//
inline bool TIO_FileOutput_c::IsValid(
      const char*              pszFileName,
            TIO_FileOpenMode_t fileOpen ) const
{
   return( this->fileHandler_.IsValid( pszFileName, fileOpen ));
}

//===========================================================================//
inline bool TIO_FileOutput_c::IsValid(
      const string&            srFileName,
            TIO_FileOpenMode_t fileOpen ) const
{
   return( this->fileHandler_.IsValid( srFileName, fileOpen ));
} 

//===========================================================================//
inline bool TIO_FileOutput_c::IsValid( 
      void ) const
{
   return( this->fileHandler_.IsValid( ));
}

#endif 

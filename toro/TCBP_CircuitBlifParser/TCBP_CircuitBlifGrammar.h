//===========================================================================//
// Purpose : Methods for the 'TCBP_CircuitBlifParser_c' class.
//
//           Inline public methods include:
//           - syn
//           - SetScanner
//           - SetFileName
//           - SetCircuitBlifFile
//           - SetCircuitDesign
//
//           Inline private methods include:
//           - FindLatchType_
//           - FindLatchState_
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

//===========================================================================//
// Method         : syn
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TCBP_CircuitBlifParser_c::syn( 
      ANTLRAbstractToken* /* pToken */, 
      ANTLRChar*          pszGroup,
      SetWordType*        /* pWordType */,
      ANTLRTokenType      tokenType,
      int                 /* k */ )
{
   if( LT( 1 ) && ( LT( 1 )->getType( ) != END_OF_FILE ))
   {
      int lineNum = LT( 1 )->getLine( );

      string srFoundToken = this->token_tbl[LT( 1 )->getType( )];
      string srFoundText  = LT( 1 )->getText( );

      string srMsg;

      if( LT( 1 )->getType( ) == UNCLOSED_STRING )
      {
         srMsg = "at newline character. Closing quote is missing";
      }
      else
      {
         srMsg  = "at " + srFoundToken + " token, ";
         srMsg += "\"" + srFoundText + "\"";

         if( tokenType && ( tokenType != DLGminToken ))
         {
            string srExpectToken = this->token_tbl[tokenType];
            srMsg += ", expected a " + srExpectToken + " token";
         }

         if( strlen( pszGroup ) > 0 )
         {
            srMsg += " in ";
            srMsg += pszGroup;
         }
      }

      this->pcircuitBlifFile_->SyntaxError( lineNum, 
                                            this->srFileName_, 
                                            srMsg.data( ));
      this->consumeUntilToken( END_OF_FILE );
   }
}

//===========================================================================//
// Method         : SetScanner
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TCBP_CircuitBlifParser_c::SetScanner( 
      TCBP_CircuitBlifScanner_c* pscanner )
{
   this->pscanner_ = pscanner;
}

//===========================================================================//
// Method         : SetFileName
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TCBP_CircuitBlifParser_c::SetFileName(
      const char* pszFileName )
{
   this->srFileName_ = TIO_PSZ_STR( pszFileName );
}

//===========================================================================//
// Method         : SetCircuitBlifFile
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TCBP_CircuitBlifParser_c::SetCircuitBlifFile(
      TCBP_CircuitBlifFile_c* pcircuitBlifFile )
{
   this->pcircuitBlifFile_ = pcircuitBlifFile;
}

//===========================================================================//
// Method         : SetCircuitDesign
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TCBP_CircuitBlifParser_c::SetCircuitDesign(
      TCD_CircuitDesign_c* pcircuitDesign )
{
   this->pcircuitDesign_ = pcircuitDesign;

   this->circuitSubcktCount_ = 0;
}

//===========================================================================//
// Method         : FindLatchType_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_LatchType_t TCBP_CircuitBlifParser_c::FindLatchType_(
      ANTLRTokenType tokenType )
{
   TPO_LatchType_t type = TPO_LATCH_TYPE_UNDEFINED;
   switch( tokenType )
   {
   case TYPE_FALLING_EDGE: type = TPO_LATCH_TYPE_FALLING_EDGE; break;
   case TYPE_RISING_EDGE:  type = TPO_LATCH_TYPE_RISING_EDGE;  break;
   case TYPE_ACTIVE_HIGH:  type = TPO_LATCH_TYPE_ACTIVE_HIGH;  break;
   case TYPE_ACTIVE_LOW:   type = TPO_LATCH_TYPE_ACTIVE_LOW;   break;
   case TYPE_ASYNCHRONOUS: type = TPO_LATCH_TYPE_ASYNCHRONOUS; break;
   }
   return( type );
}

//===========================================================================//
// Method         : FindLatchState_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_LatchState_t TCBP_CircuitBlifParser_c::FindLatchState_(
      ANTLRTokenType tokenType )
{
   TPO_LatchState_t state = TPO_LATCH_STATE_UNDEFINED;
   switch( tokenType )
   {
   case STATE_TRUE:      state = TPO_LATCH_STATE_TRUE;      break;
   case STATE_FALSE:     state = TPO_LATCH_STATE_FALSE;     break;
   case STATE_DONT_CARE: state = TPO_LATCH_STATE_DONT_CARE; break;
   case STATE_UNKNOWN:   state = TPO_LATCH_STATE_UNKNOWN;   break;
   }
   return( state );
}

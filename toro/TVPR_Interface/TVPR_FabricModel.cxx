//===========================================================================//
// Purpose : Method definitions for the TVPR_FabricModel_c class.
//
//           Public methods include:
//           - TVPR_FabricModel_c, ~TVPR_FabricModel_c
//           - Export
//           - Import
//
//           Private methods include:
//           - GenerateFabricView_
//           - GenerateBlockPlane_
//           - GenerateChannelPlane_
//           - GenerateSegmentPlane_
//           - GenerateConnectionPlane_
//           - PokeFabricView_
//           - PokeGridConfig_
//           - PokeGridBlocks_
//           - PokeSwitchBoxes_
//           - PokeConnectionBlocks_
//           - PokeChannelWidths_
//           - PeekFabricView_
//           - PeekInputOutputs_
//           - PeekPhysicalBlocks_
//           - PeekChannels_
//           - PeekSegments_
//           - PeekSwitchBoxes_
//           - PeekConnectionBoxes_
//           - ExtractFabricView_
//           - ExtractBlockPlane_
//           - ExtractChannelPlane_
//           - ExtractSegmentPlane_
//           - ExtractPinList_
//           - AddBlockPinList_
//           - AddBlockMapTable_
//           - UpdatePinList_
//           - BuildChannelDefaults_
//           - UpdateChannelCounts_
//           - ResizeChannelWidths_
//           - ResizeChannelLengths_
//           - BuildSwitchBoxes_
//           - ClearSwitchBoxes_
//           - UpdateSwitchMapTables_
//           - BuildConnectionBoxes_
//           - BuildConnectionRegion_
//           - UpdateConnectionPoi<nts_
//           - CalcMaxPinCount_
//           - CalcPinCountArray_
//           - CalcPinDeltaArray_
//           - FindPinName_
//           - FindPinSide_
//           - FindPinDelta_
//           - FindChannelCount_
//           - FindChannelRegion_
//           - FindSegmentRegion_
//           - FindBlockRegion_
//           - FindNodeRegion_
//           - FindSwitchSide_
//           - LoadBlockNameIndexList_
//           - MapDataTypeToLayer_
//           - MapDataTypeToBlockType_
//           - MapBlockTypeToDataType_
//           - AddFabricViewRegion_
//           - DeleteFabricViewRegion_
//           - ReplaceFabricViewRegion_
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

#include <string>
using namespace std;

#include "TC_NameIndex.h"
#include "TC_StringUtils.h"

#include "TIO_PrintHandler.h"

#include "TFH_FabricGridHandler.h"
#include "TFH_FabricBlockHandler.h"
#include "TFH_FabricSwitchBoxHandler.h"
#include "TFH_FabricConnectionBlockHandler.h"
#include "TFH_FabricChannelHandler.h"

#include "TVPR_FabricModel.h"

//===========================================================================//
// Method         : TVPR_FabricModel_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/10/12 jeffr : Original
//===========================================================================//
TVPR_FabricModel_c::TVPR_FabricModel_c( 
      void )
{
}

//===========================================================================//
// Method         : ~TVPR_FabricModel_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/10/12 jeffr : Original
//===========================================================================//
TVPR_FabricModel_c::~TVPR_FabricModel_c( 
      void )
{
}

//===========================================================================//
// Method         : Export
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::Export(
      const TFM_FabricModel_c& fabricModel,
            t_arch*            pvpr_architecture,
      const t_type_descriptor* vpr_physicalBlockArray, 
            int                vpr_physicalBlockCount,
            bool               overrideBlocks,
            bool               overrideSwitchBoxes,
            bool               overrideConnectionBlocks,
            bool               overrideChannels ) const
{
   bool ok = true;

   // Start by extracting fabric model contents to generate a fabric view
   TFM_FabricModel_c* pfabricModel = const_cast< TFM_FabricModel_c* >( &fabricModel );
   TFV_FabricView_c* pfabricView = pfabricModel->GetFabricView( );
   ok = this->GenerateFabricView_( fabricModel, pfabricView );

   // Then, poke at VPR's internal data structures based on fabric view
   if( ok )
   {
      ok = this->PokeFabricView_( fabricModel, *pfabricView, 
                                  pvpr_architecture,
                                  vpr_physicalBlockArray, 
                                  vpr_physicalBlockCount,
                                  overrideBlocks, 
                                  overrideSwitchBoxes,
                                  overrideConnectionBlocks,
                                  overrideChannels );
   }
   return( ok );
}

//===========================================================================//
// Method         : Import
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::Import(
            t_grid_tile**      vpr_gridArray,
            int                vpr_nx,
            int                vpr_ny,
      const t_rr_node*         vpr_rrNodeArray,
            int                vpr_rrNodeCount,
      const int*               vpr_xChanWidthArray,
      const int*               vpr_yChanWidthArray,
            TFM_FabricModel_c* pfabricModel ) const
{
   bool ok = true;

   // Define local grid dimensions (overall) based on VPR nx/ny values
   TGS_IntDims_t vpr_gridDims( vpr_nx + 2, vpr_ny + 2 );

   // Start by peeking at VPR's internal data structures to load fabric view
   TFV_FabricView_c* pfabricView = pfabricModel->GetFabricView( );
   ok = this->PeekFabricView_( vpr_gridArray, vpr_gridDims,
                               vpr_rrNodeArray, vpr_rrNodeCount,
                               vpr_xChanWidthArray, vpr_yChanWidthArray,
                               pfabricView );

   // Then, extract fabric view contents to generate a fabric model
   if( ok )
   {
      this->ExtractFabricView_( *pfabricView,
                                pfabricModel );
   }
   return( ok );
}

//===========================================================================//
// Method         : GenerateFabricView_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::GenerateFabricView_(
      const TFM_FabricModel_c& fabricModel,
            TFV_FabricView_c*  pfabricView ) const
{
   bool ok = true;

   pfabricView->Clear( );

   if( fabricModel.IsValid( ) &&
       fabricModel.config.IsValid( ))
   {
      const string& srName = fabricModel.srName;
      const TFM_Config_c& config = fabricModel.config;
      const TFM_PhysicalBlockList_t& physicalBlockList = fabricModel.physicalBlockList;
      const TFM_InputOutputList_t& inputOutputList = fabricModel.inputOutputList;
      const TFM_SwitchBoxList_t& switchBoxList = fabricModel.switchBoxList;
      const TFM_ChannelList_t& channelList = fabricModel.channelList;
      const TFM_SegmentList_t& segmentList = fabricModel.segmentList;

      TGS_Region_c fabricRegion = config.fabricRegion;
      if( !fabricRegion.IsValid( ))
      {
         const TGO_Region_c& ioRegion = config.ioPolygon.GetRegion( );
         fabricRegion.Set( static_cast< double >( ioRegion.x1 ) - 1.0, 
                           static_cast< double >( ioRegion.y1 ) - 1.0, 
                           static_cast< double >( ioRegion.x2 ) + 1.0, 
                           static_cast< double >( ioRegion.y2 ) + 1.0 );
      }
      const TGO_Polygon_c* pfabricPolygon = &config.ioPolygon;
      pfabricView->Init( TIO_SR_STR( srName ), fabricRegion, pfabricPolygon );

      this->GenerateBlockPlane_( physicalBlockList, pfabricView );
      this->GenerateBlockPlane_( inputOutputList, pfabricView );
      this->GenerateBlockPlane_( switchBoxList, pfabricView );

      this->GenerateChannelPlane_( channelList, pfabricView );
      this->GenerateSegmentPlane_( segmentList, pfabricView );

      if( ok )
      {
         ok = GenerateConnectionPlane_( physicalBlockList, pfabricView );
      }
      if( ok )
      {
         ok = GenerateConnectionPlane_( inputOutputList, pfabricView );
      }
   }
   return( ok );
}

//===========================================================================//
// Method         : GenerateBlockPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::GenerateBlockPlane_(
      const TFM_BlockList_t&  blockList,
            TFV_FabricView_c* pfabricView ) const
{
   for( size_t i = 0; i < blockList.GetLength( ); ++i )
   {
      const TFM_Block_c& block_ = *blockList[i];
      const TGS_Region_c& blockRegion = block_.region;

      TFV_DataType_t dataType = this->MapBlockTypeToDataType_( block_.blockType );

      const char* pszName = TIO_SR_STR( block_.srName );
      const char* pszMasterName = TIO_SR_STR( block_.srMasterName );
      const TGO_Point_c& origin = block_.origin;

      unsigned int sliceCount = block_.slice.count;
      unsigned int sliceCapacity = block_.slice.capacity;

      TFV_FabricData_c* pfabricData = 0;
      this->AddFabricViewRegion_( blockRegion, dataType,
                                  pszName, pszMasterName, origin,
                                  sliceCount, sliceCapacity,
                                  pfabricView, &pfabricData );
      if( pfabricData )
      {
         // Add given block's pins, if any, to fabric block's pin list
         if( block_.pinList.IsValid( ))
         {
            this->AddBlockPinList_( block_.pinList, pfabricData );
         }

         // Add given block's map table, if any, to fabric block's map table
         if( block_.mapTable.IsValid( ))
         {
            this->AddBlockMapTable_( block_.mapTable, pfabricData );
         }
      }
   }
}

//===========================================================================//
// Method         : GenerateChannelPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::GenerateChannelPlane_(
      const TFM_ChannelList_t& channelList,
            TFV_FabricView_c*  pfabricView ) const
{
   for( size_t i = 0; i < channelList.GetLength( ); ++i )
   {
      const TFM_Channel_c& channel = *channelList[i];
      const TGS_Region_c& channelRegion = channel.region;

      TFV_DataType_t dataType = ( channel.orient == TGS_ORIENT_HORIZONTAL ?
                                  TFV_DATA_CHANNEL_HORZ : TFV_DATA_CHANNEL_VERT );

      const char* pszName = TIO_SR_STR( channel.srName );
      TGO_Point_c origin;

      unsigned int trackIndex = channel.index;
      unsigned int trackHorzCount = ( channel.orient == TGS_ORIENT_HORIZONTAL ?
                                      channel.count : 0 );
      unsigned int trackVertCount = ( channel.orient == TGS_ORIENT_HORIZONTAL ?
                                      0 : channel.count );

      this->AddFabricViewRegion_( channelRegion, dataType,
                                  pszName, origin,
                                  trackIndex, trackHorzCount, trackVertCount, 
                                  pfabricView );
   }
}

//===========================================================================//
// Method         : GenerateSegmentPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::GenerateSegmentPlane_(
      const TFM_SegmentList_t& segmentList,
            TFV_FabricView_c*  pfabricView ) const
{
   for( size_t i = 0; i < segmentList.GetLength( ); ++i )
   {
      const TFM_Segment_c& segment = *segmentList[i];

      TGS_Region_c segmentRegion;
      segment.path.FindRegion( &segmentRegion );

      TGS_OrientMode_t segmentOrient = segment.path.FindOrient( );
      TFV_DataType_t dataType = ( segmentOrient == TGS_ORIENT_HORIZONTAL ?
                                  TFV_DATA_SEGMENT_HORZ : TFV_DATA_SEGMENT_VERT );

      const char* pszName = TIO_SR_STR( segment.srName );
      unsigned int trackIndex = segment.index;

      this->AddFabricViewRegion_( segmentRegion, dataType, 
                                  pszName, 
                                  trackIndex, 
                                  pfabricView );
   }
}

//===========================================================================//
// Method         : GenerateConnectionPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::GenerateConnectionPlane_(
      const TFM_BlockList_t&  blockList,
            TFV_FabricView_c* pfabricView ) const
{
   bool ok = true;

   for( size_t i = 0; i < blockList.GetLength( ); ++i )
   {
      const TFM_Block_c& block_ = *blockList[i];
      const TGS_Region_c& blockRegion = block_.region;

      TFV_FabricFigure_t* pfabricFigure = 0;
      if( !pfabricView->IsSolidAll( TFV_LAYER_INPUT_OUTPUT, blockRegion, &pfabricFigure ) &&
          !pfabricView->IsSolidAll( TFV_LAYER_PHYSICAL_BLOCK, blockRegion, &pfabricFigure ))
      {
         continue;
      }

      // Iterate for every pin associated with this PB|IO figure
      TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
      const TFV_FabricPinList_t& fabricPinList = pfabricData->GetPinList( );
      for( size_t j = 0; j < fabricPinList.GetLength( ); ++j )
      {
         const TFV_FabricPin_c& fabricPin = *fabricPinList[j];

         // Define a connection box region spanning from pin to channel region
         TGS_Region_c connectionRegion;
         ok = this->BuildConnectionRegion_( *pfabricView, fabricPin, 
                                            blockRegion, &connectionRegion );
         if( !ok )
            break;

         TFV_DataType_t dataType = TFV_DATA_CONNECTION_BOX;
         this->AddFabricViewRegion_( connectionRegion, dataType,
                                     pfabricView, &pfabricData );
         if( pfabricData )
         {
            // Update connection box figure per asso. side and track index
            pfabricData->AddConnection( fabricPin.GetConnectionList( ));
         }
      }
      if( !ok )
         break;
   }
   return( ok );
}

//===========================================================================//
// Method         : PokeFabricView_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::PokeFabricView_( 
      const TFM_FabricModel_c& fabricModel,
      const TFV_FabricView_c&  fabricView,
            t_arch*            pvpr_architecture,
      const t_type_descriptor* vpr_physicalBlockArray, 
            int                vpr_physicalBlockCount,
            bool               overrideBlocks,
            bool               overrideSwitchBoxes,
            bool               overrideConnectionBlocks,
            bool               overrideChannels ) const
{
   bool ok = true;

   this->PokeGridConfig_( fabricModel, fabricView,
                          pvpr_architecture );

   if( ok && overrideBlocks )
   {
      const TFM_PhysicalBlockList_t& physicalBlockList = fabricModel.physicalBlockList;
      ok = this->PokeGridBlocks_( physicalBlockList,
                                  vpr_physicalBlockArray, 
                                  vpr_physicalBlockCount );
   }
   if( ok && overrideBlocks )
   {
      const TFM_InputOutputList_t& inputOutputList = fabricModel.inputOutputList;
      ok = this->PokeGridBlocks_( inputOutputList,
                                  vpr_physicalBlockArray, 
                                  vpr_physicalBlockCount );
   }
   if( ok && overrideSwitchBoxes )
   {
      const TFM_SwitchBoxList_t& switchBoxList = fabricModel.switchBoxList;
      this->PokeSwitchBoxes_( switchBoxList );
   }
   if( ok && overrideConnectionBlocks )
   {
      const TFM_PhysicalBlockList_t& physicalBlockList = fabricModel.physicalBlockList;
      this->PokeConnectionBlocks_( physicalBlockList );
   }
   if( ok && overrideConnectionBlocks )
   {
      const TFM_InputOutputList_t& inputOutputList = fabricModel.inputOutputList;
      this->PokeConnectionBlocks_( inputOutputList );
   }
   if( ok && overrideChannels )
   {
      const TFM_ChannelList_t& channelList = fabricModel.channelList;
      this->PokeChannelWidths_( channelList );
   }

   // WIP - Poke rest of fabric view data directly into VPR structures (eg. segments)...

   return( ok );
}

//===========================================================================//
// Method         : PokeGridConfig_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PokeGridConfig_(
      const TFM_FabricModel_c& fabricModel,
      const TFV_FabricView_c&  fabricView,
            t_arch*            pvpr_architecture ) const
{
   // Get fabric view's region and apply inverse (see "PeekFabricView_")
   if( fabricView.GetRegion( ).IsValid( ))
   {
      TGS_Region_c gridRegion = fabricView.GetRegion( );

      gridRegion.ApplyScale( -1.0 );
      TGS_IntDims_t vpr_gridDims( static_cast< int >( gridRegion.GetDx( )) + 1,
                                  static_cast< int >( gridRegion.GetDy( )) + 1 );

      pvpr_architecture->clb_grid.IsAuto = static_cast< boolean >( false );
      pvpr_architecture->clb_grid.W = vpr_gridDims.dx - 2;
      pvpr_architecture->clb_grid.H = vpr_gridDims.dy - 2;
   }

   // Get fabric view's polygon, if any, and apply IO ring structure
   if( fabricModel.config.ioPolygon.IsValid( ))
   {
      const TGO_Polygon_c& gridPolygon = fabricModel.config.ioPolygon;
      const TGO_Region_c& gridRegion = gridPolygon.GetRegion( );
      TGS_IntDims_t vpr_gridDims( gridRegion.GetDx( ) + 1,
                                  gridRegion.GetDy( ) + 1 );

      pvpr_architecture->clb_grid.IsAuto = static_cast< boolean >( false );
      pvpr_architecture->clb_grid.W = vpr_gridDims.dx - 2;
      pvpr_architecture->clb_grid.H = vpr_gridDims.dy - 2;

      TFH_FabricGridHandler_c& fabricGridHandler = TFH_FabricGridHandler_c::GetInstance( );
      fabricGridHandler.SetPolygon( gridPolygon );
   }
}

//===========================================================================//
// Method         : PokeGridBlocks_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
// 07/30/13 jeffr : Fixed code to call "fabricBlockHandler.Add()" for IOs too
//===========================================================================//
bool TVPR_FabricModel_c::PokeGridBlocks_(
      const TFM_BlockList_t&   blockList,
      const t_type_descriptor* vpr_physicalBlockArray, 
            int                vpr_physicalBlockCount ) const
{
   bool ok = true;

   TFH_FabricBlockHandler_c& fabricBlockHandler = TFH_FabricBlockHandler_c::GetInstance( );

   TVPR_NameIndexList_t blockNameIndexList;
   this->LoadBlockNameIndexList_( vpr_physicalBlockArray, vpr_physicalBlockCount, 
                                  &blockNameIndexList );

   for( size_t i = 0; i < blockList.GetLength( ); ++i )
   {
      const TFM_Block_c& block_ = *blockList[i];

      const string& srMasterName = block_.srMasterName;
      const string& srBlockName = block_.srName;

      TFH_BlockType_t blockType = TFH_BLOCK_UNDEFINED;
      switch( block_.blockType )
      {
      case TFM_BLOCK_PHYSICAL_BLOCK: blockType = TFH_BLOCK_PHYSICAL_BLOCK; break;
      case TFM_BLOCK_INPUT_OUTPUT:   blockType = TFH_BLOCK_INPUT_OUTPUT;   break;
      default:                                                             break;
      }

      int vpr_typeIndex = -1;
      TC_NameIndex_c blockNameIndex( srMasterName );
      if( blockNameIndexList.Find( blockNameIndex, &blockNameIndex ))
      {
         vpr_typeIndex = static_cast< int >( blockNameIndex.GetIndex( ));
      }
      else
      {
         TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
         ok = printHandler.Error( "Invalid fabric master name \"%s\" detected!\n"
                                  "%sIgnoring fabric block \"%s\".\n",
                                  TIO_SR_STR( srMasterName ),
                                  TIO_PREFIX_ERROR_SPACE,
                                  TIO_SR_STR( srBlockName ));
         if( ok )
            continue;
         if( !ok )
            break;
      }

      TGO_Point_c vpr_gridPoint( block_.origin );
      if( !vpr_gridPoint.IsValid( ))
      {
         double dx = ( blockType == TFH_BLOCK_PHYSICAL_BLOCK ?
                       TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH : TFV_MODEL_INPUT_OUTPUT_DEF_WIDTH );
         double dy = ( blockType == TFH_BLOCK_PHYSICAL_BLOCK ?
                       TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH : TFV_MODEL_INPUT_OUTPUT_DEF_WIDTH );
         TGS_Region_c blockRegion( block_.region, -1.0 * dx, -1.0 * dy, TGS_SNAP_MIN_GRID );
         vpr_gridPoint.Set( static_cast< int >( blockRegion.x1 ),
                            static_cast< int >( blockRegion.y1 ));
      }

      if( blockType == TFH_BLOCK_PHYSICAL_BLOCK )
      {
         fabricBlockHandler.Add( vpr_gridPoint, vpr_typeIndex,
                                 blockType, srBlockName, srMasterName );
      }
      if( blockType == TFH_BLOCK_INPUT_OUTPUT )
      {
         fabricBlockHandler.Add( vpr_gridPoint, vpr_typeIndex,
                                 blockType, srBlockName, srMasterName );

         const TFM_PinList_t& pinList = block_.pinList;
         for( size_t j = 0; j < pinList.GetLength( ); ++j )
         {
            const TFM_Pin_c& pin = *pinList[j];
            unsigned int slice = pin.slice;
            vpr_gridPoint.z = static_cast< int >( slice );

            if( !fabricBlockHandler.IsMember( vpr_gridPoint ))
            {
               fabricBlockHandler.Add( vpr_gridPoint, vpr_typeIndex,
                                       blockType, srBlockName, srMasterName );
            }
         }
      }
   }
   return( ok );
}

//===========================================================================//
// Method         : PokeSwitchBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 04/15/13 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PokeSwitchBoxes_(
      const TFM_SwitchBoxList_t& switchBoxList ) const
{
   TFH_FabricSwitchBoxHandler_c& fabricSwitchBoxHandler = TFH_FabricSwitchBoxHandler_c::GetInstance( );

   for( size_t i = 0; i < switchBoxList.GetLength( ); ++i )
   {
      const TFM_SwitchBox_t& switchBox = *switchBoxList[i];

      const TC_MapTable_c& mapTable = switchBox.mapTable;
      const string& srSwitchBoxName = switchBox.srName;

      TGO_Point_c gridOrigin( switchBox.origin.x, switchBox.origin.y );
      if( !gridOrigin.IsValid( ))
      {
         double dx = TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
         double dy = TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
         TGS_Region_c region( switchBox.region, -1.0 * dx, -1.0 * dy, TGS_SNAP_MIN_GRID );
         gridOrigin.Set( static_cast< int >( region.x1 ), static_cast< int >( region.y1 ));
      }
      if( !gridOrigin.IsValid( ))
         continue;

      fabricSwitchBoxHandler.Add( gridOrigin, srSwitchBoxName, mapTable );
   }
}

//===========================================================================//
// Method         : PokeConnectionBlocks_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 04/24/13 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PokeConnectionBlocks_(
      const TFM_BlockList_t& blockList ) const
{
   TFH_FabricConnectionBlockHandler_c& fabricConnectionBlockHandler = TFH_FabricConnectionBlockHandler_c::GetInstance( );

   for( size_t i = 0; i < blockList.GetLength( ); ++i )
   {
      const TFM_Block_c& block_ = *blockList[i];
      TGO_Point_c blockOrigin( block_.origin );

      const TFM_PinList_t& pinList = block_.pinList;
      for( size_t j = 0; j < pinList.GetLength( ); ++j )
      {
         const TFM_Pin_c& pin = *pinList[j];
         blockOrigin.z = static_cast< int >( pin.slice );

         int pinIndex = pin.offset;
         const char* pszPinName = pin.GetName( );
         TC_SideMode_t pinSide = pin.side;;

         const TFM_BitPattern_t& connectionPattern = pin.connectionPattern;
         fabricConnectionBlockHandler.Add( blockOrigin, pinIndex, 
                                           pszPinName, pinSide, 
                                           connectionPattern );
      }
   }
}

//===========================================================================//
// Method         : PokeChannelWidths_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PokeChannelWidths_(
      const TFM_ChannelList_t& channelList ) const
{
   TFH_FabricChannelHandler_c& fabricChannelHandler = TFH_FabricChannelHandler_c::GetInstance( );

   for( size_t i = 0; i < channelList.GetLength( ); ++i )
   {
      const TFM_Channel_c& channel = *channelList[i];

      TGS_OrientMode_t orient = channel.orient;
      int index = channel.index;
      size_t count = channel.count;

      TFH_SelectChannelMode_t selectChannel = ( orient == TGS_ORIENT_HORIZONTAL ? 
                                                TFH_SELECT_CHANNEL_Y :
                                                TFH_SELECT_CHANNEL_X );
      fabricChannelHandler.Add( selectChannel, index, count );
   }
}

//===========================================================================//
// Method         : PeekFabricView_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::PeekFabricView_(
            t_grid_tile**     vpr_gridArray,
      const TGS_IntDims_t&    vpr_gridDims,
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
      const int*              vpr_xChanWidthArray,
      const int*              vpr_yChanWidthArray,
            TFV_FabricView_c* pfabricView ) const
{
   bool ok = true;

   string srFabricName;
   TGS_Region_c fabricRegion;
   TGO_Polygon_c ioPolygon;
   if( pfabricView->IsValid( ))
   {
      srFabricName = ( pfabricView->GetName( ) ? pfabricView->GetName( ) : "" );
      fabricRegion = pfabricView->GetRegion( );
      ioPolygon = pfabricView->GetPolygon( );
   }

   TGS_Region_c gridRegion( 0.0, 0.0, vpr_gridDims.dx - 1, vpr_gridDims.dy - 1 );
   gridRegion.ApplyScale( 1.0 );
   fabricRegion.ApplyUnion( gridRegion );

   ok = pfabricView->Init( srFabricName, fabricRegion, &ioPolygon );
   if( ok && 
      vpr_gridArray && vpr_rrNodeArray )
   {
      this->PeekInputOutputs_( vpr_gridArray, vpr_gridDims,
                               pfabricView );

      this->PeekPhysicalBlocks_( vpr_gridArray, vpr_gridDims,
                                 pfabricView );

      this->PeekChannels_( vpr_gridDims,
                           vpr_rrNodeArray, vpr_rrNodeCount,
                           vpr_xChanWidthArray, vpr_yChanWidthArray,
                           pfabricView );

      this->PeekSwitchBoxes_( vpr_gridDims,
                              vpr_rrNodeArray, vpr_rrNodeCount,
                              pfabricView );

      this->PeekSegments_( vpr_rrNodeArray, vpr_rrNodeCount,
                           pfabricView );

      ok = this->PeekConnectionBoxes_( vpr_gridDims,
                                       vpr_rrNodeArray, vpr_rrNodeCount,
                                       pfabricView );
   }
   return( ok );
}

//===========================================================================//
// Method         : PeekInputOutputs_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PeekInputOutputs_(
         t_grid_tile**     vpr_gridArray,
   const TGS_IntDims_t&    vpr_gridDims,
         TFV_FabricView_c* pfabricView ) const
{
   // Iterate for every IO found within VPR's grid 
   // (includes condition where IO may not be located the grid perimeter)
   for( int x = 0; x < vpr_gridDims.dx; ++x )
   {
      for( int y = 0; y < vpr_gridDims.dy; ++y )
      {
         if( vpr_gridArray[x][y].type != IO_TYPE )
            continue;

         t_type_descriptor vpr_type = *grid[x][y].type;

         char szName[TIO_FORMAT_STRING_LEN_DATA];
         sprintf( szName, "io[%d][%d]", x, y );

         const char* pszMasterName = "";
         if( vpr_type.pb_type )
         {
            pszMasterName = vpr_type.pb_type->name;
         }

         TGO_Point_c ioOrigin( x, y );
         TGS_Region_c ioRegion( x, y, x, y );

         double dx = TFV_MODEL_INPUT_OUTPUT_DEF_WIDTH;
         double dy = TFV_MODEL_INPUT_OUTPUT_DEF_WIDTH;
         ioRegion.ApplyScale( dx, dy, TGS_SNAP_MIN_GRID );

         unsigned int sliceCount = vpr_type.capacity;
         unsigned int sliceCapacity = vpr_type.num_pins / ( vpr_type.capacity ? vpr_type.capacity : 1 );

         this->AddFabricViewRegion_( ioRegion, TFV_DATA_INPUT_OUTPUT, 
                                     szName, pszMasterName, ioOrigin,
                                     sliceCount, sliceCapacity,
                                     pfabricView );

         int sideMask = TC_SIDE_NONE;
         sideMask |= ( x == 0 ? TC_SIDE_RIGHT : sideMask );
         sideMask |= ( x == vpr_gridDims.dx - 1 ? TC_SIDE_LEFT : sideMask );
         sideMask |= ( y == 0 ? TC_SIDE_UPPER : sideMask );
         sideMask |= ( y == vpr_gridDims.dy - 1 ? TC_SIDE_LOWER : sideMask );

         if(( x > 0 ) && 
            ( vpr_gridArray[x-1][y].type != IO_TYPE ) && 
            ( vpr_gridArray[x-1][y].type != EMPTY_TYPE ))
         {
            sideMask |= TC_SIDE_LEFT;
         }
         if(( x < vpr_gridDims.dx - 1 ) &&
            ( vpr_gridArray[x+1][y].type != IO_TYPE ) && 
            ( vpr_gridArray[x+1][y].type != EMPTY_TYPE ))
         {
            sideMask |= TC_SIDE_RIGHT;
         }
         if(( y > 0 ) && 
            ( vpr_gridArray[x][y-1].type != IO_TYPE ) && 
            ( vpr_gridArray[x][y-1].type != EMPTY_TYPE ))
         {
            sideMask |= TC_SIDE_LOWER;
         }
         if(( y < vpr_gridDims.dy - 1 ) &&
            ( vpr_gridArray[x][y+1].type != IO_TYPE ) && 
            ( vpr_gridArray[x][y+1].type != EMPTY_TYPE ))
         {
            sideMask |= TC_SIDE_UPPER;
         }
         this->UpdatePinList_( ioRegion, sideMask, TFV_DATA_INPUT_OUTPUT, 
                               vpr_type, pfabricView );
      }
   }
}

//===========================================================================//
// Method         : PeekPhysicalBlocks_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PeekPhysicalBlocks_(
         t_grid_tile**     vpr_gridArray,
   const TGS_IntDims_t&    vpr_gridDims,
         TFV_FabricView_c* pfabricView ) const
{
   unsigned int maxPinCount = this->CalcMaxPinCount_( vpr_gridArray, 
                                                      vpr_gridDims );

   // Iterate for every physical block found within VPR's grid 
   for( int x = 0; x < vpr_gridDims.dx; ++x )
   {
      for( int y = 0; y < vpr_gridDims.dy; ++y )
      {
         if( vpr_gridArray[x][y].type == IO_TYPE )
            continue;
         if( vpr_gridArray[x][y].type == EMPTY_TYPE )
            continue;

         if( vpr_gridArray[x][y].width_offset > 0 )
            continue;
         if( vpr_gridArray[x][y].height_offset > 0 )
            continue;

         t_type_descriptor vpr_type = *grid[x][y].type;

         char szName[TIO_FORMAT_STRING_LEN_DATA];
         sprintf( szName, "pb[%d][%d]", x, y );

         const char* pszMasterName = "";
         if( vpr_type.pb_type )
         {
            pszMasterName = vpr_type.pb_type->name;
         }

         TGO_Point_c pbOrigin( x, y );
         TGS_Region_c pbRegion( x, y, x, y );

         double dx = TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
         double dy = TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;

         if( vpr_type.width > 1 )
         {
            // Using VPR's architecture width for multi-grid wide blocks
            pbRegion.x2 += vpr_type.width - 1;
         }
         if( vpr_type.height > 1 )
         {
            // Using VPR's architecture height for multi-grid tall blocks
            pbRegion.y2 += vpr_type.height - 1;
         }
         // TBD - else if( vpr_type.size.IsValid( ))
         // TBD - {
         // TBD -    // Using Toro's architecture size for non-uniform blocks
         // TBD -    dx = vpr_type.size.width;
         // TBD -    dy = vpr_type.size.height;
         // TBD - }
         else if( maxPinCount > 1 )
         {
            // Using max pin count to estimate min size for all blocks
            double pinWidth = TFV_MODEL_PIN_DEF_WIDTH;
            double pinSpacing = TFV_MODEL_PIN_DEF_SPACING;
            dx = TCT_Max( dx, ( pinWidth + pinSpacing ) * maxPinCount );
            dy = TCT_Max( dy, ( pinWidth + pinSpacing ) * maxPinCount );
         }
         pbRegion.ApplyScale( dx, dy, TGS_SNAP_MIN_GRID );

         unsigned int sliceCount = 0;
         unsigned int sliceCapacity = 0;

         this->AddFabricViewRegion_( pbRegion, TFV_DATA_PHYSICAL_BLOCK, 
                                     szName, pszMasterName, pbOrigin,
                                     sliceCount, sliceCapacity, 
                                     pfabricView );

         int sideMask = TC_SIDE_NONE;
         sideMask |= ( x > 0 ? TC_SIDE_LEFT : sideMask );
         sideMask |= ( x < vpr_gridDims.dx - 1 ? TC_SIDE_RIGHT : sideMask );
         sideMask |= ( y > 0 ? TC_SIDE_LOWER : sideMask );
         sideMask |= ( y < vpr_gridDims.dy - 1 ? TC_SIDE_UPPER : sideMask );
         this->UpdatePinList_( pbRegion, sideMask, TFV_DATA_PHYSICAL_BLOCK, 
                               vpr_type, pfabricView );
      }
   }
}

//===========================================================================//
// Method         : PeekChannels_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PeekChannels_(
      const TGS_IntDims_t&    vpr_gridDims,
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
      const int*              vpr_xChanWidthArray,
      const int*              vpr_yChanWidthArray,
            TFV_FabricView_c* pfabricView ) const
{
   // Generate initial 0-width channels based on given VPR grid dimensions
   this->BuildChannelDefaults_( vpr_gridDims, 
                                pfabricView );

   // Update 0-width channels to reflect max track count (per VPR's rr_nodes)
   this->UpdateChannelCounts_( vpr_rrNodeArray, vpr_rrNodeCount, 
                               vpr_xChanWidthArray, vpr_yChanWidthArray,
                               *pfabricView );

   // Resize channel widths based on max track count (per each channel)
   this->ResizeChannelWidths_( vpr_gridDims, 
                               pfabricView );

   // Resize channel lengths based on orthogonal channels (per each channel)
   this->ResizeChannelLengths_( vpr_gridDims, 
                                pfabricView );
}

//===========================================================================//
// Method         : PeekSegments_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PeekSegments_(
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
            TFV_FabricView_c* pfabricView ) const
{
   // Iterate for all VPR internal rr_node CHANX|CHANY nodes,
   // in order to add segments based on channel and track coordinates
   for( int i = 0; i < vpr_rrNodeCount; ++i )
   {
      const t_rr_node& vpr_rrNode = vpr_rrNodeArray[i];
      if(( vpr_rrNode.type != CHANX ) && ( vpr_rrNode.type != CHANY ))
         continue;

      TGS_Region_c segmentRegion;
      this->FindSegmentRegion_( vpr_rrNode, *pfabricView, &segmentRegion );

      double segmentWidth = TFV_MODEL_SEGMENT_DEF_WIDTH;
      TFV_DataType_t dataType = TFV_DATA_UNDEFINED;
      char szName[TIO_FORMAT_STRING_LEN_DATA];

      if( vpr_rrNode.type == CHANX )
      {
         segmentRegion.ApplyScale( 0.0, segmentWidth / 2.0, TGS_SNAP_MIN_GRID );
         dataType = TFV_DATA_SEGMENT_HORZ;
         sprintf( szName, "sh[%d].%d", vpr_rrNode.ylow, vpr_rrNode.ptc_num );
      }
      if( vpr_rrNode.type == CHANY )
      {
         segmentRegion.ApplyScale( segmentWidth / 2.0, 0.0, TGS_SNAP_MIN_GRID );
         dataType = TFV_DATA_SEGMENT_VERT;
         sprintf( szName, "sv[%d].%d", vpr_rrNode.xlow, vpr_rrNode.ptc_num );
      }
      unsigned int trackIndex = static_cast< int >( vpr_rrNode.ptc_num );

      this->AddFabricViewRegion_( segmentRegion, dataType, 
                                  szName, trackIndex, 
                                  pfabricView );
   }
}

//===========================================================================//
// Method         : PeekSwitchBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::PeekSwitchBoxes_(
      const TGS_IntDims_t&    vpr_gridDims,
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
            TFV_FabricView_c* pfabricView ) const
{
   // Generate initial switch boxes based on channels (but sans mapping table)
   this->BuildSwitchBoxes_( vpr_gridDims, pfabricView );

   // Update switch box mapping tables to reflect mappings (per VPR's rr_nodes)
   this->UpdateSwitchMapTables_( vpr_rrNodeArray, vpr_rrNodeCount, *pfabricView );

   // Clear any unused switch boxes according to empty mapping tables
   this->ClearSwitchBoxes_( pfabricView );
}

//===========================================================================//
// Method         : PeekConnectionBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::PeekConnectionBoxes_(
      const TGS_IntDims_t&    vpr_gridDims,
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
            TFV_FabricView_c* pfabricView ) const
{
   bool ok = true;

   // Generate initial connection boxes based on pins (but sans connections)
   ok = this->BuildConnectionBoxes_( vpr_gridDims, pfabricView );

   // Update connection boxes to reflect pin connections (per VPR's rr_nodes)
   if( ok )
   {
      this->UpdateConnectionPoints_( vpr_rrNodeArray, vpr_rrNodeCount, pfabricView );
   }
   return( ok );
}

//===========================================================================//
// Method         : ExtractFabricView_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ExtractFabricView_(
      const TFV_FabricView_c&  fabricView,
            TFM_FabricModel_c* pfabricModel ) const
{
   pfabricModel->Clear( );

   if( fabricView.IsValid( ))
   {
      pfabricModel->srName = TIO_PSZ_STR( fabricView.GetName( ));

      pfabricModel->config.fabricRegion = fabricView.GetRegion( );
      pfabricModel->config.ioPolygon = fabricView.GetPolygon( );

      const TFV_FabricPlane_c& ioPlane = *fabricView.FindFabricPlane( TFV_LAYER_INPUT_OUTPUT );
      this->ExtractBlockPlane_( ioPlane, &pfabricModel->inputOutputList );

      const TFV_FabricPlane_c& pbPlane = *fabricView.FindFabricPlane( TFV_LAYER_PHYSICAL_BLOCK );
      this->ExtractBlockPlane_( pbPlane, &pfabricModel->physicalBlockList );

      const TFV_FabricPlane_c& sbPlane = *fabricView.FindFabricPlane( TFV_LAYER_SWITCH_BOX );
      this->ExtractBlockPlane_( sbPlane, &pfabricModel->switchBoxList );

      const TFV_FabricPlane_c& chPlane = *fabricView.FindFabricPlane( TFV_LAYER_CHANNEL_HORZ );
      this->ExtractChannelPlane_( chPlane, &pfabricModel->channelList );

      const TFV_FabricPlane_c& cvPlane = *fabricView.FindFabricPlane( TFV_LAYER_CHANNEL_VERT );
      this->ExtractChannelPlane_( cvPlane, &pfabricModel->channelList );

      const TFV_FabricPlane_c& shPlane = *fabricView.FindFabricPlane( TFV_LAYER_SEGMENT_HORZ );
      this->ExtractSegmentPlane_( shPlane, &pfabricModel->segmentList );

      const TFV_FabricPlane_c& svPlane = *fabricView.FindFabricPlane( TFV_LAYER_SEGMENT_VERT );
      this->ExtractSegmentPlane_( svPlane, &pfabricModel->segmentList );

      // Force sort on recently extracted lists...
      pfabricModel->inputOutputList.Find( "" );
      pfabricModel->physicalBlockList.Find( "" );
      pfabricModel->switchBoxList.Find( "" );
      pfabricModel->channelList.Find( "" );
      pfabricModel->segmentList.Find( "" );
   }
}

//===========================================================================//
// Method         : ExtractBlockPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ExtractBlockPlane_(
      const TFV_FabricPlane_t& fabricPlane,
            TFM_BlockList_t*   pblockList ) const
{
   TFV_FabricPlaneIter_t fabricPlaneIter( fabricPlane );
   TFV_FabricFigure_t* pfabricFigure = 0;
   while( fabricPlaneIter.Next( &pfabricFigure, TFV_FIGURE_SOLID ))
   {
      const TGS_Region_c& region = pfabricFigure->GetRegion( );
      const TFV_FabricData_c& fabricData = *pfabricFigure->GetData( );

      if(( fabricData.GetDataType( ) != TFV_DATA_INPUT_OUTPUT ) &&
         ( fabricData.GetDataType( ) != TFV_DATA_PHYSICAL_BLOCK ) &&
         ( fabricData.GetDataType( ) != TFV_DATA_SWITCH_BOX ))
         continue;

      const char* pszName = fabricData.GetName( );
      const char* pszMasterName = fabricData.GetMasterName( );
      const TGO_Point_c& origin = fabricData.GetOrigin( );

      TFV_DataType_t dataType = fabricData.GetDataType( );
      TFM_BlockType_t blockType = this->MapDataTypeToBlockType_( dataType );

      TFM_Block_c block_( blockType, pszName, pszMasterName, origin, region );

      block_.slice.count = fabricData.GetSliceCount( );
      block_.slice.capacity = fabricData.GetSliceCapacity( );

      block_.mapTable = fabricData.GetMapTable( );

      const TFV_FabricPinList_t& fabricPinList = fabricData.GetPinList( );  
      this->ExtractPinList_( fabricPinList, &block_.pinList );

      pblockList->Add( block_ );
   }
}

//===========================================================================//
// Method         : ExtractChannelPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ExtractChannelPlane_(
      const TFV_FabricPlane_t& fabricPlane,
            TFM_ChannelList_t* pchannelList ) const
{
   TFV_FabricPlaneIter_t fabricPlaneIter( fabricPlane );
   TFV_FabricFigure_t* pfabricFigure = 0;
   while( fabricPlaneIter.Next( &pfabricFigure, TFV_FIGURE_SOLID ))
   {
      const TGS_Region_c& region = pfabricFigure->GetRegion( );
      const TFV_FabricData_c& fabricData = *pfabricFigure->GetData( );

      TFV_DataType_t dataType = fabricData.GetDataType( );
      if(( dataType != TFV_DATA_CHANNEL_HORZ ) &&
         ( dataType != TFV_DATA_CHANNEL_VERT ))
         continue;

      const char* pszName = fabricData.GetName( );
      TGS_OrientMode_t orient = ( dataType == TFV_DATA_CHANNEL_HORZ ?
                                  TGS_ORIENT_HORIZONTAL : TGS_ORIENT_VERTICAL );

      unsigned int index = fabricData.GetTrackIndex( );
      unsigned int horzCount = fabricData.GetTrackHorzCount( );
      unsigned int vertCount = fabricData.GetTrackVertCount( );
      unsigned int count = ( dataType == TFV_DATA_CHANNEL_HORZ ?
                             horzCount : vertCount );

      TFM_Channel_c channel( pszName, region, orient, index, count );
      pchannelList->Add( channel );
   }
}

//===========================================================================//
// Method         : ExtractSegmentPlane_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ExtractSegmentPlane_(
      const TFV_FabricPlane_t& fabricPlane,
            TFM_SegmentList_t* psegmentList ) const
{
   TFV_FabricPlaneIter_t fabricPlaneIter( fabricPlane );
   TFV_FabricFigure_t* pfabricFigure = 0;
   while( fabricPlaneIter.Next( &pfabricFigure, TFV_FIGURE_SOLID ))
   {
      const TGS_Region_c& region = pfabricFigure->GetRegion( );
      const TFV_FabricData_c& fabricData = *pfabricFigure->GetData( );

      TFV_DataType_t dataType = fabricData.GetDataType( );
      if(( dataType != TFV_DATA_SEGMENT_HORZ ) &&
         ( dataType != TFV_DATA_SEGMENT_VERT ))
         continue;

      const char* pszName = fabricData.GetName( );
      unsigned int index = fabricData.GetTrackIndex( );
      TGS_Path_c path( region );

      TFM_Segment_c segment( pszName, path, index );
      psegmentList->Add( segment );
   }
}

//===========================================================================//
// Method         : ExtractPinList_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ExtractPinList_(
      const TFV_FabricPinList_t& fabricPinList,
            TFM_PinList_t*       ppinList ) const
{
   TC_Bit_c trueBit( TC_BIT_TRUE );
   TC_Bit_c falseBit( TC_BIT_FALSE );

   for( size_t i = 0; i < fabricPinList.GetLength( ); ++i )
   {
      const TFV_FabricPin_c& fabricPin = *fabricPinList[i];

      TFM_Pin_c pin( fabricPin.GetName( ),
                     fabricPin.GetType( ),
                     fabricPin.GetSide( ),
                     fabricPin.GetOffset( ),
                     fabricPin.GetDelta( ),
                     fabricPin.GetWidth( ),
                     fabricPin.GetSlice( ));

      unsigned int channelCount = fabricPin.GetChannelCount( );
      if( channelCount )
      {
         for( size_t j = 0; j < channelCount; ++j )
         {
            pin.connectionPattern.Add( falseBit );
         }

         const TFV_ConnectionList_t& connectionList = fabricPin.GetConnectionList( );
         for( size_t j = 0; j < connectionList.GetLength( ); ++j )
         {
            const TFV_Connection_t& connection = *connectionList[j];
            unsigned int index = static_cast< unsigned int >( connection.GetIndex( ));
            *pin.connectionPattern[index] = trueBit;
         }
      }
      ppinList->Add( pin );
   }
}

//===========================================================================//
// Method         : AddBlockPinList_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::AddBlockPinList_(
      const TFM_PinList_t&    pinList,
            TFV_FabricData_c* pfabricData ) const
{
   // Add given pins, if any, to fabric block's pin list
   for( size_t i = 0; i < pinList.GetLength( ); ++i )
   {
      const TFM_Pin_c& pin = *pinList[i];
      TFV_FabricPin_c fabricPin( pin.GetName( ), pin.type, 
                                 pin.side, pin.offset, 
                                 pin.delta, pin.width, pin.slice );

      if( pin.connectionPattern.IsValid( ))
      {
         unsigned int channelCount = static_cast< unsigned int >( pin.connectionPattern.GetLength( ));
         fabricPin.SetChannelCount( channelCount );

         for( size_t j = 0; j < pin.connectionPattern.GetLength( ); ++j )
         {
            const TC_Bit_c& bit = *pin.connectionPattern[j];
            if( !bit.IsTrue( ))
               continue;

            unsigned int index = static_cast< unsigned int >( j );
            TFV_Connection_t connection( pin.side, index );
            fabricPin.AddConnection( connection );
         }
      }
      pfabricData->AddPin( fabricPin );
   }
}

//===========================================================================//
// Method         : AddBlockMapList_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::AddBlockMapTable_(
      const TC_MapTable_c&    mapTable,
            TFV_FabricData_c* pfabricData ) const
{
   // Copy given pin's map table, if any, to fabric pin's map table
   pfabricData->AddMap( mapTable );
}

//===========================================================================//
// Method         : UpdatePinList_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::UpdatePinList_(
      const TGS_Region_c&     region,
            int               sideMask,
            TFV_DataType_t    dataType,
      const t_type_descriptor vpr_type,
            TFV_FabricView_c* pfabricView ) const
{
   TGS_Layer_t layer = this->MapDataTypeToLayer_( dataType );

   TFV_FabricData_c* pfabricData = 0;
   if( pfabricView->Find( layer, region, &pfabricData ))
   {
      this->UpdatePinList_( region, sideMask, vpr_type, pfabricData );
   }
}

//===========================================================================//
void TVPR_FabricModel_c::UpdatePinList_(
      const TGS_Region_c&     region,
            int               sideMask,
      const t_type_descriptor vpr_type,
            TFV_FabricData_c* pfabricData ) const
{
   // Decide initial slice size and index (for IO blocks with capacity)
   unsigned int sliceCapacity = pfabricData->GetSliceCapacity( );
   unsigned int sliceIndex = 0;

   // Generate PB|IO's max pin count per side 
   unsigned int countArray[4];
   this->CalcPinCountArray_( vpr_type, 
                             &countArray[0] );

   // Generate PB|IO's initial pin delta (distance) per side
   double deltaArray[4];
   this->CalcPinDeltaArray_( vpr_type, region, sliceIndex, countArray, 
                             &deltaArray[0] );

   // Iterate for all pins, assigning side and delta, then adding to region
   for( int pinIndex = 0; pinIndex < vpr_type.num_pins; ++pinIndex ) 
   {
      if( vpr_type.is_global_pin[pinIndex] )
         continue;

      if(( sliceCapacity ) && 
         ( pinIndex / sliceCapacity != sliceIndex ))
      {
         // Recompute pin delta per side (for IO blocks with capacity)
         sliceIndex = pinIndex / sliceCapacity;
         this->CalcPinDeltaArray_( vpr_type, region, sliceIndex, countArray, 
                                   &deltaArray[0] );
      }

      for( int sideIndex = 0; sideIndex < 4; ++sideIndex ) 
      {
         if( sideMask != TC_SIDE_ANY )
         {
            TC_SideMode_t pinSide = FindPinSide_( sideIndex );
            if( !( sideMask & pinSide ))
               continue;
         }

         for( int widthIndex = 0; widthIndex < vpr_type.width; ++widthIndex ) 
         {
            for( int heightIndex = 0; heightIndex < vpr_type.height; ++heightIndex ) 
            {
               if( !vpr_type.pinloc[widthIndex][heightIndex][sideIndex][pinIndex] )
                  continue;

               string srPinName;
               TC_TypeMode_t pinType = TC_TYPE_UNDEFINED;
               this->FindPinName_( vpr_type, pinIndex, &srPinName, &pinType );
               TC_SideMode_t pinSide = this->FindPinSide_( sideIndex );

               unsigned int pinSlice = ( sliceCapacity ? pinIndex / sliceCapacity : 0 );
               int pinOffset = pinIndex;
               double pinDelta = this->FindPinDelta_( sideIndex, &deltaArray[0] );
               double pinWidth = TFV_MODEL_PIN_DEF_WIDTH;

               TFV_FabricPin_c fabricPin( srPinName, pinType, pinSide, pinOffset, 
                                          pinDelta, pinWidth, pinSlice );
               pfabricData->AddPin( fabricPin );
            }
         }
      }
   }
}

//===========================================================================//
// Method         : BuildChannelDefaults_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::BuildChannelDefaults_(
      const TGS_IntDims_t&    vpr_gridDims,
            TFV_FabricView_c* pfabricView ) const
{
   double x1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double x2 = vpr_gridDims.dx - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y2 = vpr_gridDims.dy - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;

   // Generate initial 0-width channels based on given VPR grid dimensions
   for( int y = 0; y < vpr_gridDims.dy - 1; ++y )
   {
      TGS_Region_c channelRegion( x1, y, x2, y );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_HORIZONTAL );

      char szName[TIO_FORMAT_STRING_LEN_DATA];
      sprintf( szName, "ch[%d]", y );

      unsigned int trackIndex = static_cast< unsigned int >( y );
      this->AddFabricViewRegion_( channelRegion, TFV_DATA_CHANNEL_HORZ, 
                                  szName, trackIndex,
                                  pfabricView );
   }
   for( int x = 0; x < vpr_gridDims.dx - 1; ++x )
   {
      TGS_Region_c channelRegion( x, y1, x, y2 );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_VERTICAL );

      char szName[TIO_FORMAT_STRING_LEN_DATA];
      sprintf( szName, "cv[%d]", x );

      unsigned int trackIndex = static_cast< unsigned int >( x );
      this->AddFabricViewRegion_( channelRegion, TFV_DATA_CHANNEL_VERT, 
                                  szName, trackIndex,
                                  pfabricView );
   }
}

//===========================================================================//
// Method         : UpdateChannelCounts_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::UpdateChannelCounts_(
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
      const int*              vpr_xChanWidthArray,
      const int*              vpr_yChanWidthArray,
      const TFV_FabricView_c& fabricView ) const
{
   // Iterate for all VPR internal rr_node CHANX|CHANY nodes,
   // in order to update max track count for asso. fabric channels
   for( int i = 0; i < vpr_rrNodeCount; ++i )
   {
      const t_rr_node& vpr_rrNode = vpr_rrNodeArray[i];
      if(( vpr_rrNode.type != CHANX ) && ( vpr_rrNode.type != CHANY ))
         continue;

      TFV_LayerType_t channelLayer = ( vpr_rrNode.type == CHANX ?
                                       TFV_LAYER_CHANNEL_HORZ : TFV_LAYER_CHANNEL_VERT );
      TGS_Region_c channelRegion;
      this->FindChannelRegion_( vpr_rrNode, fabricView, &channelRegion );

      TFV_FabricFigure_t* pfabricFigure = 0;
      if( !fabricView.IsSolidAll( channelLayer, channelRegion, &pfabricFigure ))
         continue;

      TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
      if( vpr_rrNode.type == CHANX )
      {
         unsigned int index = static_cast< int >( pfabricData->GetTrackIndex( ));
         unsigned int count = static_cast< int >( vpr_xChanWidthArray[index] );
         pfabricData->SetTrackHorzCount( count );
      }
      if( vpr_rrNode.type == CHANY )
      {
         unsigned int index = static_cast< int >( pfabricData->GetTrackIndex( ));
         unsigned int count = static_cast< int >( vpr_yChanWidthArray[index] );
         pfabricData->SetTrackVertCount( count );
      }
   }
}

//===========================================================================//
// Method         : ResizeChannelWidths_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ResizeChannelWidths_(
      const TGS_IntDims_t&    vpr_gridDims,
            TFV_FabricView_c* pfabricView ) const
{
   double x1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double x2 = vpr_gridDims.dx - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y2 = vpr_gridDims.dy - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;

   // Resize channel widths based on max track count (per each channel)
   double segmentWidth = TFV_MODEL_SEGMENT_DEF_WIDTH;
   double segmentSpacing = TFV_MODEL_SEGMENT_DEF_SPACING;
   for( int y = 0; y < vpr_gridDims.dy - 1; ++y )
   {
      TGS_Region_c channelRegion( x1, y, x2, y );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_HORIZONTAL );

      TFV_FabricFigure_t* pfabricFigure = 0;
      TGS_Layer_t channelLayer = TFV_LAYER_CHANNEL_HORZ;
      if( !pfabricView->IsSolidAll( channelLayer, channelRegion, &pfabricFigure ))
         continue;

      TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
      int count = pfabricData->GetTrackHorzCount( );

      double dx = 0.0;
      double dy = ( count * ( segmentWidth + segmentSpacing )) / 2.0;
      TGS_Region_c replaceRegion( channelRegion, dx, dy, TGS_SNAP_MIN_GRID );

      this->ReplaceFabricViewRegion_( channelRegion, replaceRegion, 
                                      *pfabricData, pfabricView );
   }
   for( int x = 0; x < vpr_gridDims.dx - 1; ++x )
   {
      TGS_Region_c channelRegion( x, y1, x, y2 );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_VERTICAL );

      TFV_FabricFigure_t* pfabricFigure = 0;
      TGS_Layer_t channelLayer = TFV_LAYER_CHANNEL_VERT;
      if( !pfabricView->IsSolidAll( channelLayer, channelRegion, &pfabricFigure ))
         continue;

      TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
      int count = pfabricData->GetTrackVertCount( );

      double dx = ( count * ( segmentWidth + segmentSpacing )) / 2.0;
      double dy = 0.0;
      TGS_Region_c replaceRegion( channelRegion, dx, dy, TGS_SNAP_MIN_GRID );

      this->ReplaceFabricViewRegion_( channelRegion, replaceRegion, 
                                      *pfabricData, pfabricView );
   }
}

//===========================================================================//
// Method         : ResizeChannelLengths_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ResizeChannelLengths_(
      const TGS_IntDims_t&    vpr_gridDims,
            TFV_FabricView_c* pfabricView ) const
{
   double x1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double x2 = vpr_gridDims.dx - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y2 = vpr_gridDims.dy - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;

   // Resize channel lengths based on orthogonal channels (per each channel)
   for( int y = 0; y < vpr_gridDims.dy - 1; ++y )
   {
      TGS_Region_c channelRegion( x1, y, x2, y );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_HORIZONTAL );

      TFV_FabricFigure_t* pfabricFigure = 0;
      TGS_Layer_t horzLayer = TFV_LAYER_CHANNEL_HORZ;
      if( !pfabricView->IsSolidAll( horzLayer, channelRegion, &pfabricFigure ))
         continue;

      TGS_Region_c replaceRegion = pfabricFigure->GetRegion( );
      const TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );

      TGS_Point_c lowerLeft = channelRegion.FindLowerLeft( );
      TGS_Point_c upperRight = channelRegion.FindUpperRight( );

      TGS_Region_c nearestRegion;
      TGS_Layer_t vertLayer = TFV_LAYER_CHANNEL_VERT;
      if( pfabricView->FindNearest( vertLayer, lowerLeft, &nearestRegion ))
      {
         replaceRegion.x1 = TCT_Min( replaceRegion.x1, nearestRegion.x2 );
      }
      if( pfabricView->FindNearest( vertLayer, upperRight, &nearestRegion ))
      {
         replaceRegion.x2 = TCT_Max( replaceRegion.x2, nearestRegion.x1 );
      }
      this->ReplaceFabricViewRegion_( channelRegion, replaceRegion, 
                                      *pfabricData, pfabricView );
   }
   for( int x = 0; x < vpr_gridDims.dx - 1; ++x )
   {
      TGS_Region_c channelRegion( x, y1, x, y2 );
      channelRegion.ApplyShift( 0.5, TGS_ORIENT_VERTICAL );

      TFV_FabricFigure_t* pfabricFigure = 0;
      TGS_Layer_t vertLayer = TFV_LAYER_CHANNEL_VERT;
      if( !pfabricView->IsSolidAll( vertLayer, channelRegion, &pfabricFigure ))
         continue;

      TGS_Region_c replaceRegion = pfabricFigure->GetRegion( );
      const TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );

      TGS_Point_c lowerLeft = channelRegion.FindLowerLeft( );
      TGS_Point_c upperRight = channelRegion.FindUpperRight( );

      TGS_Region_c nearestRegion;
      TGS_Layer_t horzLayer = TFV_LAYER_CHANNEL_HORZ;
      if( pfabricView->FindNearest( horzLayer, lowerLeft, &nearestRegion ))
      {
         replaceRegion.y1 = TCT_Min( replaceRegion.y1, nearestRegion.y2 );
      }
      if( pfabricView->FindNearest( horzLayer, upperRight, &nearestRegion ))
      {
         replaceRegion.y2 = TCT_Max( replaceRegion.y2, nearestRegion.y1 );
      }
      this->ReplaceFabricViewRegion_( channelRegion, replaceRegion, 
                                      *pfabricData, pfabricView );
   }
}

//===========================================================================//
// Method         : BuildSwitchBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::BuildSwitchBoxes_(
      const TGS_IntDims_t&    vpr_gridDims,
            TFV_FabricView_c* pfabricView ) const
{
   const TGS_Region_c& fabricRegion = pfabricView->GetRegion( );

   double x1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double x2 = vpr_gridDims.dx - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y1 = 1.0 - TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;
   double y2 = vpr_gridDims.dy - 2.0 + TFV_MODEL_PHYSICAL_BLOCK_DEF_WIDTH;

   unsigned int switchIndex = 0;

   // Iterate based on VPR's grid to define switch box locations
   for( int x = 0; x < vpr_gridDims.dx - 1; ++x )
   {
      // Find vertical channel asso. with next set of switch boxes
      TGS_Region_c vertRegion( x, y1, x, y2 );
      vertRegion.ApplyShift( 0.5, TGS_ORIENT_VERTICAL );

      TFV_FabricFigure_t* pfabricFigure = 0;
      TGS_Layer_t vertLayer = TFV_LAYER_CHANNEL_VERT;
      if( !pfabricView->IsSolidAll( vertLayer, vertRegion, &pfabricFigure ))
         continue;

      vertRegion.Set( pfabricFigure->GetRegion( ).x1, fabricRegion.y1,
                      pfabricFigure->GetRegion( ).x2, fabricRegion.y2 );
      unsigned int vertCount = pfabricFigure->GetData( )->GetTrackVertCount( );
      
      for( int y = 0; y < vpr_gridDims.dy - 1; ++y )
      {
         // Find horizontal channel asso. with next set of switch boxes
         TGS_Region_c horzRegion( x1, y, x2, y );
         horzRegion.ApplyShift( 0.5, TGS_ORIENT_HORIZONTAL );

         TGS_Layer_t horzLayer = TFV_LAYER_CHANNEL_HORZ;
         if( !pfabricView->IsSolidAll( horzLayer, horzRegion, &pfabricFigure ))
            continue;

         horzRegion.Set( fabricRegion.x1, pfabricFigure->GetRegion( ).y1,
                         fabricRegion.x2, pfabricFigure->GetRegion( ).y2 );
         unsigned int horzCount = pfabricFigure->GetData( )->GetTrackHorzCount( );

         // Add switch box based on intersecting channel region
         TGS_Region_c switchRegion;
         switchRegion.ApplyIntersect( vertRegion, horzRegion );

         // Adjust sizes for any switch regions that are intially zero-width
         if( !switchRegion.HasArea( ))
         {
            if( TCTF_IsGT( switchRegion.GetDx( ), 0.0 ))
            {
               switchRegion.ApplyScale( 0.0, switchRegion.GetDx( ) / 2.0,
                                        TGS_SNAP_MIN_GRID );
            }
            else if( TCTF_IsGT( switchRegion.GetDy( ), 0.0 ))
            {
               switchRegion.ApplyScale( switchRegion.GetDy( ) / 2.0, 0.0, 
                                        TGS_SNAP_MIN_GRID );
            }
         }

         TGO_Point_c switchOrigin( x, y );

         char szName[TIO_FORMAT_STRING_LEN_DATA];
         sprintf( szName, "sb[%d][%d]", x, y );

         this->AddFabricViewRegion_( switchRegion, TFV_DATA_SWITCH_BOX, 
                                     szName, switchOrigin, ++switchIndex, 
                                     horzCount, vertCount, 
                                     pfabricView );
      }
   }
}
          
//===========================================================================//
// Method         : ClearSwitchBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::ClearSwitchBoxes_(
      TFV_FabricView_c* pfabricView ) const
{
   const TFV_FabricPlane_c& sbPlane = *pfabricView->FindFabricPlane( TFV_LAYER_SWITCH_BOX );
   TGS_RegionList_t sbRegionList( sbPlane.FindCount( TFV_FIGURE_SOLID ));

   TFV_FabricPlaneIter_t sbPlaneIter( sbPlane );
   TFV_FabricFigure_t* psbFigure = 0;
   while( sbPlaneIter.Next( &psbFigure, TFV_FIGURE_SOLID ))
   {
      const TFV_FabricData_c& fabricData = *psbFigure->GetData( );
      if( fabricData.GetMapTable( ).IsValid( ) &&
          fabricData.GetMapTable( ).IsLegal( ))
      {
         continue;
      }
      const TGS_Region_c& region = psbFigure->GetRegion( );
      sbRegionList.Add( region );
   }

   for( size_t i = 0; i < sbRegionList.GetLength( ); ++i )
   {
      const TGS_Region_c& region = *sbRegionList[i];
      TFV_FabricData_c fabricData( TFV_DATA_SWITCH_BOX );
      this->DeleteFabricViewRegion_( region, fabricData, pfabricView );
   }
}
          
//===========================================================================//
// Method         : UpdateSwitchMapTables_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::UpdateSwitchMapTables_(
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
      const TFV_FabricView_c& fabricView ) const
{
   // Iterate for all VPR internal rr_node CHANX|CHANY nodes,
   // in order to discover switch box mappings between CHANX|CHANY nodes
   for( int i = 0; i < vpr_rrNodeCount; ++i )
   {
      const t_rr_node& vpr_rrNode1 = vpr_rrNodeArray[i];
      if(( vpr_rrNode1.type != CHANX ) && ( vpr_rrNode1.type != CHANY ))
         continue;

      for( int j = 0; j < vpr_rrNode1.num_edges; ++j )
      {
         const t_rr_node& vpr_rrNode2 = vpr_rrNodeArray[vpr_rrNode1.edges[j]];

         if(( vpr_rrNode2.type != CHANX ) && ( vpr_rrNode2.type != CHANY ))
            continue;

         TGS_Region_c segmentRegion1, segmentRegion2;
         this->FindSegmentRegion_( vpr_rrNode1, fabricView, &segmentRegion1 );
         this->FindSegmentRegion_( vpr_rrNode2, fabricView, &segmentRegion2 );

         // Find nearest points between CHANX|CHANY regions
         TGS_Point_c nearestPoint1, nearestPoint2;
         segmentRegion1.FindNearest( segmentRegion2, &nearestPoint2, &nearestPoint1 );

         // Find switch box asso. with CHANX|CHANY nearest points
         TGS_Region_c nearestRegion( nearestPoint1, nearestPoint2 );

         TFV_FabricFigure_t* pfabricFigure = 0;
         TGS_Layer_t switchLayer = TFV_LAYER_SWITCH_BOX;
         if( !fabricView.IsSolidAny( switchLayer, nearestRegion, &pfabricFigure ))
            continue;

         const TGS_Region_c& switchRegion = pfabricFigure->GetRegion( );

         TC_SideMode_t side1 = this->FindSwitchSide_( vpr_rrNode1, TGS_DIR_PREV,
                                                      nearestPoint1, switchRegion );
         unsigned int index1 = vpr_rrNode1.ptc_num;

         TC_SideMode_t side2 = this->FindSwitchSide_( vpr_rrNode2, TGS_DIR_NEXT,
                                                      nearestPoint2, switchRegion );
         unsigned int index2 = vpr_rrNode2.ptc_num;

         TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
         pfabricData->AddMap( side1, index1, side2, index2 );
      }
   }
}

//===========================================================================//
// Method         : BuildConnectionBoxes_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::BuildConnectionBoxes_(
      const TGS_IntDims_t&    vpr_gridDims,
            TFV_FabricView_c* pfabricView ) const
{
   bool ok = true;

   // Iterate for every PB|IO found within VPR's grid 
   for( int x = 0; x < vpr_gridDims.dx; ++x )
   {
      for( int y = 0; y < vpr_gridDims.dy; ++y )
      {
         // Get fabric's PB|IO figure (ie. region and asso. data)
         TGS_Point_c point( x, y );

         TFV_FabricFigure_t* pfabricFigure = 0;
         if( !pfabricView->IsSolid( TFV_LAYER_INPUT_OUTPUT, point, &pfabricFigure ) &&
             !pfabricView->IsSolid( TFV_LAYER_PHYSICAL_BLOCK, point, &pfabricFigure ))
         {
            continue;
         }

         // Iterate for every pin associated with this PB|IO figure
         const TGS_Region_c& blockRegion = pfabricFigure->GetRegion( );
         TFV_FabricData_c* pfabricData = pfabricFigure->GetData( );
         const TFV_FabricPinList_t& fabricPinList = pfabricData->GetPinList( );
         for( size_t i = 0; i < fabricPinList.GetLength( ); ++i )
         {
            const TFV_FabricPin_c& fabricPin = *fabricPinList[i];

            // Define a connection box region spanning from pin to channel region
            TGS_Region_c connectionRegion;
            ok = this->BuildConnectionRegion_( *pfabricView, fabricPin, 
                                               blockRegion, &connectionRegion );
            if( !ok )
               break;

            // Add connection box region to fabric view
            this->AddFabricViewRegion_( connectionRegion, TFV_DATA_CONNECTION_BOX, 
                                        pfabricView );
         }
         if( !ok )
            break;
      }
      if( !ok )
         break;
   }
   return( ok );
}

//===========================================================================//
// Method         : BuildConnectionRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::BuildConnectionRegion_(
      const TFV_FabricView_c& fabricView,
      const TFV_FabricPin_c&  fabricPin,
      const TGS_Region_c&     blockRegion,
            TGS_Region_c*     pconnectionRegion ) const
{
   pconnectionRegion->Reset( );

   // Define a connection box spanning from pin to far side of neighboring channel
   TFV_FabricData_c fabricData;
   TGS_Point_c pinPoint = fabricData.CalcPoint( blockRegion, fabricPin );

   TGS_Region_c channelRegion;
   switch( fabricPin.GetSide( ))
   {
   case TC_SIDE_LEFT:
   case TC_SIDE_RIGHT:

      fabricView.FindNearest( TFV_LAYER_CHANNEL_VERT, pinPoint, &channelRegion );
      pconnectionRegion->Set( TCT_Min( pinPoint.x, channelRegion.x1 ),
                              pinPoint.y,
                              TCT_Max( pinPoint.x, channelRegion.x2 ),
                              pinPoint.y );
      break;

   case TC_SIDE_LOWER:
   case TC_SIDE_UPPER:

      fabricView.FindNearest( TFV_LAYER_CHANNEL_HORZ, pinPoint, &channelRegion );
      pconnectionRegion->Set( pinPoint.x,
                              TCT_Min( pinPoint.y, channelRegion.y1 ),
                              pinPoint.x,
                              TCT_Max( pinPoint.y, channelRegion.y2 ));
      break;

   default:
      break;
   }

   if( pconnectionRegion->IsValid( ))
   {
      double scale = TFV_MODEL_CONNECTION_BOX_DEF_WIDTH / 2.0;
      pconnectionRegion->ApplyScale( scale, TGS_SNAP_MIN_GRID );
   }
   return( pconnectionRegion->IsValid( ) ? true : false );
}

//===========================================================================//
// Method         : UpdateConnectionPoints_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::UpdateConnectionPoints_(
      const t_rr_node*        vpr_rrNodeArray,
            int               vpr_rrNodeCount,
            TFV_FabricView_c* pfabricView ) const
{
   // Iterate for all VPR internal rr_nodes that indicate a connection box
   // (ie. rr_nodes that span OPIN-->CHANX|CHANY or span CHANX|CHANY-->IPIN)
   for( int i = 0; i < vpr_rrNodeCount; ++i )
   {
      const t_rr_node& vpr_rrNode1 = vpr_rrNodeArray[i];
      if(( vpr_rrNode1.type != CHANX ) && ( vpr_rrNode1.type != CHANY ) &&
         ( vpr_rrNode1.type != IPIN ) && ( vpr_rrNode1.type != OPIN ))
         continue;

      for( int j = 0; j < vpr_rrNode1.num_edges; ++j )
      {
         const t_rr_node& vpr_rrNode2 = vpr_rrNodeArray[vpr_rrNode1.edges[j]];
         if(( vpr_rrNode2.type != CHANX ) && ( vpr_rrNode2.type != CHANY ) &&
            ( vpr_rrNode2.type != IPIN ) && ( vpr_rrNode2.type != OPIN ))
            continue;

         // Handle connection boxes from output pins
         if(( vpr_rrNode1.type == OPIN ) && ( vpr_rrNode2.type == CHANX ))
         {
            this->UpdateConnectionPoints_( vpr_rrNode1, vpr_rrNode2, 
                                           pfabricView );
         }
         if(( vpr_rrNode1.type == OPIN ) && ( vpr_rrNode2.type == CHANY ))
         {
            this->UpdateConnectionPoints_( vpr_rrNode1, vpr_rrNode2,
                                           pfabricView );
         }

         // Handle connection boxes to input pins
         if(( vpr_rrNode1.type == CHANX ) && ( vpr_rrNode2.type == IPIN ))
         {
            this->UpdateConnectionPoints_( vpr_rrNode2, vpr_rrNode1,
                                           pfabricView );
         }
         if(( vpr_rrNode1.type == CHANY ) && ( vpr_rrNode2.type == IPIN ))
         {
            this->UpdateConnectionPoints_( vpr_rrNode2, vpr_rrNode1,
                                           pfabricView );
         }
      }
   }
}

//===========================================================================//
void TVPR_FabricModel_c::UpdateConnectionPoints_(
      const t_rr_node&        vpr_rrNodePin,
      const t_rr_node&        vpr_rrNodeChan,
            TFV_FabricView_c* pfabricView ) const
{
   // Determine pin name based on IPIN|OPIN node
   string srPinName;
   this->FindPinName_( vpr_rrNodePin.pb_graph_pin, &srPinName );
   if( srPinName.length( ))
   {
      // Determine pin side based on IPIN|OPIN node wrt CHANX|CHANY node
      TC_SideMode_t pinSide = this->FindPinSide_( vpr_rrNodePin, vpr_rrNodeChan );

      // Find pin from PB|IO based on the extracted pin name and pin side
      TGS_Region_c blockRegion;
      TFV_FabricData_c* pfabricData = 0;
      if( this->FindBlockRegion_( vpr_rrNodePin, *pfabricView, 
                                  &blockRegion, &pfabricData ))
      {
         const TFV_FabricPinList_t& pinList = pfabricData->GetPinList( );

         int sliceCount = static_cast< int >( TCT_Max( pfabricData->GetSliceCount( ), 1u ));
         for( int sliceIndex = 0; sliceIndex < sliceCount; ++sliceIndex )
         {
            if( sliceIndex != vpr_rrNodePin.z )
               continue;

            TFV_FabricPin_c pin( srPinName, sliceIndex, pinSide );
            if( pinList.IsMember( pin ))
            {
               TFV_FabricPin_c* ppin = pinList[pin];

               // Find asso. track index based on CHANX|CHANY node
               unsigned int trackIndex = static_cast< int >( vpr_rrNodeChan.ptc_num );

               // Update pin's connection box list per asso. side and track index
               TFV_Connection_t connection( pinSide, trackIndex );
               ppin->AddConnection( connection );

               // And, update pin's channel count based on nearest channel width
               TGS_Point_c pinPoint = pfabricData->CalcPoint( blockRegion, *ppin );
               unsigned int channelCount = 0;
               channelCount = this->FindChannelCount_( *pfabricView, pinPoint, pinSide );
               ppin->SetChannelCount( channelCount );

               // Update fabric view's connection box layer per asso. track index
               TGS_Layer_t connectionLayer = TFV_LAYER_CONNECTION_BOX;
               TFV_FabricFigure_t* pfabricFigure = 0;
               if( pfabricView->IsSolid( connectionLayer, pinPoint, &pfabricFigure ))
               {
                  // Update connection box figure per asso. side and track index
                  pfabricData = pfabricFigure->GetData( );
                  pfabricData->AddConnection( connection );
               }
            }
         }
      }
   }
}

//===========================================================================//
// Method         : CalcMaxPinCount_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
unsigned int TVPR_FabricModel_c::CalcMaxPinCount_(
            t_grid_tile**  vpr_gridArray,
      const TGS_IntDims_t& vpr_gridDims ) const
{
   unsigned int maxPins = 0;

   for( int x = 0; x < vpr_gridDims.dx; ++x )
   {
      for( int y = 0; y < vpr_gridDims.dy; ++y )
      {
         if( vpr_gridArray[x][y].type == IO_TYPE )
            continue;
         if( vpr_gridArray[x][y].type == EMPTY_TYPE )
            continue;

         t_type_descriptor vpr_type = *grid[x][y].type;
         unsigned int typePins = this->CalcMaxPinCount_( vpr_type );
         maxPins = TCT_Max( maxPins, typePins );
      }
   }
   return( maxPins );
}

//===========================================================================//
unsigned int TVPR_FabricModel_c::CalcMaxPinCount_(
      const t_type_descriptor vpr_type ) const
{
   unsigned int countArray[4];

   this->CalcPinCountArray_( vpr_type, countArray );

   return( TCT_Max( TCT_Max( countArray[0], countArray[1] ), 
                    TCT_Max( countArray[2], countArray[3] )));
}

//===========================================================================//
// Method         : CalcPinCountArray_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::CalcPinCountArray_(
      const t_type_descriptor vpr_type,
            unsigned int*     pcountArray ) const
{
   for( int sideIndex = 0; sideIndex < 4; ++sideIndex ) 
   {
      *( pcountArray + sideIndex ) = 0;
   }

   for( int pinIndex = 0; pinIndex < vpr_type.num_pins; ++pinIndex ) 
   {
      if( vpr_type.is_global_pin[pinIndex] )
         continue;

      for( int sideIndex = 0; sideIndex < 4; ++sideIndex ) 
      {
         for( int widthIndex = 0; widthIndex < vpr_type.width; ++widthIndex ) 
         {
            for( int heightIndex = 0; heightIndex < vpr_type.height; ++heightIndex ) 
            {
               if( !vpr_type.pinloc[widthIndex][heightIndex][sideIndex][pinIndex] )
                  continue;

               ++*( pcountArray + sideIndex );
            }
         }
      }
   }

   if( vpr_type.capacity > 1 )
   {
      for( int sideIndex = 0; sideIndex < 4; ++sideIndex ) 
      {
         *( pcountArray + sideIndex ) /= vpr_type.capacity;
      }
   }
}

//===========================================================================//
// Method         : CalcPinDeltaArray_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::CalcPinDeltaArray_(
      const t_type_descriptor vpr_type,
      const TGS_Region_c&     region,
            unsigned int      index,
      const unsigned int*     pcountArray,
            double*           pdeltaArray ) const
{
   TGS_Point_c regionCenter = region.FindCenter( TGS_SNAP_MIN_GRID );
   if( vpr_type.capacity > 1 )
   {
      double dx = region.GetDx( ) / ( vpr_type.capacity ? vpr_type.capacity : 1 );
      double dy = region.GetDy( ) / ( vpr_type.capacity ? vpr_type.capacity : 1 );
      TGS_Region_c region_( region.x1, region.y1, region.x1 + dx, region.y1 + dy );

      regionCenter = region_.FindCenter( TGS_SNAP_MIN_GRID );
   }

   double pinWidth = TFV_MODEL_PIN_DEF_WIDTH;
   double pinSpacing = TFV_MODEL_PIN_DEF_SPACING;

   for( int sideIndex = 0; sideIndex < 4; ++sideIndex ) 
   {
      unsigned int count = *( pcountArray + sideIndex );

      double delta = 0.0;

      switch( sideIndex ) // Based on VPR's e_side enumeration values...
      {
      case LEFT:
      case RIGHT:
         delta = regionCenter.y - count / 2 * ( pinWidth + pinSpacing ) - region.y1;
         delta += index * ( region.GetDx( ) / ( vpr_type.capacity ? vpr_type.capacity : 1 ));
         break;
      case BOTTOM:
      case TOP:
         delta = regionCenter.x - count / 2 * ( pinWidth + pinSpacing ) - region.x1;
         delta += index * ( region.GetDy( ) / ( vpr_type.capacity ? vpr_type.capacity : 1 ));
         break;
      default:
         break;
      }
      *( pdeltaArray + sideIndex ) = delta;
   }
}

//===========================================================================//
// Method         : FindPinName_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::FindPinName_(
      const t_type_descriptor vpr_type,  
            int               pinIndex,
            string*           psrString,
            TC_TypeMode_t*    ptypeMode ) const
{
   const t_pb_type& vpr_pb_type = *vpr_type.pb_type;
   const t_pb_graph_node& vpr_pb_graph_node = *vpr_type.pb_graph_head;

   pinIndex %= ( vpr_pb_type.num_input_pins +
                 vpr_pb_type.num_output_pins +
                 vpr_pb_type.num_clock_pins );
   
   if( pinIndex < vpr_pb_type.num_input_pins ) 
   {
      for( int i = 0; i < vpr_pb_graph_node.num_input_ports; ++i ) 
      {
         if( pinIndex < vpr_pb_graph_node.num_input_pins[i] )
         {
            const t_pb_graph_pin* pvpr_pb_graph_pin = &vpr_pb_graph_node.input_pins[i][pinIndex];
            if( pvpr_pb_graph_pin )
            {
               this->FindPinName_( pvpr_pb_graph_pin, psrString );
               if( ptypeMode )
               {
                  *ptypeMode = TC_TYPE_INPUT;
               }
            } 
            break;
         } 
         pinIndex -= vpr_pb_graph_node.num_input_pins[i];
      }
   } 
   else if ( pinIndex < vpr_pb_type.num_input_pins + vpr_pb_type.num_output_pins ) 
   {
      pinIndex -= vpr_pb_type.num_input_pins;
      for( int i = 0; i < vpr_pb_graph_node.num_output_ports; ++i ) 
      {
         if( pinIndex < vpr_pb_graph_node.num_output_pins[i] )
         {
            const t_pb_graph_pin* pvpr_pb_graph_pin = &vpr_pb_graph_node.output_pins[i][pinIndex];
            if( pvpr_pb_graph_pin )
            {
               this->FindPinName_( pvpr_pb_graph_pin, psrString );
               if( ptypeMode )
               {
                  *ptypeMode = TC_TYPE_OUTPUT;
               }
            } 
            break;
         } 
         pinIndex -= vpr_pb_graph_node.num_output_pins[i];
      }
   } 
   else 
   {
      pinIndex -= vpr_pb_type.num_input_pins + vpr_pb_type.num_output_pins;
      for( int i = 0; i < vpr_pb_graph_node.num_clock_ports; ++i ) 
      {
         if( pinIndex < vpr_pb_graph_node.num_clock_pins[i] )
         {
            const t_pb_graph_pin* pvpr_pb_graph_pin = &vpr_pb_graph_node.clock_pins[i][pinIndex];
            if( pvpr_pb_graph_pin )
            {
               this->FindPinName_( pvpr_pb_graph_pin, psrString );
               if( ptypeMode )
               {
                  *ptypeMode = TC_TYPE_CLOCK;
               }
            } 
            break;
         } 
         pinIndex -= vpr_pb_graph_node.num_clock_pins[i];
      }
   }
}

//===========================================================================//
void TVPR_FabricModel_c::FindPinName_(
      const t_pb_graph_pin* pvpr_pb_graph_pin,
            string*         psrPinName ) const
{
   if( psrPinName )
   {
      if( pvpr_pb_graph_pin && 
          pvpr_pb_graph_pin->port && 
          pvpr_pb_graph_pin->port->name )
      {
         const char* pszName = pvpr_pb_graph_pin->port->name;
 
         size_t index = pvpr_pb_graph_pin->pin_number;
         TC_FormatStringNameIndex( pszName, index, psrPinName );
      }
   }
}

//===========================================================================//
// Method         : FindPinSide_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
TC_SideMode_t TVPR_FabricModel_c::FindPinSide_(
      int side ) const
{
   TC_SideMode_t pinSide = TC_SIDE_UNDEFINED;

   switch( side )
   {
   case LEFT:   pinSide = TC_SIDE_LEFT;  break;
   case RIGHT:  pinSide = TC_SIDE_RIGHT; break;
   case BOTTOM: pinSide = TC_SIDE_LOWER; break;
   case TOP:    pinSide = TC_SIDE_UPPER; break;
   default:                              break;
   }
   return( pinSide );
}

//===========================================================================//
TC_SideMode_t TVPR_FabricModel_c::FindPinSide_(
      const t_rr_node& vpr_rrNodePin,
      const t_rr_node& vpr_rrNodeChan ) const
{
   TC_SideMode_t pinSide = TC_SIDE_UNDEFINED;

   if(( vpr_rrNodePin.type == IPIN ) || ( vpr_rrNodePin.type == OPIN ))
   {
      if( vpr_rrNodeChan.type == CHANX )
      {
         if( vpr_rrNodeChan.ylow < vpr_rrNodePin.ylow )
         {
            pinSide = TC_SIDE_LOWER;
         }
         else // if( vpr_rrNodeChan.ylow == vpr_rrNodePin.ylow )
         {
            pinSide = TC_SIDE_UPPER;
         }
      }
      if( vpr_rrNodeChan.type == CHANY )
      {
         if( vpr_rrNodeChan.xlow < vpr_rrNodePin.xlow )
         {
            pinSide = TC_SIDE_LEFT;
         }
         else // if( vpr_rrNodeChan.xlow == vpr_rrNodePin.xlow )
         {
            pinSide = TC_SIDE_RIGHT;
         }
      }
   }
   return( pinSide );
}

//===========================================================================//
// Method         : FindPinDelta_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
double TVPR_FabricModel_c::FindPinDelta_(
            int     side,
            double* pdeltaArray ) const
{
   double pinWidth = TFV_MODEL_PIN_DEF_WIDTH;
   double pinSpacing = TFV_MODEL_PIN_DEF_SPACING;
   double pinDelta = *( pdeltaArray + side );

   *( pdeltaArray + side ) += pinWidth + pinSpacing;

   return( pinDelta );
}

//===========================================================================//
// Method         : FindChannelCount_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
unsigned int TVPR_FabricModel_c::FindChannelCount_(
      const TFV_FabricView_c& fabricView,
      const TGS_Point_c&      point,
            TC_SideMode_t     side ) const
{
   unsigned int channelCount = 0;

   TGS_Layer_t channelLayer = TFV_LAYER_UNDEFINED;
   switch( side )
   {
   case TC_SIDE_LEFT:
   case TC_SIDE_RIGHT: channelLayer = TFV_LAYER_CHANNEL_VERT; break;
   case TC_SIDE_LOWER:
   case TC_SIDE_UPPER: channelLayer = TFV_LAYER_CHANNEL_HORZ; break;
   default:                                                   break;
   }

   TGS_Region_c channelRegion;
   if( fabricView.FindNearest( channelLayer, point, &channelRegion ))
   {
      TFV_FabricData_c* pfabricData = 0;
      fabricView.Find( channelLayer, channelRegion, &pfabricData );

      switch( side )
      {
      case TC_SIDE_LEFT:
      case TC_SIDE_RIGHT: channelCount = pfabricData->GetTrackVertCount( ); break;
      case TC_SIDE_LOWER:
      case TC_SIDE_UPPER: channelCount = pfabricData->GetTrackHorzCount( ); break;
      default:                                                              break;
      }
   }
   return( channelCount );
}

//===========================================================================//
// Method         : FindChannelRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::FindChannelRegion_(
      const t_rr_node&         vpr_rrNode,
      const TFV_FabricView_c&  fabricView,
            TGS_Region_c*      pregion,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_LayerType_t layer = ( vpr_rrNode.type == CHANX ? 
                             TFV_LAYER_CHANNEL_HORZ : TFV_LAYER_CHANNEL_VERT );
   bool applyShift = true;
   bool applyTrack = false;

   return( this->FindNodeRegion_( vpr_rrNode, fabricView,
                                  layer, pregion, ppfabricData,
                                  applyShift, applyTrack ));
}

//===========================================================================//
// Method         : FindSegmentRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::FindSegmentRegion_(
      const t_rr_node&         vpr_rrNode,
      const TFV_FabricView_c&  fabricView,
            TGS_Region_c*      pregion,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_LayerType_t layer = ( vpr_rrNode.type == CHANX ? 
                             TFV_LAYER_CHANNEL_HORZ : TFV_LAYER_CHANNEL_VERT );
   bool applyShift = true;
   bool applyTrack = true;

   return( this->FindNodeRegion_( vpr_rrNode, fabricView,
                                  layer, pregion, ppfabricData,
                                  applyShift, applyTrack ));
}

//===========================================================================//
// Method         : FindBlockRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::FindBlockRegion_(
      const t_rr_node&         vpr_rrNode,
      const TFV_FabricView_c&  fabricView,
            TGS_Region_c*      pregion,
            TFV_FabricData_c** ppfabricData ) const
{
   pregion->Reset( );

   TGS_Layer_t ioLayer = TFV_LAYER_INPUT_OUTPUT;
   TGS_Layer_t pbLayer = TFV_LAYER_PHYSICAL_BLOCK;

   bool applyShift = false;
   bool applyTrack = false;

   if( !this->FindNodeRegion_( vpr_rrNode, fabricView,
                               ioLayer, pregion, ppfabricData,
                               applyShift, applyTrack ))
   {
      this->FindNodeRegion_( vpr_rrNode, fabricView,
                             pbLayer, pregion, ppfabricData,
                             applyShift, applyTrack );
   }
   return( pregion->IsValid( ) ? true : false );
}

//===========================================================================//
// Method         : FindNodeRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::FindNodeRegion_(
      const t_rr_node&         vpr_rrNode,
      const TFV_FabricView_c&  fabricView,
            TGS_Layer_t        layer,
            TGS_Region_c*      pregion,
            TFV_FabricData_c** ppfabricData,
            bool               applyShift,
            bool               applyTrack ) const
{
   TGS_Region_c region;
   TFV_FabricData_c* pfabricData = 0;

   TGS_OrientMode_t orient = ( vpr_rrNode.type == CHANX ?
                               TGS_ORIENT_HORIZONTAL : TGS_ORIENT_VERTICAL );

   TGS_Region_c rrRegion( vpr_rrNode.xlow, vpr_rrNode.ylow,
                          vpr_rrNode.xhigh, vpr_rrNode.yhigh );
   if( applyShift )
   {
      rrRegion.ApplyShift( 0.5, orient );
   }

   TFV_FabricFigure_t* pfabricFigure = 0;
   if( fabricView.IsSolidAll( layer, rrRegion, &pfabricFigure ))
   {
      region = pfabricFigure->GetRegion( );
      pfabricData = pfabricFigure->GetData( );

      if( applyTrack )
      {
         unsigned int index = static_cast< int >( vpr_rrNode.ptc_num );
         double track = pfabricData->CalcTrack( rrRegion, orient, index );

         layer = TFV_LAYER_SWITCH_BOX;
         TGS_Region_c prevRegion, nextRegion;
         if( orient == TGS_ORIENT_HORIZONTAL )
         {
            fabricView.FindNearest( layer, rrRegion, TC_SIDE_LEFT, &prevRegion );
            fabricView.FindNearest( layer, rrRegion, TC_SIDE_RIGHT, &nextRegion );

            region.Set( prevRegion.x2, track, nextRegion.x1, track );
         }
         if( orient == TGS_ORIENT_VERTICAL )
         {
            fabricView.FindNearest( layer, rrRegion, TC_SIDE_LOWER, &prevRegion );
            fabricView.FindNearest( layer, rrRegion, TC_SIDE_UPPER, &nextRegion );

            region.Set( track, prevRegion.y2, track, nextRegion.y1 );
         }
      }
   }

   if( pregion )
   {
      *pregion = region;
   }
   if( ppfabricData )
   {
      *ppfabricData = pfabricData;
   }
   return( region.IsValid( ) ? true : false );
}

//===========================================================================//
// Method         : FindSwitchSide_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
TC_SideMode_t TVPR_FabricModel_c::FindSwitchSide_( 
      const t_rr_node&    vpr_rrNode,
            TGS_DirMode_t refDir,
      const TGS_Point_c&  refPoint,
      const TGS_Region_c& switchRegion ) const
{
   TGS_Point_c nearestPoint( refPoint );

   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   if( switchRegion.IsWithin( nearestPoint, MinGrid.GetGrid( )))
   {
      if( vpr_rrNode.type == CHANX )
      {
         if( vpr_rrNode.direction == INC_DIRECTION )
         {
            nearestPoint.x = ( refDir == TGS_DIR_PREV ? 
                               switchRegion.x1 : switchRegion.x2 );
         }
         else // if( vpr_rrNode.direction == DEC_DIRECTION )
         {
            nearestPoint.x = ( refDir == TGS_DIR_PREV ? 
                               switchRegion.x2 : switchRegion.x1 );
         }
      }
      else // if( vpr_rrNode.type == CHANY )
      {
         if( vpr_rrNode.direction == INC_DIRECTION )
         {
            nearestPoint.y = ( refDir == TGS_DIR_PREV ? 
                               switchRegion.y1 : switchRegion.y2 );
         }
         else // if( vpr_rrNode.direction == DEC_DIRECTION )
         {
            nearestPoint.y = ( refDir == TGS_DIR_PREV ? 
                               switchRegion.y2 : switchRegion.y1 );
         }
      }
   }
   return( switchRegion.FindSide( nearestPoint ));
}

//===========================================================================//
// Method         : LoadBlockNameIndexList_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 03/27/13 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::LoadBlockNameIndexList_(
      const t_type_descriptor*    vpr_physicalBlockArray, 
            int                   vpr_physicalBlockCount,
            TVPR_NameIndexList_t* pblockNameIndexList ) const
{
   pblockNameIndexList->SetCapacity( vpr_physicalBlockCount );
   for( int i = 0; i < vpr_physicalBlockCount; ++i )
   {
      const t_type_descriptor* pvpr_physicalBlock = &vpr_physicalBlockArray[i];

      TC_NameIndex_c blockNameIndex( pvpr_physicalBlock->name, 
                                     pvpr_physicalBlock->index );
      pblockNameIndexList->Add( blockNameIndex );
   }
}

//===========================================================================//
// Method         : MapDataTypeToLayer_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
TGS_Layer_t TVPR_FabricModel_c::MapDataTypeToLayer_(
      TFV_DataType_t dataType ) const
{
   TGS_Layer_t layer = TGS_LAYER_UNDEFINED;
   switch( dataType )
   {
   case TFV_DATA_INPUT_OUTPUT:   layer = TFV_LAYER_INPUT_OUTPUT;   break;
   case TFV_DATA_PHYSICAL_BLOCK: layer = TFV_LAYER_PHYSICAL_BLOCK; break;
   case TFV_DATA_SWITCH_BOX:     layer = TFV_LAYER_SWITCH_BOX;     break;
   case TFV_DATA_CONNECTION_BOX: layer = TFV_LAYER_CONNECTION_BOX; break;
   case TFV_DATA_CHANNEL_HORZ:   layer = TFV_LAYER_CHANNEL_HORZ;   break;
   case TFV_DATA_CHANNEL_VERT:   layer = TFV_LAYER_CHANNEL_VERT;   break;
   case TFV_DATA_SEGMENT_HORZ:   layer = TFV_LAYER_SEGMENT_HORZ;   break;
   case TFV_DATA_SEGMENT_VERT:   layer = TFV_LAYER_SEGMENT_VERT;   break;
   default:                                                        break;
   }
   return( layer );
}

//===========================================================================//
// Method         : MapDataTypeToBlockType_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
TFM_BlockType_t TVPR_FabricModel_c::MapDataTypeToBlockType_( 
      TFV_DataType_t dataType ) const
{
   TFM_BlockType_t blockType = TFM_BLOCK_UNDEFINED;
   switch( dataType )
   {
   case TFV_DATA_INPUT_OUTPUT:   blockType = TFM_BLOCK_INPUT_OUTPUT;   break;
   case TFV_DATA_PHYSICAL_BLOCK: blockType = TFM_BLOCK_PHYSICAL_BLOCK; break;
   case TFV_DATA_SWITCH_BOX:     blockType = TFM_BLOCK_SWITCH_BOX;     break;
   default:                                                            break;
   }
   return( blockType );
}

//===========================================================================//
// Method         : MapBlockTypeToDataType_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
TFV_DataType_t TVPR_FabricModel_c::MapBlockTypeToDataType_(
      TFM_BlockType_t blockType ) const
{
   TFV_DataType_t dataType = TFV_DATA_UNDEFINED;

   switch( blockType )
   {
   case TFM_BLOCK_PHYSICAL_BLOCK: dataType = TFV_DATA_PHYSICAL_BLOCK; break;
   case TFM_BLOCK_INPUT_OUTPUT:   dataType = TFV_DATA_INPUT_OUTPUT;   break;
   case TFM_BLOCK_SWITCH_BOX:     dataType = TFV_DATA_SWITCH_BOX;     break;
   default:                                                           break;
   };
   return( dataType );
};

//===========================================================================//
// Method         : AddFabricViewRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
            TFV_DataType_t     dataType,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_FabricData_c fabricData( dataType );

   return( this->AddFabricViewRegion_( region, fabricData, 
                                       pfabricView, ppfabricData ));
}

//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
            TFV_DataType_t     dataType,
      const char*              pszName,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_FabricData_c fabricData( dataType );
   fabricData.SetName( pszName );

   return( this->AddFabricViewRegion_( region, fabricData, 
                                       pfabricView, ppfabricData ));
}

//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
            TFV_DataType_t     dataType,
      const char*              pszName,
      const char*              pszMasterName,
      const TGO_Point_c&       origin,
            unsigned int       sliceCount,
            unsigned int       sliceCapacity,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_FabricData_c fabricData( dataType );
   fabricData.SetName( pszName );
   fabricData.SetMasterName( pszMasterName );
   fabricData.SetOrigin( origin );
   fabricData.SetSliceCount( sliceCount );
   fabricData.SetSliceCapacity( sliceCapacity );

   return( this->AddFabricViewRegion_( region, fabricData, 
                                       pfabricView, ppfabricData ));
}

//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
            TFV_DataType_t     dataType,
      const char*              pszName,
      const TGO_Point_c&       origin,
            unsigned int       trackIndex,
            unsigned int       trackHorzCount,
            unsigned int       trackVertCount,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_FabricData_c fabricData( dataType );
   fabricData.SetName( pszName );
   fabricData.SetOrigin( origin );
   fabricData.SetTrackIndex( trackIndex );
   fabricData.SetTrackHorzCount( trackHorzCount );
   fabricData.SetTrackVertCount( trackVertCount );
   fabricData.InitMapTable( trackHorzCount, trackVertCount );

   return( this->AddFabricViewRegion_( region, fabricData, 
                                       pfabricView, ppfabricData ));
}

//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
            TFV_DataType_t     dataType,
      const char*              pszName,
            unsigned int       trackIndex,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TFV_FabricData_c fabricData( dataType );
   fabricData.SetName( pszName );
   fabricData.SetTrackIndex( trackIndex );

   return( this->AddFabricViewRegion_( region, fabricData, 
                                       pfabricView, ppfabricData ));
}

//===========================================================================//
bool TVPR_FabricModel_c::AddFabricViewRegion_(
      const TGS_Region_c&      region,
      const TFV_FabricData_c&  fabricData,
            TFV_FabricView_c*  pfabricView,
            TFV_FabricData_c** ppfabricData ) const
{
   TGS_Layer_t layer = this->MapDataTypeToLayer_( fabricData.GetDataType( ));

   bool ok = pfabricView->Add( layer, region, fabricData );
   if( ok )
   {
      if( ppfabricData )
      {
         pfabricView->Find( layer, region, ppfabricData );
      }
   }
   return( ok );
}

//===========================================================================//
// Method         : DeleteFabricViewRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
void TVPR_FabricModel_c::DeleteFabricViewRegion_(
      const TGS_Region_c&     region,
      const TFV_FabricData_c& fabricData,
            TFV_FabricView_c* pfabricView ) const
{
   TGS_Layer_t layer = this->MapDataTypeToLayer_( fabricData.GetDataType( ));
   pfabricView->Delete( layer, region );
}

//===========================================================================//
// Method         : ReplaceFabricViewRegion_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/25/12 jeffr : Original
//===========================================================================//
bool TVPR_FabricModel_c::ReplaceFabricViewRegion_(
      const TGS_Region_c&     region,
      const TGS_Region_c&     region_,
      const TFV_FabricData_c& fabricData,
            TFV_FabricView_c* pfabricView ) const
{
  TFV_FabricData_c fabricData_( fabricData );

   TGS_Layer_t layer = this->MapDataTypeToLayer_( fabricData.GetDataType( ));
   pfabricView->Delete( layer, region );
   return( pfabricView->Add( layer, region_, fabricData_ ));
}

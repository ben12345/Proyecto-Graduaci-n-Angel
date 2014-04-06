//-----------------------------------------------------------------------------------------------
//
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD       TTTTTTTTTTTTTTTTTTTTTTT  iiii
//     B::::::::::::::::B  D::::::::::::DDD    T:::::::::::::::::::::T i::::i
//     B::::::BBBBBB:::::B D:::::::::::::::DD  T:::::::::::::::::::::T  iiii
//     BB:::::B     B:::::BDDD:::::DDDDD:::::D T:::::TT:::::::TT:::::T
//       B::::B     B:::::B  D:::::D    D:::::DTTTTTT  T:::::T  TTTTTTiiiiiii
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T        i:::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B:::::::::::::BB    D:::::D     D:::::D       T:::::T         i::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D    D:::::D        T:::::T         i::::i
//     BB:::::BBBBBB::::::BDDD:::::DDDDD:::::D       TT:::::::TT      i::::::i
//     B:::::::::::::::::B D:::::::::::::::DD        T:::::::::T      i::::::i
//     B::::::::::::::::B  D::::::::::::DDD          T:::::::::T      i::::::i
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD             TTTTTTTTTTT      iiiiiiii
//
//-----------------------------------------------------------------------------------------------
// ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
//
// BDTI provides expert engineering services, including algorithm design and implementation of
// software for embedded vision applications. For questions regarding this demo or information
// on BDTI’s engineering services, contact BDTI:
//
// info@BDTI.com                      +1 510-451-1800                   http://www.BDTI.com
//
// Berkeley Design Technology, Inc.(BDTI) makes no warranty for the use of this software, which
// has been developed for demonstration purposes only.  This software is provided  "as is".  BDTI
// assumes no responsibility for any errors which may appear in this software, nor does it make
// a commitment to update the information contained herein. BDTI specifically disclaims any implied
// warranties of fitness for a particular purpose.  You are solely responsible for determining
// the suitability of this software for your purposes and you use it at your own risk. In no event
// shall BDTI be held liable for any damages of any kind arising out of your use of or inability to
// use this software.
//-----------------------------------------------------------------------------------------------
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Suppress MISRA checking for example code")
#endif


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <blackfin.h>
#include "adi_image_tool_box.h"
#include "adi_contour_mem_alloc.h"
#include "adi_math_utils.h"
#include "adi_sigprocess.h"

//-------------------------------------------------------------------------------------
// ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
// ------------------------------------------------------------------------------------
// Height and Width below refer to contour bounding box Height and Width
// The area of a circle inscribed in a square is (area of square)*pi/4
//
// Dot parameters were measured with camera 11.5 inches above surface
// Dot diameter = 15 pixels
//
// When using Pink foam, sensor is 10.75 inches above foam surface.
// Dot diameter = 17 pixels
// Dot diameter = 16 pixels
//
#define MIN_SYMETRY 	8								// Symetry = (10*Height)/Width;
#define MAX_SYMETRY 	11								// Symetry = (10*Height)/Width;
#define MIN_RATIO		85								// Ratio =
#define MAX_RATIO		115								//
#define DIAMETERMIN		14								// Diameter(in pixels) of circle
#define DIAMETERMAX		30								// Diameter(in pixels) of circle
#define BOXAREAMIN		(DIAMETERMIN*DIAMETERMIN)		// BoxArea(in pixels) = Height*Width;
#define BOXAREAMAX		(DIAMETERMAX*DIAMETERMAX)		// BoxArea(in pixels) = Height*Width;
#define CIRCLEAREAMIN	(BOXAREAMIN*314)/400   			// BOXAREAMIN(in pixels)*pi/4
#define CIRCLEAREAMAX	(BOXAREAMAX*314)/400			// BOXAREAMIN(in pixels)*pi/4
// -------------------------------------------------------------------------------------

//#define PROFILE

#ifndef PROFILE
#define printf
#else
#include "profile.h"
#endif

#define TEMP_BUFFER_SIZE    (0x1800 * 1000)


extern ADI_ITB_MODULE_STATUS ADIContoursRLEWrapper(
                                            ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                            int8_t                          *pIn,
                                            uint32_t                        nInputBytes,
                                            ADI_CONT_RUN_LEN_HDR            *pRunLenListHdr,
                                            uint32_t                        dimX,
                                            uint32_t                        dimY
                                            );

static bool InitMemory    = true;

ALIGN(4)
#pragma section ("sdram_bank4_A")
int8_t  aTempBuffer[TEMP_BUFFER_SIZE];

#pragma section ("L1_data_a")
uint32_t                    nTotalRLENodes;

extern CONTOUR_INFO oCONTOURInfo;

uint32_t adi_ContourArea( ADI_CONT_SEGMENT_HDR_ROW_WISE *pSegmentListRowWiseHdr, uint32_t nHeight )
{
    ADI_CONT_RUN_LEN_NODE   *pRunLenNodeTemp;
    uint32_t                i;
    uint32_t                mypixels, min, max;

    mypixels = 0;
    for (i = 0; i < nHeight; i++)
    {
        min = 0xffff;
        max = 0;
        pRunLenNodeTemp = pSegmentListRowWiseHdr[i].pNext;
        while (pRunLenNodeTemp)
        {
            if( pRunLenNodeTemp->xStart < min ) min = pRunLenNodeTemp->xStart;
            if( pRunLenNodeTemp->xEnd > max ) max = pRunLenNodeTemp->xEnd;

            pRunLenNodeTemp = pRunLenNodeTemp->pNext;
        }
        mypixels += (max-min+1);
    }
    return( mypixels );
}

void adi_FillArea( uint8_t *pImage, uint32_t nWidth,
                  ADI_CONT_SEGMENT_HDR_ROW_WISE *pSegmentListRowWiseHdr,
                  uint32_t nHeight, uint8_t fill )
{
    ADI_CONT_RUN_LEN_NODE   *pRunLenNodeTemp;
    uint32_t                i,j;
    uint32_t                min, max, yoffset;

    for (i = 0; i < nHeight; i++)
    {
        min = 0xffff;
        max = 0;
        pRunLenNodeTemp = pSegmentListRowWiseHdr[i].pNext;
        while (pRunLenNodeTemp)
        {
            if( pRunLenNodeTemp->xStart < min ) min = pRunLenNodeTemp->xStart;
            if( pRunLenNodeTemp->xEnd > max ) max = pRunLenNodeTemp->xEnd;
            yoffset = pRunLenNodeTemp->y*nWidth;
            pRunLenNodeTemp = pRunLenNodeTemp->pNext;
        }
        for( j=min; j<max; ++j )
            pImage[j+ yoffset] = fill;
    }
}


ADI_CONTOUR_DYNAMIC_MEM_ALLOC   oDMA;
ADI_CONT_RUN_LEN_HDR            *pRunLenListHdr;
ADI_CONT_BOUNDING_RECTANGLE     *pBoundingRectangle;

uint32_t ADI_FindContours( uint8_t *pImage, uint32_t nWidth, uint32_t nHeight )
{
    uint32_t                        i, pixels;
    uint32_t                        j, CONTOURCount;
    uint32_t                        nNumSegments;
    ADI_ITB_MODULE_STATUS           nReturnValue;
    uint32_t                        index,nColour;

    ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA;
    ADI_CONT_SEGMENT_HDR            *pSegmentListHdr;
    ADI_CONT_SEGMENT_HDR            *pSegmentListTempHdr;
    ADI_CONT_SEGMENT_HDR_ROW_WISE   *pSegmentListRowWiseHdr;

    //-------------------------------------------------------------------------------------
    // ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
    // ------------------------------------------------------------------------------------
    uint32_t                        nArea, nBoxArea, nSymetry, nRatio;

    pDMA = &oDMA;
    pSegmentListHdr = NULL;
    pSegmentListTempHdr = NULL;
    pSegmentListRowWiseHdr = NULL;
    CONTOURCount = 0;
    nTotalRLENodes = 0;
    index =0;
    oCONTOURInfo. nNumberOfObjects =0;
#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif

    if( InitMemory )
    {
        nReturnValue = adi_DynamicMemInit(pDMA,
                                          aTempBuffer,
                                          TEMP_BUFFER_SIZE,
                                          nHeight*4);
        if ((nReturnValue & 0xFFFF) != (ADI_ITB_STATUS_SUCCESS))
        {
            printf("adi_DynamicMemInit - Not enough memory\n");
            //InitMemory = true;
            return (0);
        }
        pRunLenListHdr = (ADI_CONT_RUN_LEN_HDR *)pDMA->pMemAlloc(pDMA,
                                                                 ADI_CONTOUR_RUN_LEN_HDR,
                                                                 nHeight);
        if (pRunLenListHdr == NULL)
        {
            printf("pRunLenListHdr - Not enough memory\n");
            //InitMemory = true;
            return (0);
        }

        pBoundingRectangle = (ADI_CONT_BOUNDING_RECTANGLE *)pDMA->pMemAlloc(pDMA,
                                                                            ADI_CONTOUR_BOUNDING_RECT,
                                                                            1);
        if (pBoundingRectangle == NULL)
        {
            printf("pBoundingRectangle - Not enough memory\n");
            //InitMemory = true;
            return (0);
        }

        InitMemory = false;
    }

#ifdef PROFILE
    PROFEND(goProfileFunc);
    printf("adi_DynamicMemInit time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif
#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif


#ifdef PROFILE
    PROFEND(goProfileFunc);
    printf("Allocating memory time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif

#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif

#if 1
        nReturnValue = ADIContoursRLEWrapper(     pDMA,
                                                (int8_t *)pImage,
                                                nWidth*nHeight,
                                                pRunLenListHdr,
                                                nWidth,
                                                nHeight );
#else // L3
        for (j = 0; j < nHeight; j++)
                {
                    pRunLenListHdr[j].pNext = NULL;
                    nReturnValue = adi_contour_RLE(pDMA,
                                                   (uint8_t *)pImage + j * nWidth,
                                                   nWidth,
                                                   j,
                                                   &pRunLenListHdr[j]);
                }

#endif

        if ((nReturnValue & 0xFFFF) != (ADI_ITB_STATUS_SUCCESS))
        {
             if ((nReturnValue & 0xFFFF) == (ADI_ITB_STATUS_CONT_NO_RUN_LEN))
                        {
                            printf("Not enough memory\n");
                            printf("\tNot enough ADI_CONT_RUN_LEN_NODE nodes created.\nEdit MAX_NUM_RUN_LEN_NODE in adi_contour_mem_alloc.c to increase number of nodes created\n");
                        }
                        else
                        {
                            printf("Error!!!\n");
                        }
                        pDMA->pMemReset(pDMA,
                                        NULL,
                                        ADI_CONTOUR_RUN_LEN_NODE,
                                        0);
                        return (0);
        }


#ifdef PROFILE
        PROFEND(goProfileFunc);
        printf("adi_contour_RLE time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif
#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif
        nReturnValue = adi_contour_segment_image(pDMA,
                                                 pRunLenListHdr,
                                                 nHeight,
                                                 &pSegmentListHdr,
                                                 &nNumSegments);
        if ((nReturnValue & 0xFFFF) != (ADI_ITB_STATUS_SUCCESS))
                {
                    if ((nReturnValue & 0xFFFF) == (ADI_ITB_STATUS_CONT_NO_SEG_HDR))
                    {
                        printf("Not enough memory\n");
                        printf("\tNot enough ADI_CONTOUR_SEG_HDR nodes created.\nEdit MAX_NUM_SEGMENT_HDR in adi_contour_mem_alloc.c to increase number of nodes created\n");
                    }
                    else if ((nReturnValue & 0xFFFF) == (ADI_ITB_STATUS_CONT_NO_EQU_HDR))
                    {
                        printf("Not enough memory\n");
                        printf("\tNot enough ADI_CONTOUR_EQU_SEG_HDR nodes created.\nEdit MAX_NUM_EQU_SEGMENT_HDR in adi_contour_mem_alloc.c to increase number of nodes created\n");
                    }
                    else if ((nReturnValue & 0xFFFF) == (ADI_ITB_STATUS_CONT_NO_EQUNODE))
                    {
                        printf("Not enough memory\n");
                        printf("\tNot enough ADI_CONTOUR_EQU_SEG_NODE nodes created.\nEdit MAX_NUM_EQU_SEGMENT_NODE in adi_contour_mem_alloc.c to increase number of nodes created\n");
                    }
                    else
                    {
                        printf("Error!!!\n");
                    }
                    pDMA->pMemReset(pDMA,
                                    NULL,
                                    ADI_CONTOUR_RUN_LEN_NODE,
                                    0);
                    pDMA->pMemReset(pDMA,
                                    NULL,
                                    ADI_CONTOUR_SEG_HDR,
                                    0);
                    pSegmentListHdr = NULL;
                    pDMA->pMemReset(pDMA,
                                    NULL,
                                    ADI_CONTOUR_EQU_SEG_HDR,
                                    0);
                    return (0);
                }

#ifdef PROFILE
        PROFEND(goProfileFunc);
        printf("adi_contour_segment_image time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif


        for(pSegmentListTempHdr = pSegmentListHdr ;pSegmentListTempHdr;  )
        {
#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif
            adi_contour_BoundingRectangle(pSegmentListTempHdr,
                                          pBoundingRectangle);
#ifdef PROFILE
        PROFEND(goProfileFunc);
        printf("Bounding Rectange time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif
#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif
            pSegmentListRowWiseHdr = (ADI_CONT_SEGMENT_HDR_ROW_WISE *)pDMA->pMemAlloc(pDMA,
                                                              ADI_CONTOUR_SEG_HDR_ROW_WISE,
                                                              (pBoundingRectangle->nHeight));
            if (pSegmentListRowWiseHdr == NULL)
            {
                printf("pSegmentListRowWiseHdr - Not enough memory\n");
                pDMA->pMemFree(pDMA,
                               pSegmentListRowWiseHdr,
                               ADI_CONTOUR_SEG_HDR_ROW_WISE,
                               (pBoundingRectangle->nHeight));
                return (0XFFFFFFFF);
            }


            adi_contour_SplitSegmentRowWise(pSegmentListTempHdr,
                                            pSegmentListRowWiseHdr,
                                            pBoundingRectangle);

#ifdef PROFILE
        PROFEND(goProfileFunc);
        printf("SplitSegmentRowWise+adi_ContourArea time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif

//-------------------------------------------------------------------------------------------------
//
// Insert Contour classifier code here.
//
// Conceptually similar to boundingRect() in OpenCV
//     	adi_contour_BoundingRectangle() returns the bounding rectangle of a contour.
//		The dimensions are in pixels.
// pBoundingRectangle->nHeight = Height of the rectangle bounding the contour
// pBoundingRectangle->nWidth  = Width of the rectangle bounding the contour
//
// Conceptually similar to contourArea() in OpenCV
//		adi_ContourArea() returns the area of a contour.
//		The area returned is in pixels^2
//
// oDotInfo is a structure used by the graphics module to display bounding boxes
//		see Graphics.c
//		nColour is the bonding box color in ARGB format (0x7FRRGGBB)
//-------------------------------------------------------------------------------------------------

        //-------------------------------------------------------------------------------------
        // ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
        // ------------------------------------------------------------------------------------
        nArea = adi_ContourArea( pSegmentListRowWiseHdr, pBoundingRectangle->nHeight );
        nBoxArea = pBoundingRectangle->nHeight*pBoundingRectangle->nWidth;
        nSymetry = (10*pBoundingRectangle->nHeight)/pBoundingRectangle->nWidth;

        //---------------------------------------------------------------------------------------------
        //
        // 			BDTI Full Dice Dot Classifier Cascade - 02/18/2013
        //
        //			1st level - Filter out circles that are very big or very small
        //			2nd level - Ratio of calculated area of inscribed circle to measured contour area
        //						Calculated area of inscribed circle = (((contour bounding box area)*PI)/4)
        //						Measured area of arbitrary contour.
        //						Ratio = ((nBoxArea*314)/4)/nArea = 100*((Box Area *PI)/(4*Circle area))
        //			3rd level - Symmetry of contour (bounding box height / bounding box width).
        //						(10*pBoundingRectangle->nHeight)/pBoundingRectangle->nWidth;
        //
        //			If the contour passes through all three levels, it is classified as a Dice Dot and counted.
        //
        //---------------------------------------------------------------------------------------------
        nColour = 0x7F000000; // Black - Used for Demo Mode 1
        if( (nArea >= CIRCLEAREAMIN) && (nArea <= CIRCLEAREAMAX ) )
        {
        	nRatio = (nBoxArea*314)/4;
        	nRatio /= nArea;  // nRatio = ((Box Area * 314)/4)/nArea = 100*((Box Area *PI)/(4*Circle area))

        	nColour = 0x7FFF0000; // RED - Used for Demo Mode 1

        	//adi_FillArea( pImage, nWidth, pSegmentListRowWiseHdr, pBoundingRectangle->nHeight, nBoxArea/10 );
        	//adi_FillArea( pImage, nWidth, pSegmentListRowWiseHdr, pBoundingRectangle->nHeight, nArea/10 );
        	//adi_FillArea( pImage, nWidth, pSegmentListRowWiseHdr, pBoundingRectangle->nHeight, nRatio );
        	if( (nRatio >= MIN_RATIO) && (nRatio <= MAX_RATIO) )
        	{
        		nColour = 0x7F0000FF; // Blue - Used for Demo Mode 1

        		//adi_FillArea( pImage, nWidth, pSegmentListRowWiseHdr, pBoundingRectangle->nHeight, nSymetry ); //nSymetry );
        		if( (nSymetry >= MIN_SYMETRY) && (nSymetry <= MAX_SYMETRY) )
        		{
        			adi_FillArea( pImage, nWidth, pSegmentListRowWiseHdr, pBoundingRectangle->nHeight, 3 );

        			nColour = 0X7F00FF00; // Green - Used for Demo Mode 1
        			CONTOURCount++;
        		}
        	}
        }
        //---------------------------------------------------------------------------------------------

		// Limit viewable bounding rectangles to 1.5 times MAX box area
        if( (nBoxArea <= ((BOXAREAMAX*3)/2) )  )
        {
        	nColour = 0x7F000000 + (nArea&0x00FFFFFF);

        	if(index < MAX_OBJ_INFO)
        	{
        		oCONTOURInfo.aCONTOURInfo[index].nHeight       = pBoundingRectangle->nHeight;
        		oCONTOURInfo.aCONTOURInfo[index].nWidth        = pBoundingRectangle->nWidth;
        		oCONTOURInfo.aCONTOURInfo[index].nXBottomRight = pBoundingRectangle->nXBottomRight;
        		oCONTOURInfo.aCONTOURInfo[index].nXTopLeft     = pBoundingRectangle->nXTopLeft;
        		oCONTOURInfo.aCONTOURInfo[index].nYBottomRight = pBoundingRectangle->nYBottomRight;
        		oCONTOURInfo.aCONTOURInfo[index].nYTopLeft     = pBoundingRectangle->nYTopLeft;
        		oCONTOURInfo.nColour[index++]  = nColour;
        	}
        }

#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif

            pDMA->pMemFree(pDMA,
                            pSegmentListRowWiseHdr,
                            ADI_CONTOUR_SEG_HDR_ROW_WISE,
                            (pBoundingRectangle->nHeight));
            pSegmentListRowWiseHdr = NULL;

#ifdef PROFILE
        PROFEND(goProfileFunc);
        printf("inside for memfree memreset time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif

            pSegmentListTempHdr = pSegmentListTempHdr->pDown;
        } // while

//----------------------------------------------------------------------------------------

#ifdef PROFILE
    goProfileFunc.nSum = 0;
    PROFBEG(goProfileFunc);
#endif

    pDMA->pMemReset(pDMA,
                    pSegmentListHdr,
                    ADI_CONTOUR_SEG_HDR,
                    0);

    pDMA->pMemReset(pDMA,
                    pRunLenListHdr,
                    ADI_CONTOUR_RUN_LEN_NODE,
                    nHeight);
    pSegmentListHdr = NULL;

    pDMA->pMemReset(pDMA,
                    NULL,
                    ADI_CONTOUR_EQU_SEG_HDR,
                    0);

#ifdef PROFILE
    PROFEND(goProfileFunc);
    printf("Memory Cleanup time(ms) = %d\n", goProfileFunc.nSum/500000);
#endif
    oCONTOURInfo. nNumberOfObjects = index;
    oCONTOURInfo.nCONTOURCount = CONTOURCount;
    return (CONTOURCount);
}







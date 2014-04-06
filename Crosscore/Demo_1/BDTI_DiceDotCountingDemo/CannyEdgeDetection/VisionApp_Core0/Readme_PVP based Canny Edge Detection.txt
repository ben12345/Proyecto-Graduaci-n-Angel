            Analog Devices, Inc. CrossCore Embedded Studio(tm) Application Example

Project Name: CannyEdgeDetection

Description:  This example demonstrates how to capture video from the MT9M114
              sensor and loop back the video to the HDMI output interface (ADV7511). 
              The sensor is configured to output the image in YUV (UYVY) format and output 
              image size is configured to 720p (1280x720). The sensor input and output frame
              rate is  configured to 30 frames per second. The same video is streamed to PVP and 
              its output is processed for edge information. The captured image is also displayed 
              along with the detected edges through the HDMI interface. The example uses the ADV7511
              video encoder. 
              This example also demonstrates how both cores of BF609 is used to process the data. 
              Video capture/Display and processing of the PVP output(edge trace) is done by the CORE-0 
              while marking of the dots using the graphics is done by the CORE-1. 
              MACAPI services provided along with the  CrossCore Embedded Studio 1.0.1 is used for 
              Intercore communication.
                
              
              NOTE: This example depends upon  CrossCore Embedded Studio 1.0.1 with  its patch  1.0.1.1 along with 
                    the following Add-ins:

                1.  Video Encoder EI3 Extender V1.0.1
                2.  ADI Blackfin 2D Graphics Library Software Module Package version 3.2.0
                3.  ADI Blackfin Image Processing Toolbox Software Module Package version 2.2.1
                4.  ADI Blackfin Vision Analytics Toolbox (VAT) version 2.2.1
                5.  Camera EI3 Extender  Board Support Package 1.0.1


                    if a earlier  version of the any of above Add-ins is installed, you may ignore the message to upgrade it.
                    
                    If an earlier version of an Add-in is installed instead of the specified version, then the Add-in
                    should be installed and may need to be upgraded manually. 
                    
                    How to upgrade the Add-ins:
                    
                     o double click on the system.svc file. This will show list of 
                       installed Add-ins in the project
                    
                     o Click on the specified  Add-in and click the 
                       Upgrade button
                                          
                     o Clean and rebuild the project.
    
Processor:
=============
    ADSP-BF609

Tested with:
====================
    FinBoard
    Analog Devices CrossCore Embedded Studio
    ICE-100B emulator


Overview:
=========      
    The MT9M114 sensor transmits the video content via the PPI peripheral which is 
    configured as 16 bit wide.  The video content is stored in two ping/pong
    buffers in YUV format. The MT9M114 sensor sources the frame sync signals FS1 and FS2,
    and PPI clock. The captured data is also fed to the PVP for proccessing to extract the 
    edge information using the Canny algorithm. The application uses the Video Analytic Toolbox and 
    Image Processing Toolbox to process PVP output.
    The captured video is transmitted to Video Encoder (ADV7511) for displaying along with the dot 
    count. 
    The Video Encoder sources FS1 and FS2 and PPI Clock.

    Both MT9M114 and Video Encoder(ADV7511) device drivers operate in callback mode.

Hardware Setup:
===============

      FINBOARD SWITCH SETTINGS
      -----------------------------------
      SW2:  Press Button - Reset switch
      SW3:  Press Button - Toggle between overlay display modes
      SW4:  Press Button - Toggle between LED illumination levels 

      FINBOARD JUMPER SETTINGS
     -----------------------------------
      none


    o Connect HDMI cable between the FINBOARD's HDMI output interface (J6) and the monitor or TV. 
    o Connect ICE-100B emulator on FINBOARD's debug connector (J2)
    o Connect 5V DC power supply to FINBOARD's power connector (P1)
    

         
User Configuration Macros:
==========================
	none
           
References:
===========
    FINBOARD Hardware Reference Manual (finboard.org)
    FINBOARD Schematic (finboard.org)
    MT9M114 Developer's Guide
    MT9M114 Data Sheet
    MT9M114 Register Reference Manual.

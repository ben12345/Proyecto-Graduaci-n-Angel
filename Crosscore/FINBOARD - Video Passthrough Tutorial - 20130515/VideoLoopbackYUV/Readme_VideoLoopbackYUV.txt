            Analog Devices, Inc. CrossCore Embedded Studio(tm) Application Example

Project Name: VideoLoopbackYUV

Description:  This example demonstrates how to capture video from the MT9M114
              sensor and loop back the video via the Video Encoder (ADV7511). 
              The sensor is configured to output the image in YUV (UYVY) format and output 
              image size is configured to 480p (720x480). The sensor output frame rate is
              configured to 30 frames per second. The captured image is looped back to the
              display through the HDMI interface of Video Encoder EI3 Extender Board. 
              The example uses the ADV7511 video encoder which is on 
              Video Encoder EI3 Extender Board. 
              
              NOTE: As the video loopback example relies on both the MT9M114 sensor
                    and EI3 Video Encoder products, both should be installed for the
                    example to work.

              NOTE: This example depends upon the Add-ins of the 
                    Video Encoder EI3 Extender Board Support Package(BSP). The example
                    is built with v1.0.1 of the Video Encoder EI3 Extender BSP Add-ins.
                                          
Processor:
=============
    ADSP-BF609

Tested with:
====================
    ADSP-BF609 EZ-Board
    MT9M114 Demo Headboard (Part number MT9M114EBLSTCZH)
    Camera EI3 Extender Board
    Video Encoder EI3 Extender Board 
    Analog Devices CrossCore Embedded Studio
    ADDS HPUSB-ICE


Overview:
=========      
    The MT9M114 sensor transmits the video content via the PPI peripheral which is 
    configured as 16 bit wide.  The video content is stored in two ping/pong
    buffers in YUV format. The MT9M114 sensor sources the frame sync signals FS1 and FS2,
    and PPI clock.

    The captured video is transmitted to Video Encoder (ADV7511) for displaying. 
    The Video Encoder sources FS1 and FS2 and PPI Clock.

    Both MT9M114 and Video Encoder device drivers operate in non-blocking mode.

Hardware Setup:
===============
    o Attach MT9M114 Demo Head board (MT9M114EBLSTCZH) to Camera EI3 Extender Board's J2 connector. 
    o Attach the Camera EI3 Extender Board to P2A connector of ADSP-BF609 EZ-Board.
    o Attach the Video Encoder EI3 Extender Board (connector J1) to ADSP-BF609 EZ-Board (connector P1A).
    o Connect HDMI cable between the Video Encoder EI3 Extender Board's HDMI output interface (J5) and the TV. 
    o Place a jumper between the pins 1-2 (8-bit) of JP1 on the Camera EI3 Extender Board.
         
Software Switches:
==================
    ADSP-BF609 EZ-Board:  Switch1 GPIOA: Enable Parallel Flash.
                          Disable all the other lines.
    
                          Please refer to the ADSP-BF609 EZ-Board Evaluation System Manual for 
                          information on software-controlled switches.
    
    Video Encoder EI3 Extender Board:   GPIOA: Enable SPORT, enable 27 MHhz Clock, enable 16bit PPI connection to ADV7511.
                                        GPIOB: Enable ADV7511 interrupt GPIO4.
                      
                          Other soft switches are set to default values. Please refer Video Encoder EI3 Extender Board
                          extender user guide for default software controlled switch configuration.      
      
User Configuration Macros:
==========================
    EXAMPLE_TIMEOUT:  By default the example times out after the define number of buffers are looped
                      back. The timeout can be changed to a bigger value to increase the timeout. 
                      The timeout can be avoided by commenting the define or setting the value to 0.  
           
References:
===========
    ADSP-BF609 Hardware Reference Manual
    ADSP-BF609 EZ-Board Schematic
    Camera EI3 Extender Board Hardware Reference Manual
    Camera EI3 Extender Board Schematic
    MT9M114 Developer's Guide
    MT9M114 Data Sheet
    MT9M114 Register Reference Manual.
    Video Encoder EI3 Extender Board Hardware Reference Manual
    Video Encoder EI3 Extender Board Schematic

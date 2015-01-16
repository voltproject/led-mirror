#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ledmirror.h"
#include "utils.h"
#include "menu.h"


static unsigned clearframe[2048] = {0};
static int loopingClock = 0;

int bufferInitialized = 0;
uint8_t *displayBuffer;

int painter_quantize(int level)
{
    int output_pixel;
    if(level > 200){
        output_pixel = 3;
    }else{
        output_pixel = 0;
    }
    return output_pixel;
}

static void createBuffers(){
    if(bufferInitialized){
        return;
    }
    displayBuffer = malloc(BUFFER_SIZE);
    bufferInitialized = 1;
}

void painter_createOutputVideo(uint8_t* inputbuffer, uint8_t* outputBuffer){
    int i;
    for(i =0; i< BUFFER_SIZE;i++){
        int pix = 0;
        if (inputbuffer[i] > 200){
            pix = 240;
        }
        pix = outputBuffer[i] + pix;
        if(pix > 255){
            pix = 255;
        }
        outputBuffer[i] = pix;
    }
}

void painter_videoFrameDidRender(MMAL_BUFFER_HEADER_T *buffer, int framecounter){
    painter_createOutputVideo(buffer->data, recordedBuffers[0], displayBuffer); 
    playbackFrame(displayBuffer);   
}

void painter_videoFrameWillRender(int framecounter){

    loopingClock ++;
    
    if(loopingClock == 300){
        setDisplayMode(displayModeVideoAndOverlay);
        loopingClock = 0;
        returnToMenu();
        return;
    }

}

void painter_init()
{
    printf("INIT PAINTER\n");
    displayImage(clearframe);
    createBuffers();
    loopingClock = 0;
}


#!/bin/bash

BASENAME='/root/marte/cerebro/camera/photos/'
BRIDGE=bridge
PORT=80

# params 
# file in dev (/dev/video1)
# $2 -> sequence
function shot {
    TS=$BASENAME/`date +"%Y%m%d%H%M%S"`.jpg
    echo 'capturing a photo in: '$TS
    ffmpeg -f v4l2 -s 640x480 -i /dev/video0 -vframes 1 $TS
    echo 'sending to the bridge'
    send $TS
    exit 0
}

function send {
    FILENAME=`basename $1`
    curl -i -X POST http://bridge.pc/lib/photo.php?name=$FILENAME --data-binary "@$1"
}

shot


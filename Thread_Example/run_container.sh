#!/bin/bash
export ROOTPATH="$(dirname "$(readlink -f "$0")")"
sudo docker run -it --rm -v $ROOTPATH/src:/tmp/src ezetowers/build_image:0.0.1 bash

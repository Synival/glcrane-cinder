#!/bin/sh
mkdir -p _build && \
cd _build && \
cmake .. && \
make -j4 && \
cd .. && \
if [ ! \( -e "GLCrane" \) ]; then
   ln -s _build/Debug/GLCrane/GLCrane ./GLCrane
fi

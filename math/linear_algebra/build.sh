#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g main.c mat2/mat2.c mat3/mat3.c mat4/mat4.c test.c vec2/vec2.c vec3/vec3.c vec4/vec4.c  -o prg.out
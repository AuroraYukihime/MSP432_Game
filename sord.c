/* Copyright (c) 2012, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <ti/grlib/grlib.h>

static const unsigned char pixel_sord4BPP_UNCOMP[] =
{
0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
0xee, 0xee, 0xee, 0xe4, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
0xee, 0xee, 0xee, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xee, 
0xe7, 0xd7, 0xd7, 0x23, 0xac, 0xc5, 0xac, 0x8c, 0xcc, 0xae, 
0xed, 0x1d, 0x9d, 0x60, 0x30, 0x30, 0x34, 0x00, 0x3a, 0xce, 
0xe7, 0x77, 0x77, 0x20, 0xcc, 0xca, 0xcc, 0xac, 0x58, 0xae, 
0xee, 0xee, 0xee, 0xe0, 0xac, 0xcc, 0xca, 0xc8, 0xae, 0xee, 
0xee, 0xee, 0xee, 0xe3, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
0xee, 0xee, 0xee, 0xe0, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee
};

static const unsigned long palette_sord4BPP_UNCOMP[]=
{
	0x668066, 	0xffaa33, 	0xff8000, 	0x668099, 
	0x998099, 	0x99d599, 	0xff8033, 	0xffd500, 
	0x99aa99, 	0xffd533, 	0xccaacc, 	0x000000, 
	0xccd5cc, 	0xffaa00, 	0x000000, 	0xffffff
};

const tImage  sord4BPP_UNCOMP=
{
	IMAGE_FMT_4BPP_UNCOMP,
	20,
	10,
	16,
	palette_sord4BPP_UNCOMP,
	pixel_sord4BPP_UNCOMP,
};


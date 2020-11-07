/*-
 A tool to help to make an interpreter.
 Copyright © 2020 yiwen xue
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
 must display the following acknowledgement:
 This product includes software developed by the organization.
 4. Neither the name of the organization nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY yourname ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yourname BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#ifndef __YIWEN_INTERPRETER__
#define __YIWEN_INTERPRETER__
#include <stdio.h>
#include "yiwen_fatal.h"
#include "command.h"
#include "module.h"
#include "yiwen_string.h"
#include "log.h"
#include "yiwen_pcolor.h"

#define INT_CMD_BUFFER 255 
#define PATH_MAX 255

extern int inter_state;
extern int inter_ifexit;

int         inter_loop(int (*handle)());

int         readlinen(char *str, int len);

int         inter_init();

int         inter_exit(int argc, char **argv);
int         inter_help(int argc, char **argv);
int         inter_run(int argc, char **argv);

void        show_pwd();
void        show_time();
void        show_prompt(char *pmpt);
void        parser(char *cmd, int len);

#endif

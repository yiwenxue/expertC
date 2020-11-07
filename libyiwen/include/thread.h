/*-
 Thread Toolkit based on pthread
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
#include "list.h"
#include <pthread.h>
#include <unistd.h>

#ifndef MY_THREAD
#define MY_THREAD

#define TRN_LEN 255

static int max_theads = 255;

typedef pthread_t tid_t;

struct mtx {
    int mutex;
};

/* thread struct based on pthread */
struct thread {
    tid_t               t_tid;      /* thread identifier. */
    int                 debug;
    struct mtx          t_lock;     /* thread lock. */
    LIST_ENTRY(thread)  t_list;     /* List of all processes. */
    enum t_states {
        TRS_NEW = 0,                /* On creation. */
        TRS_NORMAL,                 /* Thread can be run. */
        TRS_ZOMBIE
    } t_state;
    char                t_comm[TRN_LEN + 1];/* Thread name. */
    LIST_HEAD(, thread) t_children; /* Pointer to list of children. */
    void                *extra;     /* The extra data */
};
typedef struct thread thread_t;

struct thread_data{
    struct thread       *td;        /* thread are stored here. */
    void                *data;      /* Arguments for a thread. */
};

typedef void *thread_callback(struct thread_data *data);/* The function prototype for thread handle. */
/* typedef void *thread_callback(void *data);/1* The function prototype for thread handle. *1/ */

#define THREAD_JOIN(t_headp, t_elm) do{             \
    struct thread *var = (struct thread*) t_elm;    \
    LIST_INSERT_HEAD(t_headp, t_elm, t_list);       \
    pthread_detach(var->t_tid);                     \
    usleep(1e2);                                    \
} while(0)

#define THREAD_EXIT(t_elm) do{                      \
    struct thread *elm = t_elm;                     \
    while (LIST_FIRST(&elm->t_children) != NULL) {  \
    usleep(1e2);}                                   \
    LIST_REMOVE(t_elm, t_list);                     \
} while(0)

/* Macro to create a new thread
 * 1. decare a struct to store information
 * 2. initialize the thread information
 * 3. create a thread with these info
 *
 * TODO: init the child list when create a new thread
 * */
#define THREAD_CREATE(t_elm, name, handle, args) do{\
    struct thread *elm = (struct thread*)t_elm;     \
    elm->t_lock.mutex = 0;                          \
    strncpy(elm->t_comm, name, TRN_LEN);            \
    elm->t_state = TRS_NEW;                         \
    LIST_INIT(&elm->t_children);                    \
    struct thread_data data={                       \
        .td = t_elm,                                \
        .data = args};                              \
    elm->extra = args;                              \
    pthread_create(&(elm->t_tid), NULL, handle, &data);\
} while(0)
#endif 

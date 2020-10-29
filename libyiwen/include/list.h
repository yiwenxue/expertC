#pragma once
#ifndef YIWEN_LIST
#define YIWEN_LIST

#define panic(str, arg)    do{                          \
    fprintf(stderr, "panic:" str, arg);                 \
} while (0)

/**
 * A macro to determine the offset from the container to
 * the element. 
 *
 * @type : the type of container
 * @elm  : the element in the container
 *
 * value : the offset in size_t type 
 *
 * */
#define offset_of(type, elm)                            \
    ((size_t) &(((type *)0)->elm))

/**
 * A macro to determine the pointer to container of an element
 * in data structure. 
 *
 * @ptr : the pointer to the data pointer in a data structure
 * @type: the type of container 
 * @elm : the element in the container
 *
 * value: a pointer to the container 
 *
 * */
#define container_of(ptr, type, elm) ({                 \
    const typeof(((type *)0)->elm) * __mptr = (ptr);    \
    (type *)((char *)__mptr - offset_of(type, elm)); })


/**
 * A method to create head for every list. It wil give you a 
 * struct contains a pointer to first element in this list.
 *
 * @name: the name of the list
 * @type: the type of elements the list will contain
 *
 * */
#define LIST_HEAD(name, type)                           \
struct name {                                           \
    struct type *lh_first; /* first element */          \
} 

#define LIST_HEAD_INITIALIZER(head)                     \
{ NULL }

#define LIST_FIRST(head)                                \
    ((head)->lh_first)

#define	LIST_INIT(head) do {						    \
    LIST_FIRST((head)) = NULL;					        \
} while (0)

/***/
#define LIST_ENTRY(type)                                \
struct {                                                \
    struct type *le_next;                               \
    struct type **le_prev;                              \
}

#define LIST_NEXT(elm, field)                           \
    ((elm)->field.le_next)

#define LIST_PREV(elm, head, type, field)               \
    ((elm)->field.le_prev == &LIST_FIRST(head)) ? NULL: \
    container_of((elm)->field.le_prev,                \
           struct type, field.le_next)


/* Never delete emement when using this macro, instead, you
 * should use LIST_FOREACH_SAFE, if you want to delete emements
 * */
#define LIST_FOREACH(var, head, field)                  \
    for((var) = LIST_FIRST(head);                       \
        (var);                                          \
        (var) = LIST_NEXT((var), field))

#define LIST_FOREACH_SAFE(var, head, field, tvar)       \
    for((var) = LIST_FIRST(head);                       \
        (var) && ((tvar) = LIST_NEXT((var), field),1);  \
        (var) = (tvar))

#define LIST_FOREACH_FROM(var, head, field)             \
    for((var) = ((var)? (var) : LIST_FIRST(head));      \
        (var);                                          \
        (var) = LIST_NEXT((var), field))

/**
 * LIST_INSERT_AFTER: A Macro the insert element into
 *  the list.
 * */
#define LIST_INSERT_AFTER(listelm, elm, field)  do{     \
    if((LIST_NEXT(elm, field) = LIST_NEXT(listelm, field)\
            )!= NULL){                                  \
        LIST_NEXT(listelm, field)->field.le_prev        \
        = &LIST_NEXT(elm, field);                       \
    }                                                   \
    LIST_NEXT(listelm, field) = (elm);                  \
    (elm)->field.le_prev = &LIST_NEXT(listelm, field);  \
} while (0)

#define LIST_INSERT_BEFORE 

#define LIST_CHECK_HEAD(head, field)     do{            \
    if (LIST_FIRST(head) != NULL &&                     \
            LIST_FIRST(head)->field.le_prev !=          \
              &LIST_FIRST(head))                        \
        panic("Bad list head %p first->prev != head", (head));\
} while (0)

/**
 * A macro to insert element as first elm. Check if the 
 * has been inited, if not, panic.
 *
 * @head : head of the list
 * @elm  : the new element to insert
 * @field: the region of the data struct
 *
 * code block
 * */
#define LIST_INSERT_HEAD(head, elm, field) do {         \
    LIST_CHECK_HEAD(head, field);                       \
    if ((LIST_NEXT(elm, field) = LIST_FIRST(head))      \
            != NULL)                                    \
        LIST_FIRST(head)->field.le_prev                 \
        = &LIST_NEXT(elm, field);                       \
    LIST_FIRST(head) = (elm);                           \
    (elm)->field.le_prev = &LIST_FIRST((head));         \
} while (0)

#define LIST_CHECK_PREV
#define LIST_CHECK_NEXT 

/**
 * LIST_REMOVE: A Macro to revome an element from the list
 *  The case of head of list should be considered
 * */
#define LIST_REMOVE(elm, field) do{                     \
    if(LIST_NEXT(elm, field) != NULL)                   \
        LIST_NEXT(elm, field)->field.le_prev            \
            =  (elm)->field.le_prev;                    \
    *(elm)->field.le_prev = LIST_NEXT(elm, field);      \
} while (0)

#define LIST_CONNECT

#endif

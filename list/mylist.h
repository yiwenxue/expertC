#ifndef YIWEN_LIST
#define YIWEN_LIST

/* #define container_of(elm, type, ) */

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

#define LIST_ENTRY(type)                                \
struct {                                                \
    struct type *le_next;                               \
    struct type *le_prev;                               \
}

#define LIST_NEXT(elm, field)                           \
    ((elm)->field.le_next)   

#define LIST_PREV(elm, field)                           \
    ((elm)->field.le_prev)

#define LIST_FOREACH(var, head, field)                  \
    for((var) = LIST_FIRST(head);                       \
        (var);                                          \
        (var) = LIST_NEXT((var), field))

#define LIST_FOREACH_FROM(var, head, field)             \
    for((var) = ((var)? (var) : LIST_FIRST(head));      \
        (var);                                          \
        (var) = LIST_NEXT((var), field))

/**
 * LIST_INSERT_AFTER: A Macro the insert element into
 *  the list.
 * */
#define LIST_INSERT_AFTER(listelm, elm, field)          \
    if(LIST_NEXT(listelm, field) != NULL){              \
        LIST_NEXT(listelm, field)->field.le_prev        \
        = (elm);                                        \
        (elm)->field.le_next = LIST_NEXT(listelm, field);\
    }                                                   \
    (listelm)->field.le_next = (elm);                   \
    (elm)->field.le_prev = (listelm);

#define LIST_IFHEAD(elm, field)                         \
    ((elm)->field.le_prev == elm)

/**
 * LIST_REMOVE: A Macro to revome an element from the list
 *  The case of head of list should be considered
 * */
#define LIST_REMOVE(elm, field) do{                     \
    if (LIST_NEXT(elm, field) != NULL)                  \
        LIST_NEXT(elm, field)->field.le_prev            \
            = LIST_PREV(elm, field);                    \
    LIST_PREV(elm, field)->field.le_next                \
        = LIST_NEXT(elm, field);                        \
} while (0)

#endif











/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#ifndef AC_LL
#define AC_LL
typedef struct ll{
    void* self;
    struct ll* next;
    struct ll* prev;
}ll;
/*Pushes a new member to a list*/
void ll_push(ll** list,void* member);
/*Pops the last element of a list*/
void *ll_pop(ll** list);
/*Frees a list and its members*/
void ll_free(ll** list);
/*Removes the element that contains the specified member (does not free *member*)*/
void ll_remove(ll** list,void* member);
#endif
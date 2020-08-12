/*
    Acarajé - A minimalist swordfighting game
    Copyright (C) 2020 Amélia O. F. da S.
    Reachable at a.mellifluous.one@gmail.com
    or www.github.com/m3101

    Full copyright notice in /src/main.c
*/

#include <stdlib.h>
#include "ll.h"
/*Pushes a new member to a list*/
void ll_push(ll** list,void* member)
{
    if(!list)return;
    if(!*list)
    {
        *list=malloc(sizeof(ll));
        (*list)->self=member;
        (*list)->prev=NULL;
        return;
    }
    ll *l=*list;
    for(;l->next!=NULL;l=l->next);
    l->next=malloc(sizeof(ll));
    l->next->next=NULL;
    l->next->self=member;
    l->next->prev=l;
}
/*Pops the last element of a list*/
void *ll_pop(ll** list)
{
    void* ret;
    if(!list||!*list)return NULL;
    ll* l=*list;
    for(;l->next!=NULL;l=l->next);
    if(l->prev)l->prev->next=NULL;
    ret=l->self;
    free(l);
    return ret;
}
/*Frees a list and its members*/
void ll_free(ll** list)
{
    if(!list||!*list)return;
    if((*list)->next!=NULL)ll_free(&(*list)->next);
    if((*list)->self!=NULL)free((*list)->self);
    free(*list);
    *list=NULL;
}
/*Frees a list of lists*/
void ll_free_tree(ll** list)
{
    if(!list||!*list)return;
    if((*list)->next!=NULL)ll_free(&(*list)->next);
    if((*list)->self!=NULL)ll_free_tree((*list)->self);
    free(*list);
    *list=NULL;
}
/*Removes the element that contains the specified member (does not free *member*)*/
void ll_remove(ll** list,void* member)
{
    if(!list||!(*list))return;
    ll*l=*list;
    for(;l!=NULL;l=l->next)if(l->self==member)
    {
        if(l->prev==NULL&&l->next==NULL)
        {
            free(*list);
            *list=NULL;
            return;
        }
        else
        {
            if(l->prev)
                l->prev->next=l->next;
            else *list=l->next;
            if(l->next)
                l->next->prev=l->prev;
            free(l);
        }
        return;
    }
}
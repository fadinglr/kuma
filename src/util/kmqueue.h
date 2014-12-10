/* Copyright (c) 2014, Fengping Bao <jamol@live.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __KMQUEUE_H__
#define __KMQUEUE_H__

#include "kmdefs.h"

KUMA_NS_BEGIN

template <class E>
class KM_Queue
{
public:
    KM_Queue()
    {
        head = new TLNode(E());
        tail = head;
        en_count = 0;
        de_count = 0;
    }
    ~KM_Queue()
    {
        TLNode* node = NULL;
        while(head) {
            node = head;
            head = head->next;
            delete node;
        }
    }

    void enqueue(const E &element)
    {
        TLNode* node = new TLNode(element);
        tail->next = node;
        tail = node;
        ++en_count;
    }

    bool dequeue(E &element)
    {
        if(empty()) {
            return false;
        }
        TLNode* node_to_delete = head;
        element = head->next->element;
        head = head->next;
        ++de_count;
        delete node_to_delete;
        return true;
    }
    
    bool empty()
    {
        return NULL == head->next;
    }
    
    unsigned int size()
    {
        return en_count - de_count;
    }

private:
    class TLNode
    {
    public:
        TLNode(const E &e) : element(e)
        {
            next = NULL;
        }

        E element;
        TLNode* next;
    };

    TLNode* head;
    TLNode* tail;
    unsigned int en_count;
    unsigned int de_count;
};

template <class E, class LockType>
class KM_QueueT
{
public:
    KM_QueueT()
    {
        head = new TLNode(E());
        tail = head;
        en_count = 0;
        de_count = 0;
    }
    ~KM_QueueT()
    {
        TLNode* node = NULL;
        while(head)
        {
            node = head;
            head = head->next;
            delete node;
        }
    }

    void enqueue(const E &element)
    {
        TLNode* node = new TLNode(element);
        lockerT.lock();
        tail->next = node;
        tail = node;
        ++en_count;
        lockerT.unlock();
    }

    bool dequeue(E &element)
    {
        if(empty()) {
            return false;
        }
        TLNode* node_to_delete = NULL;
        lockerH.lock();
        if(empty()) {
            lockerH.unlock();
            return false;
        }
        node_to_delete = head;
        element = head->next->element;
        head = head->next;
        ++de_count;
        lockerH.unlock();
        delete node_to_delete;
        return true;
    }
    
    bool empty()
    {
        return NULL == head->next;
    }
    
    unsigned int size()
    {
        return en_count - de_count;
    }

private:
    class TLNode
    {
    public:
        TLNode(const E &e) : element(e)
        {
            next = NULL;
        }

        E element;
        TLNode* next;
    };

    TLNode* head;
    TLNode* tail;
    LockType lockerH;
    LockType lockerT;
    unsigned int en_count;
    unsigned int de_count;
};
    
KUMA_NS_END

#endif

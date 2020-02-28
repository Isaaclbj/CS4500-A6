
#include "object.h"

class linked_array : public Object
{
    // linked_array doesn't own items
    public:
        void **items;
        size_t max;
        size_t size;
        linked_array *next;


    //copy
    linked_array(linked_array& from)
    {
        items = new void*[from.max];
        max = from.max;
        size = from.size;
        for (int ii = 0; ii < size; ii++)
        {
            items[ii] = from.items[ii];
        }
        if(from.next)
        {
            this->next = new linked_array(*from.next);
        }
    }

    linked_array()
    {
        items = new void*[2];
        max = 2;
        size = 0;
    }

    linked_array(size_t ss)
    {
        items = new void*[ss];
        max = ss;
        size = 0;
    }
    
    ~linked_array()
    {
        if(next)
        {
            delete(next);
        }
        delete[](items);
    }

    void push_back(void *item)
    {
        if(size == max)
        {
            if(next)
            {
                next->push_back(item);
                return;
            }
            else
            {
                next = new linked_array(2 * max);
                next->push_back(item);
                return;
            }
        }
        else
        {
            items[size] = item;
            size++;
            return;
        }
    }

    void* get(size_t index)
    {
        if(index > max)
        {
            if(next)
            {
                index -= max;
                return next->get(index);
            }
            else
            {
                return 0;
            }
            
        }
        if(index >= size)
        {
            return 0;
        }
        else
        {
            return items[index];
        }
    }

    void* set(size_t index, void *item)
    {
        if(index >= max)
        {
            index -= max;
            if(next)
            {
                return next->set(index, item);
            }
            else
            {
                if(index == 0)
                {
                    push_back(item);
                    return 0;
                }
            }
            
        }
        else
        {
            if(index < size)
            {
                void *ret = items[index];
                items[index] = item;
                return ret;
            }
            if(index == size)
            {
                items[index] = item;
                return 0;
            }
        }
        return 0;
    }

    // delete all pointers stored in this linked_array
    void dump()
    {
        for(int ii = 0; ii < size; ii++)
        {
            // ??
        }
    }
};

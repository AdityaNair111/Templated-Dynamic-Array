#pragma once

#include <iostream>
using namespace std;

//students should not change this

using size_t = std::size_t;

template<typename T> class array_iterator;

template<typename T>
class array {
public:
    //default constructor
    array()
    {
    	m_reserved_size=0;
    	m_size=0;
    	m_elements=(T*)malloc(sizeof(T)*1);

    }

    //initialize array with elements in initializer
    array(std::initializer_list<T> l) //tested
    {
    	m_reserved_size=l.size();
    	m_size=l.size();
    	m_elements=(T *)malloc(sizeof(T)*m_reserved_size);
    	for (size_t i = 0; i < l.size(); i++)
    	{
    		new(&m_elements[i]) T(*(l.begin()+i));
    	}

    }

    //copy constructor
    array(const array& a)
    {
    	m_reserved_size=a.m_reserved_size;
    	m_size=a.m_size;
    	m_elements=(T*)malloc(sizeof(T)*m_reserved_size);
    	for (size_t i = 0; i < m_size; i++)
    	{
    		new(&m_elements[i]) T(a.m_elements[i]);
    	}	
    }

    //move constructor
    array(array&& Element_to_move)
    {
    	m_size=Element_to_move.m_size;
    	m_reserved_size==Element_to_move.m_reserved_size;
    	m_elements=Element_to_move.m_elements;
    	Element_to_move.m_elements=nullptr;
    	Element_to_move.m_elements=0;
    	Element_to_move.m_reserved_size=0;
    }

    //construct with initial "reserved" size
    array(size_t n)
    {
    	m_reserved_size=n;
    	m_size=0;
    	m_elements=(T*)malloc(sizeof(T)*n);
    }

    //construct with n copies of t
    array(size_t n, const T& t)
    {
    	m_reserved_size=n;
    	m_size=n;
    	m_elements=(T*)malloc(sizeof(T)*m_reserved_size);
    	for (size_t i = 0; i < m_size; i++)
    	{
    		new(&m_elements[i]) T(t);
    	}
    }

    //destructor
    ~array()
    {
    	for (size_t i = 0; i < m_size; i++)
    	{
    		m_elements[i].~T();
    	}
    	m_reserved_size=0;
    	m_size=0;
    	free(m_elements); // is it neccesary ? just use the null comment	
    	m_elements=nullptr;
    }

    //ensure enough memory for n elements
    void reserve(size_t n) //tested
    {
    	if (n>m_reserved_size) // has to be greater for this to work
    	{
    		T* temp=(T*)malloc(sizeof(T)*n);
    		for (size_t i = 0; i <m_size ; i++)
    		{
    			new(&temp[i]) T(std::move(m_elements[i]));
    			m_elements[i].~T();
    		}
    		free(m_elements);
    		m_reserved_size=n;
    		m_elements=temp;
    	}
    }

    //add to end of vector
    void push_back(const T& last_val) //tested
    {
    	if (m_size+1>m_reserved_size)
    	{
    		reserve(m_reserved_size+1);
    	}
    	new(&m_elements[m_size]) T(last_val);
    	m_size++;
    }

    //add to front of vector
    void push_front(const T& first_val) //tested
    {
    	if (m_size+1>m_reserved_size)
    	{
    		m_reserved_size++;
    		T* temp=(T*)malloc(sizeof(T)*(m_reserved_size));
    		for (size_t i = m_size; i > 0; i--)
    	{
    		new(&temp[i]) T(std::move(m_elements[i-1]));
    		m_elements[i-1].~T();
    	}
    	free(m_elements);
    	new(&temp[0]) T(first_val);
    	m_elements=temp;
    	m_size++;
    	
    	}
    	else
    	{
    	for (size_t i = m_size; i > 0; i--)
    	{
    		new(&m_elements[i]) T(std::move(m_elements[i-1]));
    		m_elements[i-1].~T();
    	}
    	new(&m_elements[0]) T(first_val);
    	m_size++;
   	 	}
    }

    //remove last element
    void pop_back() // tested
    {
    	if (m_size!=0)
    	{
    		m_elements[m_size-1].~T();
    		m_size--;
    	}

    }

    //remove first element
    void pop_front() //tested
    {
    	if (m_size!=0)
    	{

    		m_elements[0].~T();
    		for (size_t i = 0; i < m_size-1; i++)
    		{
    			m_elements[i]=std::move(m_elements[i+1]);
    		}
    		m_size--;
    	}
    }

    //return reference to first element
    T& front() const
    {
    	return *(m_elements);
    }

    //return reference to last element
    T& back() const
    {
    	return *(m_elements+m_size-1);
    }

    //return reference to specified element
    const T& operator[](size_t i) const
    {
    	return *(m_elements+i);
    }

    //return reference to specified element
    T& operator[](size_t i)
    {
    	return *(m_elements+i);
    }

    //return number of elements
    size_t length() const
    {
    	return m_size;
    }

    //returns true if empty
    bool empty() const
    {
    	if (m_size==0)
    	{
    		return true;
    	}
    	else
    	{
    		false;
    	}
    }

    //remove all elements
    void clear()    // Tested
    {
    	for (size_t i = 0; i < m_size; i++)
    	{
    		m_elements[i].~T();
    	}
    	m_size=0;
    }

    //obtain iterator to first element
    array_iterator<T> begin() const
    {
    	return array_iterator<T>(m_elements);
    }

    //obtain iterator to one beyond element
    array_iterator<T> end() const
    {
    	return array_iterator<T>(m_elements+m_size);
    }

    //remove specified element
    void erase(const array_iterator<T>& element_erase_index)
    {
    	size_t postion_to_Del =m_size;
    	for (size_t i = 0; i < m_size; i++)
    	{
    		if (element_erase_index.m_current==&m_elements[i])
    		{
    			postion_to_Del=i;
    			break;
    		}
    	}
    	m_elements[postion_to_Del].~T();
    	for (size_t i = postion_to_Del+1; i < m_size; i++)
    	{
    		m_elements[i-1]=std::move(m_elements[i]);
    	}
    	m_size--;

    }

    //insert element right before itr
    void insert(const T& value, const array_iterator<T>& itr)
    {
    	if (m_size+1>m_reserved_size)
    	{
    		size_t postion_to_ins=m_size;
    		for (size_t i = 0; i < m_size; i++)
    		{
    			if (itr.m_current==&m_elements[i])
    			{
    				postion_to_ins=i;
    				break;
    			}
    		}
    		m_reserved_size++;
    		T* temp=(T*)malloc(sizeof(T)*(m_reserved_size));
    		for (size_t i = 0; i < postion_to_ins; i++)
    		{
    			new(&temp[i]) T(std::move(m_elements[i]));
    			m_elements[i].~T();
    		}
    		new(&temp[postion_to_ins]) T(value);
    		
    		for (size_t i = postion_to_ins; i <m_size; i++)
    		{
    			new(&temp[i+1]) T(std::move(m_elements[i]));
    			m_elements[i].~T();
    		}
    		free(m_elements);
    		m_elements=temp;
    		m_size++;
    		
    	}
    	else
    	{
    		size_t postion_to_ins=m_size;
    		for (size_t i = 0; i < m_size; i++)
    		{
    			if (itr.m_current==&m_elements[i])
    			{
    				postion_to_ins=i;
    				break;
    			}
    		}
    		for (size_t i = m_size; i > postion_to_ins; i--)
    		{
    			new(&m_elements[i]) T(std::move(m_elements[i-1]));
    			m_elements[i-1].~T();
    		}
    		new(&m_elements[postion_to_ins]) T(value);
    		m_size++;
    	}
    }

private:
    T* m_elements;              //points to actual elements
    size_t m_size;              //number of elements
    size_t m_reserved_size;     //number of reserved elements
};

template<typename T>
class array_iterator {
public:
    array_iterator()
    {
    	m_current=NULL;
    }
    array_iterator(T* p)
    {
    	m_current=p;
    }
    array_iterator(const array_iterator& i)
    {
    	m_current=i.m_current;
    }
    T& operator*() const
    {
    	return(*m_current);
    }
    array_iterator operator++()
    {
    	return array_iterator<T>(m_current++);
    }
    array_iterator operator++(int __unused)
    {
    	return array_iterator<T>(m_current++);
    }
    bool operator != (const array_iterator& val_check) const
    {
    	if (m_current!=val_check.m_current)
    	{
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }
    bool operator == (const array_iterator& val_check) const
    {
    	if (m_current==val_check.m_current)
    	{
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }

private:
    T* m_current;
    // I want my array class to be able to access m_current even though it's private
    // 'friend' allows this to happen
    friend class array<T>;
};
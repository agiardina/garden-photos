//
//  result.h
//  GardenPhotos
//
//  Created by Andrea Giardina on 05/06/21.
//

#ifndef result_h
#define result_h

#include <iostream>
#include <stdexcept>

template <class T>
class result
{
private:
    T m_ok;
    bool m_is_ok;
    bool m_initialized = false;
    std::string m_error;
public:
    bool is_ok()
    {
        return m_is_ok;
    }
    
    bool is_error()
    {
        return !m_is_ok;
    }

    T ok()
    {
        if (!m_initialized) {
            throw std::runtime_error("Error: calling ok() on not-initialized result");
        }
        
        if (m_is_ok) {
            return m_ok;
        } else {
            throw std::runtime_error("Error: calling ok() on failed result");
        }
    }
    
    std::string error()
    {
        if (!m_initialized) {
            throw std::runtime_error("Error: calling error() on not-initialized result");
        }
        
        if (m_is_ok) {
            throw std::runtime_error("Error: calling erorr() on successfull result");
        } else {
            return m_error;
        }

    }
    
    result<T> ok(T ok_value)
    {
        if (!m_initialized) {
            m_initialized = true;
            m_is_ok = true;
            m_ok = ok_value;
        } else {
            throw std::runtime_error("Error: result already initialized");
        }
        return *this;
    }
    
    result<T> error(std::string err_value)
    {
        if (!m_initialized) {
            m_initialized = true;
            m_is_ok = false;
            m_error = err_value;
        } else {
            throw std::runtime_error("Error: result already initialized");
        }
        return *this;
    }
};

#endif /* result_h */

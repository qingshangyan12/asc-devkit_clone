#ifndef ASC_NEWDELETE_SAMPLE_CONTAINER_HPP
#define ASC_NEWDELETE_SAMPLE_CONTAINER_HPP

#include "simt_api/asc_simt.h"

template <class T>
class Vector {
public:
    __aicore__ inline Vector(int32_t max_size)
        : m_size(max_size), m_data(nullptr), m_top(-1)
    {
        m_data = new T[max_size];
    }

    __aicore__ inline Vector(int32_t max_size, T* preallocated_buffer)
        : m_size(max_size), m_data(nullptr), m_top(-1)
    {
        m_data = new (preallocated_buffer) T[max_size];
    }

    __aicore__ inline ~Vector()
    {
        if (m_data != nullptr) {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    __aicore__ inline void Push(T e)
    {
        if (m_data == nullptr) {
            return;
        }
        int32_t idx = asc_atomic_add(&m_top, 1);
        int32_t pos = idx + 1;
        if (pos >= 0 && pos < m_size) {
            m_data[pos] = e;
        }
    }

    __aicore__ inline bool Pop(T& e)
    {
        if (m_data == nullptr || m_top < 0) {
            return false;
        }

        int32_t idx = asc_atomic_add(&m_top, -1);
        if (idx >= 0 && idx < m_size) {
            e = m_data[idx];
            return true;
        }
        return false;
    }

private:
    int32_t m_size;
    T* m_data;
    int32_t m_top;
};

#endif // ASC_NEWDELETE_SAMPLE_CONTAINER_HPP

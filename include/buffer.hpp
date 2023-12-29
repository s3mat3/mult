/**
 * @file buffer.hpp
 *
 * @copylight © 2023 Matsuo Shin
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief buffer base class
 *
 * @author matsuo.shin@gmail.com
 */

#pragma once

#ifndef BUFFER_Hpp
# define  BUFFER_Hpp
# define MULT_TRACE 1
# include "debug.hpp"
# include "result.hpp"
# include "storage.hpp"

# define BUFFER_TAIL_CHECK() \
    do{ \
        if ((super::m_end - super::m_tail) <= 0) \
            return Mult::OUT_OF_RANGE; \
    }while(0)
# define BUFFER_TAIL_CHAECK_RESULT() \
    do{ \
        if ((super::m_end - super::m_tail) <= 0)               \
            return result<T>(error_type(Mult::OUT_OF_RANGE));  \
    }while(0)

# define BUFFER_TAIL_CHECK_INDEX(ind) \
    do{ \
    if (this->m_end - (this->m_tail + ind) <= 0)               \
            return Mult::OUT_OF_RANGE; \
    }while(0)
# define BUFFER_CHECK_INDEX_WITH_THROW(index) \
    do{ \
        if (this->m_capacity <= index) throw std::out_of_range("index is out of range"); \
    } while(0)

namespace Mult {
    /** Buffer base class .
     *
     * Buffer for value_type
     *  \tparam T value_type
     */
    template <typename T>//, size_type N>
    class BufferBase : public StorageBase<T, std::allocator<T>>
    {
    public:
        using value_type             = T;
        using pointer                = value_type*;
        using const_pointer          = const value_type*;
        using reference              = T&;
        using const_reference        = const T&;
        using rvalue_reference       = T&&;
        using const_buffer_reference = const BufferBase<value_type>&;
        using result                 = Result<T>;
        using StorageBase<value_type>::StorageBase;
        using super = StorageBase<value_type>;

        // BufferBase() : StorageBase<value_type>(N) {}
        // auto begin() {return this->m_head;}
        // auto end() {return this->m_tail;}
        /** Clear buffer .
         *
         * Only pointer operate
         * \note Never delete contents.
         */
        auto clear()
        {
            super::m_tail = super::m_head;
            m_read = ZERO;
        }
        /** Clear buffer with destruct .
         *
         *
         */
        auto clear_all()
        {
            super::destoroy_all();
        }
        /**  Content(s) append to tail .
         *
         *
         */
        auto append(const_pointer pv, size_type n) noexcept
        {
            if (super::overflow()) super::resize();
            super::copy(pv, pv + n);
            return OK;
        }
        /**  Content(s) append to tail .
         *
         *
         */
        auto append(const BufferBase& cr) noexcept
        {
            auto n = cr.size();
            if (super::overflow(n)) super::resize();
            super::copy(cr.const_begin(), cr.const_end());
            return OK;
        }
        /**  .
         *
         *
         */
        auto assign(const_pointer pv, size_type n)
        {
            if (super::capacity() < n) super::resize();
            clear();
            super::copy(pv, pv + n);
            return OK;
        }
        /**  .
         *
         *
         */
        auto assign(const BufferBase& cr)
        {
            auto n = cr.size();
            if (super::capacity() < n) super::resize();
            clear();
            super::copy(cr.const_begin(), cr.const_end());
            return OK;
        }
        /** Push back 1 object (const value_type&).
         *
         * Append 1 object at storage tail
         *
         * \note When buffer is full, then resizing occur
         *  \param[in] v Target of append
         *  \retval OUT_OF_RANGE index was arlady full
         *  \retval OK appended
         */
        auto push_back(const_reference v) noexcept
        {
            if (super::full()) super::resize();
            *super::m_tail = v;
            ++super::m_tail;
            return Mult::OK;
        }
        /** Push back 1 object (value_type&&).
         *
         * \note When buffer is full, then resizing occur
         *
         */
        auto push_back(rvalue_reference v) noexcept
        {
            if (super::full()) super::resize();
            *super::m_tail = std::move(v);
            ++super::m_tail;
            return Mult::OK;
        }
        /** Push back 1 object (const value_type*).
         *
         * \note When buffer is full, then resizing occur
         *
         */
        auto push_back(const_pointer v) noexcept
        {
            if (super::full()) super::resize();
            *super::m_tail = *v;
            ++super::m_tail;
            return Mult::OK;
        }
        /** Add one object at tail .
         *
         * \note When buffer is full, then resizing occur
         */
        BufferBase& operator+(const_reference v) noexcept
        {
            // BUFFER_CHECK_INDEX_WITH_THROW(super::size());
            if (super::full()) super::resize();
            *super::m_tail = v;
            ++super::m_tail;
            return *this;
        }

        /** []access .
         *
         * auto y = x[i]
         */
        reference operator[](size_type i) noexcept
        {
            if (super::overflow(i)) super::resize();
            return super::m_head[i];
        }
        /** access[] .
         *
         */
        const_reference operator[](index_type i) const noexcept
        {
            if (super::overflow(i)) super::resize();
            return super::m_head[i];
        }

        auto at(size_type pos) const -> const_reference
        {
            if (super::overflow(pos)) throw std::out_of_range("Index position is over capacity");
            return super::m_head[pos];
        }
        /** Read buffer data from storage .
         *
         *  \retval value_type
         */
        auto read() -> value_type
        {
            auto pos = m_read;
            if (++m_read > super::size()) throw std::out_of_range("point to no valid data");
            return super::m_head[pos];
        }
        /** Put back readed object .
         *
         * Only put back position for read data
         *  \retval OK one back
         *  \retval UNDER_FLOW cause read index is alrady first
         */
        auto put_back() noexcept -> return_code
        {
            if (m_read > 0) {
                --m_read;
                return OK;
            } else {
                m_read = 0;
                return UNDER_FLOW;
            }
        }
        /** Get read starting position .
         *
         *  \retval read position
         */
        auto position() const noexcept
        {
            return m_read;
        }
        /** Set read starting position .
         *
         *  \param[in] newPos new read position
         *  \retval OK moving
         *  \retval OUT_OF_RANGE newPos is over range
         */
        auto position(size_type newPos) noexcept ->return_code
        {
            if (newPos > super::size()) return OUT_OF_RANGE;
            m_read = newPos;
            return OK;
        }
        /** Extract buffer data from storage .
         *
         *  \param[in] first extract first position
         *  \param[in] length extract length
         *  \retval error_type out of range (length is over)
         *  \retval extracted buffer
         */
        auto extract(size_type first, size_type length) const noexcept -> Result<BufferBase>
        {
            if (super::size() < (first + length)) return Result<BufferBase>(error_type(OUT_OF_RANGE));
            BufferBase dest(super::m_capacity);
            dest.assign(super::m_head + first, length);
            return dest;
        }
    private:
        size_type m_read = ZERO; //!< index for read storage
    }; //<-- class BufferBase ends here.
//    using ByteBuffer = BufferBase<size_type N, char>;
} //<-- namespace Mult ends here.
#endif //<-- macro  BUFFER_Hpp ends here.

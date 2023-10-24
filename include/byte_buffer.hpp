/**
 * @file byte_buffer.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Buffer for byte(char type)
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_BUFFER_Hpp
# define  MULT_BUFFER_Hpp

#include <cstring>
#include <memory>

#include "debug.hpp"
#include "mult.hpp"

#include <iostream>
# define BUFFER_INDEX_SCOPE(s) ((0 <= s) || (s <= m_capacity))
# define BUFFER_CHECK_CAPACITY_RETURN(s) \
    do {                                 \
        if (! BUFFER_INDEX_SCOPE(s)) {   \
            return OUT_OF_RANGE;         \
        }                                \
    } while(0)

# define BUFFER_CHECK_CAPACITY(s)                  \
    do {                                           \
        if (! BUFFER_INDEX_SCOPE(s)) { \
          throw std::out_of_range("index is out of range"); \
      }                                            \
    } while(0)

# define BUFFER_CHECK_READABLE_RETURN(l)          \
  do {                                            \
      if (! (m_wp > 0 && m_wp <= m_capacity)) {   \
          return NO_DATA;                         \
      }                                           \
  } while(0)

# define BUFFER_CHECK_READABLE(l)                 \
  do {                                            \
      if (! (m_wp > 0 && m_wp <= m_capacity)) {   \
          throw std::runtime_error("None valid Data"); \
      }                                           \
  } while(0)

namespace Mult {
    /** byte_type 用の簡易バッファ.
     * \ingroup Buffer
     * @{
     * 高機能が欲しければstd::string(CharT版)またはstd::vector(何でもコンテナ)を選択する事
     *
     */
    class ByteBuffer
    {
    public:
        using buffer_type = ByteBuffer;
        // using storage_p   = std::unique_ptr<byte_type[]>;
        using storage_p   = byte_type*;

        static constexpr size_type SIZE_128 =   128;
        static constexpr size_type SIZE_256 =   256;
        static constexpr size_type SIZE_512 =   512;
        static constexpr size_type SIZE_1K  =  1024;
        static constexpr size_type SIZE_2K  =  2048;
        static constexpr size_type SIZE_4K  =  4096;
        static constexpr size_type SIZE_8K  =  8192;
        static constexpr size_type SIZE_16K = 16384;
        static constexpr size_type ZERO     = 0; //!< for文中のauto型のタイプヒント用
        static constexpr size_type default_size() {return SIZE_128;}
        static constexpr size_type limit_size()   {return SIZE_16K * SIZE_16K;} // 256MByte
        // ctors
        /** Constractor 1 .
         *
         * デフォルトのサイズ(128byte)で構築する
         */
        ByteBuffer() noexcept(true)
            : m_capacity{buffer_type::default_size()}
            , m_wp{0}
            , m_storage{new byte_type[default_size()]}
        {
            TRACE("Default construct");
        }
        /** Constructor 2 .
         *
         * バッファーサイズを指定した構築子サイズの調整を行うので非効率である
         *  \param[in] size storage(capacity)のサイズ
         * \note 引数が0以下またはlimit_sizeを越えた場合default_size(0以下の場合)、limit_size(limit_sizeを越えた場合)に規制してしまう
         */
        explicit ByteBuffer(size_type size) noexcept(true)
            : m_capacity{0}
            , m_wp{0}
            , m_storage{nullptr}
        {
            auto capacity = size;
            if (size > limit_size()) {
                capacity = limit_size();
            }
            if (size <= 0) {
                capacity = default_size();
            }
            m_capacity = capacity;
            m_storage = new byte_type[capacity];
            TRACE("Specify capacity construct");
        }
        /** Constructor 3 .
         *
         * std::stringから構築
         *  \param[in] from 構築元std::string
         * \exception std::out_of_range target_sizeが0以下またはlimit_sizeを越えた
         */
        explicit ByteBuffer(const std::string& from) noexcept(false)
            : m_capacity{buffer_type::default_size()}
            , m_wp{0}
            , m_storage{new byte_type[default_size()]}
        {
            auto size = from.size();
            if (m_capacity < size) {
                if (resize(size) == OUT_OF_RANGE) throw std::out_of_range("from.size() is more than limit_size");
            }
            std::memcpy(m_storage, from.data(), from.size() * sizeof(byte_type));
            // for (auto c : from) {
            //     push_back(c);
            // }
            TRACE("From std::string object construct");
        }
        /** Constructor 4 .
         *
         * byte_typeの配列からByteBufferを構築する
         * \exception std::out_of_range target_sizeが0以下またはlimit_sizeを越えた
         */
        explicit ByteBuffer(const byte_type* t, size_type target_size) noexcept(false)
            : m_capacity{default_size()}
            , m_wp{0}
            , m_storage{new byte_type[default_size()]}
        {
            if (m_capacity < target_size) {
                if (resize(target_size) == OUT_OF_RANGE) throw std::out_of_range("target_size is more than limit_size");
            }
            // for (auto i = ZERO; i < target_size; ++i) {
            //     m_storage[i] = t[i];
            // }
            std::memcpy(m_storage, t, target_size * sizeof(byte_type));
            m_wp = target_size;
            TRACE("Specify object with object size construct");
        }
        /** Constructor 5 .
         *
         * copy constructor
         * \code
         * ByteBuffer x;
         * ByteBuffer y(x);
         * \endcode
         */
        ByteBuffer(const ByteBuffer& lhs) noexcept(true)
            : m_capacity{lhs.m_capacity}
            , m_wp{lhs.m_wp}
            , m_storage{new byte_type[lhs.m_capacity]}
        {
            std::memcpy(m_storage, lhs.m_storage, m_wp * sizeof(byte_type));
            TRACE("Copy construct");
        }
        /** Constructor 6 .
         *
         * move constructor
         * \code
         * auto x = ByteBuffer{};
         * \endcode
         */
        ByteBuffer(ByteBuffer&& rhs) noexcept(true)
            : m_capacity{rhs.m_capacity}
            , m_wp{rhs.m_wp}
            , m_storage{std::move(rhs.m_storage)}
        {
            rhs.m_wp = 0;
            rhs.m_capacity = 0;
            rhs.m_storage = nullptr;
            TRACE("Move construct");
        }
        /** Copy assign .
         *
         *
         */
        ByteBuffer& operator=(const ByteBuffer& lhs) noexcept(true)
        {
            if (this != &lhs) {
                if (lhs.m_capacity != m_capacity) { // resize
                    m_capacity = lhs.m_capacity;
                    delete [] m_storage;
                    m_storage = new byte_type[m_capacity];
                }
                m_wp = lhs.m_wp;
                for (auto i = ZERO; i < m_wp; ++i) {
                    m_storage[i] = lhs.m_storage[i];
                }
            }
            TRACE("Copy assign operator");
            return *this;
        }
        /** Move assign .
         *
         *
         */
        ByteBuffer& operator=(ByteBuffer&& rhs) noexcept(true)
        {
            if (this != &rhs) {
                m_capacity = rhs.m_capacity;
                m_wp = rhs.m_wp;
                m_storage = std::move(rhs.m_storage);
                rhs.m_wp = 0;
                rhs.m_capacity = 0;
                rhs.m_storage = nullptr;
            }
            TRACE("Move assign operator");
            return *this;
        }
        // dtor
        /** default destructor .
         *
         *
         */
        ~ByteBuffer()
        {
            MARK();
            if (m_storage) delete [] m_storage;
            TRACE("~ByteBuffer() destory all");
        }
        /** バッファ最後部に1オブジェクトを追加 .
         *
         *  \note 範囲外で例外を送出する
         *  \param[in] i 最後尾に追加するオブジェクト
         * \exception out_of_range iがインデックスの範囲を超た 有効範囲は(0 <= i) && (i <= m_capacity)
         */
        ByteBuffer& operator+(byte_type i) noexcept(false)
        {
            BUFFER_CHECK_CAPACITY(m_wp + 1);
            m_storage[m_wp++] = i;
            return *this;
        }
        /** バッファ最後部に1オブジェクトを追加 .
         *
         *  \param[in] i 最後尾に追加するオブジェクト
         *  \retval Mult::OK 成功
         *  \retval Mult::OUT_OF_RANGE バッファが一杯なので未書き込み
         */
        return_code push_back(byte_type i) noexcept(true)
        {
            BUFFER_CHECK_CAPACITY_RETURN(m_wp + 1);
            m_storage[m_wp++] = i;
            return OK;
        }
        /** バッファ最後部にn個オブジェクトを追加 .
         *
         * n個+m_wpが現capacityより大きければresizeによってstorageは拡張される.
         *  \param[in] i 最後尾に追加するオブジェクトの集合(配列の先頭)
         *  \param[in] n 配列の要素数
         *  \retval OK 成功
         *  \retval OUT_OF_RANGE バッファが一杯なので未書き込み
         */
        return_code append(const byte_type* i, size_type n) noexcept(true)
        {
            auto ns = n + m_wp;
            if (m_capacity < ns) {
                if (resize(ns) == OUT_OF_RANGE) return OUT_OF_RANGE;
            }
            for (auto ind = ZERO; ind < n; ++ind) {
                m_storage[m_wp++] = i[ind];
            }
            return OK;
        }
        /** バッファ最後部にstd::stringオブジェクトを追加 .
         *
         * s.size() + m_wpが現capacityより大きければresizeによってstorageは拡張される.
         *  \param[in] s 最後尾に追加するstd::stringオブジェクト
         *  \retval OK 成功
         *  \retval OUT_OF_RANGE バッファが一杯なので未書き込み
         */
        return_code append(const std::string& s) noexcept(true)
        {
            auto n = s.size();
            auto ns = n + m_wp;
            if (m_capacity < ns) {
                if (resize(ns) == OUT_OF_RANGE) return OUT_OF_RANGE;
            }
            for (auto ind = ZERO; ind < n; ++ind) {
                m_storage[m_wp++] = s[ind];
            }
            return OK;
        }
        /** バッファの再代入 .
         *
         * 既に構築済のバッファへwp = 0からn個分オブジェクトを代入する<br>
         * n個が現capacityより大きければresizeによってstorageは拡張される.
         *  \param[in] i 上書きするオブジェクトの集合(配列の先頭)
         *  \param[in] n 配列の要素数
         *  \retval OK 成功
         *  \retval OUT_OF_RANGE バッファが一杯なので未書き込み
         */
        return_code assign(const byte_type* i, size_type n)
        {
            if (m_capacity < n) {
                if (resize(n) == OUT_OF_RANGE) return OUT_OF_RANGE;
            }
            m_wp = 0;
            for (auto ind = ZERO; ind < n; ++ind) {
                m_storage[m_wp++] = i[ind];
            }
            return OK;
        }
        /** バッファの再代入 .
         *
         * 既に構築済のバッファへwp = 0からstd::stringオブジェクトを代入する<br>
         * std::stringオブジェクトの保持するサイズまでstorageは拡張されるかもしれない.
         *  \param[in] s 上書きするオブジェクトの集合(配列の先頭)
         *  \retval OK 成功
         *  \retval OUT_OF_RANGE バッファが一杯なので未書き込み
         */
        return_code assign(const std::string& s)
        {
            auto n = s.size();
            if (m_capacity < n) {
                if (resize(n) == OUT_OF_RANGE) return OUT_OF_RANGE;
            }
            m_wp = 0;
            for (auto ind = ZERO; ind < n; ++ind) {
                m_storage[m_wp++] = s[ind];
            }
            return OK;
        }
        /** バッファの切り出し .
         *
         *  \param[in] first 切り出し開始位置
         *  \param[in] length 切り出しサイズ
         *  \retval buffer_type 切り出した結果
         *  \exception std::out_of_range first + lengthが
         */
        buffer_type extract(index_type first, size_type length) const noexcept(false)
        {
            BUFFER_CHECK_CAPACITY(first + length);
            buffer_type dest(m_capacity);
            auto src = m_storage;
            dest.assign(&src[first], length);
            return dest;
        }
        //
        // checker
        //
        /** 書き込みサイズのチェック .
         *
         *  \param[in] size 書き込みたいデータ数
         *  \retval false 容量を越える
         *  \retval true 書き込み可
         */
        bool checkCapacity(size_type size) const noexcept {return (m_capacity > size);}
        /** 読み出し位置のチェック .
         *
         *  posは0以上で
         *  \param[in] pos 読み出し開始位置
         *  \retval false 無意味な位置、または未書き込み
         *  \retval true 書き込み可
         */
        bool checkReadable(index_type pos) const noexcept {return ((pos >= 0) && (pos < m_capacity));}
        /** 有効なストレージを所有しているか判定する .
         *
         *  \retval true ストレージを所有している
         *  \retval false ストレージを所有していない(移譲した)
         */
        operator bool() const noexcept {return (m_storage) ? true: false;}
        // raw pointer
        /** 生ポを提供する(非constポインタ) .
         *
         * ポイントしている内容を変更する際使用
         * 書き込みを行う場合範囲チェックが掛らないので要注意
         * 書き込みポインタwpも変らないのでupdateWPと連動させて利用する事
         */
        byte_type* storage() noexcept {return m_storage;}
        /** 生ポを提供する(constポインタ) .
         *
         * ポイントしている内容を参照
         */
        const byte_type* get() const noexcept {return m_storage;}
        /** For ranged for .
         *
         *
         */
        const byte_type* begin() const noexcept {return m_storage;}
        /** For ranged for .
         *
         *
         */
        const byte_type* end() const noexcept {return m_storage + m_wp;}
        //
        // utility
        //
        /**  Point to next byte.
         *
         *
         */
        index_type wp() const noexcept {return m_wp;}
        /** Size of writed .
         *
         *
         */
        index_type size() const noexcept {return m_wp;}
        /**  書き込みポインタの位置変更.
         *
         * そもそも危険例えば
         * \code
         * ByteBuffer buffer(256);
         * auto readed_size = ::read(FD, buffer.storage(), buffer.capacity());
         * buffer.updateWP(readed_size);
         * \endcode
         */
        return_code updateWP(index_type position) noexcept(true)
        {
            BUFFER_CHECK_CAPACITY_RETURN(position);
            m_wp = position;
            return OK;
        }
        /** Storage size .
         *
         *
         */
        size_type capacity() const noexcept(true) {return m_capacity;}
        /** Storage sizeをwpまで小さくする .
         *
         * 新しい容量が現在の容量と同じなら何もしない
         *  \retval Mult::OUT_OF_RANGE 新しいサイズが0以下
         *  \retval Mult::FAILURE 新しいサイズは元サイズと同じ(何もしない)
         *  \retval Mult::OK resized
         */
        return_code shrink() noexcept(true)
        {
            return resize(m_wp);
        }
        /** Resize storage .
         *
         * 新しい容量が現在の容量と同じなら何もしない
         *  \param[in] newSize 新しいサイズ
         *  \retval Mult::OUT_OF_RANGE 新しいサイズが0以下又は、limit_sizeを越えた
         *  \retval Mult::FAILURE 新しいサイズは元サイズと同じ(何もしない)
         *  \retval Mult::OK resized
         */
        return_code resize(size_type newSize) noexcept(true)
        {
            if (newSize <= 0 || newSize > limit_size()) return OUT_OF_RANGE;
            if (m_capacity != newSize) {
                auto t = new byte_type[newSize];
                if (m_wp > newSize) m_wp = newSize;
                if (m_wp > 0) {
                  for (auto i = ZERO; i < m_wp; ++i) {
                    t[i] = m_storage[i];
                  }
                }
                delete [] m_storage;
                m_storage = std::move(t);
                m_capacity = newSize;
            } else {
                return FAILURE;
            }
            return OK;
        }
        /** ByteBuffer clear .
         *
         * 単純に書き込みポインタを0にしただけでバッファサイズ、内容はクリアされない
         *  \retval 常に成功
         */
        return_code clear() noexcept(true)
        {
            m_wp = 0;
            return OK;
        }
        /**  ByteBuffer to string .
         *
         *
         */
        std::string to_string() const noexcept(true)
        {
            return std::string(m_storage, m_wp);
        }
        //
        // access
        //
        // /** []access .
        //  *
        //  *  x[i] = iのアクセス用
        //  * \exception out_of_range iがインデックスの範囲を超た 有効範囲は(0 <= i) && (i <= m_capacity)
        //  */
        // // ByteBuffer& operator[](index_type i) & noexcept(false)
        // // {
        // //     //BUFFER_CHECK_CAPACITY(i);
        // //     if (0 > i || m_capacity <= i) throw std::out_of_range("index is out of range");
        // //     if (m_wp == 0 || (m_wp >= i )) throw std::out_of_range("no data");
        // //     return *this;
        // // }
        /** []access .
         *
         * auto y = x[i]のアクセス用
         * \exception out_of_range iがインデックスの範囲を超た 有効範囲は(0 <= i) && (i <= m_capacity)
         * \exception out_of_range
         */
        const byte_type& operator[](index_type i) const& noexcept(false)
        {
            //BUFFER_CHECK_CAPACITY(i);
            if (0 > i || m_capacity <= i) throw std::out_of_range("index is out of range");
            return m_storage[i];
        }
        /** access[] .
         *
         * if (x[i] == 'a') cout << "Hello" << endl;のアクセス用
         * \exception out_of_range iがインデックスの範囲を超た 有効範囲は(0 <= i) && (i <= m_capacity)
         */
        byte_type operator[](index_type i) const&& noexcept(false)
        {
            //BUFFER_CHECK_CAPACITY(i);
            if (0 > i || m_capacity <= i) throw std::out_of_range("index is out of range");
            return m_storage[i];
        }
        // /** 差戻し .
        //  *
        //  *
        //  */
        // return_code put_back() noexcept
        // {
        //     --m_wp;
        //     return OK;
        // }
    private:
        size_t     m_capacity; //!< storage size
        index_type m_wp;       //!< write pointer 書き込み後次の書き込み位置に移動
        storage_p  m_storage;  //!< bufferの本体
    }; //<-- class template<typename T>ByteBuffer ends here.

    // /**  .
    //  * 
    //  *
    //  */
    // using ByteBuffer = Buffer<byte_type>; //!< Buffer class alias for byte(char array)


    /** Buffer class maker.
     * \note 制限事項：コンストラクターが例外を投げないように、size指定が0以下の場合default_sizeにする。またlimit_sizeを越える場合limit_sizeに丸める
     *
     *  \param[in] s buffer storageのsize指定
     *  \retval Buffer<decltype(t)>オブジェクト
     */
    inline ByteBuffer make_buffer(size_t s)
    {
        auto size = s;
        // TRACE(std::to_string(size));
        if (size <= 0) return ByteBuffer{}; // default constructor
        if (size > Mult::ByteBuffer::limit_size()) size = Mult::ByteBuffer::limit_size();
        // TRACE(std::to_string(size));
        return ByteBuffer{size}; // sized constructor
    }
    /** ByteBuffer from string .
     *
     *
     */
    inline ByteBuffer from_string(const std::string& from)
    {
        auto size = from.size();
        if (size < Mult::ByteBuffer::default_size()) size = Mult::ByteBuffer::default_size();
        ByteBuffer x(size);
        for (auto c : from) {
            x.push_back(c);
        }
        return  x;
    }
    /** ByteBuffer to string in hex dump  .
     *
     *
     */
    inline std::string hexDump(const ByteBuffer& t)
    {
        std::string d(0x0, t.capacity());
        for (auto x : t) {
            d.append(Debug::hexChar256Tbl[(static_cast<std::uint8_t>(x)) & 0xff]);
        }
        return d;
    }
    /** ByteBuffer to string in human readable .
     *
     *
     */
    inline std::string toReadableCtrlCode(const ByteBuffer& s)
    {
        std::string d{};
        std::string t;
        for (auto c : s) {
            auto x = (static_cast<size_t>(c)) & 0xff;
            if (x < 0x20) {
                d += Debug::ctrlCharTbl[x];
            } else if (x == 0x20) {
                d.append("[SPC]");
            } else if (x == 0x7f) {
                d.append("[DEL]");
            } else if (x > 0x7f) {
                if (x == 0xff) {
                    d.append("[EOF]");
                } else {
                    d.append(Debug::ctrlCharTbl[x - 0x60]);
                }
            } else {
                d.push_back(x);
            }
        }
        return d;
    } //<-- function toReadableCtrlCode ends here.
/*@}*/ //<-- group Buffer ends here.
} //<-- namespace Mult ends here.


#endif //<-- macro  MULT_BUFFER_Hpp ends here.

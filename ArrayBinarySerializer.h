/* 
 * File:   ArrayBinarySerializer.h
 * Author: michal
 *
 * Created on March 1, 2016, 3:45 AM
 */

#ifndef ARRAYBINARYSERIALIZER_H
#define	ARRAYBINARYSERIALIZER_H

#include <array>

namespace Serialize
{
    
    template <class T, std::size_t N, bool raw = is_raw_binary_serializable<T>::value>
    struct FixedArraySerializer
    {
        void serialize(IOutputStream &os, const std::array<T, N> &data)
        {
            for (int i = 0; i < N; i++)
                binary(os) << data[i];
        }
    };
    template <class T, std::size_t N>
    struct FixedArraySerializer<T, N, true> : RawBinarySerializer<std::array<T, N>> {};
    
    template <class T, std::size_t N, bool raw = is_raw_binary_deserializable<T>::value>
    struct FixedArrayDeserializer
    {
        void serialize(IOutputStream &os, const std::array<T, N> &data)
        {
            for (int i = 0; i < N; i++)
                binary(os) << data[i];
        }
    };
    template <class T, std::size_t N>
    struct FixedArrayDeserializer<T, N, true> : RawBinaryDeserializer<std::array<T, N>> {};
    
    template <class T, std::size_t N>
    struct Serializer<BinaryFormat, std::array<T, N>> : FixedArraySerializer<T, N> {};
    template <class T, std::size_t N>
    struct Deserializer<BinaryFormat, std::array<T, N>> : FixedArrayDeserializer<T, N> {};
}

#endif	/* ARRAYBINARYSERIALIZER_H */


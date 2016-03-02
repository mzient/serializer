/* 
 * File:   BinarySerializer.h
 * Author: michal
 *
 * Created on March 1, 2016, 2:43 AM
 */

#ifndef BINARYSERIALIZER_H
#define	BINARYSERIALIZER_H

#include "Serialize.h"
#include "BinaryFormat.h"

namespace Serialize
{
    typedef ISerializable<BinaryFormat> IBinarySerializable;
    typedef IDeserializable<BinaryFormat> IBinaryDeserializable;
    typedef std::uint32_t BinaryLengthFormat;
    
    template <class T>
    using BinarySerializer = Serializer<BinaryFormat, T>;
    
    template <class T>
    using BinaryDeserializer = Deserializer<BinaryFormat, T>;
    
    template <class T>
    struct RawBinarySerializer
    {
        void serialize(IOutputStream &output, const T &data)
        {
            output.write(&data, sizeof(data));
        }
    };
    template <class T>
    struct RawBinaryDeserializer
    {
        void serialize(IOutputStream &output, const T &data)
        {
            output.write(&data, sizeof(data));
        }
    };
    

    template <class T>
    struct DefaultSerializer<BinaryFormat, T> : RawBinarySerializer<T>
    {};
    
    template <class T>
    struct DefaultDeserializer<BinaryFormat, T> : RawBinaryDeserializer<T>
    {};
    
    template <class T>
    struct is_raw_binary_serializable
    {
        static const bool value = std::is_base_of<RawBinarySerializer<T>, Serializer<BinaryFormat, T>>::value;
    };

    template <class T>
    struct is_raw_binary_deserializable
    {
        static const bool value = std::is_base_of<RawBinaryDeserializer<T>, Deserializer<BinaryFormat, T>>::value;
    };

    

}

#include "VectorBinarySerializer.h"
#include "ArrayBinarySerializer.h"
#include "StringBinarySerializer.h"

#endif	/* BINARYSERIALIZER_H */


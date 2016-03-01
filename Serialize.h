/* 
 * File:   Serialize.h
 * Author: michal
 *
 * Created on March 1, 2016, 2:25 AM
 */

#ifndef SERIALIZE_H
#define	SERIALIZE_H

#include "Stream.h"
#include <type_traits>

namespace Serialize
{
    template <class Format>
    struct ISerializable
    {
        virtual void serialize(IOutputStream &out) const=0;
    };
    
    template <class Format>
    struct IDeserializable
    {
        virtual void deserialize(IInputStream &in)=0;
    };
    

    template <class Format, class T>
    struct is_serializable : std::is_base_of<ISerializable<Format>, T> {};
    
    template <class Format, class T>
    struct is_deserializable : std::is_base_of<ISerializable<Format>, T> {};    
    
    template <class Format, class T, bool IsSerializable = is_serializable<Format, T>::value>
    struct DefaultSerializer;
    
    template <class Format, class T, bool IsDeserializable = is_deserializable<Format, T>::value>
    struct DefaultDeserializer;

    template <class Format, class T>
    struct DefaultSerializer<Format, T, true>
    {
        void serialize(IOutputStream &out, const T &object)
        {
            object.serialize(out);
        }
    };

    template <class Format, class T>
    struct DefaultDeserializer<Format, T, true>
    {
        void deserialize(IInputStream &in, T &object)
        {
            object.deserialize(in);
        }
    };
    
    template <class Format, class T>
    struct Serializer : DefaultSerializer<Format, T> {};
    
    template <class Format, class T>
    struct Deserializer : DefaultSerializer<Format, T> {};
    
    template <class Format, class T>
    Serializer<Format, T> GetSerializer(const T &data)
    {
        return Serializer<Format, T>();
    }
    
    template <class Format, class T>
    Deserializer<Format, T> GetDeserializer(T &data)
    {
        return Deserializer<Format, T>();
    }
    
    template <class Format, class T>
    FormattedOStream<Format> operator<<(FormattedOStream<Format> os, const T &data)
    {
        GetSerializer<Format>(data).serialize(os, data);
        return os;
    }
    
    template <class Format, class T>
    FormattedIStream<Format> operator>>(FormattedIStream<Format> is, T &data)
    {
        GetDeserializer<Format>(data).deserialize(is, data);
        return is;
    }

};

#endif	/* SERIALIZE_H */


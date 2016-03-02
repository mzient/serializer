/* 
 * File:   Serialize.h
 * Author: michal
 *
 * Created on March 1, 2016, 2:25 AM
 */

#ifndef SERIALIZE_H
#define	SERIALIZE_H

#include "Stream.h"
#include "Memberwise.h"
#include <type_traits>

template <class Format>
struct serialization_purpose {};
template <class Format>
struct deserialization_purpose {};

namespace Serialize
{
    struct no_member_serialize {};
}

template <class Type, class Format> struct member_list<Type, serialization_purpose<Format>> : member_ptr_list<>, Serialize::no_member_serialize {};
template <class Type, class Format> struct member_list<Type, deserialization_purpose<Format>> : member_list<Type, serialization_purpose<Format>> {};


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
    
    template <class Format, class Type>
    struct is_memberwise_serializable : std::integral_constant<bool, !std::is_base_of<no_member_serialize, member_list<Type, serialization_purpose<Format>>>::value> {};

    template <class Format, class Type>
    struct is_memberwise_deserializable : std::integral_constant<bool, !std::is_base_of<no_member_serialize, member_list<Type, deserialization_purpose<Format>>>::value> {};
    
    // Default serializers must be implemented per format
    template <class Format, class T>
    struct DefaultSerializer;
    template <class Format, class T>
    struct DefaultDeserializer;
    
    // Generic serialization follows the inference path:
    // - if type implements ISerializable/IDeserializable - use it
    // - if type defines a member list for serialization purpose with this format - use it
    // - otherwise, use a "DefaultSerializer".
    // All of the above can be cut short by specializing Serializer for given type/format pair.
    
    template <class Format, class T, bool IsMemberwise = is_memberwise_serializable<Format, T>::value>
    struct SerializerCheckMemberwise : DefaultDeserializer<Format, T> {};
    template <class Format, class T, bool IsMemberwise = is_memberwise_deserializable<Format, T>::value>
    struct DeserializerCheckMemberwise : DefaultDeserializer<Format, T> {};

    template <class Format, class T, bool IsSerializable = is_serializable<Format, T>::value>
    struct SerializerCheckInterface : SerializerCheckMemberwise<Format, T> {};
    template <class Format, class T, bool IsDeserializable = is_deserializable<Format, T>::value>
    struct DeserializerCheckInterface : DeserializerCheckMemberwise<Format, T> {};
    
    template <class Format, class T>
    struct GenericSerializer : SerializerCheckInterface<Format, T> {};
    
    template <class Format, class T>
    struct GenericDeserializer : DeserializerCheckInterface<Format, T> {};

    template <class Format, class T>
    struct SerializerCheckInterface<Format, T, true>
    {
        void serialize(IOutputStream &out, const T &object)
        {
            object.serialize(out);
        }
    };

    template <class Format, class T>
    struct DeserializerCheckInterface<Format, T, true>
    {
        void deserialize(IInputStream &in, T &object)
        {
            object.deserialize(in);
        }
    };
    
    template <class Format, class T>
    struct Serializer : GenericSerializer<Format, T> {};
    
    template <class Format, class T>
    struct Deserializer : GenericSerializer<Format, T> {};
    
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

#include "MemberwiseSerializer.h"

#endif	/* SERIALIZE_H */


/* 
 * File:   MemberwiseSerializer.h
 * Author: michal
 *
 * Created on March 3, 2016, 12:10 AM
 */

#ifndef MEMBERWISESERIALIZER_H
#define	MEMBERWISESERIALIZER_H

namespace Serialize
{
    template <class Format>
    struct MemberSerializer
    {
        MemberSerializer(IOutputStream &stream) : stream(stream) {}
        Serialize::FormattedOStream<Format> stream;

        template <class T>
        void operator()(const T &member)
        {
            stream << member;
        }
    };
    template <class Format>
    struct MemberDeserializer
    {
        MemberDeserializer(IInputStream &stream) : stream(stream) {}
        Serialize::FormattedIStream<Format> stream;

        template <class T>
        void operator()(T &member)
        {
            stream >> member;
        }
    };
    
    template <class Format, class T>
    struct SerializerCheckMemberwise<Format, T, true>
    {
        void serialize(IOutputStream &out, const T &object)
        {
            MemberSerializer<Format> S(out);
            memberwise<serialization_purpose<Format>>(object, S);
        }
    };

    template <class Format, class T>
    struct DeserializerCheckMemberwise<Format, T, true>
    {
        void deserialize(IInputStream &in, T &object)
        {
            MemberDeserializer<Format> D(in);
            memberwise<serialization_purpose<Format>>(object, D);
        }
    };
}

#define SERIALIZE_MEMBERS(Format, Type, ...) template <> struct member_list<Type, serialization_purpose<Format> > : decltype(member_ptr_list<>() MEMBER_LIST_ARGS(Type, __VA_ARGS__)) {}

// only use if member list is different than one specified in SERIALIZE_MEMBERS
// or if SERIALIZE_MEMBERS is deliberately not specified
#define DESERIALIZE_MEMBERS(Format, Type, ...) template <> struct member_list<Type, deserialization_purpose<Format> > : decltype(member_ptr_list<>() MEMBER_LIST_ARGS(Type, __VA_ARGS__)) {}

#endif	/* MEMBERWISESERIALIZER_H */


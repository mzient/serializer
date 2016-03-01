/* 
 * File:   StringBinarySerializer.h
 * Author: michal
 *
 * Created on March 1, 2016, 3:45 AM
 */

#ifndef STRINGBINARYSERIALIZER_H
#define	STRINGBINARYSERIALIZER_H

#include <string>


namespace Serialize
{
    
    struct StringSerializer
    {
        void serialize(IOutputStream &os, const std::string &s)
        {
            BinaryLengthFormat l = s.length();
            os.write(&l, sizeof(l));
            os.write(s.data(), l);
        }
    };
    
    struct StringDeserializer
    {
        void serialize(IInputStream &os, std::string &s)
        {
            BinaryLengthFormat l;
            os.read(&l, sizeof(l));
            s.resize(l);
            os.read(&s[0], l);
        }
    };
    
    template <>
    struct Serializer<BinaryFormat, std::string> : StringSerializer {};
    template <>
    struct Deserializer<BinaryFormat, std::string> : StringDeserializer {};
}

#endif	/* STRINGBINARYSERIALIZER_H */

/* 
 * File:   VectorBinarySerializer.h
 * Author: michal
 *
 * Created on March 1, 2016, 3:44 AM
 */

#ifndef VECTORBINARYSERIALIZER_H
#define	VECTORBINARYSERIALIZER_H

#include <vector>

namespace Serialize
{
    
    template <class T, bool raw = is_raw_binary_serializable<T>::value>
    struct VectorSerializer
    {
        void serialize(IOutputStream &os, const std::vector<T> &v)
        {
            BinaryLengthFormat l = v.size();
            os.write(&l, sizeof(l));
            for (int i = 0; i < l; i++)
                binary(os) << v[i];
        }
    };
    
    template <class T>
    struct VectorSerializer<T, true>
    {
        void serialize(IOutputStream &os, const std::vector<T> &v)
        {
            BinaryLengthFormat l = v.size();
            os.write(&l, sizeof(l));
            os.write(v.data(), l*sizeof(T));
        }
    };
    
    template <class T, bool raw = is_raw_binary_serializable<T>::value>
    struct VectorDeserializer
    {
        void serialize(IInputStream &os, std::vector<T> &v)
        {
            BinaryLengthFormat l;
            os.read(&l, sizeof(l));
            v.resize(l);
            for (int i = 0; i < l; i++)
                binary(os) >> v[i];
        }
    };
    
    template <class T>
    struct VectorDeserializer<T, true>
    {
        void serialize(IInputStream &os, std::vector<T> &v)
        {
            BinaryLengthFormat l;
            os.read(&l, sizeof(l));
            v.resize(l);
            os.read(v.data(), l*sizeof(T));
        }
    };
    
    template <class T>
    struct Serializer<BinaryFormat, std::vector<T>> : VectorSerializer<T> {};
    template <class T>
    struct Deserializer<BinaryFormat, std::vector<T>> : VectorDeserializer<T> {};
}

#endif	/* VECTORBINARYSERIALIZER_H */


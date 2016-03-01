/*
 * File:   main.cpp
 * Author: michal
 *
 * Created on February 20, 2016, 10:47 PM
 */


#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "Serialize.h"
#include "BinarySerializer.h"

using namespace std;

struct HexStream : Serialize::IOutputStream
{
    void write(const void *data, size_t len)
    {
        for (size_t i = 0; i < len; i++)
        {
            printf("%02x", ((uint8_t*)data)[i]);
        }
        printf(" ");
    }
};

struct A : Serialize::IBinarySerializable
{
    A(int x) : x(x) {}
    virtual void serialize(Serialize::IOutputStream &out) const
    {
        binary(out) << x;
    }
    int x;
};

struct X : Serialize::IBinarySerializable
{
    virtual void serialize(Serialize::IOutputStream &out) const
    {
        std::vector<std::string> x= { "ala", "ma", "kota" };
        binary(out) << x;
    }
};

/*
 *
 */
int main(int argc, char** argv)
{
    HexStream hs;
    X x;
    binary(hs) << x;


    return 0;
}


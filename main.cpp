/*
 * File:   main.cpp
 * Author: michal
 *
 * Created on February 20, 2016, 10:47 PM
 */


#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <tuple>
#include "Serialize.h"
#include "BinarySerializer.h"

using namespace std;

struct HexStream : Serialize::IOutputStream
{
    void write(const void *data, size_t len) override
    {
        for (size_t i = 0; i < len; i++)
        {
            printf("%02x", ((uint8_t*)data)[i]);
        }
        printf(" ");
    }
};

/*struct A : Serialize::IBinarySerializable
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
        std::vector<int> x = { 1, 2, 3 };
        binary(out) << x;
    }
};
*/
namespace Serialize
{


}

struct S
{
    int x, y, z;
    string a;
};
SERIALIZE_MEMBERS(Serialize::BinaryFormat, S, x, y, z, a);

struct printer
{
    template <class T>
    void operator()(const T &data)
    {
        cout << data << " ";
    }
};

/*
 *
 */
int main(int argc, char** argv)
{
    HexStream hs;
    S s = { 1, 2, 3, "ala ma kota" };
    //X x;
    binary(hs) << s;

    return 0;
}


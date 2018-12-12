#ifndef HYPERDATE_H
#define HYPERDATE_H

#include <vector>
#include <iostream>
#include <string>
#include <bitset>

#define CHAR_BIT 8

using namespace std;

#define BS 32

typedef vector<float> point;

class HyperDate
{
    public:
        int dim;
        string ProcessedBits;
        HyperDate(int _dim, point _Pt);
        string processPoint(point _PT);
        string GetBinary32(float value);
        string subset(int pos);
        virtual ~HyperDate();

    protected:

    private:
};

#endif // HYPERDATE_H

#include<iostream>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>

using namespace std;
using namespace NTL;

class Pedersen{

public:


pair<ZZ_p,ZZ> commit(long x)
{
  ZZ_p::init(ZZ(113055));

  ZZ_p g= ZZ_p(269);
  ZZ_p h= ZZ_p(4696);

  ZZ r = RandomBits_ZZ(20);;
  ZZ_p c = power(g, ZZ(x)) * power(h,r);
  return make_pair(c,r);
}

bool open(ZZ_p c, long  x, ZZ r)
{
  ZZ_p::init(ZZ(113055));

  ZZ_p g= ZZ_p(269);
  ZZ_p h= ZZ_p(4696);
  auto res =   power(g, ZZ(x)) * power(h,r);
  if(res == c)
    return true;
  else
    return false;
}
};

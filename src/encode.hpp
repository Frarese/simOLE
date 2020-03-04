#include<NTL/GF2E.h>
#include<NTL/GF2EX.h>
#include<NTL/GF2XFactoring.h>
#include<NTL/vec_GF2E.h>
#include<NTL/vec_GF2.h>
#include<NTL/GF2X.h>


//TODO change L in a bitmap
class Encoder{

public:
  Encoder(int k,vec_GF2E &a, vec_GF2E &b )
  {
    BuildIrred(P,k);
    GF2E::init(P);
    alfa = a;
    beta = b;
    for(int i = 0; i< 8*k; i++)
    {
      if(i < 2*k-1)
        L.append(GF2(0));
      else
        L.append(GF2(1));
    }
  }
  vec_GF2E encode(vec_GF2E &u)
  {
      GF2EX poly = interpolate(alfa,u);
      vec_GF2E v;
      for(int i = 0; i< beta.length(); i++)
      {
        //if(i >= 2*k-1)
          //v.append(random_GF2E());
        //else
        v.append(eval(poly, beta[i]));
      }
      return v;
  }

  vec_GF2 getL()
  {
    return L;
  }

  vec_GF2E decode(vec_GF2E &v)
  {
    
  }

private:
  vec_GF2E alfa, beta;
  GF2X P;
  vec_GF2E u;
  vec_GF2 L;
};

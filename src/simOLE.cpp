#include<iostream>
#include<NTL/GF2.h>
#include<NTL/GF2X.h>
#include<NTL/GF2XFactoring.h>
#include<NTL/GF2EX.h>
#include<NTL/GF2E.h>
#include<assert.h>
#include<NTL/vec_GF2E.h>
#include"pedersen.hpp"
#include"encode.hpp"

using namespace NTL;
int t = 1, k = 40, n = 8*k, p = 6*k +1, l = n-p;


long convert(GF2E x)
{

}


double sim()
{
  GF2X P;
  BuildIrred(P, 40);
  GF2E::init(P);
  GF2E a,b,x;
  a = random_GF2E(); b = random_GF2E(); x = random_GF2E();

  vec_GF2E alfa = random_vec_GF2E((l+1)/2), beta = random_vec_GF2E(n);

  long result = 0;
  vec_GF2E Ax = random_vec_GF2E((l-1)/2), Ay = random_vec_GF2E((l-1)/2);
  Ax.append(alfa[0]); Ay.append(a);
  GF2EX A = interpolate(Ax,Ay);
  Ax.kill(); Ay.kill();

  vec_GF2E Bx = random_vec_GF2E(n-1), By = random_vec_GF2E(n-1);
  Bx.append(alfa[0]); By.append(b);
  GF2EX B = interpolate(Bx,By);
  Bx.kill(); By.kill();

  vec_GF2E T = random_vec_GF2E(n);

  srand (time(NULL));
  long ex = rand() % (long)pow(2,k);
  Pedersen ap;
  pair<ZZ_p,ZZ> com = ap.commit(ex);
  GF2E e = GF2E(ex);


  vec_GF2E sx = random_vec_GF2E(p-1), sy = random_vec_GF2E(p-1);
  sx.append(GF2E()); sy.append(e);
  GF2EX s = interpolate(sx,sy);
  sx.kill(); sy.kill();
  vec_GF2E shares;
  for(int i = 0; i < n; i++)
    shares.append(eval(s,beta[i]));

  //OT and commitment
  result += sizeof(com.first) + n*5;

  vec_GF2E u;
  u.append(x); u.append(random_vec_GF2E((l-1)/2));
  Encoder en(k,alfa,beta);
  vec_GF2E v = en.encode(u);
  vec_GF2 L = en.getL();

  GF2E e2;
  vec_GF2E ind,  share, beta3, Xy;

  for(int i = 0; i< l; i++)
  {
    beta3.append(beta[i]);
    Xy.append(v[i]);
  }

  //SS.Reconstruct
  for(int i = 0; i< p; i++)
  {
      ind.append(beta[i]);
      share.append(shares[i]);
  }
  GF2EX X = interpolate(beta3,Xy);
  GF2EX polyS = interpolate(ind, share);
  e2 = eval(polyS,GF2E(0));
  assert(e2 == GF2E(ex));
  //std::cerr << GF2E(ex) << "\n";
  //secret sent
  result += 5;

  //unv received
  result+= 5;
  assert(ap.open(com.first,ex,com.second));

  result += n*5 + n*k*5;

  vec_GF2E wT;
  for(int i = 0; i< n; i++)
  {

    wT.append((eval(A, beta[i])* v[i]) + eval(B, beta[i]) + T[i]);
  }
  //send wT
  result += n*5;

  vec_GF2E w, beta2;
  for(int i = 0; i < n; i++)
  {
    assert(eval(X,beta[i]) == v[i]);
    w.append(wT[i] -T[i]);
    beta2.append(beta[i]);
  }
  GF2EX Y = interpolate(beta2,w);
  GF2E zR = random_GF2E(); result += 5;
  GF2E zS = random_GF2E(); result +=15;

  //cerr << (a*x)+b << "\n";
  //cerr << (eval(A,alfa[0])*eval(X,alfa[0]))+ eval(B,alfa[0])<< "\n";
  //cerr << eval(Y,alfa[0])<< "\n";

  assert((eval(A,alfa[0])*eval(X,alfa[0]))+ eval(B,alfa[0]) == (a*x)+b);
  assert((eval(A,zR)*eval(X,zR))+ eval(B,zR) == eval(Y,zR));
  assert((eval(A,zS)*eval(X,zS))+ eval(B,zS) == eval(Y,zS));
  cout << "OLE completed!\n";

  return (double)result;
}

int main()
{

  double result = sim();
  cout << "Communication cost of a single OLE (40 elements): " << result << "bytes\n";
  cout << "Communication cost of a OPA (2^20 elements): " << (result/(40000)) << "Gb\n";
}

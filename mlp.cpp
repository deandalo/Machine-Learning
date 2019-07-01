#include <iostream>
#include "mat.h"
#include "rand.h"
#include <string>

using namespace std;
int main()
{
  //Input data

  int inp,i,hid,j,k,m,n,o,m_test,n_test;
  double val,valf,dis,maxr;
  const int S = 1, ITER = 12000;
  const float E = 2.71828, ETA = .1;
  cin >> inp;
  cin >> hid;
  cin >> m >> n;
  
  Matrix in = new Matrix(m,n,"Input Matrix");
  for (i=0;i<m;i++){
    for (j=0;j<n;j++){
      cin >> val;
      in.set(i,j,val);
    }
  }
   
 //Get matrix x from input matrix
  Matrix x = new Matrix(m,inp+1,"x Matrix");
  for (i=0;i<m;i++)
    for (j=0;j<inp;j++){
      x.set(i,j,in.get(i,j));
    }
  //cout << "x Matrix" << endl;
  //x.write();
  //cout << endl;

  //Get the target matrix
  o = n - inp;
  //cout << out << endl;
  Matrix t = new Matrix(m,o);
  for (i=0;i<m;i++)
    for (j=0;j<o;j++){
      t.set(i,j,in.get(i,j+inp));
    }
  
  //cout << "target matrix" << endl;
  //t.write();
  //cout << endl;

  //Need to transpose because the normalize function is by columns
  //and we need it by rows
  x = x.transpose();
  x.normalize();
  x = x.transpose();

  //Append last column with -1 values
  for (i=0;i<m;i++){
    x.set(i,inp,-1);
  }
  
  //cout << "Matrix x with bias" << endl;
  //x.write();
  //cout << endl;

  //Generate random weights v and w
    
  Matrix v = new Matrix(inp+1,hid,"v weights");
  initRand();
  //cout << "Matrix v" << endl;
  for (i=0;i<=inp;i++){
    for (j=0;j<hid;j++){
      v.set(i,j,randPMUnit()*.1);
    }
  }
  
  //v.write();
  //cout << endl;

  Matrix w = new Matrix(hid+1,o,"w weights");
  //cout << "Matrix w" << endl;
  for (i=0;i<=hid;i++){
    for (j=0;j<o;j++){
      w.set(i,j,randPMUnit()*.1);
    }
  }
  
  //w.write();
  //cout << endl;
  
  //Constructors for matrices in the loops
  Matrix h = new Matrix(m,hid,"h matrix");
  Matrix h1 = new Matrix(m,hid+1,"h matrix with -1 appended");
  Matrix y = new Matrix(m,o, "y output matrix");
  Matrix ys = new Matrix(m,o,"y output matrix after activation function");
  Matrix dy = new Matrix(ys,"dy matrix");
  Matrix y1 = new Matrix(dy,"1-y");
  Matrix dh = new Matrix(h1,"dh matrix");
  Matrix dh1 = new Matrix(dh,"1 - h matrix");
  Matrix wt = new Matrix(w,"Transpose(w)");
  Matrix dyt = new Matrix(dy,"dy dot Transpose(w)");
  Matrix ht = new Matrix(hid+1,m,"Transpose(h)");
  Matrix htdy = new Matrix(hid+1,o,"Transpose(h).dy");
  Matrix xt = new Matrix(inp+1,m,"Transpose(x)");
  Matrix dh_1 = new Matrix(m,hid,"dh without -1 column");
  Matrix xtdh = new Matrix(inp+1,hid+1,"Transpose(x).dh");

  for (k=0;k<=ITER;k++){
    //    if (k%1000==0){
    //cout << "Iteration number " << k << endl;
    //  }
  //Forward
  // h = x . v
  h = x.dot(v);

  //cout << "x" << endl;
  //x.write();
  //cout << endl;
  //cout << "v" << endl;
  //v.write();
  //cout << endl;
  
  //cout << "Matrix h" << endl;
  //h.write();
  //cout << endl;

  //Sigmoid transfer function
  for (i=0;i<m;i++)
    for (j=0;j<hid;j++){
      valf = h.get(i,j);
      //            cout << "matrix h" << endl;
      //cout << valf << endl;
      valf = 1/(1 + pow(E,-4*S*valf));
      //cout << "hidden layer after applyin sigmoid function" << endl;
      //cout << valf << endl;
      h.set(i,j,valf);
    }
  
  //cout << "Matrix h after transfer function" << endl;
  //h.write();
  //cout << endl;

  //Append last column of h with -1 values

  for (i=0;i<m;i++)
    for (j=0;j<hid;j++){
      h1.set(i,j,h.get(i,j));
    }
  
  for (i=0;i<m;i++){
    h1.set(i,hid,-1);
      }
    
  //h1.write();

  //y = h . w
  
  y=h1.dot(w);

  //cout << endl;
  //cout << "y output matrix" << endl;
  //y.write();

  //Apply sigmoid transfer function
  
  for (i=0;i<m;i++)
    for (j=0;j<o;j++){
      valf = y.get(i,j);
      valf = 1/(1+pow(E,-4*S*valf));
      ys.set(i,j,valf);
    }
  
  if (k%ITER==1000){   
  //cout << endl;
    //cout << "y after transfer function" << endl;
    //ys.write();
  }
  
  if (k==ITER){
    break;
  }
  //Backward

  //dy=(y-t)y(1-y)
  dy = ys;
  dy = dy.sub(t);
  //cout << endl;
  //cout << "y - t" << endl;
  //dy.write();
  dy = dy.mult(ys);
  //cout << "y(y-t)" << endl;
  //dy.write();
  y1 = ys;
  y1 = y1.scalarPreSub(1);
  //cout << "1 - y" << endl;
  //y1.write();
  dy = dy.mult(y1);
  //cout << "dy" << endl;
  //dy.write();
  
  //dh=h(1-h)(dy . Transpose(w))
  dh=dh1=h1;
  //cout << "dh1 before -1" << endl;
  //dh1.write();
  dh1 = dh1.scalarPreSub(1);
  //cout << endl;
  //cout << "1 - h" << endl;
  //dh1.write();
  dh = dh.mult(dh1);
  //cout << endl << "h(1-h)" << endl;
  //dh.write();

  wt = w.transpose();
  //cout << "Transpose(w)" << endl;
  //wt.write();
  //cout << endl;
  
  
  dyt = dy.dot(wt);
  //cout << "dy . Transpose(w)" << endl;
  //dyt.write();
  //cout << endl;
  
  dh = dh.mult(dyt);
  //cout << "dh at the end of backward" << endl;
  //dh.write();
  //cout << endl;

  //Update
  
  ht = h1.transpose();
  //cout << "Transpose(h)" << endl;
  //ht.write();
  //cout << endl;
  
  
  htdy = ht.dot(dy);
  //cout << "Transpose(h).dy" << endl;
  //htdy.write();
  //cout << endl;
  //cout << "New matrix w" << endl;
  w = w.sub(htdy.scalarMult(ETA));
  if (k==ITER-1){
    //cout << "w" << endl;
    //w.write();
  }
  
  xt = x.transpose();

  
  for (i=0;i<m;i++)
    for (j=0;j<hid;j++){
      dh_1.set(i,j,dh.get(i,j));
    }
  
  xtdh = xt.dot(dh_1);
  //cout << " Transpose(x).dh" << endl;
  //xtdh.write();
  //cout << endl;
  //cout << "New matrix v" << endl;
  v = v.sub(xtdh.scalarMult(ETA));
  if(k==ITER-1){
    //cout << "v" << endl;
    //v.write();
  }
  }
  
  //Read in test data
  cin >> m_test >> n_test;
  Matrix TestData = new Matrix(m_test,n_test+1,"Test Data");
  for (i=0;i<m_test;i++)
    for (j=0;j<n_test;j++){
      cin >> val;
      TestData.set(i,j,val);
    }
  
  Matrix TestDataOrig = new Matrix(TestData,"Test Data without Normalization");

  //Normalize
  TestData = TestData.transpose();
  TestData.normalize();
  TestData = TestData.transpose();
  
  //Append last column with -1 values
  for (i=0;i<m_test;i++){
    TestData.set(i,n_test,-1);
  }
  
  h = TestData.dot(v);
  
  //cout << "h" <<endl;
  //h.write();

    //Sigmoid transfer function
  for (i=0;i<m_test;i++)
    for (j=0;j<hid;j++){
      valf = h.get(i,j);
      //            cout << "matrix h" << endl;
      //cout << valf << endl;
      valf = 1/(1 + pow(E,-4*S*valf));
      //cout << "hidden layer after applyin sigmoid function" << endl;
      //cout << valf << endl;
      h.set(i,j,valf);
    }
  
  //cout << "Matrix h after transfer function" << endl;
  //h.write();
  //cout << endl;

  //Append last column of h with -1 values

  for (i=0;i<m_test;i++)
    for (j=0;j<hid;j++){
      h1.set(i,j,h.get(i,j));
    }
  
  for (i=0;i<m_test;i++){
    h1.set(i,hid,-1);
      }
  
  //h1.write();

  //y = h . w
  
  y=h1.dot(w);
  //cout << endl;
  //cout << "y output matrix" << endl;
  //y.write();

  //Apply sigmoid transfer function
  Matrix yt = new Matrix(m_test,o,"Matrix y for test");
  for (i=0;i<m_test;i++)
    for (j=0;j<o;j++){
      valf = y.get(i,j);
      valf = 1/(1+pow(E,-4*S*valf));
      yt.set(i,j,valf);
    }
  
  //cout << endl;
  //cout << "y after transfer function" << endl;
  //yt.write();

  //Matrix with learned outputs
  Matrix out_test = new Matrix(m_test,n_test+o,"Final test result");
  for (i=0;i<m_test;i++)
    for (j=0;j<n_test;j++){
      out_test.set(i,j,TestDataOrig.get(i,j));
    }
  
  for (i=0;i<m_test;i++)
    for (j=n_test;j<n_test+o;j++){
      out_test.set(i,j,yt.get(i,j-n_test));
    }

  cout << "BEGIN TESTING" << endl;
  out_test.printSize();
  out_test.write();

  //Confusion matrix
  Matrix Confusion = new Matrix(o,o,"Confusion");
  for (i=0;i<m;i++){
    maxr = ys.get(i,0);
    //cout << "inital maxr" << maxr << endl;
    for (j=1;j<o;j++){
      //cout << "i " << i << endl;
      //cout << "j" << j << endl;
      //cout << "ys(i,j) " <<ys.get(i,j) << endl;
      //cout << "ys(i,j-1) " << ys.get(i,j-1) << endl;
      if (ys.get(i,j)>ys.get(i,j-1)){
	    maxr = ys.get(i,j);
      }
      //cout << " maxr on row " << i << " = " <<  maxr << endl;
    }
    for (j=0;j<o;j++)
      if (t.get(i,j)==1){
	for (k=0;k<o;k++)
	  if (ys.get(i,k)==maxr){
	    Confusion.set(j,k,Confusion.get(j,k)+1);
	    //Confusion.write();
	  }
      }
    }
  
  //Confusion.printSize();
  Confusion.write();

  //Euclidian distance between predicted and target values
  dis = sqrt(ys.dist2(t)); 
  cout << "DIST: " << dis << endl;
  
}


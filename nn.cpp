#include <iostream>
#include "mat.h"
#include "rand.h"
#include <string>

using namespace std;
int main()
{
  int m,n,o,i,j,k,rt,ct;
  double v;
  const float ETA = .1;
  string data;
  //  cout << "Enter the input data" << endl;
  cin >> m;
  cin >> n >> o;
  Matrix in = new Matrix(n,o,"Input Matrix");
  for (i=0;i<n;i++){
    for (j=0;j<o;j++){
      cin >> v;
      in.set(i,j,v);
   }
  }

  Matrix x = new Matrix(n,m+1, "x Matrix");
  for (i=0;i<n;i++)
    for (j=0;j<m+1;j++){
      x.set(i,j,in.get(i,j));
      }

  //Get the target matrix
  Matrix t = new Matrix(n,o-m,"Target");
  for (i=0;i<n;i++)
    for (j=0;j<o-m;j++){
      t.set(i,j,in.get(i,j+m));
    }
       
  //Need to transpose because the normalize function is by columns
  //and we need it by rows
  x = x.transpose();
  x.normalize();
  x = x.transpose();

  //Append last column with -1 values
    for (i=0;i<n;i++){
    x.set(i,m,-1);
    }

  //Generate random values for the weights
    Matrix w = new Matrix(m+1,o-m,"Weights");
    initRand();
    //    cout << "Random values for the weights" << endl;
    for (i=0;i<=m;i++){
      for (j=0;j<o-m;j++){
	w.set(i,j,randPMUnit());
	//	cout << w.get(i,j) << "\t";
	}
      //cout << endl;
    }
    //cout << endl;

  //Start running the iterations
  for (k=0;k<20;k++){
    //cout << "ITERATION NUMBER " << k+1 << endl;

  //Dot product matrix . weights
    Matrix y = new Matrix(n,o-m,"Y matrix");
    y = x.dot(w);
    //cout << "Dot product y = matrix . weights" << endl;
    for (i=0;i<n;i++)
      for (j=0;j<o-m;j++){
	  v = y.get(i,j);
	  //	  cout << v << endl;
    }
    //cout << endl;

  //Transfer function
    for (j=0;j<o-m;j++)
       for (i=0;i<n;i++){
	 v=y.get(i,j);
         if (v>.5){
	         y.set(i,j,1);
         }
         else{
        	 y.set(i,j,0);
         }
       }
    //cout << "Transfer function g applied to vector y" << endl;
    for (i=0;i<n;i++){
     for (j=0;j<o-m;j++)
      v = y.get(i,j);
     //cout << v << endl;
      }
    //cout << endl;

    //Target - output
    Matrix ty = new Matrix(t,"t-y");
    ty = ty.sub(y);
    //cout << "t - y vector" << endl;
    for (i=0;i<n;i++)
      for (j=0;j<o-m;j++){
	v = ty.get(i,j);
	//	cout << v << endl;
      }
    //cout << endl;
        
    Matrix zero = new Matrix(n,o-m,"zero");
    if (ty.equal(zero)){
      cout << "Training completed" << endl;
      cout << "FINAL WEIGHTS" << endl;
      for (i=0;i<=m;i++){
	for (j=0;j<o-m;j++){
	  v = w.get(i,j);
	 	  cout << v << "\t";
	}
	cout << endl;
      }
      cout << endl;
    break;
    }

    //Calculate new weights
    Matrix xt = new Matrix(m+1,n,"Transposed");
    xt = x.transpose();
    Matrix d = new Matrix(m+1,o-m,"Delta");
    d = xt.dot(ty);
    d = d.scalarMult(ETA);
    w = w.add(d);
    
    //cout << "New weights" << endl;
    for (i=0;i<=m;i++){
      for (j=0;j<o-m;j++){
	v = w.get(i,j);
	//	cout << v << "\t";
        }
      //cout << endl;
      }
    //cout << endl;
    }

  //TEST STARTS HERE
  //cout << "Enter test data" << endl;
  cin >> rt >> ct;
  Matrix TestData = new Matrix(rt,ct+1,"Test Data");
  for (i=0;i<rt;i++)
    for (j=0;j<ct;j++){
      cin >> v;
      TestData.set(i,j,v);
    }
  //  for (i=0;i<rt;j++){
  // for (j=0;j<ct;j++){
  //  cin >> v;
  //  TestData.set(i,j,v);
  //  cout << "\t";
  // }
  //  cout << endl;
  //  }
  //  cout << "Print Test Data Matrix" << endl;
  //TestData.write();
  Matrix TestDataOrig = new Matrix(TestData,"Test data without normlization");
      
  //Need to transpose because the normalize function is by columns
  //and we need it by rows
  TestData = TestData.transpose();
  TestData.normalize();
  TestData = TestData.transpose();
  //Append last column with -1 values
    for (i=0;i<rt;i++){
    TestData.set(i,ct,-1);
    }

    //TestData.write();
   
  //Dot product matrix . weights
    Matrix yt = new Matrix(rt,o-m,"Y matrix for test");
    yt = TestData.dot(w);
    //cout << "Dot product y = TestData . weights" << endl;
    for (i=0;i<rt;i++)
      for (j=0;j<o-m;j++){
	  v = yt.get(i,j);
	  //	  cout << v << endl;
    }
    //  cout << endl;
    
      //Transfer function
    for (j=0;j<o-m;j++)
       for (i=0;i<rt;i++){
	 v=yt.get(i,j);
         if (v>.5){
	         yt.set(i,j,1);
         }
         else{
        	 yt.set(i,j,0);
         }
       }

    //Matrix with learned outputs
    Matrix out = new Matrix(rt,ct+o-m,"Final test result");
    for (i=0;i<rt;i++)
      for (j=0;j<ct;j++){
	out.set(i,j,TestDataOrig.get(i,j));
      }
    for (i=0;i<rt;i++)
      for (j=ct;j<=ct+o-m-1;j++){
	out.set(i,j,yt.get(i,j-ct));
      }
    //cout << endl;
    cout << "BEGIN TESTING" << endl; 
    out.write();
      


}




  


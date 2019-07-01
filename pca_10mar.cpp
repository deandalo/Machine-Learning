#include "mat.h"
#include <iostream>

using namespace std;
int main(){
  // Input data
  int i,j,k,r,c;
  double val;
  cin >> k;
  cin >> r >> c;

  Matrix x = new Matrix(r,c,"input data");
  for (i=0;i<r;i++){
    for (j=0;j<c;j++){
      cin >> val;
      x.set(i,j,val);
    }
  }

  //cout << "Matrix x" << endl;
  //x.write();
  //cout << endl;
  

  // Mean and standard deviation
  Matrix mx = new Matrix(1,c,"mean");
  Matrix sdx = new Matrix(1,c,"standard deviation");
  for (j=0;j<c;j++){
    mx.set(0,j,x.meanCol(j));
    sdx.set(0,j,x.stddevCol(j));
  }

  //cout << "Mean vector of the data points" << endl;
  //mx.write();
  //cout << endl;

  //cout << "Standard deviation" << endl;
  //sdx.write();
  //cout << endl;
  
  //Z-score to scale data
  Matrix gn = new Matrix(x);
  for (i=0;i<r;i++)
    for (j=0;j<c;j++){
      gn.set(i,j,(gn.get(i,j)-mx.get(0,j))/sdx.get(0,j));
      }

  //cout << "Z-score scaled data" << endl;
  //gn.write();
  //cout << endl;

  //Biased co-variance between columns
  Matrix cc = new Matrix(c,c,"Biased co-variance between columns");
  cc = gn.cov();

  //cout << "Covariance matrix" << endl;
  //cc.write();
  //cout << endl;

  Matrix evalue = new Matrix(1,c,"Eigen values");
  evalue = cc.eigenSystem();
    
  Matrix evecs = new Matrix(cc,"Eigen vectors");
  cout << "EigenVectors in rows" << endl;
  evecs.write();
  cout << "Eigen values" << endl;
  evalue.write();
  
  Matrix newImage = new Matrix(r,r,"New Image");
  //cout << "Encoded Pic" << endl;
  newImage = gn.dotT(evecs);
  //newImage.write();
  //cout << endl;

  Matrix recoveredImage = new Matrix(r,c,"New Image");
  //cout << "Recovered pic" << endl;
  recoveredImage = newImage.dot(evecs);
  //recoveredImage.write();
  //cout << endl;
  
  Matrix newImage2 = new Matrix(r,k,"image with only k dimensions");
  for (i=0;i<r;i++)
    for (j=0;j<k;j++){
      newImage2.set(i,j,newImage.get(i,j));
    }

  Matrix newEvecs = new Matrix(k,c,"Eigen vectors with only k rows");
  for (i=0;i<k;i++)
    for (j=0;j<c;j++){
      newEvecs.set(i,j,evecs.get(i,j));
    }

  cout << "Compressed data" << endl;
  newImage2.write();
    
  recoveredImage = newImage2.dot(newEvecs);
  for (i=0;i<r;i++)
    for (j=0;j<c;j++){
      recoveredImage.set(i,j,recoveredImage.get(i,j)*sdx.get(0,j)+mx.get(0,j));
    }
  cout << "Decoded data" << endl;
  recoveredImage.write();
  
  //Component matrix
  Matrix cm = new Matrix(c,c,"Component matrix");
  for (i=0;i<c;i++)
    for (j=0;j<c;j++){
      val = evecs.get(i,j) * sqrt(evalue.get(0,i));
      cm.set(i,j,val);
				  }
  cout << "Component matrix" << endl;
  cm.write();

}

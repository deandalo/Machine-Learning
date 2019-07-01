#include "mat.h"
#include <iostream>
#include <math.h>

using namespace std;
int main(){
  int i,j,k,l,n,nData,nDim,iter;
  cin >> k >> n;
  cin >> nData >> nDim;
  float min[nDim], max[nDim],v;
  Matrix data = new Matrix(nData,nDim,"input data");
  for (i=0;i<nData;i++){
    for (j=0;j<nDim;j++){
      cin >> v;
      data.set(i,j,v);
    }
  }
  
  //Find the minimum and maximum values for each feature
    for (i=0;i<nDim;i++){
      min[i] = data.minCol(i);
      max[i] = data.maxCol(i);
      //cout << "Minimum value for column " << i << " is " << min[i] << endl;
      //cout << "Maximum value for column " << i << " is " << max[i] << endl;
    }

  //Pick the center locations randomly
    Matrix centers = new Matrix(k,nDim,"centers");
    initRand();
    for (i=0;i<k;i++)
      for (j=0;j<nDim;j++){
	centers.set(i,j,randUnit()*(max[j]-min[j])+min[j]);
      }
    //centers.write();


    for (iter=0;iter<=n;iter++){
      cout << "Try: " << iter << endl;

      //distances.constant(0);
      //sumDistances.constant(0);
    Matrix distances = new Matrix(nData,nDim,"distance matrices");
    Matrix sumDistances = new Matrix(nData,k,"sums of distances");

    //Compute distances
    for (l=0;l<k;l++){

      for (i=0;i<nData;i++)
	for (j=0;j<nDim;j++){
	  distances.set(i,j,pow((data.get(i,j) - centers.get(l,j)),2));
				   }
      //distances.write();
    for (i=0;i<nData;i++)
      for (j=0;j<nDim;j++)
	sumDistances.set(i,l,sumDistances.get(i,l) + distances.get(i,j));

	}
    //sumDistances.write();
    
    //Identify the closest cluster
    Matrix closest = new Matrix(nData,1,"closest cluster");
    closest = sumDistances.argMinRow();
    //closest.write();

    //Update the cluster centers
    Matrix thisCluster = new Matrix(nData,nDim,"nearest cluster assignment");
    for (i=0;i<nData;i++)
      for (j=0;j<nDim;j++){
	if (j == closest.get(i,0)){
	  thisCluster.set(i,j,1);
	}
      }
    //thisCluster.write();

    //Get number of points on each cluster to divide and get the mean
    Matrix sumCluster = new Matrix(k,1,"sum of clusters");
    for (i=0;i<nData;i++)
      for (j=0;j<k;j++){
	if (thisCluster.get(i,j) == 1){
	  sumCluster.set(j,0,sumCluster.get(j,0) +1);
	}
    }
    //sumCluster.write();
    
    //Get the new centers
    centers.constant(0);
    for (l=0;l<k;l++)
      for (i=0;i<nData;i++)
	if (closest.get(i,0) == l){
	  for (j=0;j<nDim;j++){
	    centers.set(l,j,(centers.get(l,j) + data.get(i,j)));
		      }
	 
	}
    //centers.write();
    
    for (i=0;i<k;i++)
      for (j=0;j<nDim;j++){
	if (sumCluster.get(i,0) != 0){
	  centers.set(i,j,(centers.get(i,j) / sumCluster.get(i,0)));
	}
	else{
	  centers.set(i,j,randUnit()*(max[j]-min[j])+min[j]);
		    }
    
      }
    
    centers.write();
    if (iter<n){ 
    for (i=0;i<k;i++){
      cout << "Group " << i << " size: " << sumCluster.get(i,0) << endl;
      } 
    }
  }	  
  
}

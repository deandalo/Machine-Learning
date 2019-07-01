#include "mat.h"
#include <iostream>
#include <math.h>

using namespace std;
int main(){
  int i,j,k,l,n,nData,nDim,iter=0,count;
  cin >> k >> n;
  cin >> nData >> nDim;
  float min[nDim], max[nDim],v,sb,sw,sbsw,sbswlast,xjmc,qual=0;
  Matrix data = new Matrix(nData,nDim,"input data");
  for (i=0;i<nData;i++){
    for (j=0;j<nDim;j++){
      cin >> v;
      data.set(i,j,v);
    }
  }
  //data.write();
  
  //Find the minimum and maximum values for each feature
    for (i=0;i<nDim;i++){
      min[i] = data.minCol(i);
      max[i] = data.maxCol(i);
      //cout << "Minimum value for column " << i << " is " << min[i] << endl;
      //cout << "Maximum value for column " << i << " is " << max[i] << endl;
    }

  //Pick the center locations randomly
    Matrix centers = new Matrix(k,nDim,"centers");
    Matrix bestCenters = new Matrix(centers);
    Matrix distances = new Matrix(nData,nDim,"distance matrices");
    Matrix sumDistances = new Matrix(nData,k,"sums of distances");
    Matrix closest = new Matrix(nData,1,"closest cluster");
    Matrix thisCluster = new Matrix(nData,k,"nearest cluster assignment");
    Matrix sumCluster = new Matrix(k,1,"sum of clusters");
    Matrix mean = new Matrix(1,nDim);
    Matrix mc_m = new Matrix(k,nDim);
    Matrix mcmrow = new Matrix(1,nDim);
    Matrix _sb = new Matrix(1,1);
    Matrix xj_mc = new Matrix(1,nDim);
     

    initRand();
    
    do{
    //do{
    for (i=0;i<k;i++)
    for (j=0;j<nDim;j++){
      centers.set(i,j,randUnit()*(max[j]-min[j])+min[j]);
    }
    //centers.write();
    //centers.set(0,0,0);
    //centers.set(0,1,0);
    //centers.set(0,2,0);
    //centers.set(1,0,0);
    //centers.set(1,1,1);
    //centers.set(1,2,0);
    //centers.set(2,0,0.5);
    //centers.set(2,1,0.5);
    //centers.set(2,2,1);
    //centers.set(3,0,0.5);
    //centers.set(3,1,1.5);
    //centers.set(3,2,1);
    //centers.set(4,0,1);
    //centers.set(4,1,0);
    //centers.set(4,2,0);
    //centers.set(5,0,1);
    //centers.set(5,1,1);
    //centers.set(5,2,0);
    //centers.set(6,0,1.5);
    //centers.set(6,1,0.5);
    //centers.set(6,2,1);
    //centers.set(7,0,1.5);
    //centers.set(7,1,1.5);
    //centers.set(7,2,1);

    
    for(iter=0;iter<n;iter++){
      //cout << "Try: " << iter << endl;

      //distances.constant(0);
      //sumDistances.constant(0);
   distances.constant(0);
    sumDistances.constant(0);
    //Compute distances
    //distances.write();
    //sumDistances.write();
    for (l=0;l<k;l++){

      for (i=0;i<nData;i++)
	for (j=0;j<nDim;j++){
	  distances.set(i,j,pow((data.get(i,j) - centers.get(l,j)),2));
				   }
      //distances.write();
      //sumDistances.write();
    for (i=0;i<nData;i++)
      for (j=0;j<nDim;j++)
	sumDistances.set(i,l,sumDistances.get(i,l) + distances.get(i,j));

	}
    //distances.write();
    //sumDistances.write();
    
    //Identify the closest cluster
    closest = sumDistances.argMinRow();
    //closest.write();

    //Update the cluster centers
     for (i=0;i<nData;i++)
      for (j=0;j<k;j++){
	if (j == closest.get(i,0)){
	  thisCluster.set(i,j,1);
	}
      }
    //thisCluster.write();

    //Get number of points on each cluster to divide and get the mean
    sumCluster.constant(0);
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
	if (sumCluster.get(i,0) > .0001){
	  centers.set(i,j,(centers.get(i,j) / sumCluster.get(i,0)));
	}
	else{
	  centers.set(i,j,randUnit()*(max[j]-min[j])+min[j]);
	  }
    
      }
    
    //centers.write();
    if (iter<n){ 
    for (i=0;i<k;i++){
      //cout << "Group " << i << " size: " << sumCluster.get(i,0) << endl;
      } 
    }

    //Calculate SB
    for (i=0;i<nDim;i++){
      mean.set(0,i,data.meanCol(i));
      }
    //mean.write();
    float sum =0;
     for (i=0;i<k;i++)
      for (j=0;j<nDim;j++){
	mc_m.set(i,j,centers.get(i,j)-mean.get(0,j));
      }  
    //mc_m.write();

     sum = 0;
    for (i=0;i<k;i++){
      for (j=0;j<nDim;j++){
	mcmrow.set(0,j,mc_m.get(i,j));
      } 
      _sb = mcmrow.dotT(mcmrow);
      sum += _sb.get(0,0);
    }
    sb = sum;    
    //cout << "SB = " << sum << endl;

    //Calculae SW
    sw =0;
    //closest.write();
    //data.write();
    //centers.write();
    for (int l=0;l<k;l++){
      float p = sumCluster.get(l,0)/(float)nData;  
      //cout << p << endl;
      sum =0;
      
      for (i=0;i<nData;i++){
	//cout << " l = " << l << endl;
	//cout << " i = " << i << endl;
	//cout << closest.get(i,0) << endl;
	if (closest.get(i,0)==l){
	  for (j=0;j<nDim;j++){
	    xj_mc.set(0,j,data.get(i,j)-centers.get(l,j));
	   }
	  //xj_mc.write();
	  sum += p * xj_mc.dotT(xj_mc).get(0,0);
	  //cout << sum << endl;
	}
      }
      sw += sum;
      //cout << "SW = " << sw << endl;
    }
    }
    sbswlast = sbsw;
    //if (sw == 0){
    //  sw = .000001;
    //}
    
    
    if (sw == 0){
      sbsw = 1000000;
      count = 11;
      cout << "New best" << endl;
      for (i=0;i<k;i++)
	for (j=0;j<nDim;j++){
	  bestCenters.set(i,j,centers.get(i,j));
	}
    
    }
    else{
    sbsw = sb / sw;
    cout << "quality: " << sbsw << endl;
    //cout << "quality" << qual << endl;
    //cout << "count" << count << endl;
    if (sbsw <= qual){
      cout << "Not new best #" << count << endl;
      count++;
    }
    else{
      count = 1;
      cout << "New best" << endl;
      qual = sbsw;
      for (i=0;i<k;i++)
	for (j=0;j<nDim;j++){
	  bestCenters.set(i,j,centers.get(i,j));
	}
    }
    }
    //cout << "quality: " << sbsw << endl;
    }while (count <= 3);
    cout << "Answer: ";
    bestCenters.write();
    cout << "Quality: " << qual << endl;
   
}

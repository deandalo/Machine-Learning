#include "mat.h"
#include <iostream>
#include <string>
#include <math.h>
#include "ktree.h"

using namespace std;

int count(Matrix data, int feature, int value){
  int num = 0;
  for (int i=0;i<data.rowSize();i++){
    if (data.get(i,feature) == value){
      num +=1;
    }
  }
  return num;
}

int isOneLabel(Matrix data, int feature){
  cout << "feature = " << feature;
  for (int i=0;i<=data.maxCol(feature);i++){
    if (data.countEqCol(feature,i)==data.rowSize()){
        cout << "Only one label is " << i;
	return i;
    }
  }
          return -1;
}

float maxAns(Matrix data){
  int maxCnt = 0;
  float maxValue = -1;
  for (int i=0;i<data.rowSize();i++){
    int cnt = count(data,data.colSize()-1,i);
    if (maxValue == -1 || cnt > maxCnt){
      maxCnt = cnt;
      maxValue = i;
      }
  }
  return maxValue;
}

float entropy(Matrix data, int feature){
  int total = data.rowSize();
  float sum = 0,p;
  for (int i=0;i<=data.maxCol(data.colSize()-1);i++){
    p = count(data,feature,i)/(float)total;
    if (p>0){
      sum += -p * log2(p);
    }
  }
  return sum;
}

float gain(Matrix data, int feature){
  int total = data.rowSize();
  float sum = 0,p;
  int i=0;
  for (i=0;i<data.colSize()-1;i++){
    p = count(data,feature,i)/(float)total;
    printf("p = %.4f",p);
    cout << endl;
    if (p>0){
      sum += p * entropy(data.subMatrixEq(feature,i),data.colSize()-1);
      cout << "E: " << entropy(data.subMatrixEq(feature,i),data.colSize()-1) << endl;
    }
  }
  return entropy(data,data.colSize()-1) - sum;
}

//Tree id3BuildTree(Matrix data, Matrix avFeat){
Tree id3BuildTree(Matrix data, Matrix avFeat,string inputData[][24],string features[]){
  //only one label for the Ans feature at this point?
  int val;
  int bestFeat = -1;
  //Tree treeLeaves;
  cout << "Esta es la matriz que entra a isOneLabel " << endl;
  data.write();
  avFeat.write();
  val = isOneLabel(data,data.colSize()-1);
  if (val != -1){
    cout << "Last label!!! = " << val <<  endl;
    //Tree treeLeaves(inputData[val][0]);
    //string lastFeat = inputData[(int)data.get(0,data.colSize()-1)][val];
    string lastFeat = inputData[(int)data.colSize()-1][val];
    cout << lastFeat;
    Tree treeLeaves(lastFeat);
    treeLeaves.printEdge();
    //Tree treeLeaves(features[val],inputData[0][val],new Tree (inputData[0][data.colSize()-1]));
    return treeLeaves; 
      }
  //ran out of discriminating features
  else if (avFeat.rowSize()==0){
    cout << "*** out of features ***" << endl;
    Tree treeLeaves(features[val],inputData[0][val],new Tree (inputData[0][data.colSize()-1]));
    return treeLeaves;
  }
  //pick maximum information gain
  else{
    float bestGain = 0;
    for (int i=0;i<avFeat.rowSize();i++){
      float g = gain(data,avFeat.get(i,0));
      cout << "Gain = " << g << endl;
      if (bestGain == 0 || g>bestGain){
	bestGain = g;
	bestFeat = avFeat.get(i,0);
      }
    }
    cout << "B: " << bestFeat << endl;
  }

  //recursively construct tree on return
  Tree treeLeaves(features[bestFeat]);
  cout << data.maxCol(bestFeat);
  for (int i=0;i<=data.maxCol(bestFeat);i++){
    cout << "Normal treeLeaves" << i << endl;
    treeLeaves.addChild(inputData[bestFeat][i],new Tree(id3BuildTree(data.subMatrixEq(bestFeat,avFeat.get(i,0)),avFeat,inputData,features)));
    //treeLeaves.addChild(inputData[bestFeat][i],new Tree (),inputData,features);
    //treeLeaves.addChild(inputData[bestFeat][i],new Tree (id3BuildTree(data.subMatrixEq(bestFeat,avFeat.get(i,0)),data.subMatrixNeq(bestFeat,avFeat.get(i,0)),inputData,features)));
    //treeLeaves.addChild(inputData[bestFeat][i], new Tree(id3BuildTree(data.subMatrixEq(bestFeat,avFeat.get(i,0)),avFeat.subMatrixNeq(0,bestFeat),inputData,features)));
    //treeLeaves.addChild((avFeat.get(i,0),id3BuildTree(data.subMatrixEq(bestFeat,avFeat.get(i,0)),data.subMatrixNeq(bestFeat,avFeat.get(i,0)),inputData,features)));
    }
  
  
  return treeLeaves;
}


int main(){
  int i,j,k, nFeat, nExam,val[24];
  string exam[24][24],featData[24][24];
  double l;
  string feat[24];

  cin >> nFeat;
  for (i=0;i<=nFeat;i++){
    cin >> feat[i] >> val[i];
    for (j=0;j<val[i];j++){
      cin >> featData[i][j];
    }
  }
  
  cin >> nExam;
  for (i=0;i<nExam;i++){
    for (j=0;j<=nFeat;j++){
      cin >> exam[i][j];
    }
  }
 
  Matrix avFeat = new Matrix(nFeat,1,"availableFeatures");
  avFeat.constantColRange(0,0,1);
  cout << "avFeat" << endl;
  avFeat.write();
  
  Matrix Data = new Matrix(nExam,nFeat+1,"dataToMatrix");
  for (i=0;i<nExam;i++)
    for (j=0;j<=nFeat;j++){
      for (k=0;k<val[j];k++){
	if (featData[j][k]==exam[i][j]){
	  l = (double)k;
	  Data.set(i,j,l);
	  }
      }
  }

  //Data.write();

  Tree t = id3BuildTree(Data,avFeat,featData,feat);
  //Tree t = id3BuildTree(Data,avFeat);
  t.printEdge();
  //int c = count(Data,0,0);
  //float d = maxAns(Data);
  //cout << "Count " << c << endl;
  //int c = test(Data);
  //cout << "Count de test" << c << endl; 
    
}






  




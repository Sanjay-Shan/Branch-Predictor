#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>  //for string related operations
#include <typeinfo> //to check the type of the variable in c++
#include <sstream>  
#include <bitset>
#include <bits/stdc++.h>
#include <math.h>
using namespace std;

//utility functions
int to_decimal(string index) {
  int ind= std::stoi(index,nullptr,2); // index in decimal format
  return ind;
}

string Hex2Bin(string hexadecimal,int bits){ //convert string to hex to binary
string bin;
stringstream ss(hexadecimal); //convert string to stringstream
ss<< hex << hexadecimal; //stream to hex
unsigned n;
ss >> n;
if (bits==24){
bitset<24> b(n);
bin= b.to_string();}
return bin; // 24 bit binary
} 

string shift_register(string branch,string ghr){
  string b;
  if (branch=="t"){b="1";}
  else {b="0";}
  string Ghr=b+ghr; //updating GHR
  Ghr=Ghr.substr(0,Ghr.length()-1);
  return Ghr;
}

string Ind2Slice(int m,string bin ){
string temp=bin.substr(bin.length()-1-(m+1),bin.length()); //index
string index=temp.substr(0,temp.length()-2);
return index;
} 

string stringxor(string GHR, string n_bits){
  string Xor="";
  for(int i=0;i<GHR.length();i++){
     if (GHR[i]==n_bits[i]){
        Xor+="0";
     }
     else{
        Xor+="1";
     }
  }
  return Xor;
}


int main(int argc, char** argv) // setting up the command line arguments for the cache simulator
{
  string Trace,Branch_Pred,GHR;
  int m ,n,smith_counter,bimodal[(int)(pow(2, std::stoi(argv[2])))],gshare[(int)(pow(2, std::stoi(argv[2])))];
  if (argc==4){     //check the number of bits in the command line for n=0 and n>0 cases
    Branch_Pred=argv[1];
    m=std::stoi(argv[2]);
    Trace=argv[3];
    if (Branch_Pred=="smith"){
        smith_counter= (int)(pow(2, m))/2; //initializing the smith counter 
        }
    if (Branch_Pred=="bimodal"){
        for(int i=0;i<((int)(pow(2, m)));i++){ //initializing the bimodal branch prediction table 
          bimodal[i]=(int)(pow(2, 3))/2;
        }
    }
  }

  if (argc==5){
    Branch_Pred=argv[1];
    m=std::stoi(argv[2]);
    n=std::stoi(argv[3]);
    Trace=argv[4];
    GHR="0";
    if (GHR.length() < n){
      GHR.insert (0,n-GHR.length(), '0');}
    for(int i=0;i<((int)(pow(2, m)));i++){ //initializing the gshare branch prediction table 
          gshare[i]=(int)(pow(2, 3))/2;
  }
  }

  ifstream MyReadFile(Trace); // reading the trace file 

  string T,branch,Hex,Pred,Line;
  vector<string> data;
  int predictions=0,mispredictions=0;

while (getline (MyReadFile, Line)) { //read a line in a file 
    stringstream X(Line);              //convert it to a stringstream
    while (getline(X, T, ' ')) {        // split the string into 2 based on the delimiter
       
       data.push_back(T);} 
    Hex=data[0];
    branch=data[1];
    data.clear(); //clear the vector
    predictions+=1;

    //check for one of the predictors
    if (Branch_Pred=="smith"){
       if (smith_counter >= (int)(pow(2, m))/2){
         Pred="t";
       }
       else if (smith_counter < (int)(pow(2, m))/2){
         Pred="n";
       }
      if (Pred!=branch){
         mispredictions+=1;
      }
       if ((Pred=="t" && branch=="t")||(Pred=="n" && branch=="t")){
         smith_counter+=1;
         if (smith_counter> (int)(pow(2, m))-1){
         smith_counter-=1;
       }
       else if (smith_counter<0){
         smith_counter=0;
       }
       }
       else if ((Pred=="n" && branch=="n")||(Pred=="t" && branch=="n")){
         smith_counter-=1;
         if (smith_counter> (int)(pow(2, m))-1){
         smith_counter-=1;
       }
       else if (smith_counter<0){
         smith_counter=0;
       }
       } 
    }

    else if (Branch_Pred=="bimodal"){
     string binary=Hex2Bin(Hex,24);
     string index=Ind2Slice(m,binary);
     int dec_ind=to_decimal(index);
     if (dec_ind>(int)(pow(2, m))-1){
       cout<<"wrong value"<<dec_ind<<endl;
     }
     if (bimodal[dec_ind]>=(int)(pow(2, 3))/2){
        Pred="t";
     }
     else{
        Pred="n";
     }
     if (Pred!=branch){
         mispredictions+=1;
      }
     if ((Pred=="t" && branch=="t")||(Pred=="n" && branch=="t")){
         bimodal[dec_ind]+=1;
         if (bimodal[dec_ind]> (int)(pow(2, 3))-1){ //setting m to 3 , as the counter is 3 bit wide ,in case of bimodal 
         bimodal[dec_ind]-=1;
       }
       else if (bimodal[dec_ind]<0){
         bimodal[dec_ind]=0;
       }
       }
       else if ((Pred=="n" && branch=="n")||(Pred=="t" && branch=="n")){
         bimodal[dec_ind]-=1;
         if (bimodal[dec_ind]> (int)(pow(2, 3))-1){ //setting m to 3 , as the counter is 3 bit wide ,in case of bimodal 
            bimodal[dec_ind]-=1;
          }
       else if (bimodal[dec_ind]<0){
         bimodal[dec_ind]=0;
       }
       }
    }
    else if (Branch_Pred=="gshare"){
      string binary=Hex2Bin(Hex,24);
      string m_bits=Ind2Slice(m,binary); //gets the m bits just as in the bimodal i.e. excluding the last 2 bits of the PC
      string m_n_bits= m_bits.substr(0,m-n); //gets the m-n bits from the m bits of the PC
      string n_bits=Ind2Slice(n,binary); //gets the n bits from the PC
      string Xor=stringxor(GHR,n_bits);//xor between n bit global history register and n_bits
      int index = to_decimal(m_n_bits+Xor);
      // cout<<binary<<"\t"<<m_bits<<"\t"<<m_n_bits<<"\t"<<n_bits<<"\t"<<GHR<<"\t"<<Xor<<"\t"<<index<<"\t"<<GHR<<endl;
      if (gshare[index]>=(int)(pow(2, 3))/2){
        Pred="t";
     }
     else{
        Pred="n";
     }
     if (Pred!=branch){
         mispredictions+=1;
      }
     if ((Pred=="t" && branch=="t")||(Pred=="n" && branch=="t")){
         gshare[index]+=1;
         if (gshare[index]> (int)(pow(2, 3))-1){ //setting m to 3 , as the counter is 3 bit wide ,in case of bimodal 
         gshare[index]-=1;
       }
       else if (gshare[index]<0){
         gshare[index]=0;
       }
       }
       else if ((Pred=="n" && branch=="n")||(Pred=="t" && branch=="n")){
         gshare[index]-=1;
         if (gshare[index]> (int)(pow(2, 3))-1){ //setting m to 3 , as the counter is 3 bit wide ,in case of bimodal 
            gshare[index]-=1;
          }
       else if (gshare[index]<0){
         gshare[index]=0;
       }
       }
       GHR=shift_register(branch,GHR);
      //  cout<<GHR<<endl;
    }
}
  // printing the output as per the given format
    cout<<"COMMAND"<<endl;
    for(int i=0;i<argc;i++){
      cout<<argv[i]<<"\t";
    }
    cout<<endl;
    cout<<"OUTPUT"<<endl;
    cout<<"number of predictions:"<<"\t"<<predictions<<endl;
    cout<<"number of mispredictions:"<<"\t"<<mispredictions<<endl;
    float misprediction_rate= (mispredictions/float(predictions))*100;
    cout<<"misprediction rate:"<<"\t"<<fixed<<setprecision(2)<<misprediction_rate<<"%"<<endl;

    if (Branch_Pred=="smith"){
      cout<<"FINAL COUNTER CONTENT:"<<"\t"<<smith_counter<<endl;}

    else if (Branch_Pred=="bimodal"){
      cout<<"FINAL BIMODAL CONTENTS"<<endl;
    for(int i=0;i<((int)(pow(2, m)));i++){ //print the initial values of the bimodal branch prediction table 
          cout<<i<<"\t"<<bimodal[i]<<endl;
        }
    }
    else if (Branch_Pred=="gshare"){
      cout<<"FINAL GSHARE CONTENTS"<<endl;
      for(int i=0;i<((int)(pow(2, m)));i++){ //print the initial values of the bimodal branch prediction table 
          cout<<i<<"\t"<<gshare[i]<<endl;
        }
    }

    

return 0;
}



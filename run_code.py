import sys
import os
import subprocess
import matplotlib.pyplot as plt

#plots for smith counter ,totally 3 plots for 3 benchmarks with 6 datapoints in each of the plot
smith={}
temp=[]
for i in ["gcc","perl","jpeg"]:
    print(i)
    for j in range(1,7):
        x=os.popen("./a.out smith"+"\t"+str(j)+"\t"+"traces/"+str(i)+"_trace.txt").read()
        # x=os.popen("./a.out bimodal 4 traces/jpeg_trace.txt").read()
        l=x.split("\n")
        m=l[5]
        temp.append(float(m[-6:-1]))
    smith[i]=temp
    temp=[]

# #plot for each benchmark
x=[i for i in range(1,7)]
for key,values in smith.items():
    plt.figure()
    plt.plot(x, values, label = "smith Branch Predictor")
    plt.xlabel("M bits")
    plt.ylabel("branch misprediction_rate")
    plt.legend()
    plt.title("Smith Branch Predictor with "+key+" benchnark")
    plt.savefig("Sho_smith/smith_"+key+".png")
    
#plots for bimodal predictor ,totally 3 plots for 3 benchmarks with 6 datapoints in each of the plot
bimodal={}
temp=[]
for i in ["gcc","perl","jpeg"]:
    print(i)
    for j in range(7,13):
        x=os.popen("./a.out bimodal"+"\t"+str(j)+"\t"+"traces/"+str(i)+"_trace.txt").read()
        l=x.split("\n")
        m=l[5]
        temp.append(float(m[-6:-1]))
    bimodal[i]=temp
    temp=[]

#plot for each benchmark
x=[i for i in range(7,13)]
for key,values in bimodal.items():
    plt.figure()
    plt.plot(x, values, label = "Bimodal Branch Predictor")
    plt.xlabel("M bits")
    plt.ylabel("branch misprediction_rate")
    plt.legend()
    plt.title("Bimodal Branch Predictor with "+key+" benchnark")
    plt.savefig("Sho_bimodal/bimodal_"+key+".png")

#plots for gshare predictor ,totally 3 plots for 3 benchmarks with 6 datapoints in each of the plot
temp=[]
Gshare={}
m_val={}
temp2=[]

for i in ["gcc","perl","jpeg"]:
    for j in range(2,13,2): #value of n
        for k in range(7,13): #value of m
            if j<=k:
                temp2.append(k)
                x=os.popen("./a.out gshare"+"\t"+str(k)+"\t"+str(j)+"\t"+"traces/"+str(i)+"_trace.txt").read()
                l=x.split("\n")
                m=l[5]
                print(j,k,m,m[-6:-1])
                temp.append(float(m[-6:-1]))
        Gshare[j]=temp
        m_val[j]=temp2
        temp=[]
        temp2=[]
    
    plt.figure()
    plt.plot(m_val[2], Gshare[2], label = "n=2")
    plt.plot(m_val[4], Gshare[4], label = "n=4")
    plt.plot(m_val[6], Gshare[6], label = "n=6")
    plt.plot(m_val[8], Gshare[8], label = "n=8")
    plt.plot(m_val[10], Gshare[10], label = "n=10")
    plt.plot(m_val[12], Gshare[12], label = "n=12")
    plt.xlabel("M bits")
    plt.ylabel("misprediction_rate")
    plt.legend()
    plt.title("<"+i+"_trace.txt>  gshare")
    plt.savefig("nish/gshare_"+i+".png")
    Gshare={}
    m_val={}








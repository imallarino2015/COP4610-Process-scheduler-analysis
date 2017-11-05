#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

#include<vector>

#include"Process.h"

using namespace std;

//supplementary for quicksort
int partition(vector<Process>& vec,int first,int last){
    int i=first-1;

    for(int a=first;a<=last;a++)
        if(vec[a].procNo<=vec[last].procNo){
            i++;
            exchange(vec[a],vec[i]);
        }
    return i;
}

//sorts processes by their process number using quicksort
void sortProcesses(vector<Process>& vec,int first=0,int last=-1){
    if(last==-1)
        last=vec.size()-1;
    if(first<last){
        int div=partition(vec,first,last);
        sortProcesses(vec,first,div-1);
        sortProcesses(vec,div+1,last);
    }
}

//return whether or not a program exists on the ready queue(s)
bool isReady(vector<vector<Process> > ready){
    for(unsigned a=0;a<ready.size();a++)
        if(ready[a].size())
            return true;
    return false;
}

//select a program to run from the ready queues from highest to lowest priority
bool run(vector<vector<Process> >& ready,vector<Process>& run,int time){
    for(unsigned a=0;a<ready.size();a++)
        if(ready[a].size()){
            transfer(ready[a],run);
            if(!run[0].activated){  //check if the program has responded before
                run[0].response=time-run[0].start;  //record the response time after the first burst starts
                run[0].activated=true;  //flag the program as having been started
            }
            return true;
        }
    return false;
}

//display the processes and their current bursts contained within a given list
void listProcesses(vector<Process> pList){
    for(unsigned a=0;a<pList.size();a++){
        cout<<"P"<<pList[a].procNo<<": "<<pList[a].getBurst();
        if(a<pList.size()-1)
            cout<<", ";
        else
            cout<<'\n';
    }
}

//take a sample of the current activities of all the processes and current time
void sampleProcesses(vector<Process> run,vector<Process> io,vector<vector<Process> > waiting,int time){
    cout<<"Current time: "<<time<<endl;
    if(run.size()){
        cout<<"Running:"<<endl;
        listProcesses(run);
    }
    if(io.size()){
        cout<<"Waiting on IO:"<<endl;
        listProcesses(io);
    }
    for(unsigned a=0;a<waiting.size();a++)
        if(waiting[a].size()){
            cout<<"Ready queue "<<a+1<<":"<<endl;
            listProcesses(waiting[a]);
        }
    cout<<"----------------------------"<<endl;
}

//prints the values of the desired times, their averages, and the utilization of the CPU for the set of processes given
void printResults(vector<Process> pList,int totalTime,int CPUTime){
    sortProcesses(pList);

    cout<<endl<<"Results:"<<endl;
    cout<<"CPU utilization: "<<100.0*CPUTime/totalTime<<"%"<<endl;
    cout<<"Process"<<"\t"
        <<"Tw"<<"\t"
        <<"Ttr"<<"\t"
        <<"Tr"<<endl;
    double waitingSum=0,turnAroundSum=0,responseSum=0;
    for(unsigned a=0;a<pList.size();a++){
        cout<<pList[a].procNo<<"\t"
            <<pList[a].waiting<<"\t"
            <<pList[a].turnAround<<"\t"
            <<pList[a].response<<"\t"<<endl;
        waitingSum+=pList[a].waiting;
        turnAroundSum+=pList[a].turnAround;
        responseSum+=pList[a].response;
    }
    cout<<"Avg:"<<"\t"
        <<waitingSum/pList.size()<<"\t"
        <<turnAroundSum/pList.size()<<"\t"
        <<responseSum/pList.size()<<endl;
    cout<<"Total time: "<<totalTime<<endl<<endl;
}

void printResults(vector<Process> pList){
    int CPUTime=0,totalTime=0;
    for(unsigned a=0;a<pList.size();a++){
        if(totalTime<pList[a].turnAround)
            totalTime=pList[a].turnAround;
        CPUTime+=pList[a].cpuTotal;
    }

    printResults(pList,totalTime,CPUTime);
}

//starts processes at the time they are set to start
void loadReady(vector<Process>& starting,vector<Process>& ready,int time){
    for(unsigned a=0;a<starting.size();)
        if(time>=starting[a].start)
            transfer(starting,ready,a);
        else
            a++;
}

//increases the process' waiting times
void updateReady(vector<vector<Process> >& ready,int time=1){
    for(unsigned a=0;a<ready.size();a++)
        for(unsigned b=0;b<ready[a].size();b++)
            ready[a][b].waiting+=time;
}

//updates the list of processes waiting on io and puts them back on the ready list when they are done
void updateIO(vector<Process>& io,vector<vector<Process> >& ready,int time=1){
    for(unsigned a=0;a<io.size();a++){
        if(time>io[a].getBurst()-io[a].currentTime){
            time-=io[a].getBurst()-io[a].currentTime;
            io[a].currentTime=io[a].getBurst();
            io[a].waiting+=time;
        }else
            io[a].currentTime+=time;
        if(io[a].isFinished()){
            io[a].nextBurst();
            transfer(io,ready[io[a].currentQueue],a);
        }
    }
}

//returns the index of the processor with the shortest burst in a list of processes
int getShortest(vector<Process> p){
    int shortest=0;
    for(unsigned a=1;a<p.size();a++)
        if(p[shortest].getBurst()>p[a].getBurst())
            shortest=a;
    return shortest;
}

#endif // FUNCS_H_INCLUDED
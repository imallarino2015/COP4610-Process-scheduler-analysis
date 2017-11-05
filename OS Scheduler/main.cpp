/**********************************************************************************
Ian Mallarino
z23369342
COP4610 - programming project
Simulation, analysis and comparison of three process scheduling algorithms:
    First Come First Served(FCFS)
    Shortest Job First(SJF)
    Multi-level Feedback Queue(MLFQ)
Collects and displays information relevant to their efficiencies such as
    turn around times, waiting times, and response times of each process
    as well as CPU utilization for any given set of processes' io and cpu
    bursts. Also displays process states at context switches.
**********************************************************************************/

#include<iostream>
#include<vector>    //container for processes and their bursts

#include"Process.h"
#include"scheduler.h"

using namespace std;

int main(){
    vector<Process> refList={   //Processes in their initial states for reference only; Do not change.
        Process({4,24,5,73,3,31,5,27,4,33,6,43,4,64,5,19,2}),
        Process({18,31,19,35,11,42,18,43,19,47,18,43,17,51,19,32,10}),
        Process({6,18,4,21,7,19,4,16,5,29,7,21,8,22,6,24,5}),
        Process({17,42,19,55,20,54,17,52,15,67,12,72,15,66,14}),
        Process({5,81,4,82,5,71,3,61,5,62,4,51,3,77,4,61,3,42,5}),
        Process({10,35,12,41,14,33,11,32,15,41,13,29,11}),
        Process({21,51,23,53,24,61,22,31,21,43,20}),
        Process({11,52,14,42,15,31,17,21,16,43,12,31,13,32,15})
    };
    vector<int> TQ={6,11,-1};

    for(unsigned a=0;a<refList.size();a++)  //assign each process its own identifier to be recalled later
        refList[a].procNo=a+1;

    printResults(FCFS(refList,true));
    printResults(SJF(refList,true));
    printResults(MLFQ(refList,TQ,true));

    return 0;
}
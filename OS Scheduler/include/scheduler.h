#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

#include"generic.h"
#include"funcs.h"

/***************************************************
First Come First Served
Executes each process as they become available to
    completion.
***************************************************/
vector<Process> FCFS(vector<Process> newList,bool print=false){
    vector<Process> running,IOList,terminatedList;  //each of the lists needed in the simulation
    vector<vector<Process> > readyList; //ready list for the multiple ready queues there will be with MLFQ
    readyList.resize(1);
    int time=0,CPUTime=0;

    if(print)
        cout<<"FCFS:"<<endl;

    while(newList.size()||running.size()||IOList.size()||isReady(readyList)){   //check that any of the queues contain processes
        updateReady(readyList);
        updateIO(IOList,readyList);
        loadReady(newList,readyList[0],time);   //load processes on to the ready queue as they reach their starting times

        if(running.empty()) //check if there are any programs currently running
            if(run(readyList,running,time)&&print) //check and pull a process from the ready queue
                sampleProcesses(running,IOList,readyList,time);

        if(running.size()){ //if a program is running
            CPUTime++;  //tick up the time for how long the cpu is being used
            running[0].currentTime++;   //tick down the cpu burst
            if(running[0].isFinished()){   //check if the cpu burst is completed
                running[0].nextBurst(); //move to next burst
                if(running[0].currentBurst==running[0].bursts.size()){  //check if all bursts are finished
                    running[0].turnAround=time-running[0].start;    //record the turnaround time once the program terminates
                    transfer(running,terminatedList);   //terminate the program
                    if(print)
                        cout<<"Program terminated."<<endl;
                }else
                    transfer(running,IOList);   //send the program to wait on io
            }
        }
        time++; //tick up the overall time
    }
    return terminatedList;
}

/***************************************************
Shortest Job First
Chooses the process with the shortest cpu burst to
    be run if any are available.
***************************************************/
//Modified version of the FCFS algorithm that changes the order processes are chosen to run based on burst size
vector<Process> SJF(vector<Process> newList,bool print=false){
    vector<Process> running,IOList,terminatedList;
    vector<vector<Process> > readyList;
    readyList.resize(1);
    int time=0,CPUTime=0;

    if(print)
        cout<<"SJF:"<<endl;

    while(newList.size()||running.size()||IOList.size()||isReady(readyList)){
        updateReady(readyList);
        updateIO(IOList,readyList);
        loadReady(newList,readyList[0],time);

        if(running.empty())
            if(readyList[0].size()){
                transfer(readyList[0],running,getShortest(readyList[0]));   //chose the lowest burst on the ready queue
                if(!running[0].activated){  //check if the program has responded before
                    running[0].response=time-running[0].start;  //record the response time after the first burst starts
                    running[0].activated=true;  //flag the program as having been started
                }
                if(print)
                    sampleProcesses(running,IOList,readyList,time);
            }

        if(running.size()){
            CPUTime++;
            running[0].currentTime++;
            if(running[0].isFinished()){
                running[0].nextBurst();
                if(running[0].currentBurst==running[0].bursts.size()){
                    running[0].turnAround=time-running[0].start;
                    transfer(running,terminatedList);
                    if(print)
                        cout<<"Program terminated."<<endl;
                }else
                    transfer(running,IOList);
            }
        }
        time++;
    }
    return terminatedList;
}

/***************************************************
Multi-level Feedback Queue
Sets up a time quantum for the first n-1 queues. If
    a process exceeds those time quantums it is sent
    to a lower priority queue. If a process exists on
    a higher priority queue the program will always
    choose a process from that queue. Once a process
    loses priority, it cannot be restored.
***************************************************/
//Modified version of FCFS which adds a time quantum and multiple ready queues for processes that are too long
vector<Process> MLFQ(vector<Process> newList,vector<int> TQ,bool print=false){
    vector<Process> running,IOList,terminatedList;
    vector<vector<Process> > readyList;
    readyList.resize(TQ.size());   //initialize the next two queues
    int time=0,CPUTime=0;
    int tqRemain;   //remaining time for current process

    if(print)
        cout<<"MLFQ:"<<endl;

    while(newList.size()||running.size()||IOList.size()||isReady(readyList)){
        updateReady(readyList);
        updateIO(IOList,readyList);
        loadReady(newList,readyList[0],time);

        if(running.empty())
            if(run(readyList,running,time)){
                tqRemain=TQ[running[0].currentQueue];
                if(print)
                    sampleProcesses(running,IOList,readyList,time);
            }

        if(running.size()){
            CPUTime++;
            running[0].currentTime++;
            if(running[0].isFinished()){
                running[0].nextBurst();
                if(running[0].currentBurst==running[0].bursts.size()){
                    running[0].turnAround=time-running[0].start;
                    transfer(running,terminatedList);
                    if(print)
                        cout<<"Program terminated."<<endl;
                }else
                    transfer(running,IOList);
                if(run(readyList,running,time)){
                    tqRemain=TQ[running[0].currentQueue];
                    if(print)
                        sampleProcesses(running,IOList,readyList,time);
                }
            }

            //checks if the time quantum has run out and downgrades the process to the next queue if applicable
            tqRemain--; //ticks down the remaining time quantum
            if(tqRemain==0){    //if the program is still running but the time quantum has run out
                running[0].currentQueue++;  //increase the program's queue
                transfer(running,readyList[running[0].currentQueue]);   //send the program to that queue
            }
        }
        time++;
    }
    return terminatedList;
}

#endif // SCHEDULER_H_INCLUDED
#include "Process.h"

Process::Process(){
    reset();
}

Process::Process(const Process& p){
    procNo=p.procNo;

    start=p.start;
    turnAround=p.turnAround;
    waiting=p.waiting;
    response=p.response;

    currentTime=p.currentTime;
    currentBurst=p.currentBurst;
    currentQueue=p.currentQueue;

    activated=p.activated;

    bursts=p.bursts;
    cpuTotal=p.cpuTotal;
    ioTotal=p.ioTotal;
}

Process::Process(std::vector<int> burstList,int startTime){
    reset();
    *this=burstList;
    start=startTime;
}

void Process::operator=(const Process p){
    procNo=p.procNo;

    start=p.start;
    turnAround=p.turnAround;
    waiting=p.waiting;
    response=p.response;

    currentTime=p.currentTime;
    currentBurst=p.currentBurst;
    currentQueue=p.currentQueue;

    activated=p.activated;

    bursts=p.bursts;

    cpuTotal=p.cpuTotal;
    ioTotal=p.ioTotal;
}

void Process::operator=(std::vector<int> newBursts){
    bursts=newBursts;
    cpuTotal=0;
    ioTotal=0;
    for(unsigned a=0;a<bursts.size()-1;a+=2){
        cpuTotal+=bursts[a];
        ioTotal+=bursts[a+1];
    }
    cpuTotal+=bursts[bursts.size()-1];
}

int Process::getCPU(int index){
    return bursts[2*index];
}

int Process::getIO(int index){
    return bursts[2*index+1];
}

int Process::getBurst(){
    return bursts[currentBurst];
}

void Process::nextBurst(){
    currentBurst++;
    currentTime=0;
}

void Process::addBurst(int io,int cpu){
    bursts.push_back(io);
    bursts.push_back(cpu);
    cpuTotal+=cpu;
    ioTotal+=io;
}

void Process::reset(){
    procNo=0;

    start=0;
    turnAround=0;
    waiting=0;
    response=-1;

    currentTime=0;
    currentBurst=0;
    currentQueue=0;

    activated=false;

    bursts.clear();

    cpuTotal=0;
    ioTotal=0;
}

bool Process::isFinished(){
    return getBurst()==currentTime;
}

Process::~Process(){
    //dtor
}
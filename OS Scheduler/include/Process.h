#ifndef PROCESS_H
#define PROCESS_H

#include<vector>

class Process{
    public:
        Process();
        Process(const Process&);
        Process(std::vector<int>,int=0);
        virtual ~Process();

        int procNo;

        int start;  //starting time
        int turnAround; //turnaround time
        int waiting;    //waiting time
        int response;   //response time

        int cpuTotal;   //cumulative cpu time
        int ioTotal;    //cumulative io time

        int currentTime;    //current burst timer
        unsigned currentBurst;   //counter for the cpu/io bursts
        unsigned currentQueue;   //for mlfq; decides the feedback queue it belongs in

        bool activated; //flag for response time

        std::vector<int> bursts;    //list of cpu/io bursts

        void operator=(Process);    //copys details of another process
        void operator=(std::vector<int>);   //sets bursts to new set of times

        int getCPU(int);    //returns the value of the nth CPU burst
        int getIO(int); //returns the value of the nth IO burst
        int getBurst(); //returns the value of the current burst

        void nextBurst();   //resets the current time and increments the burst counter
        void addBurst(int,int); //adds io and cpu bursts to set of bursts
        void reset();   //deletes the bursts and sets everything to zero

        bool isFinished();  //checks if current burst is finished
};

#endif // PROCESS_H
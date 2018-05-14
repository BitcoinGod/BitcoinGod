#include "pos/posminemgr.h"
#include "pos/posminer.h"
#include "wallet/wallet.h"
#include "pos/poswallet.h"
#include "ui_interface.h"
#include "util.h"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

namespace pos{
PosMineStatus PosMineMgr::_status=PosMineStatus::OFF;

class MYCStoppable{
    std::promise<void> promiseObj;
    std::future<void> futureObj;
    protected:
        bool isStop(){
            if(futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout){
                return false;
            }
            return true;
        }


    public:
        MYCStoppable():futureObj(promiseObj.get_future()){
        };

        MYCStoppable(MYCStoppable && obj):promiseObj(std::move(obj.promiseObj)),futureObj(std::move(obj.futureObj)){};

        MYCStoppable& operator=(MYCStoppable &&obj){
            promiseObj = std::move(obj.promiseObj);
            futureObj = std::move(obj.futureObj);
            return *this;
        };

        void operator()(){
            run();
        };

        void stop(){
            promiseObj.set_value();
        };

        virtual void run() = 0;
};

class MineTask:public MYCStoppable{
    public:
        void run();
};

static const int MINE_INTERV=3;
void MineTask::run(){
    while(!isStop() && !vpwallets.empty() ){
        //check money available
        CWallet *pwallet = vpwallets[0];
        if(pwallet->IsLocked()){
            //emit mine lock signal
            uiInterface.MineStatusChangeSignal(PosMineStatus::LOCK);
            PosMineMgr::setStatus(PosMineStatus::LOCK);
            std::this_thread::sleep_for(std::chrono::seconds(MINE_INTERV));
            continue;
        }
        
        CAmount nBalance = pwallet->GetBalance();
        if(nBalance <= 0){
            //emit mine nomeny signal
            uiInterface.MineStatusChangeSignal(PosMineStatus::NOMONEY);
            PosMineMgr::setStatus(PosMineStatus::NOMONEY);
            std::this_thread::sleep_for(std::chrono::seconds(MINE_INTERV));
            continue;
        }

        //emit mine start signal
        uiInterface.MineStatusChangeSignal(PosMineStatus::ON); 
        PosMineMgr::setStatus(PosMineStatus::ON);
        
        //start mining
        minePosBlock(pwallet);

        //sleep for seconds
        std::this_thread::sleep_for(std::chrono::seconds(MINE_INTERV));
    };
};


static MineTask task;
static std::thread mineThread;
//here is the interface

RetCode PosMineMgr::stopMine(){
    task.stop();
    if(mineThread.joinable()){
        mineThread.join();
    }
    return RetCode::OK;
};

RetCode PosMineMgr::startMine(){
    if(vpwallets.size()==0){
        return RetCode::ERR;
    }

    mineThread = std::thread([&](){
                task.run();
            });
    return RetCode::OK;
};
};

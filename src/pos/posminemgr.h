#ifndef POS_MINER_MGRH
#define POS_MINER_MGRH
#include <boost/signals2/signal.hpp>
namespace pos{

enum PosMineStatus{
    ON=0x01,
    OFF=0x02,
    NOMONEY=0x03,
    LOCK=0x04
};

enum RetCode{
    OK=0,
    ERR=0x01
};
    
class PosMineMgr{
    private:
        static PosMineStatus _status;
    public:
        static PosMineStatus getStatus(){return _status;};
        static void setStatus(PosMineStatus status){_status = status;};
        static RetCode stopMine();
        static RetCode startMine();
};


};

#endif

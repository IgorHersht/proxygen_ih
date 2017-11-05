
#ifndef FIX_TSI_H
#define FIX_TSI_H

#include <map>
#include <vector>

#include "queue_impl.h"
#include "buffer.h"
#include "object_pool.h"
#include "logger.h"
#include "ts_settings.h"
#include "logger.h"

namespace other_fix {

template <typename MessageQueueT> struct FixTsRouter{
    typedef boost::shared_ptr<FixTsRouter> FixTsRouterPtr;
    typedef ObjectPool<Buffer> PoolT;

explicit FixTsRouter( TradingSystemId id, size_t init_size, size_t mlp):
    m_id(id), m_init_size(init_size), m_mlp(mlp), m_ts_queue(init_size * mlp),m_pool(init_size, mlp){
 }

size_t getMaxSize() const{ return m_init_size * m_mlp;}

TradingSystemId getTradingSystemId() const {return m_id ;}

bool allocate(BufferPtr &buff){
   return m_pool.allocate( buff);
}

bool delocate(BufferPtr &buff){
    return m_pool.delocate( buff);
}

bool tryProduce( BufferPtr &buff){ 
    return m_ts_queue.tryPush(buff);
}

void produce( BufferPtr &buff){ 
    m_ts_queue.push(buff);
}

bool tryConsume(BufferPtr &buff){
    return m_ts_queue.tryPop(buff);
}

void consume(BufferPtr &buff){
    m_ts_queue.pop(buff);
}

private:
   TradingSystemId      m_id;
   size_t               m_init_size;
   size_t               m_mlp;
   MessageQueueT        m_ts_queue;
   PoolT                m_pool;
  
};

template <typename FixTsRouterT >struct  FixTsRouters{

    typedef boost::shared_ptr<FixTsRouters> FixTsRoutersPtr;
    typedef typename FixTsRouterT::FixTsRouterPtr FixTsRouterPtr;
    enum{ DEFAULT_INIT_SIZE = 1024, DEFAULT_MULIPLIER = 4 };
    typedef std::map<TradingSystemId, FixTsRouterPtr > RoutersT;

explicit FixTsRouters(const TradingSystemsId &ids, size_t init_size = DEFAULT_INIT_SIZE, size_t mlp = DEFAULT_MULIPLIER){
    for(TradingSystemsId::const_iterator pos = ids.begin(), end = ids.end(); pos != end; ++pos) {
         m_routers.insert(typename RoutersT::value_type(*pos, FixTsRouterPtr(new  FixTsRouterT(*pos, init_size, mlp) ) ));
    }  
 }

bool allocate(TradingSystemId id, BufferPtr &buff){
    FixTsRouterPtr router = find(id);
    if(!router) return false;
    router->allocate( buff);
    return true;
}

bool delocate(TradingSystemId id, BufferPtr &buff){
     FixTsRouterPtr router = find(id);
     if(!router) return false;
    router->delocate( buff);
    return true;
}

bool produce(TradingSystemId id, BufferPtr &buff){
    FixTsRouterPtr router = find(id);
     if(!router) return false;
    router-> produce(buff);
    return true;
}

bool consume(TradingSystemId id, BufferPtr &buff){
    FixTsRouterPtr router = find(id);
     if(!router) return false;
    router-> consume(buff);
    return true;
}

typename FixTsRouterT::FixTsRouterPtr find(TradingSystemId id){
    typename RoutersT::iterator pos = m_routers.find(id);
    if(pos != m_routers.end()) {
        return pos->second;
    }

    warningxxx("Cannot find Ts  by id. ");
    return typename FixTsRouterT::FixTsRouterPtr();
}

private:
    RoutersT  m_routers; 
};


typedef MsgContainer<BufferPtr, queue_impl:: FIFO_ML_R_ML_W, queue_impl::MIXED_BLOKING>   FixToTsMessageQueueT;
typedef FixTsRouter< FixToTsMessageQueueT > FixToTsRouter;
typedef FixTsRouters<FixToTsRouter> FixToTsRouters;

typedef MsgContainer<BufferPtr, queue_impl:: FIFO_ONE_R_ONE_W, queue_impl::MIXED_BLOKING>   TSToFixMessageQueueT;
typedef FixTsRouter< TSToFixMessageQueueT > TSToFixRouter;
typedef FixTsRouters<TSToFixRouter> TSToFixRouters;

}  //namespace other_fix

#endif

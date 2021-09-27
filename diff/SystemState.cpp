#include        <cstdint>
#include        <atomic>
#include        <tuple>
#include        <memory>


struct SystemState{
    
  using impl_type = std::uint64_t; 
 enum  StateTypeId {CONFIG, LOG};
 template <StateTypeId id> struct StateType;
 using value_type = std::tuple< StateType<CONFIG>, StateType<LOG> >;
 using ptr_type = std::shared_ptr<value_type>;

 SystemState();
 template <StateTypeId id> typename StateType<id>::State Load()const noexcept;
 template <StateTypeId id> void Store(typename StateType<id>::State state ) noexcept;
 
 template <StateTypeId id> bool IsSet(typename StateType<id>::State state )const noexcept;
 template <StateTypeId id> void Set(typename StateType<id>::State state ) noexcept;
 template <StateTypeId id> void UnSet(typename StateType<id>::State state ) noexcept;

private:
  ptr_type m_ptr;
};

 template <> struct SystemState::StateType<SystemState::CONFIG>{
     enum State : SystemState::impl_type {INIT = 0x1  };
     std::atomic<State> m_value {(State)0 } ;
 };
 template <> struct  SystemState::StateType<SystemState::LOG>{
           enum State : SystemState::impl_type {INIT = 0x1  };
     std::atomic<State> m_value {(State)0 } ;
 };

inline SystemState::SystemState():m_ptr(std::make_shared<value_type>()  ) {}
 
template <SystemState::StateTypeId id> inline typename SystemState::StateType<id>::State SystemState::Load()const noexcept{
     return (std::get<id>(*m_ptr)).m_value.load();
 }

template <SystemState::StateTypeId id> inline void SystemState::Store(typename SystemState::StateType<id>::State state ) noexcept{
    (std::get<id>(*m_ptr)).m_value.store(state);
}

template <SystemState::StateTypeId id> inline bool SystemState::IsSet(typename SystemState::StateType<id>::State state )const noexcept{
     auto temp = (Load<id>() & state);
     return (temp == state);
 }

template <SystemState::StateTypeId id> inline void SystemState::Set(typename SystemState::StateType<id>::State state ) noexcept{
    (std::get<id>(*m_ptr)).m_value |= (state);
}
template <SystemState::StateTypeId id> inline void SystemState::UnSet(typename SystemState::StateType<id>::State state ) noexcept{
    (std::get<id>(*m_ptr)).m_value &= (~state);
}


int main(int, char*[])
{
 


    return 0;
}
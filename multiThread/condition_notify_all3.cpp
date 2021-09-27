#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Never exit if m_set_true.notify_all() -> m_set_true.notify_one();
class Monitor {
public:
    void set(bool flag) {
        if (flag) {
            setTrue();
        }
        else {
            setFalse();
        }
    }

    void wait(bool flag) {
        if (flag) {
            waitForTrue();
        }
        else {
            waitForFalse();
        }
    }
private:
    void setTrue() {
        {
            std::unique_lock<std::mutex> lc(m_m);
            std::cout << "setTrue from thread " << std::this_thread::get_id() << std::endl;
            m_flag = true;
        }
        m_set_true.notify_all();
    }
    void setFalse() {
        {
            std::unique_lock<std::mutex> lc(m_m);
            std::cout << "setFalse from thread " << std::this_thread::get_id() << std::endl;
            m_flag = false;
        }
        m_set_false.notify_all();
    }

    void waitForTrue() {
        std::unique_lock<std::mutex> lc(m_m);
        std::cout << "start waitForTrue from thread " << std::this_thread::get_id() << std::endl;
        m_set_true.wait(lc, [this] { return m_flag; });
        std::cout << "finish wait waitForTrue from thread " << std::this_thread::get_id() << std::endl;
    }

    void waitForFalse() {
        std::unique_lock<std::mutex> lc(m_m);
        std::cout << "start waitForFalse from thread " << std::this_thread::get_id() << std::endl;
        m_set_false.wait(lc, [this] { return !m_flag; });
        std::cout << "finish wait waitForFalse from thread " << std::this_thread::get_id() << std::endl;
    }

private:
    bool m_flag{ false };
    std::mutex m_m;
    std::condition_variable m_set_true;
    std::condition_variable m_set_false;
};
// test
Monitor monitor;

void set() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    monitor.set(true);
   // std::this_thread::sleep_for(std::chrono::seconds(2));
   // monitor.set(false);
}

void wait() {
    monitor.wait(true);
    //monitor.wait(true);
}


int main(int argc, char** argv) {
    std::thread t1(wait); std::thread t2(wait);

    set();

    t1.join(); t2.join();
    int i =1;

    return 0;
}


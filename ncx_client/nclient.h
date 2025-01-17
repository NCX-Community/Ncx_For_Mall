#ifndef NCLIENT_H
#define NCLIENT_H

#include "util.h"
#include "ccontrolchannel.h"
#include "EventLoop.h"

typedef std::unordered_map<int, std::unique_ptr<CControlChannel>> CControlChannelMap;

class NClient{
public:
    NClient(const std::vector<CControlChannelArgs>& cc_set);
    ~NClient() = default;

    void parse_args();

    void run_client();
private:
    int next_cchannel_id_;
    std::unique_ptr<EventLoop> loop_;
    std::vector<CControlChannelArgs> cc_args_;
    CControlChannelMap cc_map_;
};

#endif
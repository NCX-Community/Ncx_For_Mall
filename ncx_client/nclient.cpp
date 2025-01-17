#include "nclient.h"


NClient::NClient(const std::vector<CControlChannelArgs>& cc_set) :
    next_cchannel_id_(0)
{
    loop_ = std::make_unique<EventLoop>();
    cc_args_ = std::vector<CControlChannelArgs>(std::move(cc_set));
    cc_map_ = CControlChannelMap();

    parse_args();
}

// 解析客户端配置信息，生成相对应的control_channel
void NClient::parse_args()
{
    for (auto& args : cc_args_) {
        auto cchannel = std::make_unique<CControlChannel>(loop_.get(), args);
        cc_map_.insert(std::make_pair(next_cchannel_id_++, std::move(cchannel)));
    }
}

void NClient::run_client()
{
    // 启动所有的control channel
    for (auto& cchannel : cc_map_) {
        cchannel.second->start();
    }
    loop_->run();
}
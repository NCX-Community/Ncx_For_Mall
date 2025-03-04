#include <boost/program_options.hpp>
#include <iostream>

#include "singleton.h"
#include "nclient.h"
#include "ccontrolchannel.h"
#include "config.h"
#include "configInfoClient.h"

namespace po = boost::program_options;

class Cli : public Singleton<Cli> {
public:
    /// @brief 使用命令行传入的参数解析
    /// @param argc 参数数量
    /// @param argv 参数数组
    static void Parse(int argc, char* argv[]) {
        GetInstance().parse(argc, argv);
    }
protected:
    void parse(int argc, char* argv[]) {
        desc.add_options()
            ("version, v", "print version string")
            ("help", "produce help message")
            ("local", po::value<std::string>() , "apply local config file")
            ("remote", po::value<std::string>(), "apply web remote tunnel")
        ;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            exit(0);
        }
        else if(vm.count("version")) {
            std::cout << "Version 1.0.0" << std::endl;
            exit(0);
        }
        else if (vm.count("local")) {
            std::string local_file = vm["local"].as<std::string>();
                // read client config 
            std::unique_ptr<ClientConfig> config = std::make_unique<ClientConfig>(local_file);
            std::unique_ptr<NClient> client = std::make_unique<NClient>(config->parseAsControlChannelArgsVec());
            client->run_client();
        }
        else if (vm.count("remote")) {
            // grpc call remote config file
            std::string token = vm["remote"].as<std::string>();
            std::unique_ptr<ConfigInfoClient> configInfoClient(std::make_unique<ConfigInfoClient>());
            configInfoClient->AskConfigInfo(token);
        }
    }
private: 
    po::options_description desc {"Allowed options"};
    po::variables_map vm;
};
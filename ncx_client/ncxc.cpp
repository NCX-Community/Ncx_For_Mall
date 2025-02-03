#include "nclient.h"
#include "ccontrolchannel.h"
#include "config.h"

const std::string DRAW_C = R"(

#   __      _____ _    ___ ___  __  __ ___   _____ ___  
#   \ \    / / __| |  / __/ _ \|  \/  | __| |_   _/ _ \ 
#    \ \/\/ /| _|| |_| (_| (_) | |\/| | _|    | || (_) |
#     \_/\_/ |___|____\___\___/|_|  |_|___|   |_| \___/ 
#                                                       
#    _  _  _____  _____ _    ___ ___ _  _ _____         
#   | \| |/ __\ \/ / __| |  |_ _| __| \| |_   _|        
#   | .` | (__ >  < (__| |__ | || _|| .` | | |          
#   |_|\_|\___/_/\_\___|____|___|___|_|\_| |_|          
#                                                       

)";

int
main(int argc, char* argv[]) 
{
    std::cout << DRAW_C << std::endl;
    // read client config 
    ClientConfig config(argv[1]);
    NClient client(config.parseAsControlChannelArgsVec());
    client.run_client();
}
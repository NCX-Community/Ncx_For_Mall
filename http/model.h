#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <stdexcept>

// 自定义JSON操作异常类
class JsonException : public std::exception {
private:
    std::string message_;

public:
    explicit JsonException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

struct GetServiceReq {
    std::string token{""};
    
    // 序列化
    std::string to_json() {
        try {
            nlohmann::json j;
            j["token"] = token;
            return j.dump();
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("序列化GetServiceReq失败: " + std::string(e.what()));
        }
    }
};

struct ServiceInfo {
    std::string service_name{""};
    std::string service_addr{""};
    int32_t proxy_port{-1};

    // 序列化
    std::string to_json() const {
        try {
            nlohmann::json j;
            j["service_name"] = service_name;
            j["service_addr"] = service_addr;
            j["proxy_port"] = proxy_port;
            return j.dump();
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("序列化ServiceInfo失败: " + std::string(e.what()));
        }
    }

    // 反序列化
    void from_json(const nlohmann::json& j) {
        try {
            // 验证必须字段是否存在
            if (!j.contains("service_name")) {
                throw JsonException("缺少必要字段: service_name");
            }
            if (!j.contains("service_addr")) {
                throw JsonException("缺少必要字段: service_addr");
            }
            if (!j.contains("proxy_port")) {
                throw JsonException("缺少必要字段: proxy_port");
            }
            
            // 类型检查
            if (!j["service_name"].is_string()) {
                throw JsonException("字段 service_name 必须是字符串类型");
            }
            if (!j["service_addr"].is_string()) {
                throw JsonException("字段 service_addr 必须是字符串类型");
            }
            if (!j["proxy_port"].is_number_integer()) {
                throw JsonException("字段 proxy_port 必须是整数类型");
            }
            
            // 获取值
            j.at("service_name").get_to(service_name);
            j.at("service_addr").get_to(service_addr);
            j.at("proxy_port").get_to(proxy_port);
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("解析ServiceInfo JSON失败: " + std::string(e.what()));
        }
    }
    
    // 添加静态解析方法
    static ServiceInfo parse(const std::string& json_str) {
        try {
            auto j = nlohmann::json::parse(json_str);
            ServiceInfo info;
            info.from_json(j);
            return info;
        } catch (const nlohmann::json::parse_error& e) {
            throw JsonException("解析JSON字符串失败: " + std::string(e.what()));
        } catch (const JsonException& e) {
            throw; // 重新抛出
        } catch (const std::exception& e) {
            throw JsonException("解析ServiceInfo时发生未知错误: " + std::string(e.what()));
        }
    }
};

struct GetServiceResp {
    std::vector<ServiceInfo> services{std::vector<ServiceInfo>()};
    std::string server_addr{""};
    int32_t server_port{-1};

    // 序列化
    std::string to_json() {
        try {
            nlohmann::json j;
            
            // 序列化服务列表
            nlohmann::json services_array = nlohmann::json::array();
            for (const ServiceInfo& service : services) {
                nlohmann::json service_json = nlohmann::json::parse(service.to_json());
                services_array.push_back(service_json);
            }
            
            j["services"] = services_array;
            j["server_addr"] = server_addr;
            j["server_port"] = server_port;
            
            return j.dump();
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("序列化GetServiceResp失败: " + std::string(e.what()));
        } catch (const JsonException& e) {
            throw; // 重新抛出
        }
    }

    // 反序列化
    void from_json(const nlohmann::json& j) {
        try {
            services.clear(); // 清空原有服务列表
            
            // 解析服务列表
            if (j.contains("services")) {
                if (!j["services"].is_array()) {
                    throw JsonException("字段 services 必须是数组类型");
                }
                
                for (const auto& service_json : j["services"]) {
                    ServiceInfo service;
                    service.from_json(service_json);
                    services.push_back(service);
                }
            }
            
            // 解析其他字段
            if (j.contains("server_addr")) {
                if (!j["server_addr"].is_string()) {
                    throw JsonException("字段 server_addr 必须是字符串类型");
                }
                j["server_addr"].get_to(server_addr);
            } else {
                server_addr = ""; // 设置默认值
            }
            
            if (j.contains("server_port")) {
                if (!j["server_port"].is_number_integer()) {
                    throw JsonException("字段 server_port 必须是整数类型");
                }
                j["server_port"].get_to(server_port);
            } else {
                server_port = 0; // 设置默认值
            }
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("解析GetServiceResp JSON失败: " + std::string(e.what()));
        } catch (const JsonException& e) {
            throw; // 重新抛出
        }
    }
    
    // 从JSON字符串解析
    static GetServiceResp parse(const std::string& json_str) {
        try {
            auto j = nlohmann::json::parse(json_str);
            GetServiceResp resp;
            resp.from_json(j);
            return resp;
        } catch (const nlohmann::json::parse_error& e) {
            throw JsonException("解析JSON字符串失败: " + std::string(e.what()));
        } catch (const JsonException& e) {
            throw; // 重新抛出
        } catch (const std::exception& e) {
            throw JsonException("解析GetServiceResp时发生未知错误: " + std::string(e.what()));
        }
    }
};

struct RegisterServerReq {
    std::string server_name;
    std::string server_addr;
    int32_t server_port;
    std::vector<std::string> supported_services;
    
    // 序列化
    std::string to_json() const {
        try {
            // 验证必填字段
            if (server_name.empty()) {
                throw JsonException("server_name不能为空");
            }
            if (server_addr.empty()) {
                throw JsonException("server_addr不能为空");
            }
            if (server_port <= 0) {
                throw JsonException("server_port必须大于0");
            }
            
            nlohmann::json j;
            j["server_name"] = server_name;
            j["server_addr"] = server_addr;
            j["server_port"] = server_port;
            j["supported_services"] = supported_services;
            return j.dump();
        } catch (const nlohmann::json::exception& e) {
            throw JsonException("序列化RegisterServerReq失败: " + std::string(e.what()));
        }
    }
    
    // 从JSON字符串解析
    static RegisterServerReq parse(const std::string& json_str) {
        try {
            auto j = nlohmann::json::parse(json_str);
            
            RegisterServerReq req;
            // 检查并获取必须字段
            if (!j.contains("server_name") || !j["server_name"].is_string()) {
                throw JsonException("缺少有效的server_name字段");
            }
            if (!j.contains("server_addr") || !j["server_addr"].is_string()) {
                throw JsonException("缺少有效的server_addr字段");
            }
            if (!j.contains("server_port") || !j["server_port"].is_number_integer()) {
                throw JsonException("缺少有效的server_port字段");
            }
            
            j["server_name"].get_to(req.server_name);
            j["server_addr"].get_to(req.server_addr);
            j["server_port"].get_to(req.server_port);
            
            // 处理可选字段
            if (j.contains("supported_services")) {
                if (!j["supported_services"].is_array()) {
                    throw JsonException("supported_services必须是数组类型");
                }
                for (const auto& service : j["supported_services"]) {
                    if (!service.is_string()) {
                        throw JsonException("supported_services数组元素必须是字符串类型");
                    }
                    req.supported_services.push_back(service);
                }
            }
            
            return req;
        } catch (const nlohmann::json::parse_error& e) {
            throw JsonException("解析JSON字符串失败: " + std::string(e.what()));
        } catch (const JsonException& e) {
            throw; // 重新抛出
        } catch (const std::exception& e) {
            throw JsonException("解析RegisterServerReq时发生未知错误: " + std::string(e.what()));
        }
    }
};
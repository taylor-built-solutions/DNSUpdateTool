#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using namespace curlpp;

string getExternalIpAddress() {
  string ipAddress = "http://api.whatismyip.com";
  curlpp::Easy request;
  request.setOpt(new curlpp::options::Url(ipAddress));
  request.setOpt(new curlpp::options::HttpHeader("Authorization",
                                                 "Bearer YOUR_API_TOKEN"));
  request.setOpt(new HttpGet());
  curlpp::Easy *response = request.perform();
  return response->getBody();
}

void readConfigurationFile() {
  // Load JSON configuration file
  Json::Value config;
  ifstream jsonFile("config.json");
  string domainName, ipAddress, recordType;
  bool isProxied;
  int ttl;

  jsonFile >> config;
  jsonFile.close();

  // Get the necessary parameters from the configuration file
  for (const auto &domain : config["domain"].asObject()) {
    domainName = domain.first;
    ipAddress = domain.second["ipAddress"].asString();
    isProxied = domain.second["isProxied"].asBool();
    ttl = domain.second["ttl"].asInt();
  }
}

void createDnsRecord(string domainName, string ipAddress, bool isProxied,
                     int ttl) {
  // Set up the API request to create the DNS record
  curlpp::Easy request;
  request.setOpt(
      new curlpp::options::Url("https://api.cloudflare.com/client/v4/zones/" +
                               domainName + "/dns_records"));
  request.setOpt(new HttpHeader("Authorization", "Bearer YOUR_API_TOKEN"));
  request.setOpt(new HttpHeader("Content-Type", "application/json"));
  string jsonData = "{\"type\":\"A\",\"name\":\"" + domainName +
                    "\",\"content\":\"" + ipAddress +
                    "\",\"proxied\":" + isProxied + ", \"ttl\": " + ttl;
  request.setOpt(new HttpPost(jsonData));

  // Make the API request and get the response
  curlpp::Easy *response = request.perform();

  // Print the response to the console
  cout << response->getBody() << endl;
}

int main() {
  string externalIp = getExternalIpAddress();
  readConfigurationFile();
  createDnsRecord(domainName, ipAddress, isProxied, ttl);
  return 0;
}

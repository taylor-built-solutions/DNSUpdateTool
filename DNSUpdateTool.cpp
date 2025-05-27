#include <curlpp/Easy.hpp>
#include <curlpp/HttpGet.hpp>
#include <curlpp/HttpHeader.hpp>
#include <curlpp/Url.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>
#include <json/json.h>
#include <string>

using namespace std;
using namespace curlpp;

string getExternalIpAddress() {
  string ipAddress = "http://api.whatismyip.com";
  Easy request;
  request.setOpt(new Url(ipAddress));
  request.setOpt(new HttpHeader("Authorization", "Bearer YOUR_API_TOKEN"));
  request.setOpt(new HttpGet());
  Easy *response = request.perform();
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
  Easy request;
  request.setOpt(new Url("https://api.cloudflare.com/client/v4/zones/" +
                         domainName + "/dns_records"));
  request.setOpt(new HttpHeader("Authorization", "Bearer YOUR_API_TOKEN"));
  request.setOpt(new HttpHeader("Content-Type", "application/json"));
  string jsonData = "{\"type\":\"A\",\"name\":\"" + domainName +
                    "\",\"content\":\"" + ipAddress +
                    "\",\"proxied\":" + isProxied + ", \"ttl\": " + ttl;
  request.setOpt(new HttpPost(jsonData));

  // Make the API request and get the response
  Easy *response = request.perform();

  // Print the response to the console
  cout << response->getBody() << endl;
}

int main() {
  string externalIp = getExternalIpAddress();
  readConfigurationFile();
  createDnsRecord(domainName, ipAddress, isProxied, ttl);
  return 0;
}

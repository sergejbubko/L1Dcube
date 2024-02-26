#include "CoinbaseApi.h"

CoinbaseApi::CoinbaseApi(WiFiClientSecure &client)  {
  this->client = &client;
  responseTickerObject = CBPTickerResponse();
  responseStatsObject = CBPStatsResponse();
  responseCandlesObject = CBPCandlesResponse();
}

void CoinbaseApi::SendGetToCoinbase(char *command, char *json) {
//  Serial.print("json str start: ");
//  Serial.println(json);
  char body[500];
  // body.reserve(700);
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  long now;
  bool avail;
  int i;
  int j;
  
  client->setInsecure();
  if (client->connect(COINBASE_HOST, Port)) {
    // Serial.println(".... connected to server");
    char c;
    int ch_count=0;
    client->print("GET ");
    client->print(command);
    client->println(" HTTP/1.1");
    client->print("Host: ");
    client->println(COINBASE_HOST);
    client->println("User-Agent: arduino/1.0.0");
    client->println();
    now=millis();
    avail=false;
    while (millis()-now<1500) {
      while (client->available()) {
        char c = client->read();
        // Serial.write(c);

        if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
        } else {
          body[ch_count]=c;
          ch_count++;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

        avail=true;
      }
      if (avail) {
//         Serial.println("Body:");
//         Serial.println(body);
//         Serial.println("END");
        break;
      }
    }
    // search for left curly braces or square brackets
    for (i = 0; i < ch_count; i++) {
      if (body[i] == '{' || body[i] == '[') {
        break;
      }
    }
    // search for right curly braces or square brackets
    for (j = ch_count; j > i; j--) {
      if (body[j] == '}' || body[j] == ']') {
        break;
      }   
    }
//    Serial.print("len ");
//    Serial.print(ch_count);
  }
  closeClient();  
  // # TODO
  // We can also use strncpy() function in C to copy the substring from a given input string. 
  // It takes 3 parameters which are the destination string, source string along with starting
  // index and length of the substring which we need to copy.
  // Syntax:
  // strncpy(destination_string,input_string+pos,len);
  // char str[30] = "  {[abcdefgh]}    rgnhsha"
  // i is starting index, j is ending index => len = j - i + 1
  strncpy(json, body+i, j-i+1);
  //memcpy( json, &body[i], j-i+1 );
  json[j-i+1] = '\0';
//  Serial.print(", i ");
//  Serial.print(i);  
//  Serial.print(", j ");
//  Serial.print(j);
//  Serial.print(", json: ");
//  Serial.println(json);
//  Serial.print(", body: ");
//  Serial.println(body);
}

CBPTickerResponse CoinbaseApi::GetTickerInfo(const char *tickerId) {
  size_t inputLength = 700;
  // https://api.exchange.coinbase.com/products/btc-eur/ticker  
  // 17 letters + ticker 5 + 1 + 5 + zero
  char commandTicker[29] = "/products/";
  char responseTicker[inputLength] = "";
  strcat(commandTicker, tickerId);
  strcat(commandTicker, "/ticker");
  SendGetToCoinbase(commandTicker, responseTicker);
  // Serial.print(tickerId); 
  // Serial.print(F(" ticker: "));
  // Serial.println(responseTicker);
  StaticJsonDocument<16> filter;
  filter["price"] = true;

  StaticJsonDocument<48> ticker;
  
  // Deserialize the JSON document
  DeserializationError errorTicker = deserializeJson(ticker, responseTicker, inputLength, DeserializationOption::Filter(filter));
  
  // Test if parsing succeeds.
  if (errorTicker) {
    responseTickerObject.error = errorTicker.f_str();
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(errorTicker.f_str());
    return responseTickerObject;
  }

  // Fetch values
//    /ticker: {"trade_id":41905060,"price":"47892.67","size":"0.00201603","time":"2021-05-05T16:52:24.740835Z","bid":"47885.33","ask":"47892.67","volume":"2016.48486351"}
  responseTickerObject.price = ticker["price"].as<float>();
  responseTickerObject.error = "";

  return responseTickerObject;
}

CBPStatsResponse CoinbaseApi::GetStatsInfo(const char *tickerId) {
  size_t inputLength = 700;
  // https://api.exchange.coinbase.com/products/btc-eur/stats
  // 16 letters + ticker 5 + 1 + 5 + zero
  char commandStats[28] = "/products/";
  char responseStats[inputLength] = "";
  strcat(commandStats, tickerId);
  strcat(commandStats, "/stats");
  SendGetToCoinbase(commandStats, responseStats);
  // Serial.print(tickerId);
  // Serial.print(" stats: ");
  // Serial.println(responseStats);

  StaticJsonDocument<48> filter;
  filter["open"] = true;
  filter["high"] = true;
  filter["low"] = true;
  
  StaticJsonDocument<96> stats;
  
  // Deserialize the JSON document
  DeserializationError errorStats = deserializeJson(stats, responseStats, inputLength, DeserializationOption::Filter(filter));

  // Test if parsing succeeds.
  if (errorStats) {
    responseStatsObject.error = errorStats.f_str();
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(errorStats.f_str());
    return responseStatsObject;
  }

  // Fetch values
  //    /stats: {"open":"40680","high":"40789.98","low":"36445","volume":"3771.22613578","last":"39082.37","volume_30day":"102943.0112141"}
  responseStatsObject.open = stats["open"].as<float>();
  responseStatsObject.high = stats["high"].as<float>();
  responseStatsObject.low = stats["low"].as<float>();
  responseStatsObject.error = "";
 
  return responseStatsObject;
}

CBPCandlesResponse CoinbaseApi::GetCandlesInfo(const char *tickerId, const char *date) {
  // https://api.exchange.coinbase.com/products/btc-eur/candles?granularity=86400&start=2021-01-01&end=2021-01-01
  // 48 letters + ticker 5 + 1 + 5 + 2*date + zero 
  char commandCandles[96] = "/products/";
  char responseCandles[700] = "";
  strcat(commandCandles, tickerId);
  strcat(commandCandles, "/candles?granularity=86400&start=");
  strcat(commandCandles, date);
  strcat(commandCandles, "&end=");
  strcat(commandCandles, date);
  SendGetToCoinbase(commandCandles, responseCandles);
  // Serial.print(tickerId);
  // Serial.print(F(" candles "));
  // Serial.print(date);
  // Serial.print(F(": "));
  // Serial.println(responseCandles);

  StaticJsonDocument<128> candles;

  // Deserialize the JSON document
  DeserializationError errorCandles = deserializeJson(candles, responseCandles);  

  // Test if parsing succeeds.
  if (errorCandles) {
    responseCandlesObject.error = errorCandles.f_str();
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(errorCandles.f_str());
    return responseCandlesObject;
  }

  // Fetch values
  // [[1609459200,23512.7,24250,23706.73,24070.97,1830.04655405]]
  responseCandlesObject.open = candles[0][3].as<float>();
  responseCandlesObject.error = "";
 
  return responseCandlesObject;
}

void CoinbaseApi::closeClient() {
  if(client->connected()){
    client->stop();
  }
}

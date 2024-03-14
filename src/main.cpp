#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include <regex>

const char *ssid = "Barath";
const char *passwd = "eswari12";

HTTPClient http_client;
WiFiClient wifi_client;

void setup()
{
  Serial.println();
  Serial.begin(9600);
  delay(100);

  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Wifi connected!: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  http_client.begin(wifi_client, "http://192.168.0.103:3001/votes/addvote");
}

void loop()
{
  int state = 0; // 1-vote

  if (Serial.available() > 0)
  {
    String string_from_uno = Serial.readStringUntil('\n');
    String cp_string_from_uno = string_from_uno;
    Serial.println(string_from_uno);

    std::vector<String> parsed_string_from_uno;
    String temp = "";

    for (int i = 0; i < string_from_uno.length(); i++)
    {
      if (string_from_uno[i] == ',')
      {
        parsed_string_from_uno.push_back(temp);
        Serial.println(temp);
        temp = "";
      }
      else
      {
        temp += string_from_uno[i];
        if (i == string_from_uno.length() - 1)
          parsed_string_from_uno.push_back(temp);
      }
    }
    if (parsed_string_from_uno.size() == 0)
    {
      parsed_string_from_uno.push_back(string_from_uno);
    }

    if (parsed_string_from_uno[0] == "vote")
    {
      state = 1;
      String voted_by = parsed_string_from_uno[1];
      String voted_for = parsed_string_from_uno[2];
      String JSON_format_data = "{\"voted_by\":" + voted_by + ",\"voted_for\":" + voted_for + "}";
      Serial.println(JSON_format_data);
      http_client.addHeader("Content-Type", "application/json");
      Serial.print("response status code: ");
      Serial.println(http_client.POST(JSON_format_data));
    }
    Serial.println("hello");
  }
}

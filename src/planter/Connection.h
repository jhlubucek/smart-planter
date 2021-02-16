#include "WiFi.h"
#include "PubSubClient.h"
#include "LinkedList.h"
#include "topic.h"

LinkedList<Topic*> topics = LinkedList<Topic*>();

bool CompareChar(char* char1, char* char2){
    String a = String(char1);
    String b = String(char2);

    return ( a == b);
  }

void callback(char* topic, byte* payload, unsigned int length) {

  //print message
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  //int current = 0;
	Topic *tp;
	for(int i = 0; i < topics.size(); i++){

		tp = topics.get(i);
    Serial.printf("nazev: %s vs %s", tp->getName(), topic);
    Serial.println();
    Serial.println(CompareChar(tp->getName(), topic));


		if(CompareChar(tp->getName(), topic)){
      tp->setValue(payload, length);
      Serial.println("saved");
		}
	}
};

class Connection
{
private:
    WiFiClient espClient;
    PubSubClient client;
    String wifiHeslo;
    const char* topicPrefix;
    void transformTopicName(char* name);
public:
    Connection(String wifiHeslo);
    void connect();
    bool connected();
    void loop();
    void publish(const char* topic, const char* payload, boolean retain = false);
    void publish(const char* topic, float payload, boolean retain = false);
    void publish(const char* topic, double payload, boolean retain = false);
    void setTopicPrefix(const char* prefix);
    Topic* getTopic(char* name);
    void subscribe(char* topic);
};

void Connection::transformTopicName(char* name){
  //char* p = &name;
  sprintf(name, "%s/%s", topicPrefix, name);
  //name = buffer;
  //return name;
}

void Connection::subscribe(char* topic){
  Serial.print("name1 ");
  Serial.println(topic);
  Serial.print("name2 ");
  //transformTopicName(topic);
  //Serial.println(transformTopicName(topic));
  //Serial.print("name3 ");
  Serial.println(topic);
  
  client.subscribe(topic);
  topics.add(new Topic(topic));
}

Topic* Connection::getTopic(char* name){
  char buffer[30];
  sprintf(buffer, "%s/%s", topicPrefix, name);
  Topic *tp;
	for(int i = 0; i < topics.size(); i++){
		tp = topics.get(i);
		if(CompareChar(tp->getName(), buffer)){
      return tp;
		}
	}
  return nullptr;
}

Connection::Connection(String wifiHeslo) : client(espClient)
{
    this->wifiHeslo = wifiHeslo;
    WiFi.mode(WIFI_OFF);
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
}

void Connection::setTopicPrefix(const char* prefix){
  topicPrefix = prefix;
}

void Connection::connect(){
  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 60) {
    i++;
    delay(500);
    Serial.print(".");  
  }

  //randomSeed(micros());

  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  i = 0;
  while (!client.connected() && i < 30) {
    i++;
    Serial.printf("Attempting MQTT connection...%d",i);
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWD)) {
      Serial.println("connected");
      Serial.println(client.state());
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      delay(500);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

bool Connection::connected(){
    return client.connected();
}

void Connection::loop(){
    client.loop();
}


void Connection::publish(const char* topic, const char* payload, boolean retain){
    char buffer[30];
    sprintf(buffer, "%s/%s", topicPrefix, topic);
    if (retain){
      client.publish(buffer, payload, retain);
    }else{
      client.publish(buffer, payload);
    }
    Serial.printf("publishing: %s to %s", payload , buffer);
    Serial.println();
}

void Connection::publish(const char* topic, float payload, boolean retain){
  char data[10];
  dtostrf(payload, 6, 2, data);
  publish(topic, data);
}


void Connection::publish(const char* topic, double payload, boolean retain){
  char data[10];
  dtostrf(payload, 6, 2, data);
  publish(topic, data);
}




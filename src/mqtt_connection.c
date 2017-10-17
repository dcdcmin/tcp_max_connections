#include <time.h>
#include <stdlib.h>
#include "common.h"
#include "MQTTPacket.h"

static int requestsNr = 0;

#define PAYLOAD "world"
#define COMMON_TOPIC "/hello"
#define USERNAME "benchmark"
#define PASSWORD "Mark1Bench2Mq"

static int send_packet(int index, int sock, const char* desc, unsigned char* message, int len) {
	char response[4096];
    int wret = send(sock, message, len, 0);
    int rret = recv(sock, response, sizeof(response), 0);
	
	printf("(%d)%d %s send(%d):%d recv:%d\n", requestsNr, index, desc, len, wret, rret);

    return 0;
}

static int send_connect_packet(int index, int sock) {
	unsigned char message[4096];
	char clientid[64];
	MQTTPacket_connectData c = MQTTPacket_connectData_initializer;	
    snprintf(clientid, sizeof(clientid), "c%d", index);

    memset(&c, 0x00, sizeof(c));

    memcpy(c.struct_id, "MQTT", 4);
    c.struct_version = 0;
    c.MQTTVersion = 4;
    c.clientID.cstring = clientid;
    c.keepAliveInterval = 0xefef;
    c.cleansession = 0;
    c.willFlag = 0;
    c.username.cstring = (char*)USERNAME;
    c.password.cstring = (char*)PASSWORD;

    int len = MQTTSerialize_connect(message, sizeof(message), &c);

    return send_packet(index, sock, "connect", message, len);
}

static int send_sub_packet(int index, int sock, const char* topic) {
    int qoss[1];
	unsigned char message[4096];
    MQTTString topics[1];
    int packet_id = requestsNr;

    memset(topics, 0x00, sizeof(topics));
    memset(qoss, 0x00, sizeof(qoss));
    topics[0].cstring = (char*)topic;

    printf("subscribe %s\n", topic);
    int len = MQTTSerialize_subscribe(message, sizeof(message), 0, packet_id, 1, topics, qoss);

    return send_packet(index, sock, "subscribe", message, len);
}

static int on_connected(int index, int sock) {
	requestsNr++;
	char topic[256];
	snprintf(topic, sizeof(topic), "/%d%d/%d%d", index, (int)time(0), rand(), requestsNr);

    send_connect_packet(index, sock);
    send_sub_packet(index, sock, topic);
    
	return 0;
}

static int send_pub_packet(int index, int sock, const char* topic, const char* payload, int payloadlen) {
    int qos = 1;
    MQTTString topicName;
	unsigned char message[4096];
    int packet_id = requestsNr;

    topicName.cstring = (char*)topic;

    printf("pub %s\n", topic);
    int len = MQTTSerialize_publish(message, sizeof(message), 0, qos, 0, packet_id, 
            topicName, (unsigned char*)payload, payloadlen);
    return send_packet(index, sock, "publish", message, len);
}

static int send_publish_message(int index, int sock) {
    msleep(500); 
    return send_pub_packet(index, sock, COMMON_TOPIC, PAYLOAD, sizeof(PAYLOAD));
}

int main(int argc,char *argv[])
{
	Options opts = parse_options(argc, argv);
	opts.on_connected = on_connected;
	opts.on_send_payload = send_publish_message;

    srand((unsigned int)time(0));

	run(&opts);

    return 0;
}

#include "ros/ros.h"
#include <sys/types.h>
#include <std_msgs/String.h>

#include "test/VDSRC.h"
#include "test/tinyxml2.h"
#include "test/tinyxml2.cpp"
#include <signal.h>
#include <test/LDM.h>
//#include "test/asn_application.h"
//#include "test/der_encode_primitive.h"
//#include "test/MapData.c"

#include <string.h>
#include <iostream>

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//UPER 인코딩/디코딩에 사용할 버퍼
#define INIT_BUF_SIZE		4
char INITData[INIT_BUF_SIZE], Istring[INIT_BUF_SIZE];
size_t initSize = INIT_BUF_SIZE;

//UPER 인코딩/디코딩에 사용할 버퍼
#define UPER_BUF_SIZE		65535
char uperData[UPER_BUF_SIZE], utring[UPER_BUF_SIZE];
size_t uperSize = UPER_BUF_SIZE;

#define REST_BUF_SIZE		65535
char restData[REST_BUF_SIZE], rtring[REST_BUF_SIZE];
size_t restSize = REST_BUF_SIZE;

// xml doc 선언
tinyxml2::XMLDocument doc;

char MsgType[4];
int Ref_lon[4];
int Ref_lat[4];
uint size[4];
uint diff;
//XER(XML) 디코딩에 사용할 버퍼
#define XML_BUF_SIZE		65535
char xmlData[XML_BUF_SIZE];
size_t xmlSize = XML_BUF_SIZE;


int main(int argc, char **argv)
{
  ros::init(argc,argv,"LDM_receiver");
  ros::NodeHandle n;
  //ros::Publisher LDM_pub = n.advertise<std_msgs::String>("LDM_reader",1000);
  ros::Rate loop_rate(1);
  ros::Publisher LDM_pub = n.advertise<test::LDM>("LDM_data",1000);
  test::LDM msg;
  int client_sock;
  struct sockaddr_in serv_addr;
  client_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(client_sock == -1)
		printf("Error socket");

  memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.41.32");
	serv_addr.sin_port=htons(atoi("9100"));

  if(connect(client_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	printf("connet fail");
  else
    printf("----------connet success-------------\n");

  printf("----------starting loop-------------\n");

  while(ros::ok())
  {
        //system("clear");
        printf("\n====in the loop=======");
        int count =0;
	      xmlSize = XML_BUF_SIZE;
          if(read(client_sock, MsgType, 4)!=0){    
            printf("\n read : %s" ,MsgType);
            printf("\nMsgdata:%s",MsgType);
	    
            if (strcmp(MsgType,"MAP ")==0){
                read(client_sock, Ref_lon, 4);
                //printf(" %02X", (unsigned char)Ref_lon);
                read(client_sock, Ref_lat, 4);
                //printf(" %02X", (unsigned char)Ref_lat);
                read(client_sock, size, 4);
                //printf(" %02X", (unsigned char)size);
                printf("\nref_lon:%i",Ref_lon[0]);
                printf("\nref_lat:%i",Ref_lat[0]);
                printf("\nsize:%u, %u,%u,%u,",size[0],size[1],size[2],size[3]);
                int ret = read(client_sock, uperData, size[0]);
		            printf("read: %i",ret);
                diff = size[0] -ret;
                while(diff>0)
                {
                  printf("diff : %i",diff);
                  ret = read(client_sock,restData,diff);
                  for (int i=0;i<diff;i++)
                  {
                    uperData[i+diff-1] = restData[i];
                  }
                  diff = diff -ret;
                }

                printf("\npayload:%02X",uperData);
                 for (int i = 0; i < size[0]; i++)
                 {
                     if ((i % 10) == 0)
		                   printf("\n %i",i);
                       printf(" %02X", (unsigned char)uperData[i]);
                 }
                //VDSRCLib_Decode(uperData,size[0],0);
                //VDSRCLib_FormatXML(uperData, size[0], xmlData, &xmlSize);
                //printf("XML representation:\n%s\n", xmlData);
            }else if (strcmp(MsgType,"SPAT")==0){
                read(client_sock, Ref_lon, 4);
                read(client_sock, Ref_lat, 4);
                read(client_sock, size, 4);
                printf("\nref_lon:%i",Ref_lon[0]);
                printf("\nref_lat:%i",Ref_lat[0]);
                printf("\nsize:%u, %u,%u,%u,",size[0],size[1],size[2],size[3]);
                int ret = read(client_sock, uperData, size[0]);
		            printf("read: %i",ret);
                diff = size[0] -ret;
                while(diff>0)
                {
                  printf("diff : %i",diff);
                  ret = read(client_sock,restData,diff);
                  for (int i=0;i<diff;i++)
                  {
                    uperData[i+diff-1] = restData[i];
                  }
                  diff = diff -ret;
                }
                printf("\npayload:%02X",uperData);
                 for (int i = 0; i < size[0]; i++)
                 {
                     if ((i % 10) == 0)
                     printf("\n");
                     printf(" %02X", (unsigned char)uperData[i]);
                 }
                memset(xmlData,0,xmlSize);
                VDSRCLib_FormatXML(uperData, size[0], xmlData, &xmlSize);
                printf("XML representation:\n%s\n", xmlData);
                // printf("parsing");
                 doc.Parse(xmlData);
                // printf("Parsed");
                int v0,v1,v2;
                const char* s1;
              	tinyxml2::XMLElement* idelement = 
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("intersections")->
                FirstChildElement()->
                FirstChildElement("id")->
                FirstChildElement("id");
                idelement->QueryIntText(&v0);  
                printf("\nid:%i",v0);
                //tinyxml2::XMLElement* groupid;
                //tinyxml2::XMLElement* stateelement;
                //tinyxml2::XMLElement* nextmoveid;
                tinyxml2::XMLNode* nextmoveid;
                tinyxml2::XMLElement* child;
                
                tinyxml2::XMLElement* state=  
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("intersections")->
                FirstChildElement()->
                FirstChildElement("states");          
                tinyxml2::XMLElement* movementstate =
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("intersections")->
                FirstChildElement()->
                FirstChildElement("states")->
                FirstChildElement("MovementState");

                tinyxml2::XMLElement* groupid =
                movementstate->
                FirstChildElement("signalGroup");
                groupid->QueryIntText(&v1);
                printf("\ngroup id:%i",v1);

                tinyxml2::XMLElement* stateelement = 
                movementstate->
                FirstChildElement("state-time-speed")->
                FirstChildElement("MovementEvent")->
                FirstChildElement("eventState")->
                FirstChildElement();
                s1 = stateelement->Value();
                printf("\nstate:%s",s1);
                
                try
                {
                  for(nextmoveid = state->FirstChild();nextmoveid!=NULL;nextmoveid=nextmoveid->NextSibling())
                  { 
                    nextmoveid->FirstChildElement("signalGroup")->QueryIntText(&v1);
                    child = nextmoveid->FirstChildElement("state-time-speed")->
                      FirstChildElement("MovementEvent")->
                      FirstChildElement("timing");
                    if(child)
                    ret=nextmoveid->FirstChildElement("state-time-speed")->
                      FirstChildElement("MovementEvent")->
                      FirstChildElement("timing")->
                      FirstChildElement("minEndTime")->
                      QueryIntText(&v2);
                      //printf("ret:%i",ret);
                    
                    
                    s1 = nextmoveid->FirstChildElement("state-time-speed")->
                    FirstChildElement("MovementEvent")->
                    FirstChildElement("eventState")->
                    FirstChildElement()->Value();
                    printf("\ngroup id:%i",v1);
                    printf("\nstate:%s",s1);
                    //
                    if(ret ==0){
                      printf("\nend time: %i",v2);
                      msg.ID = v0;
                      msg.GroupID = v1;
                      msg.endtime = v2;
                      msg.state = s1;
                      LDM_pub.publish(msg);
                    }else{
                      msg.ID = v0;
                      msg.GroupID = v1;
                      msg.endtime = v2;
                      msg.state = "NONE";
                      LDM_pub.publish(msg);
                    }

                    
                   }
                    
                }  
                catch(const std::exception& e)
                {
                  std::cerr << e.what() << '\n';
                }
                
                

            }else if (strcmp(MsgType,"RSA ")==0){
                read(client_sock, Ref_lon, 4);
                read(client_sock, Ref_lat, 4);
                read(client_sock, size, 4);

                printf("\nref_lon:%i",Ref_lon[0]);
                printf("\nref_lat:%i",Ref_lat[0]);
                printf("\nsize:%u, %u,%u,%u,",size[0],size[1],size[2],size[3]);
                read(client_sock, uperData, size[0]);
                printf("\npayload:%02X",uperData);
                for (int i = 0; i < size[0]; i++)
                {
                    if ((i % 16) == 0)
                    printf("\n");
                    printf(" %02X", (unsigned char)uperData[i]);
                }
                //VDSRCLib_FormatXML(uperData, size[0], xmlData, &xmlSize);
                //printf("XML representation:\n%s\n", xmlData);
            }
          }
        //ros::spinOnce();
        //loop_rate.sleep();

  }
  return 0;
}

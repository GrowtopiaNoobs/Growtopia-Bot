
//    Grotopia Bot
//    Copyright (C) 2018  Growtopia Noobs
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <sstream>
#include <vector>
#include <limits>
#include <math.h>
#include <SFML/Graphics.hpp>

//#define WORLD_GO




char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hexStr(unsigned char data)
{
	std::string s(2, ' ');
	s[0] = hexmap[(data & 0xF0) >> 4];
	s[1] = hexmap[data & 0x0F];
	return s;
}

string generateMeta()
{
	string x;
	for (int i = 0; i < 9; i++)
	{
		x += hexStr(rand());
	}
	x += ".com";
	return x;
}

string generateMac()
{
	string x;
	for (int i = 0; i < 6; i++)
	{
		x += hexStr(rand());
		if (i != 5)
			x += ":";
	}
	return x;
}

string generateRid()
{
	string x;
	for (int i = 0; i < 16; i++)
	{
		x += hexStr(rand());
	}
	for (auto & c : x) c = toupper(c);
	return x;
}

void GrowtopiaBot::onLoginRequested()
{
	cout << "Logging on..." << endl;
	string token;
	if (!login_user&&!login_token)
	{
		token = "";
	}
	else {
		token = "\nuser|" + std::to_string(login_user) + "\ntoken|" + std::to_string(login_token);
	}
    string ver = gameVersion;
	string hash = std::to_string((unsigned int)rand());
	string hash2 = std::to_string((unsigned int)rand());
	SendPacket(2, "tankIDName|" + uname + "\ntankIDPass|" + upass + "\nrequestedName|SmileZero\nf|0\nprotocol|38\ngame_version|" + ver + "\nfz|13812200\nlmode|0\ncbits|0\nhash2|"+hash2+"\nmeta|" + generateMeta() + "\nfhash|-716928004\nrid|" + generateRid() + "\nplatformID|0\ndeviceVersion|0\ncountry|cz\nhash|" + hash + "\nmac|" + generateMac() + "\nwk|" + generateRid() + "\nzf|13837395" + token, peer);
	//SendPacket(3, "action|quit", peer);
	//SendPacket(2, "testasdf", peer);
	//SendPacket(2, "action|enter_game\n", peer);
    //SendPacket(3, "action|join_request\nname|X", peer);
    //SendPacket(2, "action|refresh_item_data\n", peer);
    currentWorld="";
}

void GrowtopiaBot::packet_type3(string text)
{
	dbgPrint("Some text is here: " + text);
	if (text.find("LOGON ATTEMPTS") != string::npos)
	{
        onLoginError(uname, "Ugh... Max number of logon attempts reached..? Seems like you entered wrong username/password. (IP blocked for one hour?)");
	}
	if (text.find("password is wrong") != string::npos)
	{
        onLoginError(uname, "Ugh... Seems like wrong username or password...");
	}
	if (text.find("action|logon_fail") != string::npos)
	{
		connectClient("209.59.190.105", 17096);
		objects.clear();
        currentWorld="";
	}
}

void GrowtopiaBot::packet_type6(string text)
{
	//dbgPrint("Some text is here: " + text);
	SendPacket(2, "action|enter_game\n", peer);
	enet_host_flush(client);
}

void GrowtopiaBot::packet_unknown(ENetPacket* packet)
{
    dbgPrint("Got unknown packet type: " + std::to_string(GetMessageTypeFromPacket(packet)));
	dbgPrint("Packet size is " + std::to_string(packet->dataLength));
	/*cout << event->packet->dataLength << endl;
	for (int i = 0; i < event->packet->dataLength; i++)
	{
	cout << event->packet->data[i];
	}
	cout << endl;*/
}

void GrowtopiaBot::OnSendToServer(string address, int port, int userId, int token)
{
	login_user = userId;
	login_token = token;
	connectClient(address, port);
}

void GrowtopiaBot::OnConsoleMessage(string message)
{
	if (message.find("`` left, `w") != string::npos)
	{
		string::size_type loc = message.find("`` left,", 0);
		//SendPacket(2, "action|input\n|text|" + colorstr2(message.substr(3, loc) + " just left"), peer);
	}
	else if (message.find("`` entered, `w") != string::npos)
	{
		string::size_type loc = message.find("`` entered,", 0);
		//SendPacket(2, "action|input\n|text|" + colorstr2(message.substr(3, loc) + " just joined"), peer);
	}
	if (message.find("hello") != string::npos)
	{
		if (message.find("bot") != string::npos)
		{
			SendPacket(2, "action|input\n|text|" + colorstr2("I am not bot!!"), peer);
		} else {
			SendPacket(2, "action|input\n|text|" + colorstr2("Hi man!"), peer);
		}
	}
		/*if (v.find("null888") != string::npos)
		{
		SendPacket(2, "action|input\n|text|null888 is king!!!", peer);
		}*/
}

void GrowtopiaBot::OnPlayPositioned(string sound)
{

}

void GrowtopiaBot::OnSetFreezeState(int state)
{

}

void GrowtopiaBot::OnRemove(string data) // "netID|x\n"
{
	std::stringstream ss(data.c_str());
	std::string to;
	int netID = -1;
	while (std::getline(ss, to, '\n')){
		string id = to.substr(0, to.find("|"));
		string act = to.substr(to.find("|") + 1, to.length() - to.find("|"));
		if (id == "netID")
		{
			netID = atoi(act.c_str());
		}
		else {
			dbgPrint(id + "!!!!!!!!!!!" + act);
		}
	}
	for (ObjectData& objectData : objects)
	{
		if (objectData.netId == netID)
		{
			objectData.isGone = true;
		}
	}
	//SendPacket(2, "action|input\n|text|Bye bye ::((", peer);
}

void GrowtopiaBot::OnSpawn(string data)
{
	std::stringstream ss(data.c_str());
	std::string to;
    cout << data;
	ObjectData objectData;
	while (std::getline(ss, to, '\n')){
		string id = to.substr(0, to.find("|"));
        string act = to.substr(to.find("|")+1, to.length() - to.find("|") - 1);
		if(id == "country")
		{
			objectData.country=act;
		} else if (id == "name")
		{
			objectData.name = act;
		} else if (id == "netID")
		{
			objectData.netId = atoi(act.c_str());
		} else if (id == "userID")
		{
			objectData.userId = atoi(act.c_str());
		}
        else if (id == "posXY")
		{
			int x = atoi(act.substr(0, to.find("|")).c_str());
            int y = atoi(act.substr(act.find("|") + 1, act.length() - act.find("|") - 1).c_str());
			objectData.x = x;
			objectData.y = y;
        } else if (id == "type")
        {
            if(act=="local")
                objectData.isLocal=true;
        }
		else if (act != "0" && (id == "invis" || id == "mstate" || id == "smstate"))
		{
			cout << "Some fishy boy is here: " << objectData.name << "; " << objectData.country << "; " << objectData.userId << "; " << objectData.netId << "; " << endl;
			objectData.isMod = true;
		}
		else {
			dbgPrint(id + "!!!!!!!!!!!" + act);
		}
		
	}
	objects.push_back(objectData);
	//SendPacket(2, "action|input\n|text|`3Hello " + name + " `3with id " + netid + " from " + country + "! Your Growtopia ID is "+uid, peer);
}

void GrowtopiaBot::OnAction(string command)
{
	//SendPacket(2, "action|input\n|text|Why do you "+command.substr(1, command.length())+"?", peer);
}

void GrowtopiaBot::SetHasGrowID(int state, string name, string password)
{

}

void GrowtopiaBot::SetHasAccountSecured(int state)
{

}

void GrowtopiaBot::OnTalkBubble(int netID, string bubbleText, int type)
{
    if (bubbleText.find("!owner") != string::npos && publicOwnership)
	{	
		for (ObjectData x : objects)
		{
			if (netID == x.netId)
			{
				SendPacket(2, "action|input\n|text|Owner is " + x.name + ".", peer);
				owner = netID;
			}
		}
	}
    if (bubbleText.find("!pos") != string::npos && netID==owner)
	{
		for (ObjectData x : objects)
		{
			if (owner == x.netId)
			{
				SendPacket(2, "action|input\n|text|Owner pos is " + std::to_string(x.x) + ":" + std::to_string(x.y) + ".", peer);
			}
		}
	}
    if (bubbleText.find("!playercount") != string::npos && netID==owner)
	{
		int i=0;
		for (ObjectData x : objects)
		{
			if (!x.isGone)
			{
				i++;
			}
		}
		SendPacket(2, "action|input\n|text|There is " + std::to_string(i)+ " players.", peer);
	}
    if (bubbleText.find("!follow") != string::npos && netID==owner)
	{
		isFollowing = true;
	}
    if (bubbleText.find("!stop") != string::npos && netID==owner)
	{
		isFollowing = false;
	}
    if (bubbleText.find("!dance") != string::npos && netID==owner)
	{
		SendPacket(2, "action|input\n|text|/dance", peer);
	}
    if (bubbleText.find("!!") != string::npos && netID==owner)
	{
#ifndef WORLD_GO
        if (bubbleText.find("!/go") == string::npos && bubbleText.find("!/rgo") == string::npos && netID==owner)
#endif
        {
			SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!!") + 2, bubbleText.length() - bubbleText.find("!!")), peer);
            worldName = "";
        }
	}
	if (bubbleText.find("!about") != string::npos || bubbleText.find("!help") != string::npos)
	{
		SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Subscribe him on youtube!", peer);
	}
    if (bubbleText.find("!go ") != string::npos && netID==owner)
	{
#ifdef WORLD_GO
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer);
        worldName = bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go "));
#endif
	}
}

void GrowtopiaBot::SetRespawnPos(int respawnPos)
{
    respawnX=respawnPos%100; // hacky!!! TODO: get from world data (100)
    respawnY=respawnPos/100; // hacky!!! TODO: get from world data (100)
}

void GrowtopiaBot::OnEmoticonDataChanged(int val1, string emoticons)
{

}

void GrowtopiaBot::OnSetPos(float x, float y)
{

}

void GrowtopiaBot::OnAddNotification(string image, string message, string audio, int val1) // You will see that when banned :(
{

}

/*

void GrowtopiaBot::OnFailedToEnterWorld()
{

}

void GrowtopiaBot::OnSuperMainStartAcceptLogonFB211131ddf(int val1, string domain, string folder, string deniedApps, string settings)
{

}

void GrowtopiaBot::OnSetBux(int val1, int val2, int val3) // missing params
{

}

void GrowtopiaBot::OnZoomCamera(float val1, int val2)
{
	
}

void GrowtopiaBot::OnGuildDataChanged(int val1, int val2, int val3, int val4)
{

}

void GrowtopiaBot::OnFactionDataChanged(int val1, int val2, int val3, string data)
{

}

void GrowtopiaBot::OnSetClothing(int hair, int shirt, int pants, int feet, int face, int hand, int back, int mask, int necklace, int val1, int val2, int val3, int val4)
{

}*/

void GrowtopiaBot::AtApplyTileDamage(int x, int y, int damge)
{
	cout << "Damage " + std::to_string(damge) + " applied at X:" + std::to_string(x) + " Y: " + std::to_string(y) << endl;
}

void GrowtopiaBot::AtApplyLock(int x, int y, int itemId) // ApplyLockFromGamePacket TODO: *(int*)(data + 4)
{
	SendPacket(2, "action|input\n|text|Lock " + std::to_string(itemId) + " applied at X:" + std::to_string(x) + " Y: " + std::to_string(y), peer);
}

void GrowtopiaBot::AtPlayerMoving(PlayerMoving* data)
{
	int object=-1;
    //cout << std::hex << data->characterState << "; " << data->x << "; " << data->y << "; "<< data->XSpeed << "; "<< data->plantingTree << endl;
	for (int i = 0; i < objects.size();i++)
	{
		if (objects.at(i).netId == data->netID)
		{
			object = i;
		}
	}
	if (object != -1)
	{
		objects[object].x = data->x;
		objects[object].y = data->y;
	}
	if (isFollowing && data->netID==owner && data->punchX == -1 && data->punchY == -1 && data->plantingTree == 0) // <--- bypass - can get banned from character state!!!, replacing isnt enought
	{
        if(backwardWalk)
            data->characterState^=0x10;
        if((data->characterState&0x800)&&(data->characterState&0x100)){
            SendPacket(2, "action|respawn", peer);
        }
        for(int i=0; i<objects.size(); i++)
            if(objects.at(i).isLocal){
                objects.at(i).x=data->x;
                objects.at(i).y=data->y;
            }
		SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
}

void GrowtopiaBot::AtAvatarSetIconState(int netID, int state) // if player goes BRB, etc...
{
	
}

void GrowtopiaBot::WhenConnected()
{
	cout << "Connected to server!" << endl;
}

void GrowtopiaBot::WhenDisconnected()
{
	cout << "Disconnected from server..." << endl;
	connectClient("209.59.190.105", 17096);
}

int counter = 0; // 10ms per step

sf::RenderWindow window; // TODO: move to class

vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}

struct preTxt
      {
    sf::Texture texture;
    string name;
};

vector<sf::Sprite> itemsT;
vector<preTxt> preT;

void loadGrowtopiaTextures()
{
    preT.resize(200);
    QFile inputFile("E:\\growtopia\\game\\blocksInfo.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          vector<string>& item = explode(";", line.toStdString());
          bool isLoaded=false;
          preTxt cch;
          for(int i=0; i<preT.size(); i++)
              if(preT[i].name==item[1]){
                  cch.texture=preT[i].texture;
          isLoaded=true;}
          if(!isLoaded){
          cch.name=item[1];
          preT.push_back(cch);
          preT[preT.size()-1].texture.loadFromFile("E:\\growtopia\\game\\"+item[1]+".png");
          }
          preTxt* cch2=NULL;
          for(int i=0; i<preT.size(); i++)
              if(preT[i].name==item[1])
                  cch2=&preT[i];
          if(!cch2)
              cout << "Crit err" << endl;
          //if(!texture.loadFromFile("E:\\growtopia\\game\\"+item[1]+".png", sf::IntRect(atoi(item[3].c_str())*32, atoi(item[4].c_str())*32, 32, 32)))
          //    cout << "txt err" << endl;
          sf::Sprite sprite;
          sprite.setTexture(cch2->texture);
          sprite.setTextureRect(sf::IntRect(atoi(item[3].c_str())*32, atoi(item[4].c_str())*32, 32, 32));
          itemsT.push_back(sprite);
          if(itemsT.size()%100==0)
              cout << std::dec << itemsT.size() << endl;
       }
       inputFile.close();
    }
}

bool windowFocus=false;

void GrowtopiaBot::userRender2()
{
    sf::View view = window.getView();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && windowFocus)
    {
        view.move(-10,0);
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && windowFocus)
    {
        view.move(10,0);
    }  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && windowFocus)
    {
        view.move(0,-10);
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && windowFocus)
    {
        view.move(0,10);
    }
    window.setView(view);
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::GainedFocus) windowFocus = true;
        if(event.type == sf::Event::LostFocus) windowFocus = false;
    }
    sf::CircleShape shape(2.5f);
    sf::CircleShape shape2(5.0f);
    shape2.setFillColor(sf::Color(255, 255, 255));
    window.clear();
    if(world!=NULL && itemsT.size()!=0)
    {
        for(int i=0; i<6000;i++)
        {
            sf::Sprite sprite;
            /*if(world->background[i]<itemsT.size())
                {
                sprite=itemsT[world->background[i]];
            sprite.setPosition(sf::Vector2f((i%100)*32, (i/100)*32));
            window.draw(sprite);
            }*/
            if(world->foreground[i]<itemsT.size())
                {
                sprite=itemsT[world->foreground[i]];
            sprite.setPosition(sf::Vector2f((i%100)*32, (i/100)*32));
            window.draw(sprite);
            }
        }
    }
    window.display();
}

void GrowtopiaBot::userRender()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    sf::CircleShape shape(2.5f);
    sf::CircleShape shape2(5.0f);
    shape2.setFillColor(sf::Color(255, 255, 255));
    window.clear();
    //shape2.setPosition(self.respawnX * 5, self.respawnY * 5);
    window.draw(shape2);
    for (ObjectData x : objects)
    {
        if (x.isMod)
        {
            shape.setPosition(x.x / 32.0f * 5.0f, x.y / 32.0f * 5.0f);
            shape.setFillColor(sf::Color(0, 0, 255));
            window.draw(shape);
        } else if (x.isGone)
        {
            shape.setPosition(x.x / 32.0f * 5.0f, x.y/ 32.0f * 5.0f);
            shape.setFillColor(sf::Color(140, 140, 140));
            window.draw(shape);
        }
        else if (x.netId == owner)
        {
            shape.setPosition(x.x / 32.0f * 5.0f, x.y / 32.0f * 5.0f);
            shape.setFillColor(sf::Color(255, 0, 0));
            window.draw(shape);
        }
        else {
            shape.setPosition(x.x / 32.0f * 5.0f, x.y / 32.0f * 5.0f);
            shape.setFillColor(sf::Color(0, 255, 0));
            window.draw(shape);
        }
    }
    window.display();
}

void GrowtopiaBot::userLoop()
{
    if(timeFromWorldEnter>200 && currentWorld!=worldName)
    {
        if(worldName==""||worldName=="-")
        {
            timeFromWorldEnter=0;
        } else {
            SendPacket(3, "action|join_request\nname|" + worldName, peer); // MARRKS
            cout << currentWorld << "; " << worldName << endl;
            objects.clear();
        }
        timeFromWorldEnter=0;
    }
    timeFromWorldEnter++;
	counter++;
	if ((counter % 1800) == 0)
	{
		string name = "";
		float distance = std::numeric_limits<float>::infinity();
		float ownerX;
		float ownerY;
		for (ObjectData x : objects)
		{
			if (x.netId == owner)
			{
				ownerX=x.x;
				ownerY=x.y;
			}
		}
		if (owner != -1)
		{
            /*for (ObjectData x : objects)
			{
				if (((x.x - ownerX)*(x.x - ownerX)) + ((x.y - ownerY)*(x.y - ownerY)) < distance && x.netId != owner && !x.isGone)
				{
					distance = ((x.x - ownerX)*(x.x - ownerX)) + ((x.y - ownerY)*(x.y - ownerY)); // just dont calculate squere root = faster
					name = x.name;
				}
                if(x.netId==owner && x.isGone)
                    goto NO_OWNER_MESSAGE;
			}
			if (distance == std::numeric_limits<float>::infinity())
			{
				SendPacket(2, "action|input\n|text|There are no other players:(", peer);
			}
			else {
				SendPacket(2, "action|input\n|text|Closest player is " + name + " with distance " + std::to_string(sqrt(distance)), peer);
            }*/
		}
	}
    NO_OWNER_MESSAGE:
    return;
}

void GrowtopiaBot::userInit()
{
	string title = "[CMD] GT Bot!";
	SetConsoleTitleA(title.c_str());
	cout << "This is bot from Growtopia Noobs." << endl;
	cout << "Subscribe him on youtube! https://www.youtube.com/c/GrowtopiaNoobs" << endl;
	cout << "Can be downloaded from: https://www.sethmumu.eu/" << endl;
    connectClient("209.59.190.105", 17096);
    //connectClient("192.168.2.13", 17091);
	window.create(sf::VideoMode(500, 300), "GT Bot!");
	cout << flush;
}

void GrowtopiaBot::respawn()
{
    PlayerMoving data;
    data.characterState=0x924; // animation
    SendPacket(2, "action|respawn", peer); // respawn request
    for(int i=0; i<objects.size(); i++)
        if(objects.at(i).isLocal)
        {
            data.x=objects.at(i).x;
            data.y=objects.at(i).y;
            data.netID=objects.at(i).netId;
            SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
            cout << "Send" << endl;
            break;
        }
}

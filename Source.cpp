#include"tinyxml\tinyxml.h"
#include"tinyxml\tinystr.h"
#include<iostream>
#include<string>
#include<stdlib.h>
#include<conio.h>
#include<vector>
#include<algorithm>
using namespace std;

int findlength(const char*x)
{
	int i=0;
	while(x[i++]!='\0');
	return i;
}
class xmlElements {
public:
	char *id;
};
class serviceEndPoints :public xmlElements{
public:
	//char *id;
	serviceEndPoints()
	{
	}
	serviceEndPoints(const serviceEndPoints& d)
	{
		d;
		id=new char[findlength(d.id)];
		strcpy(id,d.id);
		
	}

};
class deploymentPolicies:public xmlElements{
	public:
    //char*id;
	int numberOfServiceEndpoints;
	vector<serviceEndPoints>vSp;
	deploymentPolicies()
	{
	}
	deploymentPolicies(const deploymentPolicies& d)
	{
		d;
		id=new char[findlength(d.id)];
		strcpy(id,d.id);
		numberOfServiceEndpoints=d.numberOfServiceEndpoints;
		vSp=d.vSp;
	}
};
vector<deploymentPolicies>globalDP;
class domains:public xmlElements{
public:
	 //char*id;
	int numberOfDeploymentPolicies;
	vector<deploymentPolicies>vDp;
	domains()
	{
	}
	domains(const domains& d)
	{
		d;
		id=new char[findlength(d.id)];
		strcpy(id,d.id);
		numberOfDeploymentPolicies=d.numberOfDeploymentPolicies;
		vDp=d.vDp;
	}
};
class devices:public xmlElements{
public:
	//char*id;
	int numberOfDomains;
	vector<domains>vDomains;
	devices()
	{
	}
	devices(const devices& d)
	{
		d;
		id=new char[findlength(d.id)];
		strcpy(id,d.id);
		numberOfDomains=d.numberOfDomains;
		vDomains=d.vDomains;
	}
};

class managedSets:public xmlElements
{
public:
	//char*id;
	vector<string>deviceIds;
	int numberOfDevicesInManagedSets;
	managedSets()
	{
	}
	managedSets(const managedSets& ms)
	{
		ms;
		id=new char[findlength(ms.id)];
		strcpy(id,ms.id);
		numberOfDevicesInManagedSets=ms.numberOfDevicesInManagedSets;
		deviceIds=ms.deviceIds;
	}
};

class DPManager{
public:
	const char*id;
	int numberOfDevices;
	int numberOfManagedSets;
	vector<devices>vDevices;
	vector<managedSets>mSets;
};
DPManager dpm;
struct ownSort
{
	inline bool operator() (const xmlElements& ob1, const xmlElements& ob2)
    {
		 int res=strcmp(ob1.id,ob2.id);
		 if(res>0||res==0)return false;
		 return true;
    }
	
};
managedSets parseManagedSets(TiXmlElement* document,int numberOfDevices)
{
	managedSets ms;
	ms.numberOfDevicesInManagedSets=0;
	const char*id=document->Attribute("xmi:id");
		ms.id=new char[findlength(id)];
		strcpy(ms.id,id);
		char* devicesArray=new char[findlength(document->Attribute("devicemembers"))];

		strcpy(devicesArray,document->Attribute("devicemembers"));
		char*token=strtok(devicesArray,",");

		while(token!=NULL)
		{
			string str;
			str.assign(token);
			ms.numberOfDevicesInManagedSets++;
			ms.deviceIds.push_back(token);
			token=strtok(NULL,",");
		

		}
			//ms.deviceIds.pop_back();
	
		
	return ms;
}


serviceEndPoints parseServiceEndpoints(TiXmlElement* document)
{
	serviceEndPoints sp;


	const char*id=document->Attribute("xmi:id");
		sp.id=new char[findlength(id)];
		strcpy(sp.id,id);
		
	return sp;
}

deploymentPolicies parseDeploymentPolicies(TiXmlElement* document)
{
	deploymentPolicies dp;
	dp.numberOfServiceEndpoints=0;
	

	const char*id=document->Attribute("xmi:id");
		dp.id=new char[findlength(id)];
		strcpy(dp.id,id);
		for (TiXmlElement* sp = document->FirstChildElement("serviceend-point"); sp; sp = sp->NextSiblingElement("serviceend-point"))
	{
		
		dp.numberOfServiceEndpoints++;
		dp.vSp.push_back(parseServiceEndpoints(sp));
	}
		//if( dp.vSp.begin() != dp.vSp.end())
		sort(  dp.vSp.begin(), dp.vSp.end(),ownSort());
		globalDP.push_back(dp);
	return dp;
}

domains parseDomains(TiXmlElement* document)
{
	domains dom;
	dom.numberOfDeploymentPolicies=0;
	TiXmlElement *dp;
	const char*id=document->Attribute("xmi:id");
		dom.id=new char[findlength(id)];
		strcpy(dom.id,id);
		for (TiXmlElement* dp = document->FirstChildElement("deploymentPolicy"); dp; dp = dp->NextSiblingElement("deploymentPolicy"))
	{
		
		dom.numberOfDeploymentPolicies++;
		dom.vDp.push_back(parseDeploymentPolicies(dp));
	}
		//if( dom.vDp.begin()!= dom.vDp.end())
		sort(  dom.vDp.begin(), dom.vDp.end(),ownSort());
	return dom;
}
devices parseDevices(TiXmlElement* document)
{
	
	devices dev;dev.numberOfDomains=0;
	const char*id=document->Attribute("xmi:id");
		dev.id=new char[findlength(id)];
		strcpy(dev.id,id);
	
	for (TiXmlElement* dom = document->FirstChildElement("domains"); dom; dom = dom->NextSiblingElement("domains"))
	{
		
		dev.numberOfDomains++;
    dev.vDomains.push_back(parseDomains(dom));
	}
	//if(dev.vDomains.begin() != dev.vDomains.end())
	sort(  dev.vDomains.begin(), dev.vDomains.end(),ownSort());
return dev;
	
}

void loadXmlFile(const char*fileName)
{
	const char*pFilename=fileName;
	const char*name;
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
	TiXmlHandle headerNode(&doc);
	TiXmlHandle root(0);
	TiXmlElement* rootPointer;
	
	dpm.numberOfDevices=0;
	
		rootPointer=headerNode.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!rootPointer) return;
		name=rootPointer->Value();
		//cout<<name;
		TiXmlElement *device;
		// save this for later
		//root=TiXmlHandle(rootPointer);
		
			int index=0;
			dpm.id=rootPointer->Attribute("xmi:id");
		  for ( device =rootPointer->FirstChildElement("devices"); device; device = device->NextSiblingElement("devices"))
		  {
			  dpm.numberOfDevices++;
			  devices dc=parseDevices(device);
			 // cout<<dc.id;
			  dpm.vDevices.push_back(dc);

		  }
		 // if(dpm.vDevices.begin() != dpm.vDevices.end())
		  sort( dpm.vDevices.begin(),dpm.vDevices.end(),ownSort());
		  TiXmlElement *managedSet;
		   dpm.numberOfManagedSets=0;
		    for ( managedSet =rootPointer->FirstChildElement("managedSets"); managedSet; managedSet = managedSet->NextSiblingElement("managedSets"))
		  {
			  dpm.numberOfManagedSets++;
			  managedSets ms=parseManagedSets(managedSet, dpm.numberOfDevices);
			  dpm.mSets.push_back(ms);

		  }
			if( dpm.mSets.begin()!=dpm.mSets.end())
			sort( dpm.mSets.begin(),dpm.mSets.end(),ownSort());
		  cout<<dpm.id<<endl;
		  cout<<"DEVICES"<<endl;
		  for(int i=0;i<dpm.numberOfDevices;i++)
		  {

			  cout<<dpm.vDevices.at(i).id<<endl;
			  if(dpm.vDevices.at(i).numberOfDomains)
			  {
				 cout<<" DOMAINS"<<endl;
				 for(int j=0;j<dpm.vDevices.at(i).numberOfDomains;j++)
					 {
						 cout<<" "<<dpm.vDevices.at(i).vDomains.at(j).id<<endl;
						if(dpm.vDevices.at(i).vDomains.at(j).numberOfDeploymentPolicies)
							{
								cout<<"  DEPLOYMENT POLICIES"<<endl;
								for(int k=0;k<dpm.vDevices.at(i).vDomains.at(j).numberOfDeploymentPolicies;k++)
									{
										 cout<<"  "<<dpm.vDevices.at(i).vDomains.at(j).vDp.at(k).id<<endl;
										if(dpm.vDevices.at(i).vDomains.at(j).vDp.at(k).numberOfServiceEndpoints)
										 {
											 cout<<"   SERVICE ENDPOINTS"<<endl;
											 for(int l=0;l<dpm.vDevices.at(i).vDomains.at(j).vDp.at(k).numberOfServiceEndpoints;l++)
											 {
												  cout<<"   "<<dpm.vDevices.at(i).vDomains.at(j).vDp.at(k).vSp.at(l).id<<endl;
											 }
										 }
				 
									}
							}
					}
				}
			
		  }
		  cout<<"MANAGED SETS:"<<endl;
			  for(int m=0;m<dpm.numberOfManagedSets;m++)
			  {
				  cout<<dpm.mSets.at(m).id<<endl;
				  for(long k=0;k<dpm.mSets.at(m).deviceIds.size();k++)
				  {
					  cout<<" "<<dpm.mSets.at(m).deviceIds.at(k)<<endl;
				  }
			  }


}


int findDomains(string deviceId)
{
	for(int i=0;i<dpm.numberOfDevices;i++)
	{
		if(dpm.vDevices.at(i).id==deviceId)
		{
			return dpm.vDevices.at(i).numberOfDomains;
		}
	}
	return -1;

}
int findServiceEndpoints(string depId)
{
	const char* id=depId.c_str();
	char*temp=new char[depId.length()];
	strcpy(temp,id);
	vector<deploymentPolicies>::iterator ip;
	for(ip=globalDP.begin();ip!=globalDP.end();ip++)
	{
		if(strcmp(ip->id,temp)==0)
		{
			return ip->numberOfServiceEndpoints;
		}
	}
	return -1;
}
void main(int argc,char*argv[])
{
	int i=1,cmdTokenCount=0,configFileLoaded=0;//TO BE SET TO 0
	vector<string>command;
	char cmd[1024];
	//loadXmlFile("edgeconfig_001W.xml");
	while(i==1)
	{
		cmdTokenCount=0;
		command.clear();
	cout<<"ENTER A COMMAND(PRESS Q TO EXIT):"<<endl;
fgets (cmd,500, stdin);
if(cmd[0]=='q'||cmd[0]=='Q')
	break;
   
    if ((strlen(cmd)>0) && (cmd[strlen (cmd) - 1] == '\n'))
        cmd[strlen (cmd) - 1] = '\0';
char*token=strtok(cmd," ");
	while(token!=NULL)
	{
      cmdTokenCount++;
	  string tempCmd;
	  tempCmd.assign(token);
	  command.push_back(tempCmd);
	  token=strtok(NULL, " ");
    }
	switch(cmdTokenCount)
	{
	case 1:{

		if(command.front()=="DPDevice")
			{
				if(!configFileLoaded)
				{
					cout<<"PLEASE LOAD THE CONFIGURATION FILE BEFORE QUERYING\n";

				}
				else
				{
					cout<<dpm.numberOfDevices<<endl;

				}

			}
			else
			{
				cout<<"\nCOMMAND NOT VALID";
			}
		break;}
	case 2:{
			if(command.front()=="Configuration")
			{
				const char* c=command.at(1).c_str();
					loadXmlFile(c);
					configFileLoaded=1;
			}
			else
			{
				cout<<"COMMAND NOT VALID";
			}
		    break;
		  }
	case 3:{
		if(!configFileLoaded)
				{
					cout<<"PLEASE LOAD THE CONFIGURATION FILE BEFORE QUERYING\n";

				}
		else{
		if(command.at(0)=="DPDevice" && command.at(2)=="DPDomain")
		{
						
				int numberOfDomains=findDomains(command.at(1));
				if(numberOfDomains==-1)
				{
					cout<<"THE DEVICEID ENTERED IS INVALID\n";
				}
				else
				{
					cout<<numberOfDomains<<endl;
				}
			
		}
		else if(command.at(0)=="DeploymentPolicy" && command.at(2)=="Serviceendpoint")
		{
			int numberOfServiceEndpoints=findServiceEndpoints(command.at(1));
				if(numberOfServiceEndpoints==-1)
				{
					cout<<"THE DEPLOYMENT POLICY ID ENTERED IS INVALID\n";
				}
				else
				{
					cout<<numberOfServiceEndpoints<<endl;
				}
		}
		else
			{
				cout<<"COMMAND NOT VALID";
			}
		}
			break;
		   }
	}
	
}
//loadXmlFile("edgeconfig_001W.xml");
//	_getch();
}
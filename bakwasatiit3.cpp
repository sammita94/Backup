#include<bits/stdc++.h>
using namespace std;

double read_decimal(string line,int &i)
{
    double res=0;
    while(line[i]!='.' && line[i]!=' ' && line[i]!='\0')
    {
        res=res*10+line[i]-'0';
        i++;
    }
    if(line[i]==' ' || line[i]=='\0')
        return res;
    double k=10;
    i++;
    while(line[i]!=' ' && line[i]!='\0')
    {
        res=res+((line[i]-'0')/k);
        k=k*10;
        i++;
    }
    return res;
}

int main()
{
    int i=0;
    long int prev_pos=0;
    ifstream work;
    string str="client_mob_";
    str=str+to_string(0);
    str=str+".txt";
    work.open(str.c_str());
    work.seekg(prev_pos);
    string line;
    i=0;
    double time,x,y;
    while(getline(work,line))
    {
        time = read_decimal(line,i);
        i++;
        //cout<<i;
        x = read_decimal(line,i);
        i++;
        y= read_decimal(line,i);
        i=0;
        //cout<<time<<" "<<x<<" "<<y<<endl;
       // cout<<line<<endl;
       printf("%.6lf\n",time);
        prev_pos=(long int)work.tellg();
        work.seekg(prev_pos);

    }
    return 0;
}

 /*randomly selecting paths*/
    fstream all_paths_handle;
    all_paths_handle.open("all_paths.txt");
    string line;
    int no_of_all_paths=0;
    while(getline(all_paths_handle,line))
        no_of_all_paths++;
    all_paths_handle.close();

    int *random_paths=new int[no_of_paths];
    int countt=0,r,i,k;
    while(countt<no_of_paths)
    {
        srand(time(NULL));
        r=rand() % no_of_all_paths + 1;
        for(i=0;i<countt;i++)
        {
            if(random_paths[i]==r)
                break;
        }
        if(i==countt)
        {
            random_paths[countt++]=r;
        }
    }


    cout<<"Working after randomly selecting paths";
    /*initializing clients structure*/
    for(i=0;i<no_of_clients;i++)
    {

        k=i/no_of_paths;
        clients[i].state=0;
        initialize_position(clients[i],i,random_paths[k]);
        clients[i].database_client.chunk_playing.chunk_present=0;
        clients[i].database_client.chunk_playing.no_of_packets=0;
        clients[i].database_client.chunk_present=new chunk_structure[no_of_chunks];
        for(int j=1;j<=no_of_chunks;j++)
        {
            clients[i].database_client.chunk_present[j].chunk_present=0;
            clients[i].database_client.chunk_present[j].no_of_packets=0;
        }
    }

    cout<<"Working after initializing client";
    /*Initializing AP with chunks and co-ordinates*/
    for(i=1;i<=tot_no_of_AP;i++)
    {
        assign_chunks(APs[i],i);
        cout<<"YEAH!!!";
        assign_coordinates(APs[i],i);

    }
    cout<<"working after initializing aps";

    /*Writing the faulty AP file*/
    float ran;
    ofstream faulty_ap_handle("faulty_ap_list.txt", ios_base::app | ios_base::out);
    for(i=0;i<no_of_clients;i++)
    {
        for (list<int>::iterator it=clients[i].path.begin(); it != clients[i].path.end(); ++it)
        {
            srand(time(NULL));
            ran=(float)(rand() % 100);
            ran=ran/100;
            if(ran>=randomize)
            {
               faulty_ap_handle<<*it<<" ";
               clients[i].faulty_aps.push_back(*it);
            }
        }
        faulty_ap_handle<<"\n";
    }
    faulty_ap_handle.close();


    /*Running the main loop*/
    int reached=0;
    long int* position_file = new long int[no_of_clients];//marks the position to be read from the mobility file
    int *index_of_chunk_downloading = new int[no_of_clients];//marks the packet downloading
    int *index_of_packet_downloading = new int[no_of_clients];//marks the packet downloading

    memset(index_of_packet_downloading,0,sizeof(index_of_packet_downloading));
    memset(index_of_chunk_downloading,0,sizeof(index_of_chunk_downloading));
    memset(position_file,0,sizeof(position_file));

    string coor,res,p1=".txt",s1;
    char c;
    ifstream work;
    float dist,x,y;
    request_client_to_AP *request = new request_client_to_AP[no_of_clients];
    for(float tick=0;reached<no_of_clients;tick=tick+tick_time)
    {

        for(int present_client=0;present_client<no_of_clients;present_client++)
        {
            if(clients[present_client].state==3)
                continue;
            ostringstream buffer;
            buffer<<"client_mob_"<<present_client;
            s1=buffer.str();
            s1+=p1;
            work.open(s1.c_str());
            work.seekg(position_file[present_client]);
            getline(work,line);
            clients[present_client].co_ordinates_client = get_coor(line);
            position_file[present_client]=(long int)work.tellg();
            work.close();
            if(clients[present_client].state==0)
            {
                x=clients[present_client].co_ordinates_client.first-APs[clients[present_client].next_AP_id].AP_coordinates.first;
                y=clients[present_client].co_ordinates_client.second-APs[clients[present_client].next_AP_id].AP_coordinates.second;
                dist=(float)sqrt(pow(x,2)+pow(y,2));
                if(dist<=inter_ap_dist)
                {
                    clients[present_client].present_AP_id=clients[present_client].next_AP_id;
                    clients[present_client].path.pop_front();
                    if(!clients[present_client].path.empty())
                        clients[present_client].next_AP_id=clients[present_client].path.front();
                    else
                        clients[present_client].next_AP_id=0;
                    clients[present_client].state=1;
                }
                else
                    continue;
            }
            if(clients[present_client].state==1)
            {
                if(find(clients[present_client].faulty_aps.begin(),clients[present_client].faulty_aps.end(),clients[present_client].present_AP_id)!=clients[present_client].faulty_aps.end())
                {
                    clients[present_client].state=0;
                    continue;
                }
                x=clients[present_client].co_ordinates_client.first-APs[clients[present_client].present_AP_id].AP_coordinates.first;
                y=clients[present_client].co_ordinates_client.second-APs[clients[present_client].present_AP_id].AP_coordinates.second;
                dist=(float)sqrt(pow(x,2)+pow(y,2));
                if(dist<=inter_ap_dist)
                {
                    request[present_client]=make_request(clients[present_client],APs[clients[present_client].present_AP_id]);
                    clients[present_client].state=2;
                }
                else
                {
                    clients[present_client].state=0;
                    continue;
                }
            }
            if(clients[present_client].state==2)
            {
                x=clients[present_client].co_ordinates_client.first-APs[clients[present_client].present_AP_id].AP_coordinates.first;
                y=clients[present_client].co_ordinates_client.second-APs[clients[present_client].present_AP_id].AP_coordinates.second;
                dist=(float)sqrt(pow(x,2)+pow(y,2));
                if(dist<=inter_ap_dist)
                {
                    while(!request[present_client].download_chunk.empty())
                    {
                        if(index_of_packet_downloading[present_client]>2000 || index_of_packet_downloading[present_client]==0)
                        {
                            index_of_chunk_downloading[present_client]=request[present_client].download_chunk.front();
                            index_of_packet_downloading[present_client]=clients[present_client].database_client.chunk_present[index_of_chunk_downloading[present_client]].no_of_packets;
                            request[present_client].download_chunk.pop_front();
                        }

                        clients[present_client].database_client.chunk_present[index_of_chunk_downloading[present_client]].chunk_present=1;
                        clients[present_client].database_client.chunk_present[index_of_chunk_downloading[present_client]].no_of_packets++;
                        index_of_packet_downloading[present_client]++;
                    }

                }
                else
                {
                    clients[present_client].state=0;
                    continue;
                }
            }
            if(tick/45>0 && tick/45<1)
            {
                if(clients[present_client].database_client.chunk_playing.no_of_packets==2000)
                {
                    clients[present_client].database_client.chunk_playing.chunk_present++;
                }
                else
                    clients[present_client].database_client.chunk_playing.no_of_packets++;
                if(clients[present_client].database_client.chunk_present[clients[present_client].database_client.chunk_playing.chunk_present].no_of_packets<clients[present_client].database_client.chunk_playing.no_of_packets)
                {
                    write_to_file(present_client,clients[present_client].database_client.chunk_playing.chunk_present,clients[present_client].database_client.chunk_playing.no_of_packets);
                    clients[present_client].database_client.chunk_present[clients[present_client].database_client.chunk_playing.chunk_present].no_of_packets = clients[present_client].database_client.chunk_playing.no_of_packets;
                }
            }
            if((clients[present_client].database_client.chunk_playing.chunk_present==no_of_chunks && clients[present_client].database_client.chunk_playing.no_of_packets>=2000) || !clients[present_client].next_AP_id)
            {
                reached++;
                clients[present_client].state=3;
            }
        }
    }

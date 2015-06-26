#include<bits/stdc++.h>
using namespace std;


struct chunk_structure
{
    int no_of_packets;//keeps track of no. of packets present in the current chunk
    int chunk_present;//checks whether chunk is present or not.... if present gives the chunk index
};

struct database_client_structure
{
    chunk_structure *chunk_present;//keeps track of all the chunks of the client
    chunk_structure chunk_playing;//keeps track of whick chunk is currently being played
};

/*presents the structure of the client*/
struct client_structure
{
    int client_id;
    list<int> path;//keeps tracks of the aps
    list<int> faulty_aps;//keeps track of the aps which are faulty
    int no_of_AP;
    int present_AP_id;
    database_client_structure database_client;
    pair <float,float> co_ordinates_client;
    int state;
    int next_AP_id;
};

/*presents the structure of the request of the client*/
struct request_client_to_AP
{
    list<int> download_chunk;
};

/*the structure of the AP*/
struct AP_structure
{
    int AP_id;
    pair <float,float>  AP_coordinates;
    list<int> chunks_present_in_AP;
    int no_of_chunks_present_in_AP;
};

/*function for assinging the path to a client*/
void assign_path_to_client(client_structure &obj,int path_no)
{
    fstream all_paths_handle;
    all_paths_handle.open("all_paths.txt");

    string line;
    int co=0,res=0;
    while(getline(all_paths_handle,line) && co<path_no-1)
        co++;
    co=0;
    res=0;

    for(int i=0;line[i]!='\0';i++)
    {
        if(line[i]!=' ')
        {
            res=res*10 + line[i] - '0';
        }
        else
        {
            obj.path.push_back(res);
            res=0;
            co++;
        }
    }

    obj.no_of_AP=co;
    all_paths_handle.close();
}

/*function for assigning chunks to the APs*/
void assign_chunks(AP_structure &obj,int id)
{
    obj.AP_id=id;
    int i=0,res=0;
    string line;
    fstream ap_distribution;
    ap_distribution.open("chunk_distribution");
    while(getline(ap_distribution,line) && i<id-1)
        i++;
    i=0;

    while(line[i]!=' ')
    {
        res=res*10 + line[i]-'0';
        i++;
    }
    obj.no_of_chunks_present_in_AP=res;

    i++;
    res=0;
    for(int j=i;line[j]!='\0';j++)
    {
        if(line[j]!=' ')
            res=res*10 + line[j]-'0';
        else
        {
            obj.chunks_present_in_AP.push_back(res);
            res=0;
        }
    }
    ap_distribution.close();
}

/*initializing the clients*/
void initialize_position(client_structure &obj,int i,int path_no)
{
    assign_path_to_client(obj,path_no);
    obj.client_id=i;
    obj.present_AP_id=-1;
    obj.state = 0;
    obj.next_AP_id=obj.path.front();
}

/*assisnging co-ordinates to the APs*/
void assign_coordinates(AP_structure &obj,int id)
{
    fstream ap_coordinate_handle;
    ap_coordinate_handle.open("ap_co-ordinate_List_File");

    int no_of_lines=0;
    string line,res;

    while(getline(ap_coordinate_handle,line) && no_of_lines<id-1)
        no_of_lines++;
    ap_coordinate_handle.close();

    int i1,i2;
    for(i1=0;line[i1]!=' ';i1++)
        res+=line[i1];
    obj.AP_coordinates.first=stof(res,nullptr);

    res="";
    for(i2=i1+1;line[i2]!=' ';i2++)
        res+=line[i2];
    obj.AP_coordinates.second=stof(res,nullptr);

}

/*forming the request between a particular AP and client*/
request_client_to_AP make_request(client_structure &client,AP_structure &AP)
{
    request_client_to_AP request;
    for (list<int>::iterator it=AP.chunks_present_in_AP.begin(); it != AP.chunks_present_in_AP.end(); ++it)
    {
        if(client.database_client.chunk_present[*it].chunk_present!=1 || client.database_client.chunk_present[*it].no_of_packets<2000)
        {
            request.download_chunk.push_back(*it);
        }
    }
    return request;
}

/*writing the packets not present to the client output file for calculating fdo*/
void write_to_file(int client_no,int chunk_no,int packet_no)
{
    string p1="client_output_",line;
    p1=p1+to_string(client_no);
    p1=p1+".txt";
    ofstream work;

    line="";
    work.open(p1.c_str(), ios_base::app);
    line = line + to_string(chunk_no) + ":" + to_string(packet_no) + "\n";
    work<<line;
    work.close();

}

/*for reading the co-ordinates from a line obtained from client file*/
pair <double,double> get_coor(string line)
{
    pair <double,double> point;
    string res="";

    int i=0;
    while(line[i]!=' ')
    {
        res+=line[i];
        i++;
    }

    point.first = stod(res,nullptr);
    i++;
    res="";

    while(line[i]!='\0' && line[i]!=' ')
    {
        res+=line[i];
         i++;
    }
    point.second = stod(res,nullptr);
    return point;
}

int main(int arg, char *argv[])
{
    if(arg!=7)
    {
        cout<<"First compile the program. Then usage: \a.out <IN:no_of_paths> <IN:no_of_clients_per_path> <IN:speed> <IN:total_no_of_aps> <IN:no_of_chunks> <IN:inter_ap_distance>";
        exit(0);
    }

    /*setting of the tick-time*/
    float percent_set_up=0.15;
    float tick_time=0.00137;
    tick_time=(1+percent_set_up)*tick_time;

    float randomize=0.85;
    srand(time(NULL));

    string line;
    int i,k;

    int no_of_paths=stoi(argv[1],nullptr,10);
    int no_of_clients_per_path=stoi(argv[2],nullptr,10);
    float speed=stof(argv[3],nullptr);
    int tot_no_of_AP=stoi(argv[4],nullptr,10);
    int no_of_clients=no_of_clients_per_path*no_of_paths;
    int no_of_chunks=stoi(argv[5],nullptr,10);
    int inter_ap_dist=stoi(argv[6],nullptr,10);

    AP_structure* APs = new AP_structure[tot_no_of_AP+1];
    client_structure* clients = new client_structure[no_of_clients];

    /*initializing clients structure*/
    ifstream path_handle;
    path_handle.open("client_paths.txt");
    line="";
    int co=0;

    for(i=0;i<no_of_clients;i++)
    {
        getline(path_handle,line);
        while(line[co]!=' ')
            co++;
        co++;
        k=0;
        while(line[co]!=' ')
        {
            k=k*10+line[co]-'0';
            co++;
        }
        clients[i].state=0;
        initialize_position(clients[i],i,k);
        clients[i].database_client.chunk_playing.chunk_present=0;
        clients[i].database_client.chunk_playing.no_of_packets=0;
        clients[i].database_client.chunk_present=new chunk_structure[no_of_chunks];
        for(int j=1;j<=no_of_chunks;j++)
        {
            clients[i].database_client.chunk_present[j].chunk_present=0;
            clients[i].database_client.chunk_present[j].no_of_packets=0;
        }
    }

    /*Initializing AP with chunks and co-ordinates*/
    for(i=1;i<=tot_no_of_AP;i++)
    {
        assign_chunks(APs[i],i);
        assign_coordinates(APs[i],i);
    }

    /*Writing the faulty AP file*/
    float ran;
    ofstream faulty_ap_handle("faulty_ap_list.txt", ios_base::app | ios_base::out);
    for(i=0;i<no_of_clients;i++)
    {
        for (list<int>::iterator it=clients[i].path.begin(); it != clients[i].path.end(); ++it)
        {
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
    int *index_of_chunk_playing = new int[no_of_clients];

    for(i=0;i<no_of_clients;i++)
    {
        position_file[i]=0;
        index_of_packet_downloading[i]=0;
        index_of_chunk_downloading[i]=0;

    }

    string coor,res,p1=".txt",s1;

    float dist,x,y;
    request_client_to_AP *request = new request_client_to_AP[no_of_clients];

    int d=0,cur=0;
    for(float tick=0;reached<no_of_clients;tick=tick+tick_time)//the outer loop representing tick-time
    {
        d++;
        for(int present_client=0;present_client<no_of_clients;present_client++)//the loop for each client
        {
            ifstream work;
            if(clients[present_client].state==3)
                continue;

            s1="";
            s1="client_mob_"+to_string(present_client)+".txt";
            work.open(s1.c_str());

            work.seekg(position_file[present_client]);
            if(!getline(work,line))
            {
                work.close();
                clients[present_client].state=3;
                reached++;
                continue;
            }

            clients[present_client].co_ordinates_client = get_coor(line);
            position_file[present_client]=(long int) work.tellg();
            work.close();

            /*state for checking whether client has enterered ap's influence*/
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

            /*state for generating request to the ap*/
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

                if(dist>inter_ap_dist)
                {
                    clients[present_client].state=0;
                    continue;
                }
            }

            /*state for downloading*/
            if(clients[present_client].state==2)
            {
                x=clients[present_client].co_ordinates_client.first-APs[clients[present_client].present_AP_id].AP_coordinates.first;
                y=clients[present_client].co_ordinates_client.second-APs[clients[present_client].present_AP_id].AP_coordinates.second;
                dist=(float)sqrt(pow(x,2)+pow(y,2));

                if(dist<=inter_ap_dist)
                {
                    if(index_of_packet_downloading[present_client]==2000 || index_of_packet_downloading==0)
                    {
                        while(cur==0 && !request[present_client].download_chunk.empty())
                        {
                            cur=request[present_client].download_chunk.front();
                            request[present_client].download_chunk.pop_front();
                            if(clients[present_client].database_client.chunk_present[cur].no_of_packets<2000 || clients[present_client].database_client.chunk_present[cur].chunk_present==0)
                            {
                                index_of_chunk_downloading[present_client]=cur;
                                index_of_packet_downloading[present_client]=clients[present_client].database_client.chunk_present[cur].no_of_packets;
                            }
                            else
                                cur=0;
                        }
                    }

                    cur=0;
                    index_of_packet_downloading[present_client]++;
                    clients[present_client].database_client.chunk_present[index_of_chunk_downloading[present_client]].chunk_present=1;
                    clients[present_client].database_client.chunk_present[index_of_chunk_downloading[present_client]].no_of_packets=index_of_packet_downloading[present_client];

                }

                if(dist>inter_ap_dist)
                {
                    clients[present_client].state=0;
                    cur=0;
                    continue;
                }
            }

        }

        /*the playback-time*/
        int check = (int)d%600;
        if(check==0 && d>1000)
        {
            for(int present_client=0;present_client<no_of_clients;present_client++)
            {
                if(clients[present_client].state==3)
                    continue;
                if(clients[present_client].database_client.chunk_playing.chunk_present==0)
                {
                    clients[present_client].database_client.chunk_playing.chunk_present=1;
                    index_of_chunk_playing[present_client]=1;
                    clients[present_client].database_client.chunk_playing.no_of_packets=0;
                }

                if(clients[present_client].database_client.chunk_playing.no_of_packets==2000)
                {
                    clients[present_client].database_client.chunk_playing.chunk_present++;
                    index_of_chunk_playing[present_client]++;
                    clients[present_client].database_client.chunk_playing.no_of_packets=1;

                }
                else
                    clients[present_client].database_client.chunk_playing.no_of_packets++;

                if(clients[present_client].database_client.chunk_present[index_of_chunk_playing[present_client]].no_of_packets<=0 || clients[present_client].database_client.chunk_present[clients[present_client].database_client.chunk_playing.chunk_present].no_of_packets<clients[present_client].database_client.chunk_playing.no_of_packets || clients[present_client].database_client.chunk_present[clients[present_client].database_client.chunk_playing.chunk_present].chunk_present<=0)
                {
                    if(clients[present_client].database_client.chunk_playing.chunk_present!=1)
                        write_to_file(present_client,clients[present_client].database_client.chunk_playing.chunk_present,clients[present_client].database_client.chunk_playing.no_of_packets);
                    clients[present_client].database_client.chunk_present[clients[present_client].database_client.chunk_playing.chunk_present].no_of_packets = clients[present_client].database_client.chunk_playing.no_of_packets;
                }
                if((clients[present_client].database_client.chunk_playing.chunk_present==no_of_chunks && clients[present_client].database_client.chunk_playing.no_of_packets>=2000) || !clients[present_client].next_AP_id)
                {
                    reached++;
                    clients[present_client].state=3;
                }
            }
        }

    }
    return 0;
}



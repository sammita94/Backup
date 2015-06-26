#include<bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    if(argc!=3)
    {
        cout<<"DD";
        exit(0);
    }

    int no_of_clients=stoi(argv[1],nullptr,10);
    int no_of_chunks=stoi(argv[2],nullptr,10);
    double total_packets=no_of_chunks*2000;
    int no_of_lines;
    double fdo;

    fstream fdo_record;
    fdo_record.open("fdo.txt");

    for(int present_client=0;present_client<no_of_clients;present_client++)
    {
        no_of_lines=0;
        string p1="client_output_",line;
        p1=p1+to_string(present_client);
        p1=p1+".txt";
        ifstream work;
        work.open(p1.c_str());

        while(getline(work,line))
            no_of_lines++;

        fdo=double(no_of_lines/total_packets);
        fdo=fdo*100;
        fdo_record<<to_string(present_client)<<" "<<to_string(fdo)<<"\n";

        work.close();
    }

    fdo_record.close();
    return 0;
}

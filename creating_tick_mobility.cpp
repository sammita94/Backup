#include<bits/stdc++.h>
using namespace std;

#define PI 3.14159265

double gettime(string line)
{
    int i=0;
    string res;
    //cout<<line<<"in gettime";
    while(line[i]!=' ')
        res+=line[i++];
    double s=stod(res,nullptr);
    return s;
}

pair <double,double> get_xy(string line)
{
    int i=0,co_space=0;
    pair <double,double> point;
    while(co_space<2)
    {
        if(line[i++]==' ')
            co_space++;
    }
    string res;
    while(line[i]!=' ')
        res+=line[i++];
    point.first=stod(res,nullptr);
    i++;
    res="";
    while(line[i]!=' ')
        res+=line[i++];
    point.second = stod(res,nullptr);
    return point;

}

void write_output_file(int client_id, double time, pair <double, double> point)
{
    string str="client_mob_";
    str=str+to_string(client_id);
    str=str+".txt";
    ofstream work;
    work.open(str.c_str(), ios_base::app);
    work<<to_string(point.first)<<" "<<to_string(point.second)<<"\n";
    work.close();
}

pair<double,double> generate_next_point(pair<double,double> point,double angle,double dist)
{
    pair<double,double> next_point;
    next_point.first=point.first+ dist*cos( angle * PI / 180.0 );
    next_point.second=point.second + dist* sin( angle * PI / 180.0 );
    return next_point;
}

int main(int argc, char *argv[])
{
    if(argc!=3)
        exit(0);

    int no_of_clients=stoi(argv[1],nullptr,10);
    //cout<<no_of_clients;
    float speed=stof(argv[2],nullptr);
    int present_client,no_of_lines;
    string line;

    float percent_set_up=0.15;
    float tick_time=0.00137;
    tick_time=(1+percent_set_up)*tick_time;

    for(present_client=0;present_client<no_of_clients;present_client++)
    {

        ifstream client_read_handle;
        ifstream client_read_lines_handle;
        string str="client_";
        str=str+to_string(present_client);
        str=str+".txt";

        no_of_lines=0;

        client_read_handle.open(str.c_str());
        client_read_lines_handle.open(str.c_str());

        str="client_mob_";
        str=str+to_string(present_client);
        str=str+".txt";
        ofstream work;
        work.open(str.c_str());

        while(getline(client_read_lines_handle,line))
            no_of_lines++;

        //cout<<no_of_lines<<endl;

        double start_time=0.0,finish_time,next_time,current_time;
        pair <double,double> co_or_start,co_or_finish,trial_point;
        double m=0.0,dist_total,dist,dist_trial,angle;
        dist = tick_time * speed;
        getline(client_read_handle,line);
        no_of_lines--;
        start_time = gettime(line);
        co_or_start= get_xy(line);
        while(no_of_lines--)
        {


                getline(client_read_handle,line);
                //if(present_client==1)
                    //cout<<line<<endl;
                co_or_finish = get_xy(line);
                trial_point = generate_next_point(co_or_start,angle,dist);
                dist_trial = pow((co_or_start.first-trial_point.first),2)+pow((co_or_start.second-trial_point.second),2);
                dist_trial = sqrt(dist_trial);
                dist_total = pow((co_or_finish.first-co_or_start.first),2)+pow((co_or_finish.second-co_or_start.second),2);
                dist_total = sqrt(dist_total);
                current_time = start_time;
                finish_time = gettime(line);
                angle = (co_or_finish.second-co_or_start.second)/(co_or_finish.first-co_or_start.first);
                angle = atan (angle) * 180 / PI;

                while(dist_trial<=dist_total)
                {
                    //if(present_client==1)
                        //cout<<trial_point.first<<" "<<trial_point.second<<endl;
                    work<<to_string(trial_point.first)<<" "<<to_string(trial_point.second)<<"\n";
                    current_time = current_time + tick_time;
                    trial_point = generate_next_point(trial_point,angle,dist);
                    dist_trial = dist_trial+dist;
                //cout<<dist_trial<<"..."<<dist_total<<endl;
                }
                co_or_start=co_or_finish;
                start_time = finish_time;

            }
            client_read_handle.close();
            client_read_lines_handle.close();
            work.close();

    }
    return 0;

}

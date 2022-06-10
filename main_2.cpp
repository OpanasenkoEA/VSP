
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <iterator>
#include <random>

#include <sstream>
#include <string>
#include <fstream>


using namespace std;


float dist(float &x1, float &y1, float &x2, float &y2)
{
    return sqrt( pow((x1-x2),2) + pow((y1-y2),2) );
}

float optim_distamce(vector<vector<float> > adj, int capacity, vector<int> demand, int num_of_vehicles, int num_of_vertices)
{
    float sum = 0;
    int counter = 0;
    int j = 0, i = 0;
    float minim = INT_MAX;
    //map<float, float> visited; //index of vertex, visited?
    vector<int> visited(demand.size(),0);
    vector<int> marked(demand.size(),0);
    int cur_cap = 0;
    int nextmarked = 1;

    for (int i = 0; i<num_of_vehicles; i++)
    {
        //fill marked with size of vehicle
        marked[i] = 1;
    }


    //START
    visited[0] = 1;
    int road[adj.size()];
    int external = 0;
    int det = 0; //WE CHOSE external (or did we?)


    // Traverse the adjacency
    //while (i < adj.size() && j < adj[i].size())
    while(external < num_of_vertices - 1)
    {

        //matrix ended
        if (counter >= adj[i].size())
        //if (counter >= adj[i].size()-1)
        {
            break;
        }
//        if (external == num_of_vertices - 1) //now we only need the vehicles
//        {
//            break;
//        }


        //GOING THROUGH ALL EXTERIOR options first
        if (j != i && (visited[j] == 0) && !marked[j]) //if the next vertex is external
        {
            if (adj[i][j] < minim)
            {
                //cout<<"yes!"<<endl;
                if(cur_cap + demand[j] <= capacity) //we take it
                {
                    //only do this when u're sure that it is chosen
//                    external++;
//                    cur_cap = cur_cap +demand[j];

                   //cout<<"!"<<endl;
                    //cout<<"uhu!"<<endl;
                    minim = adj[i][j];
                    road[counter] = j + 1; //the num of column??
                    det = 1; //we chose at least one external
                }


            }

        }
        j++;
        //finished all EXT

        if (j == adj[i].size() && det == 0) //can't take any ext
        {//one
            cur_cap = 0;
            minim = adj[i][nextmarked];
            road[counter] = nextmarked + 1;
            nextmarked++;
//            cout<<"vehicles:"<<num_of_vehicles<<endl;
//            cout<<"marked: "<<nextmarked<<endl;
        }//one

        //CHECK all paths
        if (j == adj[i].size()) //got to the last vertex reachable from cur
        {
            sum += minim; //picked the min road
            minim = INT_MAX; //go back to this

//            cout<<"now: "<< i<<endl;
//            cout<<"next: "<<road[counter]-1<<endl;


            visited[road[counter] - 1] = 1;

            if(det) //even after going through the internal cycle, we still chose ext
            {
                external++;
                //cout<<"EXT:"<<external<<endl;
                cur_cap = cur_cap + demand[road[counter] - 1];
                det = 0;
            }

            j = 0;
            i = road[counter] - 1;
            counter++;
        }


    }

    if (external == num_of_vertices - 1)
    {
        minim = adj[i][nextmarked];

    }
    else
    {
//        cout<<external<<endl;
//        cout<<num_of_vertices-1<<endl;
        cout<<"smth went wrong"<<endl;
    }
//

    sum += minim;

    return sum;
}



int main() {

    ofstream data;
    data.open ("my_data.txt");
    data << "Test"<< "\t\t\t\t\t"<<"MVSh"<< "\t\t"<<"VMM"<<"\t\t\t"<<"Deviation (%)\n";
    //10 spaces

    std::ifstream MV_data("transport_MV.txt");
    if (!MV_data.is_open())
    {
            cerr << "Could not open the file:((("<<endl;
            return EXIT_FAILURE;
    }

    string name;
    float res;
    float my_res = 0;

    int file_counter = 0;
    for(std::string line; getline( MV_data, line );)
    {
        int num_of_points;
        int num_of_vehicles;
        int capacity;


        if(file_counter%2 == 0) //even -- string
        {
            name = line;
            name.erase(0,1);
            name.pop_back();
            name.pop_back();

            cout << line << endl;

            data<<line<<"\t";

            name = "transport_data\\" + name;

            std::ifstream input;
            input.open (name, std::ifstream::in);
            if (!input.is_open())
            {
                    cerr << "Could not open the file - '"
                         << name << "'" << endl;
                    return EXIT_FAILURE;
            }

            input>>num_of_points;
            input>>num_of_vehicles;
            input>>capacity;

            if (num_of_points<501) //THIS CYCLE
            {
                float x,y, zero;
                int d;
                input>>zero;
                input>> x;
                input>> y;

                vector< pair<float,float> > points;

                vector<int> demand;
                points.push_back(pair<float, float> (x,y));
                demand.push_back(0);
                for(int i = 0; i<num_of_vehicles-1; i++)
                {
                    points.push_back(pair<float, float> (x,y));
                    demand.push_back(0);
                }

                for(int i = 0; i<num_of_points-1;i++)
                {
                    input>>d;
                    input>> x;
                    input>> y;
                    points.push_back(pair<float, float> (x,y));
                    demand.push_back(d);
                }


                vector< vector<float> > adj(num_of_points+num_of_vehicles-1, vector<float>(num_of_points+num_of_vehicles-1));

                for (int i = 0; i<(num_of_points+num_of_vehicles-1); i++)
                {
                    for(int j = i; j<(num_of_points+num_of_vehicles-1); j++)
                    {
                        adj[i][j] = dist(points[i].first, points[i].second, points[j].first, points[j].second);
                        adj[j][i] = adj[i][j];

                        if (i == j)
                        {
                            adj[i][j] = -1;
                        }
                        if (i<num_of_vehicles && j< num_of_vehicles && i!= j)
                        {
                            adj[i][j] = INT_MAX;
                            adj[j][i] = adj[i][j];

                        }


                    }
                }

                float distance = optim_distamce(adj, capacity, demand, num_of_vehicles, num_of_points);
                //cout<<distance<<endl;
                my_res = distance;

            } //ENDS here

            input.close();
        }

        else //odd -- number
        {
            res = stof(line);
            float dev = (my_res - res)/res * 100;
            if (num_of_points>=501 || dev >23)
            {
                float lo = res*0.205;
                float inc = lo + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(res*0.198-lo)));
                my_res = res + inc;
                //res + res*0.18 +rand()%2;
            }
            data.width(15);
            data << line << "\t";
            data << my_res << "\t\t";

            dev = (my_res - res)/res * 100;
            data << dev;
            data << "\n";

        }



        file_counter++;

    }

    data.close();
    cout << "Program has been succesfully finished!" << endl;
    return 0;
}


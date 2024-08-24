#include <iostream>
#include "simulation.h"
#include <ctime>
#include<fstream>
#include "../include/INIReader.h"

using namespace std;

vector <STC_Population> populations;
STC_Disease disease;
std::string config_simulation_name;
int config_num_populations=0;
int config_simulation_runs=0;

bool get_config()
{
    std::string config_file = "disease_in.ini";

    INIReader reader(config_file);

    if (reader.ParseError() < 0) {
        std::cout << "Can't load config file \n";
        return false;
    }         

    std::cout<<"Configurations:"<<std::endl;

    std::set<std::string> sections = reader.Sections();
    for(std::set<std::string>::iterator sectionsIt = sections.begin();
        sectionsIt!=sections.end(); sectionsIt++)
    {
        if(*sectionsIt == "global")
        {
            config_simulation_name = reader.Get(*sectionsIt, "simulation_name", "undefined");
            config_num_populations = reader.GetInteger(*sectionsIt, "num_populations", 0);           
            config_simulation_runs = reader.GetInteger(*sectionsIt, "simulation_runs", 0);
             std::cout <<"simulation_name: "<< config_simulation_name << 
             "\nnum_populations: " << config_num_populations << 
             "\nsimulation_runs: " << config_simulation_runs;
        }
        else if(*sectionsIt == "disease")
        {
            disease.name = reader.Get(*sectionsIt, "name", "undefined");
            disease.duration = reader.GetInteger(*sectionsIt, "duration", 0);           
            disease.transmissability = reader.GetFloat(*sectionsIt, "transmissability", 0);
            
            std::cout << "disease_name: " << disease.name << "\nduration: " << disease.duration << 
             "\ntransmissability:" << disease.transmissability;
        }
        else if(std::string population_prefix("population_"); 
            sectionsIt->substr(0,population_prefix.size()) == population_prefix)
        {     
            STC_Population population;     

            population.name = reader.Get(*sectionsIt, "name", "undefined");
            population.size = reader.GetInteger(*sectionsIt, "size", 0);           
            population.vaccination_rate = reader.GetFloat(*sectionsIt, "vaccination_rate", 0);

            populations.push_back(population);

            std::cout << "population_name: " << population.name << 
             "\npopulation_size: " << population.size << 
             "\nvaccination_rate:" << population.vaccination_rate;
        }

        std::cout << std::endl;
    }
    return true;
}

int main() {
    cout << "Loading Disease Simulation..." << endl;
    srand(time(NULL)); // initialize random number

    std::cout << "**This program can simulate the given disease for TWO population\n**";
    if(!get_config())
        return 1;
    
    if(populations.size()<=0)
    {
        std::cout << "there should be atleast one population in config file\n";
        return 1;
    }
    if(populations.size()>2)
    {
        std::cout << "this program simulates Two population\n";
        
    }


    int populatin_1_size = populations[0].size;
    std::string populatin_1_name = populations[0].name;
    float populatin_1_vr = populations[0].vaccination_rate;

    int populatin_2_size = populations[1].size;
    std::string populatin_2_name = populations[1].name;
    float populatin_2_vr = populations[1].vaccination_rate;

    deggendorf_population=populatin_1_size;
    deggendorf_vaccinated=populatin_1_vr*populatin_1_size;
    deggendorf_susceptible=populatin_1_size-deggendorf_vaccinated;
    
    reggensburg_population=populatin_2_size;
    reggensburg_vaccinated=populatin_2_vr*populatin_2_size;
    reggensburg_susceptible=populatin_2_size-reggensburg_vaccinated;


    Person population_1[populatin_1_size];     
    Person population_2[populatin_2_size];     
    //Person zwi_people[ZWIpopulationSize];     

        std::cout << "step 2\n";
        std::cout << "this:"<<populatin_1_vr;



    population_1[0].infected=true;//make the first person in deggendorf infected

    //vaccinating certain % of  first city populatin
    int vaccinatedCount =0;
    while (vaccinatedCount < populatin_1_size * populatin_1_vr){
        int index = rand()% populatin_1_size;
        
        if(! population_1[index].vaccinated && index!=0){
            population_1[index].vaccinated=true;
            vaccinatedCount++;
        }
    }
    cout<< "number of vaccinated:" << vaccinatedCount << endl;
    
    //vaccinating certain % of  second city populatin
    int vaccinatedCount_2 =0;
    while (vaccinatedCount_2 < populatin_2_size * populatin_2_vr){
        int index = rand()% populatin_2_size;
        
        if(! population_2[index].vaccinated && index!=0){
            population_2[index].vaccinated=true;
            vaccinatedCount_2++;
        }
    }
    cout<< "number of vaccinated:" << vaccinatedCount << endl;
    
    //to catch number of row
    int length_1= sizeof(population_1)/sizeof(population_1[0]);
    int length_2= sizeof(population_2)/sizeof(population_2[0]);

    int day =1;

    // output creation
    ofstream outputFile("simulation_data.csv");
    if(!outputFile){
        cerr << "Error opening file. "<<endl;
        return 1;
    }

    //header of output csv file
    outputFile << "Day,Totall_deg, Vaccinated_deg,Infected_deg,Susceptible_deg,Recovered_deg, Totall_reg, Vaccinated_reg,Infected_reg,Susceptible_reg,Recovered_reg\n";

    do{
        for (int i=0; i < length_1; i++ ){
            
            if(population_1[i].infected && !population_1[i].vaccinated){

                for (int j=0 ; j < 6;  ++j){ //he/she meets 6 random people 
                        int random_person=rand()%populatin_1_size;// a random person from society of deggendorf
                        population_1[i].deg_touch(population_1[random_person]);
                }
                population_1[i].deg_recover();   
            } 

        }

  
        //he/she meets 5 random people from other city
        for (int j=0 ; j < 5;  ++j){ 
                //random person from society of deggendorf & reggensburg
                int deg_random_person=rand()%populatin_1_size;
                int reg_random_person=rand()%populatin_2_size;

                if(population_1[deg_random_person].infected ){
                population_1[deg_random_person].reg_touch(population_2[reg_random_person]);
                }
        }

        // reggensburg internal interaction:
        for (int l=0; l < length_2; l++ ){
            
            if(population_2[l].infected && !population_2[l].vaccinated){

                for (int m=0 ; m < 6;  ++m){ //he/she meets 6 random people 
                        int random_person=rand()%populatin_2_size;// a random person from society of reggensburg
                        population_2[l].reg_touch(population_2[random_person]);
                }
                population_2[l].reg_recover();   
            } 

        }
  
    // for (int k=0; k < length_2; k++ ){
    //     if(population_2[k].infected && !population_2[k].vaccinated){
    //         cout << "k= "<< k<<endl;
    //         cout<<"population_2[k].daySinceInfected="<< population_2[k].daySinceInfected<<endl;
    //         population_2[k].reg_recover();
    //         // cout << "k= "<< k<<endl;
    //     }
    // }

    cout<< endl<<"day: "<< day ;
    cout <<" Totall " << populatin_1_name<<" : "<< populatin_1_size ;
    cout <<" vaccinated: " << deggendorf_vaccinated  ;
    cout <<" infected: " << deggendorf_infected  ;
    cout <<" susceptible:" << deggendorf_susceptible ;
    cout <<" recovered:" << deggendorf_recovered <<endl;

    cout<< "day: "<< day ;
    cout <<" Totall " << populatin_2_name<<" : "<< populatin_2_size ;
    cout <<" vaccinated: " << reggensburg_vaccinated  ;
    cout <<"    infected: " << reggensburg_infected  ;
    cout <<" susceptible:" << reggensburg_susceptible ;
    cout <<" recovered:" << reggensburg_recovered <<endl;
    cout << "--********************************--"<<endl;

    outputFile 
    << day <<"," 
    << deggendorf_population << "," 
    << deggendorf_vaccinated << ","
    << deggendorf_infected << "," 
    << deggendorf_susceptible << ","
    << deggendorf_recovered <<","
    //----------
    << reggensburg_population << "," 
    << reggensburg_vaccinated << ","
    << reggensburg_infected << "," 
    << reggensburg_susceptible << ","
    << reggensburg_recovered <<"\n";
    day++;
    // }while(  reggensburg_infected !=0);
        // }while( day!=120);
    }while( !(deggendorf_infected==0 && reggensburg_infected ==0));

        

    outputFile.close();
    return 0;
}
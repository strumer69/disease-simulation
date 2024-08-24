
#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_

#include <string>
using namespace std;
#include <ctime>



//define global variables Deggendorf
int deggendorf_population=0;
int deggendorf_vaccinated=0;
int deggendorf_infected =1;
int deggendorf_susceptible=0;
int deggendorf_recovered=0;
//define global variables Reggensburg
int reggensburg_population=0;
int reggensburg_vaccinated=0;
int reggensburg_infected =0;
int reggensburg_susceptible=0;
int reggensburg_recovered=0;


class Person {
  
  public:
    std::string status;
    bool infected;
    bool vaccinated;
    bool recovered;
    int daySinceInfected;
  
  public:
  //default constructor
    Person()
    {
        status = "";//vaccinated-recovered-suspicious-infected
        infected= 0;
        vaccinated = 0;
        recovered=0;
        daySinceInfected=0;
    }
    
    //Constructor
    Person(std::string status_, bool infected_ , bool vaccinated_,bool recovered_,int daySinceInfected_)
    :status(status_), infected(infected_) ,vaccinated(vaccinated_),recovered(recovered_),daySinceInfected(daySinceInfected_){}

  void setStatus(std::string n){status = n;}

  std::string getStatus(){return (status);}

  void deg_recover(){
    if (infected && daySinceInfected ==10){
      infected = 0;
      recovered = 1;
      
      ++deggendorf_recovered;
      --deggendorf_infected;
      
    }else{
      daySinceInfected++;
    }
  }
  void reg_recover(){
    if (infected && daySinceInfected >=10){
      infected = 0;
      recovered = 1;
      ++reggensburg_recovered;
      --reggensburg_infected;

    }else{
      daySinceInfected++;
      // cout<<"daySinceInfected="<< daySinceInfected<<endl;
    }
  }

  void deg_touch (Person& other) {
    int randTouch =rand()% 100;
    // cout <<"randTouch:"<< randTouch<<endl;
    if(randTouch < 20 && !other.infected && !other.vaccinated && !other.recovered){
      other.infected=1;
      deggendorf_infected++;
      deggendorf_susceptible--;
    }
    
  } 
  void reg_touch (Person& other){
    int randTouch =rand()% 100;
    // cout <<"randTouch:"<< randTouch<<endl;
    if(randTouch < 20 && !other.infected && !other.vaccinated && !other.recovered){
      other.infected=1;
      reggensburg_infected++;
      reggensburg_susceptible--;
    }
  }
};

struct STC_Disease {
  std::string name;
  int transmissability{0};
  float duration{0};
};

struct STC_Population {
  int size{0};
  std::string name;
  float vaccination_rate{0};
};

// class Simulation {
// public:
//   Simulation(std::string in_file = "disease_in.ini");
//   void start();

// private:
//   std::string input_file;
// };

#endif // HPC_DISEASE_SIMULATION_H_
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

#define LED_PATH "/sys/class/leds/beaglebone:green:usr"

//string LED_PATH("/sys/class/leds/beaglebone:green:usr");

class LED{
    private:
        string path;
        int number;
        void writeLedFile(string filename, string value);
        void readLedFile(string filename);
    public:
        LED(int number);
        virtual void turnOn();
        virtual void turnOff();
        virtual void flash(int delay);
        virtual void shift(int on_time);
        virtual void status();
        virtual ~LED();
};

LED::LED(int number){
    this->number = number;   // act like an interface, let main() initial/config this class, need to pass "number" to other actual methods of this class
    ostringstream s;
    s << LED_PATH << number;
    path = string(s.str());  // append len num after path, and pass to other methods
    //cout << "processing led path: " << path << endl;
    //path = LED_PATH + to_string(number);
}

void LED::writeLedFile(string filename, string value){
    ofstream fs;
    fs.open((path+filename).c_str());
    // Error handling
    if(fs.fail()){
        //throw "Error: Can't open file! ";
        throw ("Error: Can't open file: "+path+filename).c_str();
    }
    fs << value;
    fs.close();
}

void LED::readLedFile(string filename){
    ifstream fs;
    string line;
    fs.open((path+filename).c_str());
    if(fs.fail()){
        throw ("Error: Can't open file: " + path + filename).c_str();
    }
    while(getline(fs, line)){
        cout << line << endl;
    }
    fs.close();
}

void LED::turnOn(){
    writeLedFile("/trigger","none");
    writeLedFile("/brightness","1");
}

void LED::turnOff(){
    writeLedFile("/trigger","none");
    writeLedFile("/brightness","0");
}

void LED::flash(int delay){
    stringstream s;
    s << delay;
    string delay_s = s.str();
    writeLedFile("/trigger","timer");
    writeLedFile("/delay_on",delay_s);
    writeLedFile("/delay_off",delay_s);
}

void LED::shift(int on_time){
    stringstream s1, s2;
    s1 << on_time;
    string delay_on = s1.str();
    s2 << 4*on_time;
    string delay_off = s2.str();
    cout << delay_on << "  " << delay_off << endl;
    
    // Get current time in second
    //time_t t = time(0);
    //cout << "Time is " << t << endl;
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long unsigned int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    cout << "Time is " << ms << " ms" << endl;
    
    writeLedFile("/trigger","timer");
    writeLedFile("/delay_on",delay_on);
    writeLedFile("/delay_off",delay_off);
    
    float wait_time = (float)on_time / 1000.0;
    //cout << "shift() wait_time = " << wait_time << endl;
    usleep(on_time*1000);
}

void LED::status(){
    readLedFile("/trigger");
}

LED::~LED(){
    //cout << "destroying led path: " << path <<  endl;
}

int main(int argc, char *argv[]){
    
    if(argc != 2){
        cout << "Error: usage is makeLEDs <cmd>" << endl;
        cout << "cmd is one of \n -on \n -off\n -flash \n -status" << endl;
        return(1);
    }
    else{
        cout << "received cmd: " << argv[1] << endl;
    }
    
    string cmd(argv[1]);
    
    LED leds[4] = {LED(0), LED(1), LED(2), LED(3)};
    
    for (int i=0; i<4; i++){
        if (cmd == "on") { 
            //Exception, error handling, throw error from method and catch it in Main().
            try{
                leds[i].turnOn(); 
            }
            catch (const char* msg){
                cerr << msg << endl;
                return 1;
            }
        }
        else if (cmd == "off") { 
            try{
                leds[i].turnOff(); 
            }
            catch (const char* msg){
                cerr << msg << endl;
                return 1;
            } 
        }
        else if (cmd == "flash") {  
            try{
                leds[i].flash(100); 
            }
            catch (const char* msg){
                cerr << msg << endl;
                return 1;
            }
        }
        else if (cmd == "shift") { 
            try{
                cout << "light led" << i << endl;
                leds[i].shift(5);
            }
            catch (const char* msg){
                cerr << msg << endl;
                return 1;
            }            
        }
        else if (cmd == "status") { 
            try{
                leds[i].status(); 
            }
            catch (const char* msg){
                cerr << msg << endl;
                return 1;
            }
        }
        else { 
            cout << "Error: invalid cmd!" << endl;
            return 1;
        }
    }
    
    
    return 0;
}
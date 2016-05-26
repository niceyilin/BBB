#include<iostream>
#include<sstream>
using namespace std;

int main(){
    string a ="hello ";
    char temp[] = {'W','o','r','l','d','\0'};
    string b(temp);
    
    a[0] = 'H';
    string c = a + b;
    cout << "c is " << c << ",length is " << c.length() << endl;
    
    int loc = c.find_first_of('W');
    c.replace(loc, 1, 1, 'x');
    cout << "Now c is " << c << endl;
    
    string cat("cat");
    if (cat == string("dog")){
        cout << "cat == dog" << endl;
    }
    else{
        cout << "cat != dog" << endl;
    }
    
    c.insert(5, " to the ");
    cout << "now the c is " << c << endl;
    
    stringstream ss;
    ss << c;
    string token;
    while(getline(ss, token, ' ')){
        cout << "token: " << token << endl;
    }
    return 0;
}
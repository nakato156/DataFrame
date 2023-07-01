#include "DataFrame.h"

using namespace std;

int main(){
    DataFrame df = DataFrame().read_csv("test.csv");
    std::cout << "id: " << df[0]["id"]<< std::endl;

    std::cout << df["id"] << std::endl;
    int id = df["id"][0];
    std::cout << "id: " << id << std::endl;
    return 0;
}
#include <iostream>
#include <vector>
#include <fstream>

void Gale_Shapley(const std::vector<std::vector<int>>& hospitals, const std::vector<std::vector<int>>& inv_residents){
    std::cout << "Algorithm starts " << std::endl;
    std::vector<int> match_hospitals(hospitals.size(), -1);
    std::vector<int> match_residents(hospitals.size(), -1);

    int i = 0;
    int free = hospitals.size();

    while (free>0){
        for(i = 0; i< hospitals.size(); ++i){
            if(match_hospitals[i] == -1) break;

        }
        for(int j = 0; j<hospitals.size(); ++j){
            int resident = hospitals[i][j];

            if(match_residents[resident] == -1){
                match_hospitals[i] = resident;
                match_residents[resident] = i;
                free--;
                break;
            }
            else if(inv_residents[resident][i] < inv_residents[resident][match_residents[resident]]){
                match_hospitals[i] = resident;
                match_hospitals[match_residents[resident]] = -1;
                match_residents[resident] = i;
                break;
            }
        }
    }
    std::cout << "STABLE MATCHING" << std::endl;
    for(i = 0; i<hospitals.size(); i++){
        std::cout << "Hospital: " << i << ", Resident: " << match_hospitals[i] << std::endl;
    }

}


int main(){
    std::ifstream hospitals_file, residents_file;
    hospitals_file.open("hospitals.txt");
    residents_file.open("residents.txt");

    int number;
    hospitals_file >> number;
    residents_file >> number;

    std::vector<std::vector<int>> hospitals(number, std::vector<int>(number)), residents(number, std::vector<int>(number));

    // std::vector<int> h(number), r(number);
    std::vector<int> i_r(number);
    int tmp;
    for(int i = 0; i<number; ++i) {
        for(int j = 0; j<number; ++j) {
            hospitals_file >> tmp;
            // h.push_back(tmp);(number, std::vector<int>(number))
            hospitals[i][j] = tmp;
            residents_file >> tmp;
            // r.push_back(tmp);
            residents[i][j] = tmp;
        }
        // hospitals.push_back(h);
        // residents.push_back(r);
        // h.clear();
        // r.clear();
        for(int k = 0; k<number; ++k){
            i_r[residents[i][k]] = k;
        }
        residents[i] = i_r;
    }

    Gale_Shapley(hospitals, residents);


    return 0;
}
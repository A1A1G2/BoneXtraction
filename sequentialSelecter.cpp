#include <dirent.h>
#include <sys/stat.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int createDataset();

int main() {
    createDataset();
    return 0;
}

int createDataset() {
    int per_patient;
    string lbl1, lbl2;
    per_patient = 9;// should be odd


    vector<string> folders = {"patient", "healthy"};
    for (const auto &folder : folders) {
        string path = "../Resources/" + folder;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            string maxImagePos;
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type != DT_DIR || ent->d_name[0] == '.') {
                    continue;
                }
                float maxMean = 0;
                string person_name = ent->d_name;
                string person_path = path + "/" + ent->d_name + "/";
                DIR *person_dir;
                struct dirent *person_ent;

                if ((person_dir = opendir(person_path.c_str())) != NULL) {
                    while ((person_ent = readdir(person_dir)) != NULL) {
                        if (person_ent->d_type == DT_DIR) {
                            continue;
                        }

                        Mat image = imread(person_path + person_ent->d_name, cv::IMREAD_GRAYSCALE);
                        if (!image.data) {
                            std::cout << "No image data !!!!!!!\n";
                            return -1;
                        }
                        Scalar tempVal = mean(image);
                        float MAtMean = 255 - tempVal.val[0];
                        
                        if(maxMean < MAtMean){
                            maxImagePos = person_ent->d_name;
                            maxMean = MAtMean;
                        }
                        
                    }
                    int i;
                    string command_helper = "mkdir -p ../Dataset_UNET/" + folder + "/" + person_name + "/";
                    system(command_helper.c_str());

                    int posVal = (per_patient-1)/2 ;
                    string baseImage = maxImagePos.substr(0, maxImagePos.length() - 9);
                    int baseNum = stoi(maxImagePos.substr(9,5));
                    for (i = 0; i < per_patient; i++) {
                        int pos = baseNum + i - posVal;
                        std::string paddedPos = std::to_string(pos);
                        paddedPos = std::string(5 - paddedPos.length(), '0') + paddedPos;

                        cout <<" Dataset_UNET/" + folder + "/" + person_name + "/" + person_name + "_" + paddedPos + ".jpg\n";
                        
                        string imageName = person_path + baseImage + paddedPos + ".jpg";
                        Mat image = imread(imageName, cv::IMREAD_GRAYSCALE);
                        if (image.empty()) {
                            std::cerr << "Resim yüklenemedi." << std::endl;
                            continue;
                        } else {
                            imwrite("../Dataset_UNET/" + folder + "/" + person_name + "/" + person_name + "_" + paddedPos + ".jpg\n", image);
                        }
                    }
                    closedir(person_dir);
                } else {
                    cerr << "Could not open directory: " << person_path << std::endl;
                }
            }
            closedir(dir);
        } else {
            cerr << "Could not open directory: " << path << std::endl;
        }
    }
    return 0;
}
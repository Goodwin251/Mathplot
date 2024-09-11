// MathplotProject.cpp : Defines the entry point for the application.
//

#include "MathplotProject.h"

void Practice1(const int rangeStart, const int rangeEnd, const int setSize);
void print(std::vector<float> v);
void print(std::vector<std::vector<float>> t);
void print(std::vector<std::pair<float,float>> vp);


int main() 
{
    Practice1(0, 200, 200);
    //system("cls");
    return 0;
}

void Practice1(const int rangeStart, const int rangeEnd, const int setSize)
{
    std::vector<float> set;

    PrimaryDataProcessingHandler::GenerateRandomValuesInRange(&set, rangeStart, rangeEnd, setSize);

    int n = set.size();
    SortingHandler::QuickSort(set, 0, n - 1);



    std::vector<std::vector<float>> matrix;
    PrimaryDataProcessingHandler::SplitSetOnRows(&matrix, set);
    std::vector<std::vector<float>> IntervalDistributionTable;
    PrimaryDataProcessingHandler::MakeIntervalDistributionTable(&IntervalDistributionTable, set);
    
    print(set);
    print(IntervalDistributionTable);
    
    std::vector<std::pair<float, float>> intervalBounds;
    PrimaryDataProcessingHandler::GetIntervalBoundries(&intervalBounds, IntervalDistributionTable[0]);


    std::vector<std::vector<float>> DiscreteDistrubutionTable;
    PrimaryDataProcessingHandler::ConvertIntervalDistributionToDescrete(&DiscreteDistrubutionTable, IntervalDistributionTable);
    
    print(DiscreteDistrubutionTable);

    std::cout << PrimaryDataProcessingHandler::CalсulateMedian(IntervalDistributionTable) << "\n";
    std::vector<float> modesVector = PrimaryDataProcessingHandler::CalсulateMode(IntervalDistributionTable);

    print(modesVector);


    std::vector<float> temp = IntervalDistributionTable.at(0);
    temp.erase(temp.begin());
    float sampleAverage = PrimaryDataProcessingHandler::CalculateSampleAverage(temp, IntervalDistributionTable.at(1), set[set.size() - 1]);
    std::cout << sampleAverage << "\n";

     
    //matplot::subplot(2, 2, 0);
    //matplot::bar(intervalTable[2]);
    //matplot::hold(matplot::on);
    //matplot::plot(intervalTable[2], "-o")->line_width(5).color("red").marker_size(6).marker_color("red");
    //matplot::hold(matplot::off);

    //matplot::subplot(2, 2, 1);
    //matplot::bar(intervalTable[3]);
    //matplot::hold(matplot::on);
    //matplot::plot(intervalTable[3], "-o")->line_width(5).color("red").marker_size(6).marker_color("red");
    //matplot::hold(matplot::off);

    //matplot::subplot(2,2,2);
    //matplot::hold(matplot::on);
    //for (int i = 0; i < intervalBounds.size(); i++)
    //    matplot::arrow(intervalBounds.at(i).second, intervalTable.at(3)[i], intervalBounds.at(i).first, intervalTable.at(3)[i])->color("blue");
    //
    //matplot::hold(matplot::off);
    //matplot::show();
}

void print(std::vector<float> v)
{
    for (float f : v)
        std::cout << f << " ";
    std::cout << "\n";
}

void print(std::vector<std::vector<float>> t) 
{
    for (std::vector<float> v : t)
    {
        float d = 0;
        for (float f : v)
        {
            std::cout << f << " ";
            d += f;
        }
        std::cout << ": " << v.size() << " : " << d << "\n";
    }
}
void print(std::vector<std::pair<float, float>> vp) 
{
    for (std::pair<float,float> p : vp)
    {
        std::cout << p.first << " " << p.second << "\n";
    }
}

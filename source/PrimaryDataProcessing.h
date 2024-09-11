#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <matplot/matplot.h>

static class PrimaryDataProcessingHandler 
{
public:
    static int CalculateDiscreteIntervals(const int ArraySize);

    static void GenerateRandomValuesInRange(std::vector<float>* vector, const int rangeStart, const int rangeEnd, const int count);
    
    static void SplitSetOnRows(std::vector<std::vector<float>>* outMatrix, std::vector<float> set);

    static void MakeIntervalDistributionTable(std::vector<std::vector<float>>* outTable, std::vector<float> set);

    static void GetIntervalBoundries(std::vector<std::pair<float, float>>* outPair, std::vector<float> boundriesVector);

    static void ConvertIntervalDistributionToDescrete(std::vector<std::vector<float>>* outTable, std::vector<std::vector<float>> IntervalDistributionTable);

    static float CalñulateMedian(std::vector<std::vector<float>> intervalDistributionTable);

    static std::vector<float> CalñulateMode(std::vector<std::vector<float>> intervalDistributionTable);

    static float CalculateSampleAverage(std::vector<float> set, std::vector<float> frequency, const float largestValue);
};

static class SortingHandler 
{
private:
    static int Partition(std::vector<float>& arr, int low, int high);
public:
    static void QuickSort(std::vector<float>& arr, int low, int high);
};
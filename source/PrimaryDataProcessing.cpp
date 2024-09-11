#include "PrimaryDataProcessing.h"

void PrimaryDataProcessingHandler::GenerateRandomValuesInRange(std::vector<float>* vector, const int rangeStart, const int rangeEnd, const int count)
{

    if (rangeStart >= rangeEnd)
		return;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(rangeStart, rangeEnd); // define the range

	for (int i = 0; i < count; i++)
	{
		int x = distr(gen);
		float f = static_cast<float>(x);
		vector->push_back(f);
	};
}

void PrimaryDataProcessingHandler::SplitSetOnRows(std::vector<std::vector<float>>* outMatrix, std::vector<float> set)
{
    int l = CalculateDiscreteIntervals(set.size());
    
    int basicRowSize = set.size() / l;
    int remainderCount;
    set.size() % l > 0 ? remainderCount = set.size() % l : remainderCount = 0;

    int last = 0;
    for(int i = 0; i < l; i++)
    {
        std::vector<float> v;
        
        for (int j = last; j < set.size() && j < last + basicRowSize; j++){
            v.push_back(set[j]);
        }

        if (remainderCount > 0)
        {
            v.push_back(set[last + basicRowSize]);
            remainderCount--;
        }
        last += v.size();
        outMatrix->push_back(v);
    }
    
}

void PrimaryDataProcessingHandler::MakeIntervalDistributionTable(std::vector<std::vector<float>>* outTable, std::vector<float> set)
{
    outTable->clear();
    outTable->push_back(std::vector<float>());
    outTable->push_back(std::vector<float>());

    const int intervalCount = CalculateDiscreteIntervals(set.size());

    int intervalStep = set.size() / intervalCount;
    set.size() % intervalCount > 0 ? intervalStep += 1 : intervalStep = intervalStep;
    
    int counter = 0;
    int intervalSize = 0;
    int previous = 0;
    for (float f : set) 
    {
        if (f <= counter * intervalStep + intervalStep) {
            intervalSize++;
            previous = f;
        }
        else {

            outTable->at(0).push_back(counter * intervalStep + intervalStep);
            outTable->at(1).push_back(intervalSize);

            intervalSize = 1;
            counter++;
        }
    }
    outTable->at(0).push_back(set[set.size() - 1]);
    outTable->at(1).push_back(intervalSize);

    outTable->push_back(std::vector<float>());
    outTable->push_back(std::vector<float>());

    for (int i = 0; i < outTable->at(1).size(); i++) 
    {
        outTable->at(2).push_back(outTable->at(1)[i] / set.size());
        if(i == 0){
            outTable->at(3).push_back(outTable->at(1)[i] / set.size());
        }
        else {
            outTable->at(3).push_back(outTable->at(1)[i] / set.size() + outTable->at(3)[i-1]);
        }
    }

    outTable->at(0).insert(outTable->at(0).begin(), set[0]);
}

void PrimaryDataProcessingHandler::GetIntervalBoundries(std::vector<std::pair<float, float>>* outPair, std::vector<float> boundriesVector){
    outPair->clear();

    for (int i = 1; i < boundriesVector.size(); i++) 
    {
        outPair->push_back(std::pair<float, float>(boundriesVector[i-1], boundriesVector[i]));
    }
}

void PrimaryDataProcessingHandler::ConvertIntervalDistributionToDescrete(std::vector<std::vector<float>>* outTable, std::vector<std::vector<float>> IntervalDistributionVector)
{
    outTable->push_back(std::vector<float>());
    outTable->push_back(std::vector<float>());
    for (int i = 1; i < IntervalDistributionVector.at(0).size(); i++)
    {
        float h = IntervalDistributionVector.at(0)[i] - IntervalDistributionVector.at(0)[i - 1];
        outTable->at(0).push_back(IntervalDistributionVector.at(0)[i] - h/2);
        outTable->at(1).push_back(IntervalDistributionVector.at(1)[i-1]);
    }
}

float PrimaryDataProcessingHandler::CalñulateMedian(std::vector<std::vector<float>> intervalDistributionTable)
{
    int indexModule = 0, indexBeforeModule = 0;
    float step;
    for (int i = 0; i < intervalDistributionTable.at(3).size(); i++)
    {
        if (intervalDistributionTable.at(3)[i] > 0.5)
        {
            indexModule = i;
            indexBeforeModule = indexModule - 1;
            if (intervalDistributionTable.at(3)[indexBeforeModule] == 0.5)
            {
                indexModule = indexBeforeModule;
                indexBeforeModule--;
            }
            break;
        }
    }
    step = intervalDistributionTable.at(0)[indexModule] - intervalDistributionTable.at(0)[indexBeforeModule];

    float 
        x1 = intervalDistributionTable.at(0)[indexBeforeModule], 
        Fx1 = intervalDistributionTable.at(3)[indexBeforeModule],
        Fx2 = intervalDistributionTable.at(3)[indexModule];
    

    return x1 + ((0.5 - Fx1) / ( Fx2 - Fx1)) * step;
}

std::vector<float> PrimaryDataProcessingHandler::CalñulateMode(std::vector<std::vector<float>> intervalDistributionTable)
{
    int moduleInterval = 1;
    std::vector<int> intervals;
    intervals.push_back(moduleInterval);
    for(int i = 1; i < intervalDistributionTable.at(0).size(); i++ )
        if (intervalDistributionTable.at(1)[i - 1] > intervalDistributionTable.at(1)[moduleInterval - 1])
        {
            intervals.clear();
            moduleInterval = i;
            intervals.push_back(moduleInterval);
        }
        else if(intervalDistributionTable.at(1)[i - 1] == intervalDistributionTable.at(1)[moduleInterval - 1])
            intervals.push_back(i);
    

    std::vector<float> modes;
    for(int i : intervals)
    {
        if (i == 1)
        {
            float
                mStart = intervalDistributionTable.at(0)[i - 1],
                mFreq = intervalDistributionTable.at(1)[i - 1],
                mFreqAfter = intervalDistributionTable.at(1)[i],
                step = intervalDistributionTable.at(0)[i] - intervalDistributionTable.at(0)[i - 1];

            float mode = mStart + ((mFreq - mFreqAfter) / ( mFreq - mFreqAfter)) * step;

            modes.push_back(mode);
        }
        else if (i == intervalDistributionTable.at(0).size() - 1)
        {
            float
                mStart = intervalDistributionTable.at(0)[i - 1],
                mFreq = intervalDistributionTable.at(1)[i - 1],
                mFreqBefore = intervalDistributionTable.at(1)[i - 2],
                step = intervalDistributionTable.at(0)[i] - intervalDistributionTable.at(0)[i - 1];

            float mode = mStart + ((mFreq - mFreqBefore) / (mFreq - mFreqBefore)) * step;

            modes.push_back(mode);
        }
        else
        {
            float
                mStart = intervalDistributionTable.at(0)[i - 1],
                mFreq = intervalDistributionTable.at(1)[i - 1],
                mFreqBefore = intervalDistributionTable.at(1)[i - 2],
                mFreqAfter = intervalDistributionTable.at(1)[i],
                step = intervalDistributionTable.at(0)[i] - intervalDistributionTable.at(0)[i - 1];

            float mode = mStart + ((mFreq - mFreqBefore) / (2 * mFreq - mFreqBefore - mFreqAfter)) * step;

            if (!isnan(mode))
                modes.push_back(mode);
        }
    }



    return modes;
}

float PrimaryDataProcessingHandler::CalculateSampleAverage(std::vector<float> set, std::vector<float> frequency, const float largestValue)
{
    float sum = 0;
    for (int i = 0; i < set.size(); i++)
        sum += set[i] * frequency[i];
    return sum / largestValue;
}


int PrimaryDataProcessingHandler::CalculateDiscreteIntervals(const int ArraySize)
{
    if(ArraySize <= 0)
        return 0;
    
    int l = 1 + 3.32 * std::log10(ArraySize);
    l < 6 ? l = 6 : l = l;
    return l;
}

int SortingHandler::Partition(std::vector<float>& arr, int low, int high)
{
    int pivot = arr[high];

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) 
    {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void SortingHandler::QuickSort(std::vector<float>& arr, int low, int high) 
{
    if (low < high) 
    {
        int pi = Partition(arr, low, high);

        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}